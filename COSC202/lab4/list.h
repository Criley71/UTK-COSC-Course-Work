/*Connor Riley
 * linked list lab
 * fall 2022 CS202
 * 9/26/22
 * */
#include <cstdio>

//start of Node class
class Node {
	public:
		Node(int);
		int data;
		Node *next;
};

class List {
	private:
		Node *head;
		Node *newNode(int);
		int count;
	public:
		List();
		~List();
		void addNode(int);
		int empty();
		int getCount();
		Node *getHead();
		void print();
};

