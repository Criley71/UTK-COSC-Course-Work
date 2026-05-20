public class methodsAndClasses { 
    public static void main(String[] args){
        test2 t = new test2();
        t.set(1234);
        System.out.println("Value = " + t.get());
    }
/**
 * Methods are fucntions of a class
 * 5 parts to a method
 * access, return type, name, parameters, body
 * access is public or private
 * 
 * access return type name(parameters){
 * body
 * }
 */
/*public void method() {
    // No return required
    return;
 }
 public int intMethod() {
    // Returning an integer required
    return 10;

 }
public static void main(String[] args){
    int i = 10;
    int j = 20;
    int k = 30;
    String out = String.format("[main] i: %d, j: %d, k: %d%n", i, j, k);
    System.out.print(out); --------> [main] i: 10, j: 10, k: 30 -----------------------------
    call_method(k, j, i); ---------> [call_method] i: 30, j: 20, k: 10 ------- this is because the paramters wer put in as k, j, i or 30, 20, 10. Order is what matters on paramters not names
}
    public static void call_method(int i, int j, int k){
        String out = String.format("[call_method] i: %d, j: %d, k: %d%n", i, j, k);
        System.out.print(out); -> 
    }
    */
}
class test {
    public static void main(String[] args){
        test t = new test();
        t.set(1234);
        System.out.println("Value = " + t.get());//-> this will result in value = 0, due to val = val having the same names

    }
    public void set(int val){
        val = val; 
    }
    public int get(){
        return val;
}
    private int val;
}

class test2 {
    public static void main(String[] args){
        test2 t = new test2();
        t.set(1234);
        System.out.println("Value = " + t.get());// value = 1234, fixed via line 55

    }
    public void set(int val){
        this.val = val; //using this.val refers to val as a variable, val refers to parameter.
    }
    public int get(){
        return val; // this val is now equal to this.val which was equal to the paramter in t.set(1234)
}
    public int val;
}

