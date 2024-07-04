package cz.cvut.fel.pjv.contacts;

import java.util.ArrayList;
import java.util.List;

public class ContactList {
    List<Contact> contacts = new ArrayList(); //zleva proste List, abychom mohli zmennit na LinkedList napr

    public void addContact(Contact contact){
        contacts.add(contact);
    }

    @Override
    public String toString() {
        String ret = "";
        for(Contact contact: contacts){
            ret += contact + "\n";
        }
        return ret;
    }
}
