//print vs format
// print uses plus sign for strings and variables
//we use specific symbols to denote variable will go
//we have to tell java what type of variable they are


import java.util.Scanner;
class FancyOutput {
    public static void main(String[] args){
        String first, last;
        double gpa; 
        Scanner myInput = new Scanner(System.in);

        System.out.println("What is your first name?");
        first = myInput.next();
        System.out.println("What is your last name?");
        last = myInput.next();
        System.out.println("What is your GPA to 4 deciaml places?");
        gpa = myInput.nextDouble();

        /*Inside (), first thing to write is formatted string in quotes
        Including special varaibls for having varaibles in a string
        After the string in quotes we list variable use in order
        ("formatted string", varaible1, variable2, variable3)
        double pi = 3.14
        ("Hello my number is %.2f and my name is %s", pi, name)
        f is float, used with doubles
        d is for decimal, used with ints
        s is for strings
        n is for new line
        */
        System.out.format("First: %s Last: %s GPA: %f%n", first, last, gpa);
        System.out.println("First: " + first + " Last: " + last + " GPA: " + gpa);
        System.out.format("First: %s Last: %s GPA: %.2f%n", first, last, gpa);
        System.out.format("First: %s Last: %s GPA: %10.2f%n", first, last, gpa);
        //%_ _____ . ____ f the minus sign, character width, a period, sig dig
        //%_ _____ s first blank is where the minus sign to determine left or right, second is charc width
        //%_ _____ d first blank is where the minus sign to determine left or right, second is charc width

        int i = 10;
        int j = 20;
        //i + j goes to %d as i + j is an int
        System.out.format("i + j = %d%n", i + j);// print i + j = 30
        System.out.format("%d + %d = %d%n", i, j,  i + j);// print 10 + 20 = 30
        //pad with zeroes %010.2, add a 0 before first number. will have 10 char including the . and the .2 from decimal
        System.out.format("First: %s Last: %s GPA: %010.2f%n", first, last, gpa);
        // pad with space on strings via %-(#of chars)s ie %-10s
        System.out.format("First: %-10s Last: %-10s GPA: %010.2f%n", first, last, gpa); 
        first = "JC";
        last = "Smith";
        System.out.format("First: %-10s Last: %-10s GPA: %010.2f%n", first, last, gpa); 
        first = "John";
        last = "Adams";
        System.out.format("First: %-10s Last: %-10s GPA: %010.2f%n", first, last, gpa); 
        //format makes neat columns 

        String sentence;
        sentence = String.format("My name is %s", first);
        System.out.println(sentence);


        double a = 9.72;
        double b = 3.00;
        int c = (int)(a + b);
        System.out.println(c);

        int y = 48 - 4 * 2 / 4 % 4;
        System.out.println(y);

        int p = 49 + 4 - 3 + 3 % 3;
        System.out.println(p);
    }

}
