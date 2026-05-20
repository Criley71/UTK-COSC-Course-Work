//Malika Arifova, Yaren Dogan, Connor Riley, Dorothy Wang
//Project 2: stl.cpp
//02/17/2022

//This program will sort the linked list using a built in C++ function sort()

#include "volsort.h"
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

void stl_sort(List& l, bool numeric) {

	vector<Node *> listCopyNums;
	Node* p = l.head;
	while (p != NULL) { //if the node is not null then you push back into the vector the value the pointer points to
		listCopyNums.push_back(p);
		p = p->next; //set the pointer to the next pointer and then continue with the loop until the condition is met
	}

	//this conditional statement checks to see if the data type in the node is a number or a string
	if(numeric == true) {
		sort(listCopyNums.begin(), listCopyNums.end(), node_number_compare);
	} else if (numeric == false) {
		sort(listCopyNums.begin(), listCopyNums.end(), node_string_compare);
	}


	//this stores all of the numbers in the vector into the linked list
	unsigned long i = 0;
	l.head = listCopyNums[0];
	Node *anotherNode = l.head;

	for(i = 1; i < listCopyNums.size(); i++) {
		anotherNode->next = listCopyNums[i];
		anotherNode = anotherNode->next;
	}
	anotherNode->next = NULL;
}
