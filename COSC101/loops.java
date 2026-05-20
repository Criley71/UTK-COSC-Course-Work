/*loops.java
repeatedly asks for an input from the user for an exponent 
depending if the user inputs a double, "quit" or not a number.
will change the outputted result and quit ends the program
Connor Riley 3/23/22
*/

import java.util.Scanner;
class loops {
   public static void main(String[] args){
      double exponent;
      Scanner s = new Scanner(System.in);

      /*we need the program to repeatedly prompt the user for an input so its in a do while loop.
      with the user's input we check for the type of input via hasNextXXXX() as if else conditions
      */ 
    do{
        System.out.print("Enter exponent: ");
        if(s.hasNextDouble()){
            exponent =  s.nextDouble();
            double result = Math.exp(exponent);
            System.out.format("Result = %.4f \n", result);
        }else if(s.hasNext("quit")){
            break;
        }else{
            System.out.println("Invalid Input.");
            s.next();
        }

    }while(true);
          /*the input decides the output and continuation of the program. 
          a number gives e to the exponent entered,
          "quit" ends the program, 
          anything else gives an invalid input 
          */
        s.close();
    

    }
}