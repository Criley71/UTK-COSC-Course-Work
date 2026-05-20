/* greeting.java
This program takes the inputs of the user's first and last name and greets them using there name
Connor Riley
Febuary 2nd, 2022
*/

import java.util.Scanner;
class greeting {
   public static void main(String[] args) {
      String first;
      String last;
      Scanner s = new Scanner(System.in);
      System.out.print("What is your first name? ");
      first = s.next();
      System.out.println("What is your last name? ");
      last = s.next();
      System.out.println("Hello " + first + " " + last + "! It's great to meet you.");
      s.close();
   }    
}


