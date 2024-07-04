package cz.cvut.fel.pjv.bikes;

public class TestBikes {
    public static void main(String[] args) {
        Bicycle bike01;
        MountainBike bike02;
        RoadBike bike03;

        bike01 = new Bicycle(20, 10, 1, Color.RED);
        bike02 = new MountainBike(20, 10, 5, Color.GREEN,"Dual");
        bike03 = new RoadBike(40, 20, 8,Color.BLUE,23);

        bike01.printDescription();
        bike02.printDescription();
        bike03.printDescription();
    }
}
