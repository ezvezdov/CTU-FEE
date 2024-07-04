package cz.cvut.fel.pjv.cars.model;

import java.util.Arrays;

public class ServiceBook {
    private final String[] serviceRecords;
    private int recordsAmount = 0;
    Car car;

    public ServiceBook(Car car){
        this.serviceRecords = new String[10];
        this.car = car;
        car.setServiceBook(this);
    }
    public void addRecord(String record){
        if(recordsAmount == serviceRecords.length) return;
        serviceRecords[recordsAmount] = record;
        recordsAmount++;
    }

    @Override
    public String toString() {
        StringBuilder output = new StringBuilder();
        for(int i = 0; i < recordsAmount; i++){
            output.append(serviceRecords[i]).append("\n");
        }
        return output.toString();
    }
}
