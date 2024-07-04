package cz.cvut.fel.agents.pdv.student;

import cz.cvut.fel.agents.pdv.dsand.Message;

public class AnswerVote extends MessageTerm {

    Boolean voteGranted;

    public AnswerVote(int term, Boolean voteGranted) {
        super(term);
        this.voteGranted = voteGranted;
    }

    public int getTerm() {
        return super.getTerm();
    }

    public void setTerm(int term) {
        super.setTerm(term);
    }

    public Boolean getVoteGranted() {
        return voteGranted;
    }

    public void setVoteGranted(Boolean voteGranted) {
        this.voteGranted = voteGranted;
    }
}
