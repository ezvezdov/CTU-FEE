package cz.cvut.fel.pjv.hangman;

import java.util.Random;

public class RandomWordSource implements WordSource {
    private final String[] HIDDEN_WORDS = {
            "GHOST", "FLOWER", "BANANA", "SNOWFLAKE", "BOOK", "SNAKE", "LIGHT",
            "TREE", "LIPS", "APPLE", "SLIDE", "SOCKS", "SMILE", "SWING", "COAT",
            "SHOE", "WATER", "HEART", "OCEAN", "KITE", "MOUTH", "MILK", "DUCK",
            "EYES", "SKATEBOARD", "BIRD", "APPLE", "PERSON", "GIRL", "MOUSE",
            "BALL", "HOUSE", "STAR", "NOSE", "WHALE", "JACKET", "SHIRT", "HIPPO",
            "BEACH", "FACE", "COOKIE", "CHEESE", "DRUM", "CIRCLE", "SPOON", "WORM"
    };

    public String getWord() {
        Random random = new Random();
        return HIDDEN_WORDS[random.nextInt(HIDDEN_WORDS.length)];
    }
}
