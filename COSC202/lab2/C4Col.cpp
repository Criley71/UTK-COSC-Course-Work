/*Connor Riley
9/12/2022
This program plays connect 4
*/
//This is the C4Col class that deals with adding disks ito the board.
#include "C4Col.h"
#include <iostream>

using namespace std;
//Set some default values when the object is made
//the numDisc is the amount of disc in each array,
//the maxDisc is how much each array is able to store and
C4Col::C4Col()
{
    numDisc = 0;
    maxDisc = 6;
	disc  = new char [maxDisc];
    for(int i = 0; i < maxDisc; i++){
        disc[i] = '*';
    }
}
//This is used to determine if the column is full
//this will be used to determine if a move is valid
int C4Col::isFull()
{
    if (numDisc == maxDisc)
    {
        return 1;
        //full 
    }
    else
    {
        return 0;
        //not full
    }
}
//This will print the disk at a point in the array, if the spot is too big
//then an error is given;
char C4Col::getDisc(int spot)
{
    if (spot > maxDisc)
    {
        cout << "That spot doesnt exist.\n";//figure out return value
        return ' ';
    }
    else
    {
        return disc[spot];
    }
}
//this will add the disc into the array for each player
void C4Col::addDisc(char playerDisc)
{
    if (isFull() == 0)
    {
        disc[numDisc] = playerDisc;
        numDisc++;
    }
    else
    {
        cout << "The column is full.\n";
    }
}
int C4Col::getMaxDiscs()
{
    return maxDisc;
}

