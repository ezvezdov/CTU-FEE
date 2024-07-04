package cz.cvut.fel.pjv;

import java.util.Scanner;

public class Main {

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        int[] arr = new int[10];
        for (int i = 0; i < arr.length; i++) {
            arr[i] = sc.nextInt();
        }
//        for(int i = 9; i >= 0; i--){
//            System.out.println(arr[i]);
//        }
        for (int i : arr) {
            System.out.println(i);
        }
    }
}
