//Malika Arifova, Yaren Dogan, Connor Riley, Dorothy Wang
//Quick Sort
//2/17/2023

//This program sorts the linked list with quick sort

#include "volsort.h"

#include <iostream>

// Prototypes
using namespace std;
Node *qsort(Node *head, bool numeric);
void  partition(Node *head, Node *pivot, Node *&left, Node *&right, bool numeric);
Node *concatenate(Node *left, Node *right);

// Implementations

//Just followed the steps on merge sort
//Received help from Maggie Kelley
void quick_sort(List &l, bool numeric) {
	Node *newNodeHead; //creating a new pointer
	newNodeHead = qsort(l.head, numeric); //set newHead to the qsort function to start the sorting algorithm
	l.head = newNodeHead; 
}

//this whole thing is jank, except for the base case maybe
Node* qsort(Node *head, bool numeric) {
	//this be the base case 
	if(head == NULL || head->next == NULL) {
		return head;
	}

	//we are creating an object of Node, and setting it to head
	Node *rightSide;
	Node *leftSide;

	//partition the list
	//set the pivot equal to the first element in the node, which is the head,
	//which is the new head now
	Node *pivot = head;

	partition(head, pivot, leftSide, rightSide, numeric);

	leftSide = qsort(leftSide, numeric); //left
	rightSide = qsort(rightSide, numeric); //right

	//TA Andrew helped me out on this one
	//this helps to get the pivot to concatenate with the left and right side
	//the pivot will be in the middle automatically
	head->next = rightSide;
	return concatenate(leftSide, head);	

}


void partition(Node *head, Node *pivot, Node *&left, Node *&right, bool numeric) {
	//we are going to set the first node to the pivot

	//This sets the current node equal to the node after the head because the head
	//is the pivot
	Node *currentNode = head->next;

	//initialize the left and the right as NULL
	left = NULL;
	right = NULL;

	//TA Andrew and TA Christian helped so much with these
	//These two new nodes keep track of the tails of the left and right side
	Node* leftTail = NULL;
	Node *rightTail = NULL;

	bool compare; //this is created and used in order to not repeat code for string and number compare

	while(currentNode != NULL) { //iterates through the list
		if(numeric == true) {
			compare = node_number_compare(currentNode, pivot); //number compare
		} else {
			compare = node_string_compare(currentNode, pivot); //string compare
		}

		if(compare == true) {
			if(left == NULL) { //if there isn't something on the left side of the list
				left = currentNode;
				leftTail = currentNode; //this keeps track of the end of the list
			} else {
				leftTail->next = currentNode;
				leftTail = leftTail->next; //updates the tail node
			}
		} else if (compare == false) { //same logic but for the right side
			if(right == NULL) {
				right = currentNode;
				rightTail = currentNode;
			} else {
				rightTail->next = currentNode;
				rightTail = rightTail->next;
			}
		}
		currentNode = currentNode->next; //iterate through the list
	}

	//We have to set the tails->next equal to null in order to keep track of the end of the left and right
	//side list
	if(leftTail != NULL) {
		leftTail->next = NULL;
	}
	if(rightTail != NULL) {
		rightTail->next = NULL;
	}
}


Node *concatenate(Node *left, Node *right) {
	//if there isn't a a left list, then return right
	if(left == NULL) {
		return right;
	}
	if(right == NULL) {
		return left;
	}

	//create a temp node to iterate through
	Node *listPeace = left;

	while(listPeace->next != NULL) {
		listPeace = listPeace->next;
	}
	//once we iterate through the left list, we connect to the head of the right list
	listPeace->next = right;
	
	return left;
}