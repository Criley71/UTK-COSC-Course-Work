import java.util.Scanner;
public class minigolfforloops {
    public static void main(String[] args){
        Scanner scan = new Scanner(System.in);
        int numOfPlayers, score, numOfHoles, currentHole;
        currentHole = 0;
        score = 0;
        numOfHoles = 0;

        //prompts user
        System.out.println("Welcome to mini golf scoring!");
        System.out.print("How many players? "); //2 for testing
        numOfPlayers = scan.nextInt();
        if (numOfPlayers > 0) {
            System.out.print("How many holes are in your course? "); //3 for testing
            numOfHoles = scan.nextInt();
        }

        for(int i = 0; i < numOfPlayers; i++){
            System.out.println("Player " + (i + 1) + ": ");

            for(int j = 0; j < numOfHoles; j++){
                System.out.print("  What is the score for hole number " + (j + 1) + "? ");
                score = score + scan.nextInt();
            }
            
            System.out.println("  Total score is " + score + ".");
            score = 0;

            
        }

    }
}
