/*classes.java
 * repeatedly asks user for an integer for calculations
 * program uses methods to find the number of values inputted, 
 * the sum, product, max and minimum values, and the average
 * Connor Riley 4/10/2022
 */
import java.util.Scanner;
class classes {
   public static void main(String[] args) {
      boolean isrunning = true;
      classes myclass = new classes(); /* we need a new object so we can assign the values to*/
      Scanner s = new Scanner(System.in);
      do {
         do {
            System.out.print("Enter an integer (\"quit\" to quit): ");
            if (!s.hasNextInt()) {
               if (s.next().equals("quit")) {
                  isrunning = false;
                  break;
               }
               System.out.println("You did not enter an integer, try again.");
            }
            else {
               /* We need the inputted number to be added into the myclass object.
               do this by making the input the addValue method parameter
               addValue will calculate the different values needed*/
               int val;
               val = s.nextInt();
               myclass.addValue(val);
               break;
            }
         } while (true);
      } while (isrunning);
      s.close();
      
      /*we need to print out the values and we do this by calling the methods on the myclass object*/
      System.out.format("Number of values entered = %d%n", myclass.getNumValues());
      System.out.format("Sum of all values        = %d%n", myclass.getSum());
      System.out.format("Product of all values    = %d%n", myclass.getProduct());
      System.out.format("Biggest value            = %d%n", myclass.getMax());
      System.out.format("Smallest value           = %d%n", myclass.getMin());
      System.out.format("Average of all values    = %.2f%n", myclass.getAverage());
   }

   public void addValue(int val) {
      /**
       * we will use a method to  calculate all the values to be outputted
       * since the method is called for each input in the loop the numValues is incremented by 1
       * the sum and product is built on the past inputs sum and product
       * the max value is compared to each input to see if the new input is larger
       * the minimum value sets the first input to it but after will be compared to the most recent input
       */
      mNumValues = mNumValues + 1;
      mSumOfValues = mSumOfValues + val; 
      mProductOfValues = mProductOfValues * val;
      if(val > mMaximumValue){
         mMaximumValue = val;
      }
      if(mMinimumValue == 0 && val != 0){
         mMinimumValue = val;
      }else if(mMinimumValue > val){
         mMinimumValue = val;
      }
   }

   public double getAverage() {
      /*the average needs to be a double so the sum and number of inputs is converted to a double
      prior to calculating the average otherwise it will do int division then convert the answer to be a double
      the average will be zero if no integer numbers are entered else it will be calculated */
      double average, dSum, dNumValues;
      dSum = mSumOfValues;
      dNumValues = mNumValues;

      if(mNumValues == 0){
         average = 0.0;
      }else{
         average = dSum / dNumValues;
      }
      return average;
   }

   public classes() {
      /**
       * this will set the initial values for when an object is created
       * these need to be set for addvalue method to calculate the inputs properly
       */
      mNumValues = 0;
      mSumOfValues = 0;
      mProductOfValues = 1;
      mMaximumValue = 0;
      mMinimumValue = 0;
   }

   private int mNumValues;
   private int mSumOfValues;
   private int mProductOfValues;
   private int mMaximumValue;
   private int mMinimumValue;

   public int getSum() {
      return mSumOfValues;
   }

   public int getNumValues() {
      return mNumValues;
   }

   public int getProduct() {
      return mProductOfValues;
   }

   public int getMin() {
      return mMinimumValue;
   }

   public int getMax() {
      return mMaximumValue;
   }
}