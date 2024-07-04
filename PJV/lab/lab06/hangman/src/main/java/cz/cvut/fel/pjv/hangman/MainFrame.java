package cz.cvut.fel.pjv.hangman;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.io.IOException;

public class MainFrame extends JFrame implements ActionListener {

    private final String LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    Model model = new Model();

    private final JLabel imageLabel;
    private final JLabel guessedWordLabel;

    public MainFrame() throws HeadlessException {
        Container cp = getContentPane();
        cp.setLayout(new BoxLayout(cp, BoxLayout.Y_AXIS));
        JPanel upperPart = new JPanel();
        imageLabel = new JLabel();
        updateImageLabel(0);
        upperPart.add(imageLabel);

        JPanel centerPart = new JPanel();
        guessedWordLabel = new JLabel();
        guessedWordLabel.setFont(guessedWordLabel.getFont().deriveFont(64.0f));
        guessedWordLabel.setText(model.initGuessedWord((WordSource) new RandomWordSource()));
        centerPart.add(guessedWordLabel);

        JPanel lowerPart = new JPanel();
        for (char c : LETTERS.toCharArray()) {
            JButton b = new JButton(String.valueOf(c));
            b.addActionListener(this);
            lowerPart.add(b);
        }

        add(upperPart);
        add(centerPart);
        add(lowerPart);

        setTitle("Hangman");
        setSize(800, 600);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }

    private void updateImageLabel(int count) {
        try {
            BufferedImage image = ImageIO.read(getClass().getResource("/hangman_" + count + ".png"));
            imageLabel.setIcon(new ImageIcon(image));
        } catch (IllegalArgumentException | IOException ex) {
            System.err.println("Can't load resource: " + ex.getMessage());
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                new MainFrame().setVisible(true);
            }
        });
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        ((JButton) e.getSource()).setEnabled(false);
        char guess = e.getActionCommand().charAt(0);
        guessedWordLabel.setText(model.updateGuessedWord(guess));
    }
}
