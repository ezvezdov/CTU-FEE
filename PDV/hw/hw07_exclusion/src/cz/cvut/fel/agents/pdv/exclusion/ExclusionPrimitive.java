package cz.cvut.fel.agents.pdv.exclusion;

import cz.cvut.fel.agents.pdv.clocked.ClockedMessage;
import cz.cvut.fel.agents.pdv.clocked.ClockedProcess;
import cz.cvut.fel.agents.pdv.dsand.Message;

import java.util.*;

class RequestMessage extends ClockedMessage{
    String criticalSectionName;

    public RequestMessage(String criticalSectionName) {
        this.criticalSectionName = criticalSectionName;
    }
}

class OkMessage extends ClockedMessage{
    String criticalSectionName;

    public OkMessage(String criticalSectionName) {
        this.criticalSectionName = criticalSectionName;
    }
}

public class ExclusionPrimitive {

    /**
     * Stavy, ve kterych se zamek muze nachazet.
     */
    enum AcquisitionState {
        RELEASED,    // Uvolneny   - Proces, ktery vlastni aktualni instanci ExclusionPrimitive o pristup do kriticke
                     //              sekce nezada

        WANTED,      // Chteny     - Proces, ktery vlastni aktualni instanci ExclusionPrimitive zada o pristup do
                     //              kriticke sekce. Ten mu ale zatim nebyl odsouhlasen ostatnimi procesy.

        HELD         // Vlastneny  - Procesu bylo prideleno pravo pristupovat ke sdilenemu zdroji.
    }

    private ClockedProcess owner;            // Proces, ktery vlastni aktualni instanci ExclusionPrimitive

    private String criticalSectionName;      // Nazev kriticke sekce. POZOR! V aplikaci se muze nachazet vice kritickych
                                             // sekci s ruznymi nazvy!

    private String[] allAccessingProcesses;  // Seznam vsech procesu, ktere mohou chtit pristupovat ke kriticke sekci
                                             // s nazvem 'criticalSectionName' (a tak spolurozhoduji o udelovani pristupu)

    private AcquisitionState state;          // Aktualni stav zamku (vzhledem k procesu 'owner').
                                             // state==HELD znamena, ze proces 'owner' muze vstoupit do kriticke sekce

    // Doplnte pripadne vlastni datove struktury potrebne pro implementaci
    // Ricart-Agrawalova algoritmu pro vzajemne vylouceni

    private Queue<String> requestsQueue = new LinkedList<>();

    private Map<String, Boolean> accessingProcessesOK;
    private int okCount = 0;
    private int myRequestTime = 0;




    public ExclusionPrimitive(ClockedProcess owner, String criticalSectionName, String[] allProcesses) {
        this.owner = owner;
        this.criticalSectionName = criticalSectionName;
        this.allAccessingProcesses = allProcesses;

        // Na zacatku je zamek uvolneny
        this.state = AcquisitionState.RELEASED;

        // Inicializace vlastnich struktur
        accessingProcessesOK = new HashMap<>();
        for (String process : allAccessingProcesses) {
            accessingProcessesOK.put(process, false);
        }
    }

    /**
     * Metoda pro zpracovani nove prichozi zpravy
     *
     * @param m    prichozi zprava
     * @return 'true', pokud je zprava 'm' relevantni pro aktualni kritickou sekci.
     */
    public boolean accept(ClockedMessage m) {

        // Implementujte zpracovani prijimane zpravy informujici
        // o pristupech ke sdilenemu zdroji podle Ricart-Agrawalova
        // algoritmu. Pokud potrebujete posilat specificke zpravy,
        // vytvorte si pro ne vlastni tridy.
        //
        // POZOR! Ne vsechny zpravy, ktere v teto metode dostanete Vas
        // budou zajimat! Budou Vam prichazet i zpravy, ktere se  napriklad
        // tykaji jinych kritickych sekci. Pokud je zprava nerelevantni, tak
        // ji nezpracovavejte a vratte navratovou hodnotu 'false'. Nekdo jiny
        // se o ni urcite postara :-)
        //
        // Nezapomente se starat o cas procesu 'owner'
        // pomoci metody owner.increaseTime(). Aktualizaci
        // logickeho casu procesu s prijatou zpravou (pomoci maxima) jsme
        // za Vas jiz vyresili.
        //
        // Cas poslani prijate zpravy muzete zjistit dotazem na hodnotu
        // m.sentOn. Aktualni logicky cas muzete zjistit metodou owner.getTime().
        // Zpravy se posilaji pomoci owner.send(prijemce, zprava) a je jim auto-
        // maticky pridelen logicky cas odeslani. Retezec identifikujici proces
        // 'owner' je ulozeny v owner.id.


        if(m instanceof RequestMessage){
            // Foreign criticalSection
            if(!Objects.equals(((RequestMessage) m).criticalSectionName, criticalSectionName)){
                return false;
            }

            if(state == AcquisitionState.HELD){
                requestsQueue.add(m.sender);
            }
            else if( (state == AcquisitionState.WANTED && myRequestTime < m.sentOn) || (state == AcquisitionState.WANTED && myRequestTime == m.sentOn && owner.id.compareTo(m.sender) < 0)){
                requestsQueue.add(m.sender);
            }
            else{
                // Send OK(K) message
                owner.increaseTime();
                owner.send(m.sender,new OkMessage(criticalSectionName));
            }

        }
        else if(m instanceof OkMessage){
            // Foreign criticalSection
            if(!Objects.equals(((OkMessage) m).criticalSectionName, criticalSectionName)){
                return false;
            }

            // Add ok message
            if(state == AcquisitionState.WANTED && !accessingProcessesOK.get(m.sender)) {
                accessingProcessesOK.put(m.sender,true);
                okCount++;

                // Set state Held, get access to critical section
                if(okCount == allAccessingProcesses.length){
                    state = AcquisitionState.HELD;

                    // Restore request variables
                    okCount = 0;
                    myRequestTime = 0;
                    for (String process : allAccessingProcesses) {
                        accessingProcessesOK.put(process, false);
                    }
                }
            }

        }
        else{
            // Foreign message
            return false;
        }

        return true;
    }

    public void requestEnter() {
        // owner's request to access to critical section 'criticalSectionName'

        // Send request to all accessing processes
        owner.increaseTime();
        for (String process : allAccessingProcesses) {
            owner.send(process, new RequestMessage(criticalSectionName));
        }

        // Make request timestamp
        myRequestTime = owner.getTime();

        // Change state to WANTED
        state = AcquisitionState.WANTED;
    }


    public void exit() {
        // Exit from critical section

        // Set state to RELEASED
        state = AcquisitionState.RELEASED;

        // Send OK to all processes from requestQueue
        owner.increaseTime();
        while (!requestsQueue.isEmpty()){
            String process = requestsQueue.poll();
            owner.send(process,new OkMessage(criticalSectionName));
        }
    }

    public String getName() {
        return criticalSectionName;
    }

    public boolean isHeld() {
        return state == AcquisitionState.HELD;
    }

}
