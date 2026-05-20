
import java.util.Scanner;
class review1{
    public static void main(String[] args){
        //This program will ask for data about a dog from the user including:
        //first and last name of owner, pets name, weight, last time medicated, pets age
        //we will output calculations and confirmation from the user.
        String first, last;//string is capitalized
        double weightLBS, weightKGS; //double is not capitalized
        Scanner myInput = new scanner(System.in);
        int medsAm, medsPm;
        String petName;


        System.out.print("Welcome to the database");
        System.out.print("Please enter your first and last name(if either are 2 words please use '_'): ");
        first = myInput.next();
        last = myInput.next();
        System.out.println("F: " + first + " L: " + last);
        System.out.println("please enter pet's name (use'_' for two words in a name): ");
        petName = myInput.next();
        System.out.println("Please enter pet's weight in pounds: ");
        weightLBS = myInput.nextDouble();
        System.out.println("Please enter last AM medication (on the hour): ");
        medsAm = myInput.nextInt();

        //calcutlation section
        //medsAm is 8, then i want to give it 10 hours later, 6pm
        //8 + 10 = 18... 18 % 12 is 6. can use modelu for this.

        medsPm = (medsAm + 10) % 12;
        System.out.println(medsPm);
        weightKGS = weightLBS/2.2046;
        
        
        //formatting the output
        //pets name : name
        //take med at ____ PM
        System.out.format("Pet's name %s\n", petName);
        System.out.format("Take weight in KGs: %.2f\n", weightKGS);
        System.out.format("Take medication at %dPM\n", medsPm);
    }
}
