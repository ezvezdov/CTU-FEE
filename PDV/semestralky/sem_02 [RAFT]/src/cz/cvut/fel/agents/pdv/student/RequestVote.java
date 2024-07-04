package cz.cvut.fel.agents.pdv.student;

import cz.cvut.fel.agents.pdv.dsand.Message;

public class RequestVote extends MessageTerm {
    private String candidateId;
    private int lastLogIndex;
    private int lastLogTerm;

    public RequestVote(int term, String candidateId, int lastLogIndex, int lastLogTerm) {
        super(term);
        setTerm(term);
        this.candidateId = candidateId;
        this.lastLogIndex = lastLogIndex;
        this.lastLogTerm = lastLogTerm;
    }

    public int getTerm() {
        return super.getTerm();
    }

    public void setTerm(int term) {
        super.setTerm(term);
    }

    public String getCandidateId() {
        return candidateId;
    }

    public void setCandidateId(String candidateId) {
        this.candidateId = candidateId;
    }

    public int getLastLogIndex() {
        return lastLogIndex;
    }

    public void setLastLogIndex(int lastLogIndex) {
        this.lastLogIndex = lastLogIndex;
    }

    public int getLastLogTerm() {
        return lastLogTerm;
    }

    public void setLastLogTerm(int lastLogTerm) {
        this.lastLogTerm = lastLogTerm;
    }
}
