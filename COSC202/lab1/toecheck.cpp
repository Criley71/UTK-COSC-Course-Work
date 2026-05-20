// Lab 1:  Tic-tac-toe solver (as requested by awesome grad TA grader Tom)
/*Connor Riley
9/5/2022
*/
#include <iostream>

using namespace std;

int main()
{
  /**
   * make variables for the board size, game win conditions,
   * whos turn it is, turn counter index and the game board being a 2d array.
   * takes in the size and then begins filling the board with the given inputs
   */
  int size, loopCount = 0;
  bool Owin = false, Xwin = false;
  cin >> size;
  char move, board[size][size];
  int index = 0;

    while (cin >> move)
    {
      if (move == 'X' || move == 'O' || move == '-')
      {
        board[index / size][index % size] = move;
        index++;
      }
    }
	
	while(loopCount != 2){
   if(loopCount == 0){
		move = 'X';
   }else if(loopCount == 1){ 
		move = 'O';
   }
		
		/*
        This will check if the player has won by a vertical column filled. it is a nested for loop,
        with the outer only checking the top row and if a player is in that spot it will check below it
        using the inner for loop and will break if the spot isnt the player and if it reaches the bottom
        and it has the same player, it will set a winner depending on who's turn it was.
        */
    for (int column = 0; column < size; column++)
    {
      if (board[0][column] == move)
      {
        for (int row = 1; row < size; row++)
        {
          if (board[row][column] != move)
          {
            break;
          }
          else if (board[row][column] == move)
          {

            if (row == size - 1)
            {

              if (move == 'X')
              {
                Xwin = true;
              }
              else if (move == 'O')
              {
                Owin = true;
              }
            }
          }
        }
      }
    }
    /*
        This will check if the player has won by a horizontal row filled. it is a nested for loop,
        with the outer only checking the laft most columm and if a player is in that spot it will check to the right of it
        using the inner for loop and will break if the spot isnt the player but if it reaches the the right most column and is the
        same player it will set a winner depending on who's turn it was.
        */
    for (int row = 0; row < size; row++)
        {
            if (board[row][0] == move)
            {
                for (int column = 1; column < size; column++)
                {
                    if (board[row][column] != move)
                    {

                        break;
                    }
                    else if (board[row][column] == move)
                    {

                        if (column == (size - 1))
                        {

                            if (move == 'X')
                            {
                                Xwin = true;
                            }
                            else if (move == 'O')
                            {
                                Owin = true;
                            }
                        }
                    }
                }
            }
        }

   /*
        This will check if the player has won by a diagonal from left to right. it initially checks
        the top left as it is required to win diagnally and will keep checking the next spot
        in the diaganol using a for loop and will set a winner depending on the who's turn it was.
        */
    if (board[0][0] == move)
    {
      for (int i = 1; i < size; i++)
      {
        if (board[i][i] == move)
        {
          if (i == size - 1)
          {
            if (move == 'X')
            {
              Xwin = true;
            }
            else if (move == 'O')
            {
              Owin = true;
            }
          }
        }
        else if (board[i][i] != move)
        {
          break;
        }
      }
    }

     /*
        This will check if the player has won by a diagonal from right to left. it initially checks
        the top right as it is required to win diagnally and will keep checking the next spot
        in the diaganol using a for loop and will set a winner depending on the who's turn it was.
        */
    if (board[0][size - 1] == move)
    {
      for (int i = 1; i < size; i++)
      {
        if (board[i][(size - 1) - i] == move)
        {
          if (i == size - 1)
          {
            if (move == 'X')
            {
              Xwin = true;
            }
            else if (move == 'O')
            {
              Owin = true;
            }
          }
        }
        else if (board[i][(size - 1) - i] != move)
        {
          break;
        }
      }
    }
	loopCount++;
 }
  /**
   * depending on the winners condition will be printed to the console
   * if neither have won it ends in a draw.
   */
  if (Xwin == true)
  {
    cout << "X wins\n";
  }
  else if (Owin == true)
  {
    cout << "O wins\n";
  }
  else
  {
    cout << "Tie\n";
  }
  return 0;
}
