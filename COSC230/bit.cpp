//Connor Riley
//11/14/2022
//bit.cpp
#include <iostream>
using namespace std;
long badd(long a, long b);
long bsub(long, long);
long btwos(long);
int main()
{
    int choice, left, right, value;
    cout << "Enter 1 to add, 2 to subtract, or 3 for the 2's compliment: ";
    cin >> choice;
    if (choice == 1 || choice == 2)
    {
        cout << "Enter left value: ";
        cin >> left;
        cout << "Enter right value: ";
        cin >> right;
        if (choice == 1)
        {
            cout << badd(left, right);
            return 0;
        }
        else if (choice == 2)
        {
            cout << bsub(left, right);
            return 0;
        }
    }else if(choice == 3){
        cout << "Enter value: ";
        cin >> value;
        cout << btwos(value);
        return 0;
    }else if(choice < 1 || choice > 3){
        return -1;
    }
}

long badd(long a, long b)
{
    // a = t1
    // b = t2
    // carry = t3
    while (b != 0)
    {
        long carry = a & b;
        a = a ^ b;
        b = carry << 1;
    }
    return a;
}

long bsub(long x, long y)
{
    y = btwos(y);
    x = badd(x, y);
    return x;
}
long btwos(long a)
{
    a = ~a;
    return badd(a, 1);
}