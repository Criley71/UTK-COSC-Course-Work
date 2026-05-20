// Challenge 01: Rotating Arrays
// Connor Riley
// 1/29/23 
// CS302 Dr.Emrich
// This program is to take an inputted array and shift it the amount
// of spaces in the direction input by the user.
#include <iostream>
#include <vector>
using namespace std;
int main(int argc, char* argv[]) {
	int size, shiftAmt, element;
	char shiftDir;
	//The main part of the code is put in this while loop that will stop once inputs stop.
	//I ask the user for the amount of numbers(size) the number of spaces shifted(shiftAmt) and the shift direction(shiftDir)
	//next I make a vector of integers and have the user input the desired values and store it in the vector.
	while (cin >> size >> shiftAmt >> shiftDir) {
		vector<int> vals;
		vals.clear();
		for (int i = 0; i < size; i++) {
			cin >> element;
			vals.push_back(element);
		}
	//	If the user chose left shift then the vector will take the first value and insert at the end of the array.
	//	once inserted at the end then the number at the front is deleted as it has been shifted to the end. 
	//	This process happens the amount of times the user wants to shift.
		if (shiftDir == 'L' || shiftDir == 'l') {
			for (int i = 0; i < shiftAmt; i++) {
				vals.insert(vals.begin() + size, vals[0]);
				vals.erase(vals.begin());
			}
		}
	// If the user chose a right shift I begin to change the shift to a left shift equivalent to what the right shift would have.
	// The math is to take the size of the array and subtract the shift amount from it. This value is the amount of shifts needed.
	// If the user inputs a shift amount greater than the size then the negative difference is made a positive.
	// The shift process is the same as the left shift above after this point.
		if (shiftDir == 'R' || shiftDir == 'r') {
			shiftAmt = size - shiftAmt;
			if(shiftAmt < 0){
				shiftAmt *= -1;
				shiftAmt = size - shiftAmt;
			}
			for (int i = 0; i < shiftAmt; i++) {
				vals.insert(vals.begin() + size, vals[0]);
				vals.erase(vals.begin());
			}
		}
	//This will print the new array to the console with formatting specifications to meet the
	//make file grading requirment to not have a space after the last number in the array.
		for (int i = 0; i < size; i++) {
			cout << vals[i];
			if(i < size - 1){
				cout << " ";
			}
		}
		cout << "\n";
	}

	return 0;
}


