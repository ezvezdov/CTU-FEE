package cz.cvut.fel.agents.pdv.student;

import cz.cvut.fel.agents.pdv.dsand.Message;

public class MessageTerm extends Message {
    private int term;

    MessageTerm(int term) {
        this.term = term;
    }

    public int getTerm() {
        return term;
    }
    public void setTerm(int term) {
        this.term = term;
    }
}
