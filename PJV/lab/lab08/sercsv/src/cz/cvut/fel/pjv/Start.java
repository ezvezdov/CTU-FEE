package cz.cvut.fel.pjv;

import java.io.*;
import java.util.ArrayList;
import java.util.List;

public class Start {

    List<Employee> employees = new ArrayList<>();

    private void readCsv(String inFileName) {
        try (
                BufferedReader br = new BufferedReader(new FileReader(inFileName));
        ) {
            for (String line; (line = br.readLine()) != null;) {
                String[] items = line.split(";");
                employees.add(new Employee(items[0], items[1], items[3], items[3]));
            }
        } catch (IOException e) {
            System.err.println("Problems with file i/o " + e.getMessage() );
        }
    }


    private void seralizeCsv(String outFileName) {
        try (
                ObjectOutputStream ous = new ObjectOutputStream(new FileOutputStream(outFileName));
        ) {
            ous.writeObject(employees);
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    private void deseralizeCsv(String inFileName) {
        try (
                ObjectInputStream ois = new ObjectInputStream(new FileInputStream(inFileName));
        ) {
            List<Employee> tmpEmployees = new ArrayList<>();
            tmpEmployees = (List<Employee>)ois.readObject();
            for (Employee e : tmpEmployees) {
                System.out.println(e);
            }
        } catch (IOException | ClassNotFoundException e) {
            e.printStackTrace();
        }
    }


    public static void main(String[] args) {
        Start start = new Start();
        start.readCsv("tel_seznam_KUp_2.csv");
        start.seralizeCsv("tel_seznam_KUp_2.bin");
        start.deseralizeCsv("tel_seznam_KUp_2.bin");
    }
}
