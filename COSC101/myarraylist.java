/*
 * This program uses different methods on arrays to 
 * manipulate the array to be able to have the same editing
 * that an arrayList has, such as adding and removing elements
 * Connor Riley 5/10/2022
 */
import java.util.Arrays;
import java.util.Scanner;

class myarraylist {
    public static void main(String[] args) {
        int i;
        int sz;
        double iv;
        myarraylist mal = new myarraylist();
        Scanner s = new Scanner(System.in);
        boolean running = true;

        do {
            System.out.print("Enter command: ");
            switch (s.next()) {
                case "new":
                    if (s.hasNextInt()) {
                        sz = s.nextInt();
                        if (s.hasNextDouble()) {
                            iv = s.nextDouble();
                            mal = new myarraylist(sz, iv);
                        } else {
                            mal = new myarraylist(sz);
                        }
                    } else {
                        mal = new myarraylist();
                    }
                    break;
                case "get":
                    i = s.nextInt();
                    if (i < 0 || i >= mal.size()) {
                        System.out.println("Index " + i + " does not exist.");
                    } else {
                        System.out.println(mal.get(i));
                    }
                    break;
                case "index":
                    iv = s.nextDouble();
                    System.out.println(mal.indexOf(iv));
                    break;
                case "set":
                    i = s.nextInt();
                    iv = s.nextDouble();
                    if (i < 0 || i >= mal.size()) {
                        System.out.println("Index " + i + " does not exist.");
                    } else {
                        mal.set(i, iv);
                    }
                    break;
                case "rem":
                    if (s.hasNextInt()) {
                        i = s.nextInt();
                        if (i < 0 || i >= mal.size()) {
                            System.out.println("Index " + i + " does not exist.");
                        } else {
                            mal.remove(i);
                        }
                    } else {
                        iv = s.nextDouble();
                        mal.remove(iv);
                    }
                    break;
                case "size":
                    System.out.println(mal.size());
                    break;
                case "add":
                    if (s.hasNextInt()) {
                        mal.add(s.nextInt(), s.nextDouble());
                    } else {
                        mal.add(s.nextDouble());
                    }
                    break;
                case "clear":
                    mal.clear();
                    break;
                case "contains":
                    System.out.println(mal.contains(s.nextDouble()));
                    break;
                case "sub":
                    i = s.nextInt();
                    double[] v = mal.sub(i, s.nextInt());
                    for (i = 0; i < v.length; i++) {
                        System.out.println(v[i]);
                    }
                    break;
                case "list":
                    if (mal.isEmpty()) {
                        System.out.println("ArrayList is empty.");
                    }
                    for (i = 0; i < mal.size(); i++) {
                        System.out.format(" [%2d] = %.4f%n", i, mal.get(i));
                    }
                    break;
                case "quit":
                    running = false;
                    break;
                default:
                    System.out.println("Invalid command!");
                    s.nextLine();
                case "h":
                case "?":
                case "help":
                    System.out.println("Help:");
                    System.out.println(" add <val>       - Add the value <val>.");
                    System.out.println(" add <idx> <val> - Add the value <val> at index <idx>.");
                    System.out.println(" clear           - Clears all elements of the list.");
                    System.out.println(" contains <val>  - Checks if the list contains <val>.");
                    System.out.println(" get <idx>       - Get the value of the element at index <idx>.");
                    System.out.println(" index <val>     - Get the index of the first element that has value <val>.");
                    System.out.println(" list            - Lists all elements in the list.");
                    System.out.println(" rem <val>       - Remove the first element that contains <val>.");
                    System.out.println(" rem <idx>       - Remove the element at index <idx>.");
                    System.out.println(" set <idx> <val> - Set the value at index <idx> to <val>.");
                    System.out.println(" size            - Get the number of elements in the list.");
                    System.out.println(" sub <f> <t>     - Get a sub-array from [<f>, <t>).");
                    System.out.println(" quit            - Quit the program.");
                    break;
            }
        } while (running);
        s.close();
    }

    public myarraylist() {
        this(0); // Call the constructor with a size of 0
    }
/*
 * There are 2 ways of creating our array when creating the object.(other than default) 
 * the first will take an int that will make our array the size of the int. this allows the
 * user to use set and add within our array. The next constructor takes an int for the array size
 * and a double value to fill our array with. We use a for loop to set each index to the input value 
 * after our array is created with the specified size.
 */
    public myarraylist(int size) {
        mValues = new double[size];
    }

    public myarraylist(int size, double defval) {
        mValues = new double[size];
        for (int i = 0; i < size; i++) {
            mValues[i] = defval;
        }
    }
/*
 * We need to be able to add elements to the array. both start by copying our array
 * and increase its size by one. if just a value is given it is added to the end of the array.
 * We can do this by looping through the copy array and our array until the index.
 * Once at the index we can stop and tell the copy array at the index to equal the input value.
 * Another for loop is need to fill the rest of the copy array with the values from our array.
 * we then set our array equal to the copy array
 * if the index is more than the size of our array we can use the previous add method and tack the value onto end.
 */
    public void add(double val) {
        mValues = Arrays.copyOf(mValues, size() + 1);
        mValues[size() - 1] = val;
    }

    public void add(int index, double val) {
        if (index >= size()) {
            add(val);
        } else {
            double[] copy = Arrays.copyOf(mValues, size() + 1);
            for (int i = 0; i < index; i++) {
                copy[i] = mValues[i];
            }
            copy[index] = val;
            for (int i = index + 1; i < size() + 1; i++) {
                copy[i] = mValues[i - 1];
            }
            mValues = copy;
        }

    }
/*
 * To clear our array we just have to make a new array with a size of 0 and
 * then tell our array to equal the empty array.
 */
    public void clear() {
        double[] empty = new double[0];
        mValues = empty;
    }

/**
 * Both the indexOf and contains method both check to see if the value
 * is in our array. The indexOf should return the first index of the input value.
 * we can loop through our array seeing if each element equals the input value and 
 * return the index once found. If the value cant be found it returns -1.
 * we can use the indexOf method in the contains and if it returns -1 then contains should be false
 * otherwise it should return true as the indexOf is not equal to 1.
 */
    public int indexOf(double val) {
        for (int i = 0; i < size(); i++) {
            if (mValues[i] == val) {
                return i;
            }
        }
        int notExist = -1;
        return notExist;
    }

    public boolean contains(double val) {
        if(indexOf(val) == -1){
            return false;
        }else{
            return true;
        }
    }
    /**
     * The get mothod allows us to see what the value is at an inputted index.
     * we can just return the value of our array at index.
     */
    public double get(int index) {
        return mValues[index];
    }
/**
 * This method checks if our array is empty or not.
 * The only way an array is empty is if the size is 0
 * we use our size method to see if it equals 0.
 */
    public boolean isEmpty() {
        /* TODO: Return true if the array is empty. */
        if (size() == 0) {
            return true;
        } else {
            return false;
        }
    }
/**
 * we want the remove method to take away an element at an inputted index.
 * we have already added an element at an index and we can use a similar function
 * to remove one. We create a copy of our array but with one less size.
 * we can loop through both and set the values from our array into the copy,
 * and when we reach the index not add the value to the copy.
 */
    public void remove(int index) {
        double[] copy = new double[size() - 1];
        int newIndex = 0;
        for (int i = 0; i < size(); i++) {
            if (i != index) {
                copy[newIndex++] = mValues[i];
            }
        }
        mValues = copy;
    }
/**
 * this remove method will remove the inputted value. we can use 
 * the previous remove method by using the indexOf method on the inputted val.
 * if the value isnt in our array the method wont do anything. if It does then
 * remove the index of the inputted value
 */
    public void remove(double val) {
        if(indexOf(val) == -1){
         }else{
            remove(indexOf(val));
         }
        
    }
/**
 * We want to set a value to a specific index we can use the set method.
 * we can overwrite the current value at the index with the newly inputted one.
 */
    public void set(int index, double val) {
        mValues[index] = val;
    }
/*
 * This method will give us the size of our array and we can use it
 * anywhere we need the size, instead of using the .length method.
 */
    public int size() {
        return mValues.length;
    }
/*
 * If we want to pull out values between 2 index we can use the sub method.
 * we need to create a new array that will have the elements from our array in it.
 * we create a new array that is the size between the 2 indexes. we loop from the first
 * index until the last index setting the new array values to our array.
 */
    public double[] sub(int first, int last) {
        int subSize = last - first;
        int subCount = 0;
        double[] subArray = new double[subSize];
        for (int i = first; i < last; i++) {
            subArray[subCount++] = mValues[i];
        }
        return subArray;
    }

    private double[] mValues;
}