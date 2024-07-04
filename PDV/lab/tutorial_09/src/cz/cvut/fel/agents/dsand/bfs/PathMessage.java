package cz.cvut.fel.agents.dsand.bfs;

import cz.cvut.fel.agents.dsand.Message;

public class PathMessage extends Message {

    String path;
    public PathMessage(String path) {
        this.path = path;
    }

    public String path() {
        return this.path;
    }
}
