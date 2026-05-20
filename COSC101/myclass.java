class myclass {
    public static void main(String[] args) {
       myclass mc1 = new myclass();
       myclass mc2 = mc1;
       mc1.setValue(111);
       mc2.setValue(555);
       System.out.print(mc1.getValue() + " " + mc2.getValue());
    }
    public void setValue(int val) {
       mValue = val;
    }
    public int getValue() {
       return mValue;
    }
    private int mValue;
 }