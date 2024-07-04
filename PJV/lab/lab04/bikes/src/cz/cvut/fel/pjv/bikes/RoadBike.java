package cz.cvut.fel.pjv.bikes;

public class RoadBike extends  Bicycle{
    private int tireWidth;

    public RoadBike(int cadence, int speed, int gear, Color color, int tireWidth) {
        super(cadence, speed, gear, color);
        this.tireWidth = tireWidth;
    }

    @Override
    public void visit(BasicService service) {
        service.accept(this);
    }

    @Override
    public void printDescription() {
        System.out.println("Bike is in gear " + gear + " with a cadence of " + cadence
                + " and travelling at a speed of " + speed + " tire width " + tireWidth + " color " + color.name +".");
    }
}
