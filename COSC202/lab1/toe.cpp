/*Connor Riley
9/5/2022
this program is a game of tic tac toe between 2 players
*/
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    // set variables for the game ending conditions, size of the square board, the player identifier and
    // make the 2d array from the size of board taken as input from the user
    int gridSize, turnCount = 0, columnIn, rowIn;
    bool Owin = false, Xwin = false, spotFilled = false, gridFull = false;
    char player;
    cout << "Enter grid size: ";
    cin >> gridSize;
    char board[gridSize][gridSize];
    // create the board using nested for loops for the 2d array
    // and fills the board with '-'
    for (int i = 0; i < gridSize; i++)
    {
        for (int j = 0; j < gridSize; j++)
        {
            board[i][j] = '-';
        }
    }

    // set the conditions to stop the game when a player wins or the board is filled
    while (Owin == false && Xwin == false && gridFull == false)
    {
        // turn count will determine the player's turn and will change after each move
        if (turnCount % 2 == 0)
        {
            player = 'X';
        }
        else if (turnCount % 2 == 1)
        {
            player = 'O';
        }
        // Prompt the user for where they want to enter by asking for column
        // and the row where they want to play this turn. If the spot is taken
        // or doesnt exist it will ask the player for a different spot and assign it to them.
        cout << "It is " << player << "'s turn\n";
        do
        {
            cout << "Enter column: ";
            cin >> columnIn;
            cout << "Enter row: ";
            cin >> rowIn;
            columnIn -= 1;
            rowIn -= 1;
            if (board[rowIn][columnIn] != '-')
            {
                spotFilled = true;
                cout << "Invalid spot please pick another.\n";
            }
            else
            {
                board[rowIn][columnIn] = player;
                spotFilled = false;
            }
        } while (spotFilled == true);
        /*
        This will check if the player has won by a vertical column filled. it is a nested for loop,
        with the outer only checking the top row and if a player is in that spot it will check below it
        using the inner for loop and will break if the spot isnt the player and if it reaches the bottom
        and it has the same player, it will set a winner depending on who's turn it was.
        */
        for (int column = 0; column < gridSize; column++)
        {
            if (board[0][column] == player)
            {
                for (int row = 1; row < gridSize; row++)
                {
                    if (board[row][column] != player)
                    {

                        break;
                    }
                    else if (board[row][column] == player)
                    {

                        if (row == gridSize - 1)
                        {

                            if (player == 'X')
                            {
                                Xwin = true;
                            }
                            else if (player == 'O')
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
        for (int row = 0; row < gridSize; row++)
        {
            if (board[row][0] == player)
            {
                for (int column = 1; column < gridSize; column++)
                {
                    if (board[row][column] != player)
                    {

                        break;
                    }
                    else if (board[row][column] == player)
                    {

                        if (column == (gridSize - 1))
                        {

                            if (player == 'X')
                            {
                                Xwin = true;
                            }
                            else if (player == 'O')
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
        if (board[0][0] == player)
        {
            for (int i = 1; i < gridSize; i++)
            {
                if (board[i][i] == player)
                {
                    if (i == gridSize - 1)
                    {
                        if (player == 'X')
                        {
                            Xwin = true;
                        }
                        else if (player == 'O')
                        {
                            Owin = true;
                        }
                    }
                }
                else if (board[i][i] != player)
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
        
        if (board[0][gridSize - 1] == player)
        {
            for (int i = 1; i < gridSize; i++)
            {
                if (board[i][(gridSize - 1) - i] == player)
                {
                    if (i == gridSize - 1)
                    {
                        if (player == 'X')
                        {
                            Xwin = true;
                        }
                        else if (player == 'O')
                        {
                            Owin = true;
                        }
                    }
                }
                else if (board[i][(gridSize - 1) - i] != player)
                {
                    break;
                }
            }
        }
        //this will increase the turn count to rotate the players' turn
        turnCount += 1;
        /**
         * a possible outcome is the board being filled with no winner
         * we check the turn count if its been enough turns to fill the grid
         * and will stop the game
         */
        if (turnCount == (gridSize * gridSize))
        {
            gridFull = true;
        }
        /**
         * This will print the board for the players to see
         * 
         */
        for (int i = 0; i < gridSize; i++)
        {
            for (int j = 0; j < gridSize; j++)
            {
                cout << board[i][j] << " ";
            }
            cout << "\n";
        }
    }
    /**
     * once the loop is broken it checks if either of the players have won
     * from the possible ways to win above. if neither have won its a draw
     * and it will print the outcome of the game to the user.
     */
    if (Xwin == true)
    {
        cout << "X wins\n";
    }
    else if (Owin == true)
    {
        cout << "O wins\n";
    }
    else if (gridFull == true)
    {
        cout << "Its a draw.\n";
    }
}
