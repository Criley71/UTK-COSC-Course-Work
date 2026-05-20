// Lab 7:  N-queens using backtracking
// Connor Riley 10/24/22 nqueens.cpp
// this program checks every chess board combination of 8 queens and 
// prints the valid positions to each board the board size is given by 
// a command line argument
#include <iostream>

using namespace std;
void nqueens(int board[], int col, int size);
bool isValid(int board[], int size);
int main(int argc, char *argv[])
{
	//first check if a command line argument is given for the size and if not it
	//prints the correct usage and exits the program.
	if (argc != 2)
	{
		cout << "Usage " << argv[0] << " <board size>\n";
		return -1;
	}
	//convert the command line argument into the size and make a board array
	//the given size with each element set to 0. it then calls nqueens and exits the
	//function.
	int size = stoi(argv[1]);
	int board[size];
	for(int i = 0; i < size; i++){
		board[i] = 0;
	}
	nqueens(board, 0, size);

	return 0;
}
//nqueens is a recursive function that once a board is filled it calls the isValid function
//to determine if the board is a valid combination. each function call whil call nqueens at a 
// column one greater than the last 
void nqueens(int board[], int col, int size)
{

//	static int counter = 0;
		if(col == size){
//			counter++;
			if(isValid(board, size)){
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
		}
	for (int i = 0; i < size; i++)
	{

		board[col] = i;
		nqueens(board, col+1, size);
	}
//	cout << counter << "\n";
}



// check if board is valid
// this checks the positions of every board and determines if the board is valid
// it checks the columns by checking  if 2 of the array elements are equal. if so the board is
// not possible. it then checks diagnols by  seeing if the slopes are equal to eachother by subtracting
// board[i] - board[j] (y value) and  i - g (x value) if both are equal it means the slope is 1 and 
// that the position is not valid due to diagnol. if it passes both checks it returns true.
bool isValid(int board[], int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++){
			if(i == j){
				continue;
			}
			if (board[i] == board[j])
			{
				return false;
			}
			if( abs(board[i] - board[j]) == abs(i - j)){
				return false;
			}
		}
	}
	return true;
}
