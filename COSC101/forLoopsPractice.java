import java.util.Scanner;
class forLoopsPractice{
    public static void main(String[] args){
        /*for loops have 4 parts
            used to count up to a value until a condition is met
        for(part1; part2; part3){
            part4
        }
        part1 initializer, set counter to initial value
        part2 condition, checks if loop can run
        part3 increment the step/ counter
        part4 code to be executed
        
        int numOfPlayers = 3;

        for (int i = 0; i < numOfPlayers; i++){
            System.out.println(i);
        }*/
        String street, petsName, middle;
        int numOfPlayers; 
        Scanner myInput = new Scanner(System.in);

        System.out.print("How many people? ");
        numOfPlayers = myInput.nextInt();

        for(int i = 0; i < numOfPlayers; i++){
            System.out.println("Player " + (i + 1) + ":");
            System.out.print("      Street?: ");
            street = myInput.next();
            System.out.print("      Pet?: ");
            petsName = myInput.next();
            System.out.print("      middle?");
            middle = myInput.next();
            System.out.println("Your movie star name is " + petsName + " " + street + " " + middle + ".");
        }

        System.out.println("Thanks for playing");


    }
}