/*Connor Riley
9/12/2022
This program plays connect 4
*/

//This facilitates the board class, where the display, playing the game, and win checking are done.
#include "C4Board.h"
#include <iostream>
using namespace std;
//first we need to make an array of C4Col objects from C4Col.cpp
//only 7 columns in the game 
C4Board::C4Board()
{
	numCol = 7;
	board = new C4Col[7];
}
//This will display the board. it begins by displaying the column numbers above each column
//0 to 6. Then the board is printed by using the getdisc function to display each spot.
void C4Board::display()
{
	cout << " ";
	for (int coordinate = 0; coordinate < numCol; coordinate++)
	{
		cout << " " << coordinate << "   ";
	}
	cout << "\n";

	for (int i = board[0].getMaxDiscs() - 1; i >= 0; i--)
	{
		for (int j = 0; j < numCol; j++)
		{
			cout << "| " << board[j].getDisc(i) << " |";
		}
		cout << "\n";
	}
}
//this is where the game is facilitated, the game is in a loop that runs until a
//winner or the board is filled. The user inputs the column and 
//it makes sure the users input is a possible
//move by wether the column is filled or it doesnt exist.
//The player is determined by the turn number
//if the win function returns 0 or 1 then a winner is determined and the function ends
void C4Board::play()
{
	int turncount = 0, inputCol, winner;
	char player;
	bool validMove;

	while (true)
	{
		if(turncount == 42){
			cout << "Tie\n"
		}
		validMove = false;
		display();
		if (turncount % 2 == 0)
		{
			player = 'X';
		}
		else
		{
			player = 'O';
		}

		while(validMove == false){
		cout << player  <<"'s turn, enter column > ";
		cin >> inputCol;
		
		if(board[inputCol].isFull() == 1){
			board[inputCol].addDisc(player);
		}else if(inputCol > numCol){
			cout << "Invalid spot\n";
		}else{
			board[inputCol].addDisc(player);
			validMove = true;
		}
		}

	
		winner = win();
		if(winner  == 0){
			display();
			cout << "X wins\n";
			return;
		}else if(winner == 1){
			display();
			cout << "0 wins\n";
			return;
		}
		turncount++;
	}
}
//this checks the winner, it runs through twice once for each player
//it checks for a horizantal win by moving bewteen columns for the winner
//vertical is checked by moving through through the rows
//the diagnols are checked both moving upwards and downwards
//moving through rows and columns each check
//it will return an int determining the winner
int C4Board::win(){

	int counter = 0;
	char player;
	do{
		if(counter == 0){
			player = 'X';
		}else if(counter == 1){
			player = 'O';
		}
		else{
			return 2;
		}
		//horizontal win check
		for (int column = 0; column < 4; column++)
		{
			for (int row = 0; row < board[column].getMaxDiscs(); row++)
			{

				if (board[column].getDisc(row) == player)
				{
					for (int winCheck = 1; winCheck < 4; winCheck++)
					{
						if (board[column + winCheck].getDisc(row) == player)
						{
							if (winCheck == 3)
							{
								if (player == 'X')
								{
									return 0;
								}
								else if (player == 'O')
								{
									return 1;
								}
							}
						}
						else
						{
							break;
						}
					}
				}
			}
		}
		// vertical win check
		for (int column = 0; column < numCol; column++)
		{
			for (int row = 0; row < board[column].getMaxDiscs(); row++)
			{
				if (board[column].getDisc(row) == player)
				{
					for (int winCheck = 1; winCheck < 4; winCheck++)
					{
						if (board[column].getDisc(row + winCheck) == player)
						{
							if (winCheck == 3)
							{
								if (player == 'X')
								{
									return 0;
								}
								else if (player == 'O')
								{
									return 1;
								}
							}
						}
						else
						{
							break;
						}
					}
				}
			}
		}
		// diag down win
		for (int column = 0; column < 4; column++)
		{
			for (int row = board[column].getMaxDiscs(); row >= 0; row--)
			{
				if (board[column].getDisc(row) == player)
				{
					for (int winCheck = 1; winCheck < 4; winCheck++)
					{
						if (board[column + winCheck].getDisc(row - winCheck) == player)
						{
							if (winCheck == 3)
							{
								if (player == 'X')
								{
									return 0;
								}
								else if (player == 'O')
								{
									return 1;;
								}
							}
						}
						else
						{
							break;
						}
					}
				}
			}
		}
		//diag up win
		for (int column = 0; column < 4; column++)
		{
			for (int row = 0; row < board[column].getMaxDiscs(); row++)
			{
				if (board[column].getDisc(row) == player)
				{
					for (int winCheck = 1; winCheck < 4; winCheck++)
					{
						if (board[column + winCheck].getDisc(row + winCheck) == player)
						{
							if (winCheck == 3)
							{
								if (player == 'X')
								{
									return 0;
								}
								else if (player == 'O')
								{
									return 1;
								}
							}
						}
						else
						{
							break;
						}
					}
				}
			}
		}
		counter++;
	}while(counter < 3);
}
