package cz.cvut.fel.agents.pdv.student;

import cz.cvut.fel.agents.pdv.dsand.Pair;

import java.io.Serializable;

public class LogEntry implements Serializable {
    private int term;
    private String operation;
    private Pair<String,String> pair;

    private String requestID;
    private String sender;

    public LogEntry(int term, String operation, Pair<String, String> pair,String requestID,String sender) {
        this.term = term;
        this.operation = operation;
        this.pair = pair;
        this.requestID = requestID;
        this.sender = sender;
    }

    public int getTerm() {
        return term;
    }

    public void setTerm(int term) {
        this.term = term;
    }

    public String getOperation() {
        return operation;
    }

    public void setOperation(String operation) {
        this.operation = operation;
    }

    public Pair<String, String> getPair() {
        return pair;
    }

    public void setPair(Pair<String, String> pair) {
        this.pair = pair;
    }

    public String getRequestID() {
        return requestID;
    }

    public void setRequestID(String requestID) {
        this.requestID = requestID;
    }

    public String getSender() {
        return sender;
    }

    public void setSender(String sender) {
        this.sender = sender;
    }
}

