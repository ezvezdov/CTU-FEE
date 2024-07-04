package cz.cvut.fel.pjv.contacts;

public class Contact {
    private String name;
    private String surname;
    private int birthYear;

    public Contact(String name, String surname, int birthYear) throws IncorrectNameException, IncorrectBirthYear{
        if(name.equals("")) {
            throw new IncorrectNameException("Empty name!");
        }
        this.name = name;
        if(surname.equals("")){
            throw new IncorrectNameException("Empty surname!");
        }
        this.surname = surname;
        if(birthYear < 1850){
            throw new IncorrectBirthYear("Invalid birth year!");
        }
        this.birthYear = birthYear;

    }

    @Override
    public String toString() {
        return "Contact{" +
                "name='" + name + '\'' +
                ", surname='" + surname + '\'' +
                ", birthYear=" + birthYear +
                '}';
    }
}
