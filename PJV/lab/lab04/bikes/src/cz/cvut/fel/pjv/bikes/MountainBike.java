package cz.cvut.fel.pjv.bikes;

public class MountainBike extends  Bicycle{
    private String suspension;

    public MountainBike(int cadence, int speed, int gear, Color color, String suspension) {
        super(cadence, speed, gear, color);
        this.suspension = suspension;
    }

    @Override
    public void visit(BasicService service) {
        service.accept(this);
    }

    @Override
    public void printDescription() {
        System.out.println("Bike is in gear " + gear + " with a cadence of " + cadence
                + " and travelling at a speed of " + speed + " suspension " + suspension + " color " + color.name +"." );
    }
}
