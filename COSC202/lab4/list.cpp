/*Connor Riley
 * linked list lab
 * fall 2022 CS202
 * this defines the node and list class for the linked list program.
 * credit to using the code blake posted for us to use that may be Dr. Emrichs.
 * */
#include "list.h"
#include <string>
#include <iostream>
using namespace std;
//This is the Node class which will be used to store a single digit int and a pointer that
//will point to the next node in the list. It has a constructor to that takes the digit 
//to store and makes a null pointer.
Node::Node(int i)
{
	data = i;
	next = NULL;
}

//this is the List class. The list class will store nodes from the node class in the form of a singulary
//linked list. This class creates a list, deconstructs a list, creates a new node and adds it into the list.
//will give the amount of nodes in the list, tell if the list is empty and print the list.
//This will construct the list with the head pointer and count being NULL and 0;
List::List()
{
	head = NULL;
	count = 0;
}

List::~List()
{
	if (!empty())
	{ 
		Node *p = head;

		while (p != NULL)
		{
			Node *next = p->next;
			delete p;
			p = next;
		}
	}
}

//creates a new node with an int and sets the next pointer to NULL, and returns the node.
Node *List::newNode(int i)
{
	Node *node = new Node(i);
	node->next = NULL;
	return node;
}

//This will call  the new node function and add it onto the end of the list.
//if it is the first node in the list its pointer is set as the head of the list
void List::addNode(int i)
{
	Node *H = head;
	Node *p = newNode(i);

	if (head == NULL)
	{
		head = p;
	}
	else
	{
		while (H->next != NULL)
		{
			H = H->next;
		}
		H->next = p;
	}
	count++;
}
//This will return if the list is empty as in it has no nodes 
// or has nodes in it as a 1 or 0 respectivly
int List::empty()
{
	if (head == NULL)
	{
		// is empty
		return 1;
	}
	else
	{
		// is full
		return 0;
	}
}
//this will return the amount of nodes found in the list
int List::getCount()
{
	return count;
}
//This will return the head of the List
Node *List::getHead()
{
	return head;
}
//this will print out a list by reading the numbers from the head to tail and storing them into a string
//the string is then iterated through backwards and prints out the number. If the list is empty then 
//it prints nothing. for example we would  
//read in 1 2 3 as a string then store it as 3 2 1 head to tail. when we read the list
//we concat it into a string as 321 then iterate it backwards as 123
void List::print()
{
	if (!empty())
	{
		string stringNum = "";
		Node *p = head;
		while (p != NULL)
		{
			stringNum += to_string(p->data); // maybe try 48
			p = p->next;
		}
		for (int i = getCount() - 1; i >= 0; i--)
		{
			cout << stringNum[i];
		}
		cout << "\n";
	}
	else
	{
	//print nothing
	}
}
