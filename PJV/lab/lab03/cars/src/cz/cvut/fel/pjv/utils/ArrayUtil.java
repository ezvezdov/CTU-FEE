package cz.cvut.fel.pjv.utils;

public class ArrayUtil {
    private ArrayUtil(){}

    public static void sort(Comparable[] data){
        for(int i = 0; i < data.length; i++){
            for(int j = 0; j < data.length - 1 - i; j++){
                if(data[j].compareTo(data[j+1]) > 0){
                    Comparable tmp = data[j];
                    data[j] = data[j+1];
                    data[j+1] = tmp;
                }
            }
        }
    }
}
