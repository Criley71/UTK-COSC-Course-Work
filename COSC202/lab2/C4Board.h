/*Connor Riley
9/12/2022
This program plays connect 4
*/
//This defines the class for C4Board.
//dont really know what else ther is to say
#include <iostream>
#include "C4Col.h"
class C4Board
{
  private:
    int numCol;
    C4Col* board;

  public:
    C4Board();
    void display();
    void play();
	int win();
};
