package cz.cvut.fel.pjv;

/**
 * Implementation of the {@link Queue} backed by fixed size array.
 */
public class CircularArrayQueue implements Queue {

    private String[] queue;
    private int elementsAmount = 0;
    private int capacity;
    private int startIndex = 0, endIndex = 0;


    /**
     * Creates the queue with capacity set to the value of 5.
     */
    public CircularArrayQueue() {
        queue = new String[5];
        this.capacity = 5;
    }


    /**
     * Creates the queue with given {@code capacity}. The capacity represents maximal number of elements that the
     * queue is able to store.
     * @param capacity of the queue
     */
    public CircularArrayQueue(int capacity) {
        queue = new String[capacity];
        this.capacity = capacity;
    }

    @Override
    public int size() {
        return elementsAmount;
    }

    @Override
    public boolean isEmpty() {
        return elementsAmount == 0;
    }

    @Override
    public boolean isFull() {
        return elementsAmount == capacity;
    }

    @Override
    public boolean enqueue(String obj){
        if(isFull()){return false;}
        queue[endIndex] = obj;
        elementsAmount++;

        endIndex++;
        endIndex %= capacity;
        return true;
    }

    @Override
    public String dequeue() {
        if(isEmpty()){return  null;}

        String poppedString = queue[startIndex];

        elementsAmount--;

        startIndex++;
        startIndex %= capacity;

        return  poppedString;
    }

    @Override
    public void printAllElements() {
        int printIndex = startIndex;
        for(int i = 0; i < elementsAmount; i++){
            System.out.println(queue[printIndex]);
            printIndex++;
            printIndex %= capacity;
        }
    }
}
