package cz.cvut.fel.pjv.cars.data;

import java.util.Objects;

public class TimeSpan {
    private int hour = 0;
    private int minute = 0;
    private int second = 0;

    public TimeSpan() {}

    public TimeSpan(int second) {
        this.second = second % 60;
        this.minute = (int)(second / 60) % 60;
        this.hour = (int)(this.minute / 60) % 60;
    }

    public TimeSpan(int minute, int second) {
        this.minute = minute % 60;
        this.hour = second / 60;
        this.second = (second >= 60) ? 0: second;
    }

    public TimeSpan(int hour, int minute, int second) {
        setTime(hour, minute, second);
    }

    public void setTime(int hour, int minute, int second){
        this.hour = hour;
        this.minute = (minute >= 60) ? 0: minute;
        this.second = (second >= 60) ? 0: second;
    }

    public int getTotalSeconds(){
        return this.hour * (60 * 60) + this.minute * 60 + this.second;
    }

    private void normalize(){
        this.second = second % 60;
        this.minute = (int)(second / 60) % 60;
        this.hour = (int)(this.minute / 60) % 60;
    }

    public TimeSpan add(int second){
        this.second += second;

        return this;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        TimeSpan timeSpan = (TimeSpan) o;
        return hour == timeSpan.hour && minute == timeSpan.minute && second == timeSpan.second;
    }

    @Override
    public int hashCode() {
        return Objects.hash(hour, minute, second);
    }

    @Override
    public String toString() {
        return "TimeSpan{" +
                "hour=" + hour +
                ", minute=" + minute +
                ", second=" + second +
                '}';
    }
}
