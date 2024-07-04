package cz.cvut.fel.pjv;

import static java.lang.Thread.sleep;

public class MyRunnable implements Runnable {
    private int counter = 0;
    private Object lock = new Object();

    @Override
    public void run() {
        for (int i = 0; i < 10; i++) {
            synchronized (lock) {
                System.out.println(Thread.currentThread().getName() + " "  + i + " " + counter);
                counter++;
            }
            try {
                sleep(100);
            } catch (InterruptedException e) {
                System.out.println(Thread.currentThread().getName() + " interrupted");
                break;
            }
        }
    }
}
