package cz.cvut.fel.pjv.cars;

import cz.cvut.fel.pjv.cars.model.Car;
import cz.cvut.fel.pjv.cars.model.ServiceBook;

public class Main {

    public static void main(String[] args) {
        Car car1 = new Car("Volkswagen", "Polo", 2010, "AKK", 40);
        Car car2 = new Car("Chevrolet", "Corvette", 1980, "LS7", 45);

        Car car3 = new Car("Trabant", "P601", 1990, "Air cooled, 0.6-liter 2-stroke", 20);
        Car car4 = new Car("BMW", "3", 2006, "318d", 42);
        Car car5 = new Car("McLaren", "F1", 2014, "V12", 107);
        Race race = new Race(1000);
        race.addRacingCar(car1);
        race.addRacingCar(car2);
        race.addRacingCar(car3);
        race.addRacingCar(car4);
        race.addRacingCar(car5);
        System.out.println("Závodníci:");
        System.out.println(race);
        System.out.printf("Výhercem se stává %s s časem %s.\n", race.getWinner(), race.getWinningTime());
        System.out.printf("Pořadí v cíli:\n%s\n", race);
    }
}
