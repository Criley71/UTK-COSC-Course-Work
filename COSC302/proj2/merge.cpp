//Malika Arifova, Yaren Dogan, Connor Riley, Dorothy Wang
//Project 2: merge.cpp
//02/17/2022

//This program will sort the linked list using merge sort

#include "volsort.h"
#include <iostream>

// Prototypes
Node* msort(Node* head, bool numeric);
void  split(Node* head, Node*& left, Node*& right);
Node* merge(Node* left, Node* right, bool numeric);

//For this program, we used a lot of help from: https://www.geeksforgeeks.org/merge-sort-for-linked-list/ 


// Implementations

//Changing the implementation of this function helped us to get it to work for strings
void merge_sort(List& l, bool numeric) {
	Node *newNodeHead; //creates a new head in order to set equal to the recursive mosrt() function
	newNodeHead = msort(l.head, numeric);
	l.head = newNodeHead; //update the list head
}

//This is the recursive function call
Node* msort(Node* head, bool numeric) {
	Node* newHead = head;
	Node* x;
	Node* y;

	//base case
	if (newHead == NULL || newHead->next == NULL) {
		return head;
	}

	if(newHead->next != NULL){
		split(newHead, x, y);
	}

	Node *left = msort(x, numeric);
	Node *right = msort(y, numeric);
	head = merge(left, right, numeric);

	return head;
}

void split(Node* head, Node*& left, Node*& right) {
	Node* slow = head;//Make a fast and slow pointer and set to head
	Node* fast = head->next;

	//move fast over 1 check if null, then move fast and slow over 1
	while (fast != NULL) {
		fast = fast->next;
		if (fast != NULL) {
			slow = slow->next;
			fast = fast->next;
		}
	}
	//left list starts at head, right starts at midpoint slow -> next
	//split at midpoint slow
	left = head;
	right = slow->next;
	slow->next = NULL;
}

Node* merge(Node* left, Node* right, bool numeric) {
	Node* sorted = NULL;

	if (left == NULL) {
		return right;
	}
	else if (right == NULL) {
		return left;
	}

	if (numeric == true) {//COMPARE NUMBERS
		if (node_number_compare(left, right)) {
			sorted = left;
			sorted->next = merge(left->next, right, numeric);
		}
		else {
			sorted = right;
			sorted->next = merge(left, right->next, numeric);
		}
	}
	else if (numeric == false) {//compare strings
		if (node_string_compare(left, right)) {
			sorted = left;
			sorted->next = merge(left->next, right, numeric);
		}
		else {
			sorted = right;
			sorted->next = merge(left, right->next, numeric);
		}
	}
	return sorted;
}
