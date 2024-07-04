package cz.cvut.fel.pjv;

public class Start {
    public static void main(String[] args) {
        // 1.
//        SpaceReplacer sr = new SpaceReplacer();
//        sr.replaceSpaces("input.txt", "output.txt");
        // 2.
//        LineCounter lc = new LineCounter();
//        lc.countLines("input.txt", "outputcnt.txt");
        // 3.
//        WordCounter wc = new WordCounter();
//        wc.countWords("input.txt");
        // 4.
        NumberWriter nw = new NumberWriter();
        nw.writeNumbers("numbers.bin");
    }
}
