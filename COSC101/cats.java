public class cats {
    //member variables
    //typically member variables are private for data integrity
    //can be accessed by public methods
    private int age;
    private String name;
    private double weight;
    private String owner;

    //constructors
    //we overload the constructor because we have multiple ways to call it
    //if we no constructor java makes one for us
    //It would look like ---------> public cats(){}
    //this will carve a space for memory for the created object to have the member variables 
    public cats(){
        System.out.println("Meow.");
    }

    public cats(int defaultAge, String name, double weight, String owner){

    }
    //this.name removes the naming conflict between parameter and variable
    public cats(String name){
        name = this.name;
    }
    public static void main(String[] args){
        cats sushi = new cats();
    }
}
