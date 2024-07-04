package cz.cvut.fel.pjv;

public class TreeImpl implements Tree{

    private NodeImpl root = null;

    public TreeImpl() {}

    private boolean makeNode(int[] values, int left, int right, NodeImpl node){
        if(left > right || left < 0 || right >= values.length){
            return false;
        }

        int top = left + (right - left) / 2;
        if((right - left) % 2 == 1){
            top++;
        }
        if(right == left){
            top = right;
        }
        node.setNodeValue(values[top]);
        if(right == left){return true;}


        NodeImpl leftNode = new NodeImpl();
        node.setLeftNode(leftNode);

        if(!makeNode(values,left,top - 1,leftNode)){
            node.setLeftNode(null);
        }

        NodeImpl rightNode = new NodeImpl();
        node.setRightNode(rightNode);
        if(!makeNode(values,top + 1, right,rightNode)){
            node.setRightNode(null);
        }

        return true;
    }

    @Override
    public void setTree(int[] values) {
        if(values.length != 0){
            root = new NodeImpl();
            makeNode(values,0,values.length-1,root);
        }
    }

    @Override
    public Node getRoot() {
        return (Node) root;
    }

    private String getStringForPrint(String out, NodeImpl node,int spaceSize){
        out+= (new String(new char[spaceSize]).replace("\0", " ")) + "- " + node.getValue() + '\n';
        if(node.getLeft() != null){
            out = getStringForPrint(out,(NodeImpl) node.getLeft(),spaceSize+1);
        }
        if(node.getRight() != null){
            out = getStringForPrint(out,(NodeImpl) node.getRight(),spaceSize+1);
        }
        return out;
    }

    @Override
    public String toString() {
        return root != null ? getStringForPrint("",root,0) : "";
    }
}