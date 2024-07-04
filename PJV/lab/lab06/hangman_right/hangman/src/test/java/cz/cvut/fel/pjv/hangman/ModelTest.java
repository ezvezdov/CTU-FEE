package cz.cvut.fel.pjv.hangman;

import static org.junit.jupiter.api.Assertions.*;
class ModelTest {

    @org.junit.jupiter.api.Test
    void testInitGuessedWord() {
        Model model = new Model();
        assertEquals("*****", model.initGuessedWord(new MockWordSource("APPLE")));

    }

    @org.junit.jupiter.api.Test
    void testUpdateGuessedWord() {
        Model model = new Model();
        model.initGuessedWord(new MockWordSource("APPLE"));
        assertEquals("*PP**", model.updateGuessedWord('P'));
    }
}