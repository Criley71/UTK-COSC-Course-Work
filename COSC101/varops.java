//This program takes 3 double inputs and stores them as varaibles a, b and c 
//Then calculates a / b + c and stores it as double variable "result"
//Then outputs the result showing the doubles a, b and c were assigned to and the equation being equal to the result 
//each variable being show to 2 decimal places in the output using system.format
// Connor Riley, 2/8/22 - 2/11/22, COSC 101 Section 4
import java.util.Scanner;
class varops {
   public static void main(String[] args) {
      double a;// declares variable "a" as a double 
      double b;// declares variable "b" as a double
      double c;// declares variable "c" as a double
      Scanner s = new Scanner(System.in); // allows user to input a value via s.next and the type of data allowed to be input. s.nextInt() allows an integer input
      System.out.print("Enter three doubles (a / b + c): ");//outputs string telling the user to input 3 doubles for the equation
      a = s.nextDouble();// allows the user to input a double value for variable a
      b = s.nextDouble();// allows the user to input a double value for variable b
      c = s.nextDouble();// allows the user to input a double value for variable c
      s.close();//closes the scanner
      double result = a / b + c; // calculates a / b + c as a double and stores it as the variable result
      System.out.format("The result of %.2f / %.2f + %.2f = %.2f", a, b, c, result);//prints the variables a, b, c and result variables to 2 decimal places
   }
}