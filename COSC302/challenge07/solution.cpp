// Challenge 07: Adding List-Based Integers
// Connor Riley
// 3/26/23
// this program takes in 2 integers and stores them into a stack as single digits and
// adds the 2 together.
#include <iostream>
#include <stack>
#include <string>
using namespace std;
int main(int argc, char *argv[])
{

    stack<int> N1;
    stack<int> N2;
    stack<int> sum;
    
    string in1, in2, stringSum;

    int padding, singleSum, carry = 0;

    //read in the 2 numbers, find which one has more digits, and pad the other with zeroes to have the same size
    //then put in the number digit by digit most significant first. this way we can add individual digits.
    while (cin >> in1)
    {
        cin >> in2;
        carry = 0;
        stringSum = ""; 
        if (in1.length() < in2.length())
        {
            padding = in2.length() - in1.length();
            for (int i = 0; i < padding; i++)
            {
                N1.push(0);
            }
        }
        else if (in1.length() > in2.length())
        {
            padding = in1.length() - in2.length();
            for (int i = 0; i < padding; i++)
            {
                N2.push(0);
            }
        }
        for (int i = 0; i < in1.size(); i++)
        {
            int num = (int)(in1[i] - '0');
            N1.push(num);
        }
        for (int i = 0; i < in2.size(); i++)
        {
            int num = (int)(in2[i] - '0');
            N2.push(num);
        }
        //we then loop through each number stack and add the top of each stack as its the least signifcant, check if its sum is greater than
        // 10 and if so mod the sum by 10 and have a carry on the next digit addition.
        while (!N1.empty())
        {
            singleSum = N1.top() + N2.top() + carry;
            if (singleSum >= 10 && N1.size() != 1)
            {
                sum.push((singleSum) % 10);
                carry = 1;
            }else if(N1.size() == 1 && singleSum >= 10){
                sum.push((singleSum) % 10);
                sum.push(1);
            }
            else
            {
                sum.push(singleSum);
                carry = 0;
            }
            N1.pop();
            N2.pop();
        }
        //we then take the sum stack and convert each digit into a string and inserting it into a string
        //and removing the digit from the stack, we then print out the final sum.
        while (!sum.empty())
        {
            stringSum += to_string(sum.top());
            sum.pop();
        }
            cout << stringSum << "\n";         
    }
    return 0;
}


//Kid-A - Radiohead