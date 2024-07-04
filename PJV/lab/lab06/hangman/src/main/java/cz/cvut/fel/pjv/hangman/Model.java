package cz.cvut.fel.pjv.hangman;

public class Model {
    private final int MAX_MISS = 6;

    private boolean finished = false;
    private int missCount = 0;
    private String hiddenWord;
    private String guessedWord;

    public String initGuessedWord(WordSource rvs) {
        hiddenWord = rvs.getWord();
        guessedWord = "";
        for(int i = 0; i < hiddenWord.length(); i++) {
            guessedWord += "*";
        }
        return guessedWord;
    }

    public String updateGuessedWord(char guess) {
        String tmp = "";
        for (int i = 0; i < hiddenWord.length(); i++) {
            if (hiddenWord.charAt(i) == guess) {
                tmp += guess;
            } else {
                tmp += guessedWord.charAt(i);
            }
        }
        guessedWord = tmp;
        return guessedWord;
    }
}
