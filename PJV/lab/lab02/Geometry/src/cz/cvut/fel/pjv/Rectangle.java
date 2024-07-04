package cz.cvut.fel.pjv;

import static java.lang.Math.PI;

public class Rectangle extends Shape2D {
    private double width;
    private double height;

    public Rectangle(double width, double height) {
        this.width = width;
        this.height = height;
    }

    public double getWidth() {
        return width;
    }

    public double getHeight() {
        return height;
    }

    public void setWidth(double width) {
        this.width = width;
    }

    public void setHeight(double height) {
        this.height = height;
    }

    @Override
    public double getArea(){
        return  width * height ;
    }

    @Override
    public double getCircumference(){
        return 2 * (width + height);
    }
}
