package cz.cvut.fel.agents.pdv.student;

import java.util.ArrayList;

public class AppendEntries extends MessageTerm {

    private String leaderId;
    private int prevLogIndex;
    private int prevLogTerm;
    private ArrayList<LogEntry> entries;
    private int leaderCommit;
    private boolean success;
    private int added;

    public AppendEntries(int term, String leaderId, int prevLogIndex, int prevLogTerm, ArrayList<LogEntry> entries, int leaderCommit) {
        super(term);
        super.setTerm(term);

        this.leaderId = leaderId;
        this.prevLogIndex = prevLogIndex;
        this.prevLogTerm = prevLogTerm;
        this.entries = entries;
        this.leaderCommit = leaderCommit;
    }

    public int getTerm() {
        return super.getTerm();
    }

    public void setTerm(int term) {
        super.setTerm(term);
    }

    public String getLeaderId() {
        return leaderId;
    }

    public void setLeaderId(String leaderId) {
        this.leaderId = leaderId;
    }

    public int getPrevLogIndex() {
        return prevLogIndex;
    }

    public void setPrevLogIndex(int prevLogIndex) {
        this.prevLogIndex = prevLogIndex;
    }

    public int getPrevLogTerm() {
        return prevLogTerm;
    }

    public void setPrevLogTerm(int prevLogTerm) {
        this.prevLogTerm = prevLogTerm;
    }

    public ArrayList<LogEntry> getEntries() {
        return entries;
    }

    public void setEntries(ArrayList<LogEntry> entries) {
        this.entries = entries;
    }

    public int getLeaderCommit() {
        return leaderCommit;
    }

    public void setLeaderCommit(int leaderCommit) {
        this.leaderCommit = leaderCommit;
    }

    public boolean isSuccess() {
        return success;
    }

    public void setSuccess(boolean success) {
        this.success = success;
    }

    public int getAdded() {
        return added;
    }

    public void setAdded(int added) {
        this.added = added;
    }
}
