package cz.cvut.fel.pjv.bikes;

public class BasicService {
    void accept(Bicycle bike){
        System.out.println("fixing Bicycle");
    }
    void accept(RoadBike bike){
        System.out.println("can`t fix");
    }
    void accept(MountainBike bike){
        System.out.println("can`t fix");
    }
}
