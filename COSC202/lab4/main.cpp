// Connor Riley
// Linked list lab
// Fall 2022 CS202
//9/26/22
#include "list.h"
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

//To add the lists together we will need to account for different sized lists,
//carrying numbers between nodes and then adding the sum into the list.
List *sumLists(List *list1, List *list2)
{
	//-handling lists of different sizes-find the smaller one, pad with 0's so 123 +123456 looks like 321000 + 654321 would look like:
	//  	3 2 1 0 0 0																   (real numbers: 000123 + 123456)
	//	+	6 5 4 3 2 1
	//	-----------------
	//		9 7 5 3 2 1
	// this is backwards so the sum is 1 2 3 5 7 9
	// store the still backwards number into a new list that will be returned
	// the print function will print it the correct way
	//
	// we create a new list and a pointer to said list, int variables for the
	// amount of 0 padding is needed, the sum between digits, and the carry
	// as well as a bool to determine if we need to carry.
	List list3;
	List *p3 = &list3;
	int padding, carry = 0, sum;
	bool isCarry;

	//first we compare the sizes of the 2 lists to be added using the getCount function
	//to determine the padding and add the amount of padding using the addNode(0)
	if (list1->getCount() > list2->getCount())
	{
		padding = list1->getCount() - list2->getCount();
		for (int i = 0; i < padding + 1; i++)
		{
			list2->addNode(0);
		}
	}
	else if (list2->getCount() > list1->getCount())
	{
		padding = list2->getCount() - list1->getCount();
		for (int i = 0; i < padding; i++)
		{	
			list1->addNode(0);
		}
	}
	else if (list2->getCount() == list1->getCount())
	{
		// do nothing no padding needed
	}
	//We first make two pointers equal to the heads of each list using the getHead function
	//we move through the lists until reaching the tail. we add each digit and if greater than 10
	//a carry is needed so the sum is subtracted by 10 and sets the carry to true and =1 so the next sum
	//has a 1 added onto it. each sum is stored into a node and added into the sum list.
	//we then print the sum using the print function
	Node *p = list1->getHead();
	Node *t = list2->getHead();
	while (p != NULL)
	{
		sum = p->data + t->data;
		if (sum >= 10)
		{
			sum -= 10;
			isCarry = true;
		}
		else if (sum < 10)
		{
			isCarry = false;
		}
		list3.addNode((sum + carry));
		if(isCarry == true){
			carry = 1;
		}
		else if(isCarry == false){
			carry = 0;
		}
		if(p->next == NULL && carry == 1){
			list3.addNode(carry);
		}
		p = p->next;
		t = t->next;
	}
	list3.print();
	return p3;

}


int main(int argc, char *argv[])
{
	//we begin the main by creating 2 lists, a string, and 2 ints, one to store the number to be added
	//into the list, the other determining which list to store
	while(cin){
		List list1;
		List list2;
		List *p1 = &list1;
		List *p2 = &list2;
		string input;
		int num, listToInput = 1;
	//we first read in the input with cin using getline to read in the whole line and stores it into the input string
	//we iterate through the string backwards storing each digit into a node in the 1st list. Once ' ' (whitespace) is found
	//it switches to adding the digit nodes into the 2nd list.
	//once complete we call the sum lists function on the 2 lists which print out the final sum
		getline(cin, input, '\n');
		for (int i = input.length() - 1; i >= 0; i--)
		{
			if (input[i] == ' ')
			{
				listToInput = 2;
				continue;
			}
			else
			{
				num = input[i] - '0';
				if (listToInput == 1)
				{
					list1.addNode(num);
				}
				else if (listToInput == 2)
				{
					list2.addNode(num);
				}
			}
		}
		sumLists(p1, p2);
	}
	return 0;
}

