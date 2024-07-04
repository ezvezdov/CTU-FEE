package cz.cvut.fel.pjv.cars.model;

import cz.cvut.fel.pjv.cars.data.TimeSpan;

public class RaceResult implements Comparable<RaceResult>{
    private final Car car;
    private final TimeSpan timeSpan;

    public RaceResult(Car car, TimeSpan timeSpan) {
        this.car = car;
        this.timeSpan = timeSpan;
    }

    public Car getCar() {
        return car;
    }

    public TimeSpan getTimeSpan() {
        return timeSpan;
    }

    @Override
    public int compareTo(RaceResult o) {
        return this.timeSpan.getTotalSeconds() - o.timeSpan.getTotalSeconds();
    }
}
