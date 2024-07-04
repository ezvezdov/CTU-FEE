package cz.cvut.fel.pjv;

import java.util.Scanner;

public class Lab01 {

   public void ErrorExit(String Message){
      System.out.println(Message);
      System.exit(1);
   }

   public void information_print(String var_name){
      System.out.println("Zadej " + var_name + ": ");
   }
   public char operation_sign(int operation){
      if(operation == 1){return '+';}
      else if(operation == 2){return '-';}
      else if(operation == 3){return '*';}
      else{return '/';} //operation = 4
   }
   public double get_answer(int operation, double a, double b){
      if(operation == 1){return a + b;}
      else if(operation == 2){return a - b;}
      else if(operation == 3){return a * b;}
      else{return  a / b;} //operation = 4
   }

   public void start(String[] args) {
      Scanner sc = new Scanner(System.in);

      int operation, after_point;
      double first_addend = 0, second_addend = 0;

      System.out.println("Vyber operaci (1-soucet, 2-rozdil, 3-soucin, 4-podil):");
      operation = sc.nextInt();

      switch (operation){
         case 1:
            information_print("scitanec");
            first_addend = sc.nextDouble();
            information_print("scitanec");
            second_addend = sc.nextDouble();
            break;
         case 2:
            information_print("mensenec");
            first_addend = sc.nextDouble();
            information_print("mensitel");
            second_addend = sc.nextDouble();
            break;
         case 3:
            information_print("cinitel");
            first_addend = sc.nextDouble();
            information_print("cinitel");
            second_addend = sc.nextDouble();
            break;
         case 4:
            information_print("delenec");
            first_addend = sc.nextDouble();
            information_print("delitel");
            second_addend = sc.nextDouble();
            if(second_addend == 0){
               ErrorExit("Pokus o deleni nulou!");
            }
            break;
         default:
            ErrorExit("Chybna volba!");
      }
      System.out.println("Zadej pocet desetinnych mist: ");
      after_point = sc.nextInt();
      if(after_point < 0){
         ErrorExit("Chyba - musi byt zadane kladne cislo!");
      }

      double answer = get_answer(operation,first_addend,second_addend);
      String format = "%." + after_point + "f";
      System.out.print(String.format(format,first_addend) + " " + operation_sign(operation) + " ");
      System.out.println(String.format(format,second_addend) + " = " + String.format(format,answer)) ;
   }
}
