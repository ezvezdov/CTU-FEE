package cz.cvut.fel.pjv.hangman;

import java.util.Random;

public class MockWordSource implements WordSource{
    private final String word;

    public MockWordSource(String word) {
        this.word = word;
    }

    public String getWord() {
        return word;
    }

}
