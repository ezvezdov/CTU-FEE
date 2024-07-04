package cz.cvut.fel.pjv.cars;

import cz.cvut.fel.pjv.cars.data.CarLinkedList;
import cz.cvut.fel.pjv.cars.data.TimeSpan;
import cz.cvut.fel.pjv.cars.model.Car;
import cz.cvut.fel.pjv.cars.model.RaceResult;
import cz.cvut.fel.pjv.utils.ArrayUtil;

import java.sql.Time;
import java.util.Arrays;

public class Race {
    private int length;
    private CarLinkedList cars;
    private boolean started;
    private RaceResult[] results;

    public Race(int length) {
        this.length = length;
        cars = new CarLinkedList();
        results = new RaceResult[length];
    }
    public boolean addRacingCar(Car car){
        if(!started){
            cars.add(car);
            return true;
        }
        return false;
    }

    public Car getWinner(){
        getRaceResults();
        return  results[0].getCar();
    }

    public TimeSpan getWinningTime(){
        getRaceResults();
        return results[0].getTimeSpan();
    }

    public RaceResult[] getRaceResults(){
        if(!started) {
            started = true;

            Car[] carsArray = cars.toArray();
            for (int i = 0; i < carsArray.length; i++) {
                results[i] = new RaceResult(carsArray[i], new TimeSpan(length / carsArray[i].getSpeed()));
            }
            ArrayUtil.sort(results);
        }
        return results;
    }

    @Override
    public String toString() {
        String str = "";
        if(started){
            for (int i = 0; i < results.length; i++) {
                str += results[i].getCar().toString() + "\n";
            }
        }
        else{
            Car[] carsArray = cars.toArray();
            for (int i = 0; i < carsArray.length; i++) {
                str+= carsArray[i].toString() + "\n";
            }
        }
        return str;
    }
}
