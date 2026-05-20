/*Connor Riley
9/12/2022
This program plays connect 4
*/
//Class definition for C4Col class
class C4Col {

    private:
        int numDisc;
        int maxDisc;
        char* disc;
    public:
        C4Col();
        int isFull();
        char getDisc(int);
        void addDisc(char);
        int getMaxDiscs();
};

