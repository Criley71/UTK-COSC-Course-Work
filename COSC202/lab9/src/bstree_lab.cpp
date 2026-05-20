/*Connor Riley
 * 11/15/22
 * bstree lab
 * */
#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include "bstree.hpp"
using namespace std;
using CS140::BSTNode;
using CS140::BSTree;


/*this calculates the depth of a node on the tree.
 * it traverses the left and right checking the given key value
 * if it finds it, it returns the depth of the node, other wise it returns -1
 * if the key doesnt exist
 * */
int BSTree::Depth(const string &key) const
{
	int depthCount = 0;
	BSTNode *n;
	n = sentinel->right;
	while (n->key != key)
	{

		if (n == sentinel)
		{
			return -1;
		}
		if (key < n->key)
		{
			n = n->left;
		}
		else
		{
			n = n->right;
		}
		depthCount++;
	}
	return depthCount;
}
/*
 * This calculates the height of the tree by calling the recursive height function below.
 * */
int BSTree::Height() const
{
	BSTNode *n = sentinel->right;
	return recursive_find_height(n);

}
/*
 * this traverses the tree making an array of the trees keys in increasing order
 * it is done by the recursive function below.
 * */
vector<string> BSTree::Ordered_Keys() const
{
	vector<string> rv;
	BSTNode *n = sentinel->right;
	make_key_vector(n, rv);

	return rv;
}

/*
 *This is a copy constructor for the tree class.
 *the code was provided by blake
 *it first sets the sentinel to the tree object
 *the sentinels "right" node is the root of the tree so there is no left node
 *it has no parent as it isnt a node, the node is recognized as sentinel so its 
 *the key value. Its not part of the hiearchal data so it has no value.
 **/
BSTree::BSTree(const BSTree &t)
{
	//allocate new sentinel
	sentinel = new BSTNode;
	sentinel->left = NULL;
	sentinel->right = sentinel;
	sentinel->parent = NULL;
	sentinel->key = "---SENTINEL---";
	sentinel->val = NULL;

	size = 0; //reset size

	*this = t; //set object (uses assignment overload)
}
/*
 *this function was prived by Blake
 *It overloads the assignment operator so we can set assign tree objects to other tree objects
 *We first empty the tree and call the make balanced tree function on the tree we are assigning it to
 *make them have equal size and return the tree.
 * */
BSTree &BSTree::operator=(const BSTree &t)
{
	Clear(); //clear tree

	//create new balanced tree
	make_balanced_tree(t.Ordered_Keys(), t.Ordered_Vals(), 0, t.size);

	size = t.size; //set size

	return *this;
}
/*
 *This finds the height of tree recursivly. it first checks if the node is the sentinel as it would have a height 0 
 *It then finds the left height by calling the function again on its left node. it does the same for the right nodes
 *It then checks for which is greater and adds 1 to it and returns the height.
 * */
int BSTree::recursive_find_height(const BSTNode *n) const
{
	if(n == sentinel){
		return 0;
	}
	int left_height = recursive_find_height(n->left);
	int right_height = recursive_find_height(n->right);
	if(left_height >= right_height){
		return left_height + 1;
	}else{
		return right_height + 1;
	}
}
/*
 *This function is similar to the find height as both are recursive and check the left and right side of the tree.
 * It traverses to the bottom of the tree and adds the key to the vector. it then does the same for the right.
 * */

void BSTree::make_key_vector(const BSTNode *n, vector<string> &v) const
{
	if (n == sentinel) return;
	make_key_vector(n->left, v);
	v.push_back(n->key);
	make_key_vector(n->right, v);
}

/*
 *This function takes the keys and values of a tree as a vector as well as the amount of nodes
 *and where to start. we first make a left and right child and set it equal to the sentinel.
 *we calculate the middle, or the root. we then add nodes to the left side of the tree by recursivly calling
 *the function and having the num of indices divided 2 and it will do this while that number is greater than 0
 *We then add nodes to right by making the first index the middle + 1 index of the array.
 *we then make a new BSTNode object and set the member variables to there appropiate value.
 * */


BSTNode *BSTree::make_balanced_tree(const vector<string> &sorted_keys,
		const vector<void *> &vals,
		size_t first_index,
		size_t num_indices) const
{
	int n = num_indices;
	BSTNode *leftChild = sentinel; 
	BSTNode *rightChild = sentinel;

	int middle = (n / 2) + first_index;
	
	if((n / 2) > 0){
	leftChild = make_balanced_tree(sorted_keys, vals, first_index, (n / 2));
	}

	if((n - (n / 2) - 1) > 0){
	rightChild = make_balanced_tree(sorted_keys, vals, middle + 1,  (n - (n / 2) - 1));
	}

	BSTNode *i = new BSTNode;
	if(n == sorted_keys.size()){
	sentinel->right = i;
	}
	leftChild->parent = i;
	rightChild->parent = i;
	i->left = leftChild;
	i->right = rightChild;
	i->parent = sentinel;
	i->key = sorted_keys[middle];
	i->val = vals[middle];
	return i;
}

