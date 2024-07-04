package cz.cvut.fel.pjv;

import java.io.*;
import java.util.Scanner;

public class WordCounter {

    public void countWords(String inFileName) {
        try (
                Scanner sc = new Scanner(new File(inFileName));
        ) {
            int counter = 0;
            for (; sc.hasNext();) {
                sc.next();
                counter++;
            }
            System.out.println(counter);
        } catch (FileNotFoundException e) {
            System.err.println("Problems with file i/o " + e.getMessage() );
        }
    }
}
