package cz.cvut.fel.pjv;

public class Main {
    public static void main(String[] args) throws InterruptedException {
        // A.
//        MyThread myThread0 = new MyThread();
//        MyThread myThread1 = new MyThread();
//        MyThread myThread2 = new MyThread();
//        myThread0.start();
//        myThread1.start();
//        myThread2.start();

        // B.
        MyRunnable myRunnable = new MyRunnable();
        Thread thread0 = new Thread(myRunnable);
        Thread thread1 = new Thread(myRunnable);
        Thread thread2 = new Thread(myRunnable);
        thread0.start();
        thread1.start();
        thread2.start();

        thread1.interrupt();

        thread0.join();

        System.out.println(Thread.currentThread().getName() +  " Finished");
    }
}
