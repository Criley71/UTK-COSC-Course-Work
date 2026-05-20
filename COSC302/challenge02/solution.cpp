// Challenge 02: Closest Numbers
// Connor Riley
//taking an input of numbers, it returns the numbers with the 
//smallest difference between them. This uses vector.sort() and 
//comparing differences between each number.

#include <algorithm>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Main Execution

int main(int argc, char* argv[]) {
	int numElements, element, minDif, minCheck;//vector size, vector value, smallest difference, new difference to compare
	vector<int> vals;//stores inputed values
	vector<int> pairs;//stores the pairs of numbers with the smallest difference
	/*Start by taking in the number of elements the user wants to enter, then
	* have the user enter the values into the vector using a for loop.
	*/
	while (cin >> numElements) {
		for (int i = 0; i < numElements; i++) {
			cin >> element;
			vals.push_back(element);
		}
		//Then we sort the vector using stl::sort, this will have the elements ascending in value.
		//This will allow us to compare the differences, as the smallest difference will always 
		//be between 2 numbers closest to eachother.
		sort(vals.begin(), vals.end());
		
		//I set the smallest difference to the first possible difference between index 1 and 0
		//and push those values into the pairs vector.
		minDif = vals[1] - vals[0];
		pairs.push_back(vals[0]);
		pairs.push_back(vals[1]);

		//Then we loop through the input starting at index 1 and subtracting index+1 - index
		//and set it to minCheck. then we can compare minCheck and see if it is smaller
		//if so then there is a new mininum difference meaning the vector needs to be cleared and have
		//the new smallest difference pair pushed into the array. if the minCheck is equal to the
		//current mininum value then the values are just pushed into the vector without clearing the vector
		//If the minCheck is greater than the minDif then nothing needs to be done.
		for (int i = 1; i < numElements - 1; i++) {
			minCheck = vals[i + 1] - vals[i];
			if (minCheck < minDif) {
				minDif = minCheck;
				pairs.clear();
				pairs.push_back(vals[i]);
				pairs.push_back(vals[i + 1]);
			}
			else if (minCheck == minDif) {
				pairs.push_back(vals[i]);
				pairs.push_back(vals[i + 1]);
			}
		}
		//Then print out the pairs, having the last value outputted to not have a space after it
		for (int i = 0; i < pairs.size(); i++) {
			if (i == pairs.size() - 1) {
				cout << pairs[i] << "\n";
			}
			else {
				cout << pairs[i] << " ";
			}
		}
		//clear the vectors so the next loop has empty vectors.
		vals.clear();
		pairs.clear();
	}

	return EXIT_SUCCESS;
}
