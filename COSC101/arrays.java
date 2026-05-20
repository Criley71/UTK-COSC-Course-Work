/**
 * arrays.java
 * asks the user for matrix values and vector values.
 * will multiply the vector by the matrix and produce a 
 * vector as the result
 * Connor Riley 5/1/2022
 */
import java.util.Scanner;
class arrays {
    private final static int VECTOR_SIZE = 3;
    private final static int MATRIX_SIZE = VECTOR_SIZE * VECTOR_SIZE;
    public static void main(String[] args) {
        /**
         * wee need an array to store the matrix and vectors value from the user
         */
        double[] matrix = new double[MATRIX_SIZE];

        double[] vector = new double[VECTOR_SIZE];
              
        Scanner s = new Scanner(System.in);

        /**
         * we need a for loop to ask the user for a double value for each position of the 
         * corresponding array. will only allow number inputs for size the array.
         * do this for the matrix array then the vector array once the matrix array is full
         */
        System.out.format("Enter %d matrix values: ", MATRIX_SIZE);
                for(int i = 0; i < MATRIX_SIZE; i++){
                    matrix[i] = s.nextDouble();
                }

        System.out.format("Enter %d vector values: ", VECTOR_SIZE);  
                for(int i = 0; i < VECTOR_SIZE; i++){
                    vector[i] = s.nextDouble();
                }

        s.close();
        /**
         * we need an array to store the result of the multiply method.
         * make the result[] array and set it equal to the return of multiply method.
         */
        double[] result = multiply(matrix, vector);
        System.out.print("Result = [");
        printVector(result);
        System.out.println("]");
    }
    private static void printVector(double[] arr) {
        /**
         * we need the result needs to be printed as [1.000 2.000 3.000]
         * we need to have the last value to not have a space after it.
         * we also need the results to have a precision of 3 decimal places.
         * We can loop through the array until and not including the last value and
         * print using format to 3 decimal places. to print the final value we use format 
         * again once the loop completes and not include a space after.
         */
            for(int i = 0; i < VECTOR_SIZE - 1;i++){
                System.out.format("%.3f ", arr[i]);
            }
            System.out.format("%.3f", arr[VECTOR_SIZE - 1]);
    }
    private static double[] multiply(double[] matrix, double[] vector) {
        /**
         * the multiply method returns a double array so I create a double array "result"
         * we set it equal to the size of the vector as it the same size as the result array
         * this will be the return value.
         * we then need to calculate the result
         * we use 2 for loops. the outer determines the result position and the matrix position.
         * the inner determines the vector position and the matrix position as well.
         * 1st loop result[0] = matrix[0, 1, 2] * vector[0, 1, 2]
         * 2nd loop result[1] = matrix[3, 4, 5] * vector[0, 1, 2] 
         * 3rd loop result[2] = matrix[6, 7, 8] * vector[0, 1, 2]
         */
            double[] result = new double[VECTOR_SIZE];
        /* TODO: Create a new array called result that stores VECTOR_SIZE number
                 of values. This will be the result you return. */
            for(int i = 0; i < VECTOR_SIZE; i++){
                for(int j = 0; j < VECTOR_SIZE ; j++){
                    result[i] +=  matrix[j + (i * VECTOR_SIZE)] * vector[j];
                }
            }
                 return result;
    }
}