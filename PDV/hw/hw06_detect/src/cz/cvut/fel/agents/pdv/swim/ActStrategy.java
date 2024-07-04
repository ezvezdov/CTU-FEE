package cz.cvut.fel.agents.pdv.swim;

import cz.cvut.fel.agents.pdv.dsand.Message;
import cz.cvut.fel.agents.pdv.dsand.Pair;

import java.util.*;

/**
 * Trida s implementaci metody act() pro proces Failure Detector. Tuto tridu (a tridy pouzivanych zprav) budete
 * odevzdavat. Do tridy si muzete doplnit vlastni pomocne datove struktury. Hodnoty muzete inicializovat primo
 * v konstruktoru. Klicova je metoda act(), kterou vola kazda instance tridy FailureDetectorProcess ve sve metode
 * act(). Tuto metodu naimplementujte podle protokolu SWIM predstaveneho na prednasce.
 *
 * Pokud si stale jeste nevite rady s frameworkem, inspiraci muzete nalezt v resenych prikladech ze cviceni.
 */

class CheckMessage extends Message{

    private String process;

    public CheckMessage(String processToCheck) {
        this.process = processToCheck;
    }

    public String getProcess() {
        return process;
    }
}

class AcknowledgementMessage extends Message{
    private String process;

    public String getProcess() {
        return process;
    }

    public AcknowledgementMessage(String process) {

        this.process = process;
    }
}

public class ActStrategy {

    // maximalni zpozdeni zprav
    private int timeToDetectKilledProcess;
    private int upperBoundOnMessages;
    private final List<String> otherProcesses;

    private int counter = 0;
    private int messagesCounter = 0;

    private int period;
    private int timeout;

    // We can make here just single variable
    private List<String> currentCheckProcess = new ArrayList<>();
    Map<String, Integer> processTimes;
    Map<String, Integer> processTimesAll;

    // ProcessToCheck, ProcessToSendAck
    Map<String, String> foreignCheck;





    public ActStrategy(int maxDelayForMessages, List<String> otherProcesses,
                       int timeToDetectKilledProcess, int upperBoundOnMessages) {
        this.otherProcesses = otherProcesses;
        this.timeToDetectKilledProcess = timeToDetectKilledProcess;
        this.upperBoundOnMessages = upperBoundOnMessages;

        this.period = 10;
        this.timeout = this.period + maxDelayForMessages;

        // Init processTimes, processTimesAll, foreignCheck
        processTimes = new HashMap<>();
        processTimesAll = new HashMap<>();
        foreignCheck = new HashMap<>();
        for (String process : otherProcesses) {
            processTimes.put(process, -1);
            processTimesAll.put(process,-1);
            foreignCheck.put(process,"");
        }
    }

    /**
     * Metoda je volana s kazdym zavolanim metody act v FailureDetectorProcess. Metodu implementujte
     * tak, jako byste implementovali metodu act() v FailureDetectorProcess, misto pouzivani send()
     * pridejte zpravy v podobe paru - prijemce, zprava do listu. Zpravy budou nasledne odeslany.
     * <p>
     * Diky zavedeni teto metody muzeme kontrolovat pocet odeslanych zprav vasi implementaci.
     */

    //https://en.wikipedia.org/wiki/SWIM_Protocol
    public List<Pair<String, Message>> act(Queue<Message> inbox, String disseminationProcess) {
        List<Pair<String, Message> > MessagesToSend = new ArrayList<>();
        if(messagesCounter > upperBoundOnMessages){
            return new ArrayList<>();
        }

        // Od DisseminationProcess muzete dostat zpravu typu DeadProcessMessage, ktera Vas
        // informuje o spravne detekovanem ukoncenem procesu.
        // DisseminationProcess muzete poslat zpravu o detekovanem "mrtvem" procesu.
        // Zprava musi byt typu PFDMessage.

        // 1. Sending Ping
        // Send ping to random process
        if (currentCheckProcess.size() == 0 &&  counter % period == 0) {
            Random rand = new Random();
            String processToSend;
            do {
                processToSend= otherProcesses.get(rand.nextInt(otherProcesses.size()));
            }while(processTimes.get(processToSend) != -1);

            MessagesToSend.add(new Pair<String, Message>(processToSend, new CheckMessage("")));
            messagesCounter++;

            processTimes.put(processToSend, counter);
            currentCheckProcess.add(processToSend);
//            System.out.println("Send Check Message to " + processToSend);
        }

        // Check currentCheckProcess
        for (String process : currentCheckProcess) {

            // Send message, that process is dead
            if(processTimesAll.get(process) != -1 && counter > processTimes.get(process) + timeToDetectKilledProcess - 3*timeout){
//                System.out.println("Process " + process + " is dead." );
                MessagesToSend.add(new Pair<String, Message>(disseminationProcess, new PFDMessage(process)));
                messagesCounter++;
            }

            // Send Check request to all processes
                if(processTimes.get(process) != -1 &&  counter > processTimes.get(process) + timeout && processTimesAll.get(process) == -1) {
//                System.out.println("Process " + process + " didn't reply, send messages to all");

                processTimesAll.put(process, counter);
                for (String processesToSend : otherProcesses) {
                    MessagesToSend.add(new Pair<String, Message>(processesToSend, new CheckMessage(process)));
                    messagesCounter++;
                }
            }
        }


        // 2. Checking inbox
        while (!inbox.isEmpty()) {
            Message message = inbox.poll();
            if(message instanceof CheckMessage ){

                // If process should check itself
                if(Objects.equals(((CheckMessage) message).getProcess(), "")){
//                    System.out.println("Send ack");
                    MessagesToSend.add(new Pair<String, Message>(message.sender, new AcknowledgementMessage("")));
                    messagesCounter++;
                }

                // If process should check another process
                else{
                    if(Objects.equals(foreignCheck.get(((CheckMessage) message).getProcess()), "")){
                        MessagesToSend.add(new Pair<String, Message>(((CheckMessage) message).getProcess(), new CheckMessage("")));
//                        foreignCheck.put(message.sender,((CheckMessage) message).getProcess());
                        foreignCheck.put(((CheckMessage) message).getProcess(),message.sender);
                        messagesCounter++;

                    }
                }

            }
            else if(message instanceof AcknowledgementMessage){

                // Receive foreign ack
                if(!Objects.equals(((AcknowledgementMessage) message).getProcess(), "")){
                    String senderProcess = ((AcknowledgementMessage) message).getProcess();
//                    System.out.println("Receive foreign ack from " + senderProcess);
                    processTimes.put(senderProcess,-1);
                    processTimesAll.put(senderProcess,-1);
                    currentCheckProcess.remove(String.valueOf(senderProcess));
                }

                // Resend foreign ack
                if(!Objects.equals(foreignCheck.get(message.sender), "")){

//                    System.out.println("Receive foreign ack from " + message.sender + " to " + foreignCheck.get(message.sender));
                    MessagesToSend.add(new Pair<String, Message>(foreignCheck.get(message.sender), new AcknowledgementMessage(message.sender)));
                    messagesCounter++;
                    foreignCheck.put(message.sender,"");
                }

//                System.out.println("Receive ack from " + message.sender);

                processTimes.put(message.sender,-1);
                processTimesAll.put(message.sender,-1);
                currentCheckProcess.remove(String.valueOf(message.sender));
            }

        }

        counter++;
        if(messagesCounter > upperBoundOnMessages){
            return new ArrayList<>();
        }
        return MessagesToSend;
    }

}

