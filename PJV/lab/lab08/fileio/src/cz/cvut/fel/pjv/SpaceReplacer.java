package cz.cvut.fel.pjv;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

public class SpaceReplacer {

    public void replaceSpaces(String inFileName, String outFileName) {
        try (
                FileReader fr = new FileReader(inFileName);
                FileWriter fw = new FileWriter(outFileName);
        ) {
            boolean wasWhite = false;
            for (int data; (data = fr.read()) != -1;) {
                if (!Character.isWhitespace(data)) {
                    if (wasWhite){
                        fw.write(" ");
                        wasWhite = false;
                    }
                    fw.write(data);
                } else {
                    wasWhite = true;
                }

            }
        } catch (IOException e) {
            System.err.println("Problems with file i/o " + e.getMessage() );
        }
    }
}
