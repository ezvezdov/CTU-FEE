package cz.cvut.fel.pjv.cars.model;

import java.util.Objects;
import java.util.UUID;

import cz.cvut.fel.pjv.cars.model.Engine;
import cz.cvut.fel.pjv.cars.model.ServiceBook;


public class Car {
    private final String manufacturer;
    private final String modelName;
    private final int  year;
    private  final UUID vinCode;
    private Engine engine;
    private ServiceBook serviceBook;
    private int speed;

    private static int counter = 0;

    public Car(String manufacturer, String modelName, int year, String engineType, int speed) {
        this.manufacturer = manufacturer;
        this.modelName = modelName;
        this.year = year;
        vinCode = UUID.randomUUID();
        engine = new Engine(engineType);
        this.speed = speed;
        //serviceBook = new ServiceBook(this);
        counter++;
    }


    public String getManufacturer() {
        return manufacturer;
    }

    public String getModelName() {
        return modelName;
    }

    public ServiceBook getServiceBook() {
        return serviceBook;
    }

    public static int getNumberOfExistingCars(){
        return counter;
    }

    public int getSpeed() {
        return speed;
    }

    public void setServiceBook(ServiceBook serviceBook){
        this.serviceBook = serviceBook;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Car car = (Car) o;
        return Objects.equals(vinCode, car.vinCode);
    }

    @Override
    public int hashCode() {
        return Objects.hash(vinCode);
    }

    @Override
    public String toString() {
        return manufacturer + " " + modelName + " year " + year + " VIN:" + vinCode;
    }
}
