// Lab 7:  N-queens using "constrained" backtracking 
// Connor Riley 10/24/22 nqueen2.cpp
#include <iostream>
#include <cstdlib>
//this is an optimized version of nqueens where it finds all
//possible poitions for queens to be in each row and not attack each other
//it does this by instead of checking the whole board it checks each attempt to place
//a queen on the board and if its safe it continues.
using namespace std;
void nqueens(int board[], int col, int size);
bool isValid(int board[], int size);
int main(int argc, char *argv[]) {
	//first check if a command line arguement was provided to set the size of the board
	//make an array with size given and initialize each to zero.
	//it then calls the recursive nqueens function
	if (argc != 2)
	{
		cout << "Usage " << argv[0] << " <board size>\n";
		return -1;
	}
	int size = stoi(argv[1]);
	int board[size];
	for(int i = 0; i < size; i++){
		board[i] = 0;
	}
	nqueens(board, 0, size);

	return 0;

}
//nqueens takes the board array, the size, and column to check.
//we call the isValid fucntion each time we try to place a piece,
//we limit it by allowing isValid() only to check upto the current column. if it isnt valid
//it  wont place and move to next row and attempt there. if it is a valid move it
//will place it. if it reaches a point where it cant place a valid move it will go back to
//the previous and move it to another safespot.
void nqueens(int board[], int col, int size){
//	static int counter = 0;
	if(col == size){
//		counter++;
		if(isValid(board, col)){
			for(int i = 0; i < size; i++){
				if(size - 1 == i){
					cout << board[i];
				}else{
					cout << board[i] << ", ";
				}
			}
			cout << "\n";
		}
		return;
	}else if(isValid(board, col)){
		for(int i = 0; i < size; i++){
			board[col] = i;
			nqueens(board, col+1, size);
		}
//		cout << counter << "\n";
	}

}


// this checks the positions of every board and determines if the board is valid
// it checks the columns by checking  if 2 of the array elements are equal. if so the board is
// not possible. it then checks diagnols by  seeing if the slopes are equal to eachother by subtracting
// board[i] - board[j] (y value) and  i - g (x value) if both are equal it means the slope is 1 and
// that the position is not valid due to diagnal. if it passes both checks it returns true.
bool isValid(int board[], int size)
{
	
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++){
			if(i == j){
				continue;
			}
			if (board[i] == board[j]){
				return false;
			}
			if( abs(board[i] - board[j]) == abs(i - j)){
				return false;
			}
		}
	}
	return true;
}
