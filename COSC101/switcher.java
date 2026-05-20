/*this program takes 2 doubles, left and right, and a string, operator
and needs to output a result of left operator right
this can be done with cases for each possible operator that can be input, (+, -, *, /)
will output the result as: left (operator input) right = result
has a default to tell user that the operator input was invalid
Connor Riley, 2/22/22, COSC 101 Section 004
*/
import java.util.Scanner;
class switcher {
   public static void main(String[] args) {
      double left, right;
      String operator;
      Scanner s = new Scanner(System.in);
      double result;//need a result variable to calculate the result and then be used to output it, 
                    //is a double because left and right are doubles and used to calculate result so no compilation error

      System.out.print("Enter <left> <op> <right>: ");
      left = s.nextDouble();//set left equal to s.nextDouble() so a double can be inputted for left 
      operator = s.next();// set operator equal to s.next() so a string can be inputted for operator
      right = s.nextDouble();//set right equal to s.nextDouble() so a double can be inputted for right


      s.close();
     
      switch (operator){//begins switch statement for the possible inputs for operator, result has to be declared in each case for the appropiate operator
         case "+"://used for the case where a "+" is input for operator
            result = left + right;//gets the result variable to be left + right
            System.out.println(left + " + " + right + " = " + result);//used to print out the input of left + the input of right and the result variable
            break;//ends the case if operator = +
         case "-"://used for the case where a "-" is input for operator
            result = left - right;//gets the result variable to be left - right
            System.out.println(left + " - " + right + " = " + result);//used to print out the input of left - the input of right and the result variable
            break;//ends the case if operator = -
         case "*"://used for the case where a "*" is input for operator
            result = left * right;//gets the result variable to be left * right
            System.out.println(left + " * " + right + " = " + result);//used to print out the input of left * the input of right and the result variable
            break;//ends the case if operator = *
         case "/"://used for the case where a "/" is input for operator
            result = left / right;//gets the result variable to be left / right
            System.out.println(left + " / " + right + " = " + result);//used to print out the input of left / the input of right and the result variable
            break;//ends the case if operator = /
         default://used for case where the operator input does not match any of the above cases 
            System.out.println("You have entered an invalid opertor, try again with +, -, *, or /");//prints out why there is no result 
            break;//ends the default case.
         }

   }
}