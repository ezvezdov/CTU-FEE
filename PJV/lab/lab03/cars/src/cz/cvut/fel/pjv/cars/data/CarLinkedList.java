package cz.cvut.fel.pjv.cars.data;

import cz.cvut.fel.pjv.cars.model.Car;

class CarListNode{
    private Car car;
    private CarListNode next;

    public CarListNode(Car car) {
        this.car = car;
    }

    public void setNext(CarListNode next) {
        this.next = next;
    }

    public Car getCar() {
        return car;
    }

    public CarListNode getNext() {
        return next;
    }
}

public class CarLinkedList {
    private CarListNode head = null;
    private CarListNode tail = null;
    private int counter = 0;

    public void add(Car car){
        CarListNode newNode = new CarListNode(car);

        if(head == null){
            head = newNode;
        }
        else{
            tail.setNext(newNode);
        }
        tail = newNode;
        counter++;
    }

    public Car[] toArray(){
        Car[] cars = new Car[counter];
        CarListNode currentNode = head;
        for(int i = 0; i < counter; i++){
            cars[i] = currentNode.getCar();
            currentNode = currentNode.getNext();
        }
        return cars;
    }
}
