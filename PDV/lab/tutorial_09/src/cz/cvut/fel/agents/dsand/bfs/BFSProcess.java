package cz.cvut.fel.agents.dsand.bfs;

import cz.cvut.fel.agents.dsand.DSProcess;
import cz.cvut.fel.agents.dsand.Message;

import java.nio.file.Path;
import java.util.*;
import java.util.function.BiConsumer;

public class BFSProcess extends DSProcess {

    /**
     * Seznam nasledniku uzlu spravovaneho aktualnim procesem
     */
    private List<String> neighbors;

    /**
     * Je uzel spravovany aktualnim procesem 'root' prohledavani?
     */
    private boolean isRoot;

    /**
     * Je uzel spravovany aktualnim procesem cilovym uzlem?
     */
    private boolean isGoal;

    /**
     * Identifikator predchazejiciho procesu ('uzlu' na nejkratsi ceste)
     */
    private String predecessor = null;

    public BFSProcess(String id, Queue<Message> inbox, BiConsumer<String, Message> outbox,
                      List<String> neighbors, boolean isRoot, boolean isGoal) {
        super(id, inbox, outbox);
        this.neighbors = neighbors;
        this.isRoot = isRoot;
        this.isGoal = isGoal;

        // Pro jednoduchost nastavime predchazeji proces korene na koren samotny.
        if(isRoot) predecessor = id;
    }

    // Root sent
    boolean sent = false;

    @Override
    public void act() {

        if( isRoot && !sent ){
            for(String neigbour : neighbors){
                send(neigbour, new SearchMessage());
            }
            sent = true;
        }

        Iterator<Message> inbox_it = inbox.iterator();
        while(inbox_it.hasNext()) {
            Message msg = inbox.poll();
            System.out.println("I " + id + " received message from " + msg.sender);

            if( msg instanceof SearchMessage){
                if(predecessor == null) {
                    predecessor = msg.sender;

                    for (String neigbour : neighbors) {
                        if (!Objects.equals(neigbour, predecessor)) {
                            send(neigbour, new SearchMessage());
                        }
                    }
                    if (isGoal) {
                        System.out.println("Reached goal");
                        send(predecessor, new PathMessage(id));
                    }
                }
            }
            else if (msg instanceof PathMessage){
                String path = id + ", " + ((PathMessage) msg).path();

                if(isRoot){
                    System.out.println(path);
                    terminateAll();
                }
                else {
                    send(predecessor, new PathMessage(path));
                }
            }

        }




        /*
            Doplnte algoritmus, ktery bude vykonavat kazdy proces

            1) Zpracujte prichozi zpravy v 'inbox'
            2) Vytvorte tridy pro posilane zpravy
            3) Doplnte logiku reakce na zpravu a odeslani novych zprav
         */
    }

    @Override
    public boolean isTerminated() {
        return false;
    }
}
