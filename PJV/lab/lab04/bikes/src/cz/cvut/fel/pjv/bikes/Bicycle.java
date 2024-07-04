package cz.cvut.fel.pjv.bikes;

import javax.xml.namespace.QName;
import java.awt.*;

public class Bicycle {
    protected int cadence;
    protected int speed;
    protected int gear;
    protected Color color;

    public Bicycle(int cadence, int speed, int gear, Color color) {
        this.cadence = cadence;
        this.speed = speed;
        this.gear = gear;
        this.color = color;
    }

    public void visit(BasicService service){
        service.accept(this);
    }

    public void printDescription(){
        System.out.println("Bike is in gear " + gear + " with a cadence of " + cadence
                + " and travelling at a speed of " + speed + " color " + color.name +".");
    }

}
