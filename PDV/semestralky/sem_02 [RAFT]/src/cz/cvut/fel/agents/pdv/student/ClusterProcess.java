package cz.cvut.fel.agents.pdv.student;

import cz.cvut.fel.agents.pdv.dsand.Message;
import cz.cvut.fel.agents.pdv.dsand.Pair;
import cz.cvut.fel.agents.pdv.raft.RaftProcess;
import cz.cvut.fel.agents.pdv.raft.messages.*;

import javax.swing.plaf.nimbus.State;
import java.lang.reflect.Parameter;
import java.util.*;
import java.util.function.BiConsumer;

/**
 * Vasim ukolem bude naimplementovat (pravdepodobne nejenom) tuto tridu. Procesy v clusteru pracuji
 * s logy, kde kazdy zanam ma podobu mapy - kazdy zaznam v logu by mel reprezentovat stav
 * distribuovane databaze v danem okamziku.
 *
 * Vasi implementaci budeme testovat v ruznych scenarich (viz evaluation.RaftRun a oficialni
 * zadani). Nasim cilem je, abyste zvladli implementovat jednoduchou distribuovanou key/value
 * databazi s garancemi podle RAFT.
 */
public class ClusterProcess extends RaftProcess<Map<String, String>> {

    private Random rand = new Random();

    // ostatni procesy v clusteru
    private final List<String> otherProcessesInCluster;

    // TIME SECTION
    // maximalni spozdeni v siti
    private final int networkDelays;
    private int currentTime = 0;
    private int timeout;
    private int electionTimer = 0;
    private int electionTimeout;
    private int heartbeatTimeout;

    private States state = States.FOLLOWER;
    private int currentTerm = 0;
    private String leaderID = "";
    private String votedFor = "";
    private int votesAmount = 0;



    private int commitIndex = 0;
    private int lastApplied = 0;
    private Map<String,String> db = new HashMap<String,String>();



    private ArrayList<LogEntry> log;
    private int getLastLogIndex(){
        return log.size()-1;
    }
    private Set<String> requestsSet = new HashSet<String>();
    private boolean addToLog(LogEntry le){
        if(!requestsSet.contains(le.getRequestID())){
            requestsSet.add(le.getRequestID());
            log.add(le);
            return true;
        }
        return false;
    }



    private ArrayList<Integer> nextIndex;
    private ArrayList<Integer> matchIndex;


    public ClusterProcess(String id, Queue<Message> inbox, BiConsumer<String, Message> outbox,
                          List<String> otherProcessesInCluster, int networkDelays) {
        super(id, inbox, outbox);
        this.otherProcessesInCluster = otherProcessesInCluster;
        this.networkDelays = networkDelays;

        // Init timeout

        int min = 10,max = 20;
        timeout = rand.nextInt(max - min + 1) + min;
        heartbeatTimeout = (int) Math.ceil(timeout/2);



        // Log initialization and adding empty logEntry
        log = new ArrayList<LogEntry>();
        log.add(new LogEntry(0,"",new Pair<>("",""),"",""));
    }

    @Override
        public Optional<Map<String, String>> getLastSnapshotOfLog() {
        return Optional.of(db);
    }

    @Override
    public String getCurrentLeader() {
        return leaderID;
    }

    private void applyOperation(){
        //apply log[lastApplied]
        LogEntry le = log.get(lastApplied);
        String operation = le.getOperation();
        Pair<String,String> pair = le.getPair();
        String key = pair.getFirst(), value = pair.getSecond();
        if(operation.equals("PUT")){
            db.put(key,value);
        }
        else if(operation.equals("APPEND")){
            if(!db.containsKey(key)){
                db.put(key,value);
            }
            else{
                String existingValue = db.get(key);
                String newValue = existingValue + value;
                db.remove(key);
                db.put(key,newValue);
            }
        }

        if(state == States.LEADER){
            if(operation.equals("GET")){
                send(le.getSender(),new ServerResponseWithContent<String>(le.getRequestID(),db.get(le.getPair().getFirst())));
            }
            else{
                send(le.getSender(),new ServerResponseConfirm(le.getRequestID()));
            }
        }
    }

    private void removeFromLogUntilIndex(int idx){
        while (getLastLogIndex() >= idx){
            log.remove(idx);
        }
    }


    public void initialHeartbeat(){
        for(int i = 0; i < otherProcessesInCluster.size(); i++){
            send(otherProcessesInCluster.get(i),
                    new AppendEntries(currentTerm,getId(),0,
                            0, new ArrayList<LogEntry>(),
                            0));
        }
    }
    private void sendHeartbeats(){
        for(int i = 0; i < otherProcessesInCluster.size(); i++){
            ArrayList<LogEntry> entriesToSend = new ArrayList<LogEntry>();
            for(int j = nextIndex.get(i); j <= getLastLogIndex(); j++){
                entriesToSend.add(log.get(j));
            }

            int prevLogIndex = nextIndex.get(i)-1;
            AppendEntries ae = new AppendEntries(currentTerm,getId(),prevLogIndex,
                    log.get(prevLogIndex).getTerm(), entriesToSend, commitIndex);

            send(otherProcessesInCluster.get(i), ae);
//            entriesToSend.clear();
        }
    }

    private void startElections(){
        state = States.CANDIDATE;
        currentTerm++;
        votedFor = getId();
        votesAmount = 1;
        electionTimer = 0;
        for(int i = 0; i < otherProcessesInCluster.size(); i++){
            send(otherProcessesInCluster.get(i),new RequestVote(currentTerm,getId(),
                    getLastLogIndex(),
                    log.get(getLastLogIndex()).getTerm()));
        }

        electionTimeout = rand.nextInt((2*timeout) - timeout + 1) + timeout;
    }
    private void initValues(){
        votedFor = "";
        votesAmount = 0;
        currentTime = 0;
    }

    private boolean majorityMatch(int n){
        int cnt = 0;
        for(int i = 0; i < matchIndex.size(); i++){
            if(matchIndex.get(i) >= n){
                cnt++;
            }
        }
        return cnt > Math.ceil(otherProcessesInCluster.size() / 2);
    }

    private void becomeLeader(){
        // Won elections
        state = States.LEADER;
        leaderID = getId();
        nextIndex = new ArrayList<Integer>(Collections.nCopies(otherProcessesInCluster.size(), getLastLogIndex()+1));
        matchIndex = new ArrayList<Integer>(Collections.nCopies(otherProcessesInCluster.size(), 0));

//        System.out.println(getId() + ": " + getId() + " is leader now!");
//                            System.out.println("I have " + votesAmount + " of " + otherProcessesInCluster.size());
        initValues();
        initialHeartbeat();
    }

    @Override
    public void act() {

        // For all
        while(commitIndex > lastApplied){
            lastApplied++;
            applyOperation();
        }
        // Start elections
        if(state != States.LEADER){
            if(currentTime > timeout && votedFor.equals("") || electionTimer > electionTimeout){
                startElections();
            }
            if(state == States.CANDIDATE && votesAmount > Math.ceil(otherProcessesInCluster.size() / 2)){
                becomeLeader();
            }
        }


        if(state == States.LEADER){
            if(currentTime > 0 && currentTime % heartbeatTimeout == 0){
                sendHeartbeats();
            }
            for(int N = commitIndex+1; N < log.size() && log.get(N).getTerm() == currentTerm && majorityMatch(N); N++){
                commitIndex = N;
            }
        }



        // Inbox part
        while (!inbox.isEmpty()) {
            Message message = inbox.poll();

            if(message instanceof MessageTerm){
                MessageTerm mt = (MessageTerm) message;
                if(mt.getTerm() > currentTerm){
                    state = States.FOLLOWER;
                    currentTerm = mt.getTerm();
                    initValues();
                }


                //------------------------------------------------------------------------
                // AppendEntries RPC
                //------------------------------------------------------------------------
                if(message instanceof AppendEntries){
                    AppendEntries ae = (AppendEntries) message;

                    if(state == States.LEADER){
                        int processIndex = otherProcessesInCluster.indexOf(ae.sender);

                        if(ae.isSuccess()){
                            nextIndex.set(processIndex,nextIndex.get(processIndex) + ae.getAdded());
                            matchIndex.set(processIndex,ae.getPrevLogIndex());
                        }
                        else{
                            nextIndex.set(processIndex,nextIndex.get(processIndex)-1);
                        }

                    }
                    else if(state == States.CANDIDATE && ae.getTerm() >= currentTerm){
                        state = States.FOLLOWER;
                        initValues();
                        leaderID = ae.getLeaderId();
                    }
                    else if(state == States.FOLLOWER) {

                        if (ae.getTerm() < currentTerm ||
                                getLastLogIndex() < ae.getPrevLogIndex() ||
                                log.get(ae.getPrevLogIndex()).getTerm() != ae.getPrevLogTerm()) {

                            if (getLastLogIndex() >= ae.getPrevLogIndex() && log.get(ae.getPrevLogIndex()).getTerm() != ae.getPrevLogTerm()) {
                                removeFromLogUntilIndex(ae.getPrevLogIndex());
                            }

                            ae.setTerm(currentTerm);
                            ae.setSuccess(false);
                            send(ae.sender, ae);
                            continue;
                        }


                        else {
                            initValues();
                            leaderID = ae.getLeaderId();


                            int addedCount = 0;

                            ArrayList<LogEntry> entries = ae.getEntries();
                            for (LogEntry entry : entries) {
                                if(addToLog(entry)){
                                    addedCount++;
                                }
                            }
                            if (ae.getLeaderCommit() > commitIndex) {
                                commitIndex = Math.min(ae.getLeaderCommit(), getLastLogIndex());
                            }

                            ae.setSuccess(true);
                            ae.setAdded(addedCount);
                            send(ae.sender,ae);
                            continue;
                        }
                    }
                }

                //------------------------------------------------------------------------
                // RequestVote RPC
                //------------------------------------------------------------------------
                else if(message instanceof RequestVote){
                    RequestVote rv = (RequestVote) message;
                    if(rv.getTerm() < currentTerm || log.get(getLastLogIndex()).getTerm() > rv.getTerm() ||
                        getLastLogIndex() > rv.getLastLogIndex()){
                        send(rv.sender,new AnswerVote(currentTerm,false));
                        continue;
                    }

                    if(votedFor.equals("") &&  rv.getLastLogIndex() >= getLastLogIndex()){
                        votedFor = rv.sender;
                        send(rv.sender,new AnswerVote(currentTerm,true));
                        continue;
                    }

                }
                else if (message instanceof AnswerVote && state == States.CANDIDATE){
                    AnswerVote av = (AnswerVote) message;

                    if(av.getVoteGranted()){
                        votesAmount++;
                        continue;
                    }
                }
            }

            //------------------------------------------------------------------------
            // Client Requests
            //------------------------------------------------------------------------
            if (message instanceof ClientRequestWhoIsLeader ||
                    message instanceof ClientRequestWithContent && state != States.LEADER) {
                ClientRequest cr = (ClientRequest) message;
                send(message.sender, new ServerResponseLeader(cr.getRequestId(),getCurrentLeader()) );
            }
            else if (message instanceof ClientRequestWithContent){
                ClientRequestWithContent crwc = (ClientRequestWithContent) message;

                Pair<String, String> content = (Pair<String, String>) crwc.getContent();
                String operation= crwc.getOperation().getName();

                LogEntry le = new LogEntry(currentTerm,operation,content,crwc.getRequestId(), crwc.sender);

                addToLog(le);
            }
        }

        if(state == States.CANDIDATE){
            electionTimer++;
        }
        else{
            currentTime++;
        }



        // doimplementuje metodu act() podle RAFT

        // krome vlastnich zprav (vasich trid), dostavate typy zprav z balicku raft.messages s rodicem
        // ClientRequest, tak si je projdete, at vite, co je ucelem a obsahem jednotlivych typu.
        // V pripade ClientRequestWithContent dostavate zpravu typu
        // ClientRequestWithContent<StoreOperationEnums, Pair<String, String>>, kde StoreOperationEnums
        // je operace, kterou mate udelat s obsahem paru Pair<String, String>, kde prvni hodnota
        // paru je klic (nikdy neni prazdny) a druha je hodnota (v pripade pozadavku GET je prazdny)

        // dejte si pozor na jednotlive akce podle RAFT. S klientem komunikujte vyhradne pomoci zprav
        // typu ServerResponse v messages

        // na pozadavky klientu odpovidate zpravami typu ServerResponse viz popis podtypu v messages.
        // !!! V TOMTO PRIPADE JE 'requestId' ROVNO POZADAVKU KLIENTA, NA KTERY ODPOVIDATE !!!

        // dalsi podrobnosti naleznete na strance se zadanim
    }
}
