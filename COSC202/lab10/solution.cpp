// Lab 10: kth smallest numbers
// Connor Riley lab10 binaryHeaps 11/22/2022
// This code reads in a number of ints as an array and
// makes a minheap from it and finds the kth smallest term in the heap

#include <iostream>
#include <vector>
using namespace std;

//Decided to make a class that stores the min heap vector and has all the functions needed 
//to make a min heap, percolate down, pop and minheapify, as well as check if the intial vector
//is a max heap or not
class MinHeap
{
	//the private members of the class include; the vector that stores the heap,
	//the percolate down function that is given a parent index that then checks the 2 children of
	//the index and swaps if the parent is larger than its child, if so their index's are swapped
	//and percolate down is called recursivly on the index where the initial parent index value ended up
	//minHeapify calls percolate down on the vector indexes backwards to make the vector a min heap
	private:
		vector<int> vectorHeap;
		void percolateDown(int index)
		{
			int size = vectorHeap.size();
			int leftIndex = (index * 2) + 1;
			int rightIndex = (index * 2) + 2;

			if (leftIndex >= size)
			{
				return;
			}

			int minIndex = index;
			if (vectorHeap[index] > vectorHeap[leftIndex])
			{
				minIndex = leftIndex;
			}

			if ((rightIndex < size) && (vectorHeap[minIndex] > vectorHeap[rightIndex]))
			{
				minIndex = rightIndex;
			}

			if (minIndex != index)
			{
				int temp = vectorHeap[minIndex];
				vectorHeap[minIndex] = vectorHeap[index];
				vectorHeap[index] = temp;
				percolateDown(minIndex);
			}
		}

		void minHeapify()
		{
			int size = vectorHeap.size();
			for (int i = size - 1; i >= 0; i--)
			{
				percolateDown(i);
			}
		}
	//The public section of the class holds a default constructor, as well as a constructor that takes in a vector, 
	//makes it the vector of the class, and checks if the vector is a maxheap. it checks if it is a max heap by comparing
	//the parent index to its children and if a child is larger than its parent it is not a maxhap and returns'N', if it reaches a leaf node 
	//and finds that none of the children were larger than their parent it returs 'Y'. It then calls minheapify to make the vector a min heap.
	//the pop funtion taks the last element in the vector moves it to the 0th index in the vector and decreases the size by 1, we then call 
	//percolate down on index 0/ the root to then make the vector a proper min heap again. we call it k - 1 times as then the root element after
	//calling percolate down is the kth smallest term and we return vector[0];
	public:
		MinHeap();
		MinHeap(const vector<int> &v)
		{
			vectorHeap = v;
			cout << isMax() << " ";
			minHeapify();
		}



		int pop(int k)
		{
			for (int j = 0; j < k - 1; j++)
			{
				vectorHeap[0] = vectorHeap[vectorHeap.size() - 1];
				vectorHeap.pop_back();
				percolateDown(0);
			}
			return vectorHeap[0];
			//this could also just run k amount of times and have a temp that stores the popped number, but that would lead
			//to having another loop and percolating down a  tree heap that we arent needing for the return value.
		}
		char isMax()
		{
			for (int i = 0; (2 * i + 1) < static_cast<int>(vectorHeap.size()); ++ i) {
				if ((2 * i + 1 < static_cast<int>(vectorHeap.size())) && (vectorHeap[i] < vectorHeap[i * 2 + 1])) {
					return 'N';
				}    
				if ((2 * i + 2 < static_cast<int>(vectorHeap.size())) && (vectorHeap[i] < vectorHeap[i * 2 + 2])) {
					return 'N';
				}
			}
			return 'Y';
		}

};

//in main we have 3 integer variables, numElements, desiredNum, element. NumElements is the size of the vector, 
//desiredNum is the kth term and element will be used to fill in a vector index. We then create a MinHeap object
//called test and pass the vector we made with all the elements inputted as a paramter. we set k to result from the
//pop function for desired num term and print out the result.
int main(int argc, char *argv[])
{
	int numElements, desiredNum, element;
	while (cin >> numElements)
	{
		cin >> desiredNum;

		vector<int> heapVector(numElements);
		for (int i = 0; i < numElements; i++)
		{
			cin >> element;
			heapVector[i] = element;
		}

		MinHeap test(heapVector);
		int k = test.pop(desiredNum);
		cout << " "  << k << "\n";
	}
}
