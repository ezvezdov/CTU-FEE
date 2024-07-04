package cz.cvut.fel.pjv;

import java.io.*;
import java.util.Scanner;

public class NumberWriter {

    public void writeNumbers(String inFileName) {
        try (
                Scanner sc = new Scanner(System.in);
                DataOutputStream dos = new DataOutputStream(new FileOutputStream(inFileName));
        ) {
            int number = 0;
            for (; (number = sc.nextInt()) != -1;) {
                dos.writeInt(number);
            }
        } catch (IOException e) {
            System.err.println("Problems with file i/o " + e.getMessage() );
        }
    }
}
