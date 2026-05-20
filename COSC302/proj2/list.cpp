//Malika Arifova, Yaren Dogan, Connor Riley, Dorothy Wang
//Project 2: list.cpp
//02/17/2022

//this program will define the functions that are inside volsort.h
//This program includes a list constructor and a list destructer as well as a push front function

#include "volsort.h"
#include <string>
#include <iostream>
using namespace std;

//I am setting head to Null, and the size equal to zero because there aren't 
//any nodes just yet in the list
List::List() {
	head = NULL;
	size = 0;
}

List::~List() {
	//list is not empty so create a pointer that points to the head
	if(head != NULL) {
		Node *p = head;
		while(p != NULL) { //while the pointer does not point to null, create
			//a next pointer so you can delete the p pointer
			Node *next = p->next;
			delete p;
			p = next; //set p to next
		}
	}
}

//this is the void push_front() function
//this creates a new node to put in the front of the linked list
void List::push_front(const std::string &s) {
	Node *tempnode = new Node();
	tempnode -> string = s;
	tempnode -> number = stoi(s); //take a string and convert it to an int for comparison
	tempnode -> next = head;
	head  = tempnode; //update the head
}

//this compares the numbers inside of the node for sorting purposes
bool node_number_compare(const Node *a, const Node *b) {
	if(a->number <= b->number) {
		return true;
	} else {
		return false;
	}
}

//this compares the strings we may have inside of our nodes
bool node_string_compare(const Node *a, const Node *b) {
	int comp = a->string.compare(b->string);
	if(comp <= 0) {
		return true;
	} else {
		return false;	
	} 
}

//this is optional for now dump_node
//debug, outputs the contents of each node start from n until nullptr is reached
void dump_node(Node *n) {
	while(n->next != NULL) {
		cout << n->number << endl;
	}
}