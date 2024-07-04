package cz.cvut.fel.pjv;

import java.io.*;

public class LineCounter {

    public void countLines(String inFileName, String outFileName) {
        try (
                BufferedReader br = new BufferedReader(new FileReader(inFileName));
                PrintWriter pw = new PrintWriter(new FileWriter(outFileName));
        ) {
            int counter = 0;
            for (String line; (line = br.readLine()) != null;) {
                pw.print(counter++);
                pw.println(line);
            }
        } catch (IOException e) {
            System.err.println("Problems with file i/o " + e.getMessage() );
        }
    }
}
