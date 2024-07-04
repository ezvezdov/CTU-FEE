package cz.cvut.fel.pjv.bikes;

public enum Color {
    RED("crimson", 985),
    GREEN("grass",654),
    BLUE("sky",112);

    public final String name;
    public final int id;

    Color(String name, int id) {
        this.name = name;
        this.id = id;
    }


}
