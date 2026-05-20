/**
 * forloops.java
 * repeatedly asks user for sum or product
 * then asks for how many numbers will the user input
 * program will use for loops for each value to be inputted and then 
 * output the product or sum to 3 decimal places
 * Connor Rilet 3/29/2022
 */
import java.util.Scanner;
class forloops {
   public static void main(String[] args) {
      String input;
      int n;
      Scanner s = new Scanner(System.in);

      
      /**
       * to keep prompting the user we can use a while loop
       * set it to true and it will only stop with a break
       */
      while(true){ 
      System.out.print("Sum or product (type 'quit' to quit): ");
      input = s.next();
      if (input.equals("quit")) {
         break;
      }
      else if (!input.equals("sum") && !input.equals("product")) {
         System.out.println("Invalid operation, try again.");
         continue;
      }
      System.out.print("How many values? ");
      n = s.nextInt();
      if (input.equals("sum")) {
         /**
          * need to use a for loop to get each value, make for loop run until
          input is no longer less than n to get each value. make a double value and sum
          sum is set to zero prior to the for loop so it isnt reset.
          value will be the user inputted numbers to be added.
          inside the for loop sum is equal to itself + value to keep adding the new value to the past values
          */
         
        double sum = 0;
        for(int i = 0; i < n; i++){
            double value;
            System.out.print("Enter value #" + i + ": ");
            value = s.nextDouble();
            sum = sum + value;
        }
        System.out.format("Sum = %.3f%n", sum);
         /* use a formatted output for sum to be to 3 decimal digits. */
      }
      else {
         /* same setup as the sum for loop.
         difference is make a double product = 1 as in initialized at zero the output will be zero
         product inside the loop is equal to itself * the entered value that loop
         */
        double product = 1;
        for(int i = 0; i < n; i++){
            double value;
            System.out.print("Enter value #" + i + ": ");
            value = s.nextDouble();
            product = product * value;
        }
        System.out.format("Product = %.3f%n", product);
        /* use a formatted output for product to be to 3 decimal digits. */
      }
    }
      s.close();
   }
}