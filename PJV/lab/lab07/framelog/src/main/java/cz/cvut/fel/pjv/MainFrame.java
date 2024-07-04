package cz.cvut.fel.pjv;

import javax.swing.*;
import java.awt.*;
import java.util.logging.Level;
import java.util.logging.Logger;

public class MainFrame extends JFrame {

    Logger ROOT_LOGGER = Logger.getLogger("");

    public MainFrame() throws HeadlessException {

        ROOT_LOGGER.setLevel(Level.ALL);
        ROOT_LOGGER.getHandlers()[0].setLevel(Level.ALL);


        setSize(300,300);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                new MainFrame().setVisible(true);
            }
        });
    }
}
