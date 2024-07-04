package cz.cvut.fel.pjv;

public class NodeImpl implements Node{

    private int nodeValue;
    private NodeImpl leftNode = null, rightNode = null;

    public NodeImpl(){
    }

    public void setNodeValue(int nodeValue) {
        this.nodeValue = nodeValue;
    }

    public void setLeftNode(NodeImpl leftNode) {
        this.leftNode = leftNode;
    }

    public void setRightNode(NodeImpl rightNode) {
        this.rightNode = rightNode;
    }

    @Override
    public Node getLeft() {
        return (Node) leftNode;
    }

    @Override
    public Node getRight() {
        return (Node) rightNode;
    }

    @Override
    public int getValue() {
        return nodeValue;
    }
}
