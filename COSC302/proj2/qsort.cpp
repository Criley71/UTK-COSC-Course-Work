//Malika Arifova, Yaren Dogan, Connor Riley, Dorothy Wang
//Project 2: qsort.cpp
//02/17/2022

//This program will sort the linked list using a built-in c++ function called qsort()

#include "volsort.h"
#include <cstdlib>
#include <array>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

//Used Rob's qsort slides to figure out the structure of the functions
//because our initial sorting functions will not work, we created new ones for numbers and strings
int c_node_num_compare(const void *a, const void *b) { //We asked TA Andrew's help for the syntax for this
	Node *a2 = *(Node**)a;
	Node *b2 = *(Node**)b;

	//This function must return an int die to the nature of the qsort() function.
	//The return statement uses this logic. Source: https://cplusplus.com/reference/cstdlib/qsort/
	//if ( *(MyType*)a <  *(MyType*)b ) return -1;
	//if ( *(MyType*)a == *(MyType*)b ) return 0;
	//if ( *(MyType*)a >  *(MyType*)b ) return 1;
	return(a2->number - b2->number);
}

//same logic here, but using strings instead
int c_node_string_compare(const void *a, const void *b) {
	Node *a2 = *(Node**)a;
	Node *b2 = *(Node**)b;

	return(a2->string.compare(b2->string));
}

//this function is basically copied and pasted from STL sort
//the only difference is the qsort() function takes in difference parameters and different compare funtions

void qsort_sort(List &l, bool numeric) {
	vector<Node *> listCopyNums;
	Node* p = l.head;
	while (p != NULL) { //if the node is not null then you push back into the vector the value the pointer points to
		listCopyNums.push_back(p);
		p = p->next; //set the pointer to the next pointer and then continue with the loop until the condition is met
	}

	if(numeric == true) {
		qsort(&listCopyNums[0], listCopyNums.size(), sizeof(Node*), c_node_num_compare) ;
	} else if (numeric == false) {
		qsort(&listCopyNums[0], listCopyNums.size(), sizeof(Node*), c_node_string_compare) ;
	}


	//this stores all of the numbers in the vector back into a linked list
	unsigned long i = 0;
	l.head = listCopyNums[0];
	Node *anotherNode = l.head;

	for(i = 1; i < listCopyNums.size(); i++) {
		anotherNode->next = listCopyNums[i];
		anotherNode = anotherNode->next;
	}
	anotherNode->next = NULL;
}



