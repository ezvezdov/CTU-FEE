package cz.cvut.fel.pjv;

public class Lab02 {

   public void start(String[] args) {
      homework();
   }

   private double findAverage(int n, double sum){
      return sum / n;
   }
   private double findDeviation(int n, double sum, double average, double sumSquare){
      return  Math.sqrt( (double) 1/n * (sumSquare + (-2 * sum * average) + ( n * Math.pow(average,2))));
   }

   private void printInterval(int n, double average, double deviation){
      System.out.format("%2d %.3f %.3f%n",n,average,deviation);
   }

   public void homework(){
      TextIO io = new TextIO();
      String in = io.getLine();

      double sum = 0;
      double average,deviation, sumSquare = 0;
      int linesCount = 1,intervalCount = 0;

      while (!in.equals("")){
         if(TextIO.isInteger(in)){
            sum += Integer.parseInt(in);
            sumSquare += Math.pow(Integer.parseInt(in),2);
         }
         else if(TextIO.isFloat(in)){
            sum += Float.parseFloat(in);
            sumSquare += Math.pow(Float.parseFloat(in),2);
         }
         else if(TextIO.isDouble(in)){
            sum += Double.parseDouble(in);
            sumSquare += Math.pow(Double.parseDouble(in),2);
         }
         else{
            System.err.println("A number has not been parsed from line " + linesCount);
            linesCount++;
            in = io.getLine();
            continue;
         }
         intervalCount++;
         linesCount++;

         if(intervalCount == 10){
            average = findAverage(intervalCount,sum);
            deviation = findDeviation(intervalCount,sum,average, sumSquare);
            printInterval(intervalCount,average,deviation);
            sum = 0;
            sumSquare = 0;
            intervalCount = 0;
         }
         in = io.getLine();

      }
      System.err.println("End of input detected!");
      if(intervalCount > 1){
         average = findAverage(intervalCount,sum);
         deviation = findDeviation(intervalCount,sum,average,sumSquare);
         printInterval(intervalCount,average,deviation);
      }


   }
}
