package cz.cvut.fel.pjv;

import static java.lang.Math.PI;

public class Circle extends Shape2D implements Positionable {
    Point point = new Point();
    private double x,y;
    private double radius;

    public Circle(double radius) {
        this.radius = radius;
    }

    public double getRadius() {
        return radius;
    }

    public void setRadius(double radius) {
        this.radius = radius;
    }

    @Override
    public double getArea(){
        return  PI * radius * radius;
    }
    @Override
    public double getCircumference(){
        return 2 * PI * radius;
    }

    public void setPosition(double x, double y){
        point.setX(x);
        point.setY(y);
    }

    public void setX(double x) {
        point.setX(x);
    }

    public void setY(double y) {
        point.setY(y);
    }

    @Override
    public void setPosision(double x, double y) {

    }
}
