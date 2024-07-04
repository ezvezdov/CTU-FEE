package cz.cvut.fel.pjv;

import java.io.Serializable;

public class Employee implements Serializable {
    private final String jmeno;
    private final String funkce;
    private final String cislDveri;
    private final String telefon;

    public Employee(String jmeno, String funkce, String cislDveri, String telefon) {
        this.jmeno = jmeno;
        this.funkce = funkce;
        this.cislDveri = cislDveri;
        this.telefon = telefon;
    }

    @Override
    public String toString() {
        return "Employee{" +
                "jmeno='" + jmeno + '\'' +
                ", funkce='" + funkce + '\'' +
                ", cislDveri='" + cislDveri + '\'' +
                ", telefon='" + telefon + '\'' +
                '}';
    }
}
