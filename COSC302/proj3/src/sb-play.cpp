/**sb-play.cpp
 * Connor Riley
 * 2023-03-08
 * COSC302 Dr. Emrich
 * This program builds off of sb-analyze and chooses the best move possible
 */
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>
#include "disjoint_set.hpp"
using namespace std;
using plank::Disjoint_Set;

#define talloc(type, num) (type *)malloc(sizeof(type) * (num))

class Superball
{
public:
  Superball(int argc, char **argv);
  Disjoint_Set *d; // may need to be pointer
  int r;
  int c;
  int mss;
  int empty;
  vector<int> board;
  vector<int> goals;
  vector<int> colors;
  vector<int> goalScoreIndex;
  vector<int> toSmallVectorFindI;
  map<int, int> goalCoordinate;
  map<int, int> goalMap;
  map<int, int> toSmallMap;
  multimap<int, int> toSmallSizeOrder;
  map<int, int>::iterator itr;
  map<int, int>::iterator itrThe2nd;
  multimap<int, int>::iterator itrThe3rd;
  void unionizeTheBoardForce(int index);
  void analyzeSuperBallSwap
();
};

void usage(const char *s)
{
  fprintf(stderr, "usage: sb-play rows cols min-score-size colors\n");
  if (s != NULL)
    fprintf(stderr, "%s\n", s);
  exit(1);
}

Superball::Superball(int argc, char **argv)
{
  int i, j;
  string s;

  if (argc != 5)
    usage(NULL);

  if (sscanf(argv[1], "%d", &r) == 0 || r <= 0)
    usage("Bad rows");
  if (sscanf(argv[2], "%d", &c) == 0 || c <= 0)
    usage("Bad cols");
  if (sscanf(argv[3], "%d", &mss) == 0 || mss <= 0)
    usage("Bad min-score-size");

  colors.resize(256, 0);

  for (i = 0; i < strlen(argv[4]); i++)
  {
    if (!isalpha(argv[4][i]))
      usage("Colors must be distinct letters");
    if (!islower(argv[4][i]))
      usage("Colors must be lowercase letters");
    if (colors[argv[4][i]] != 0)
      usage("Duplicate color");
    colors[argv[4][i]] = 2 + i;
    colors[toupper(argv[4][i])] = 2 + i;
  }

  board.resize(r * c);
  goals.resize(r * c, 0);

  d = new Disjoint_Set();
  d->Initialize(r * c);
  empty = 0;

  for (i = 0; i < r; i++)
  {
    if (!(cin >> s))
    {
      fprintf(stderr, "Bad board: not enough rows on standard input\n");
      exit(1);
    }
    if (s.size() != c)
    {
      fprintf(stderr, "Bad board on row %d - wrong number of characters.\n", i);
      exit(1);
    }
    for (j = 0; j < c; j++)
    {
      if (s[j] != '*' && s[j] != '.' && colors[s[j]] == 0)
      {
        fprintf(stderr, "Bad board row %d - bad character %c.\n", i, s[j]);
        exit(1);
      }
      board[i * c + j] = s[j];
      if (board[i * c + j] == '.')
        empty++;
      if (board[i * c + j] == '*')
        empty++;
      if (isupper(board[i * c + j]) || board[i * c + j] == '*')
      {
        goals[i * c + j] = 1;
        board[i * c + j] = tolower(board[i * c + j]);
      }
    }
  }
}
//This is the same as sb-analyze union, puts all the tiles into unions of the same color
//if the tiles are touching
void Superball::unionizeTheBoardForce(int index)
{
  int right = 0;
  int down = 0;
  int curr = board[index];
  if ((index % c) != (c - 1)) 
  {
    right = board[index + 1];
  }
  if ((index + c) < r * c)
  {
    down = board[index + c];
  }
  for (int i = 0; i < r * c; i++)
  {
    if (curr == right && index % c != c - 1 && right != 0 && d->Find(index) != d->Find(index + 1))
    {
      d->Union(d->Find(index), d->Find(index + 1));
    }
    if (curr == down && (index + c) < (r * c) && down != 0 && d->Find(index) != d->Find(index + c))
    {
      d->Union(d->Find(index), d->Find(index + c));
    }
  }
  
}

/**
 * analyzeSuperBallSwap decides the best possible move, it does so by checking if any can be scored, 
 * otherwise it attempts to swap a square to be next to the largest unscorable set on the board in a goal 
 * if it cant do either then it swaps the first 2 differing colors it sees. like analyze it places the scorable
 * sets in a map, but now also adds the sets that arent large enough to score.
 */
void Superball::analyzeSuperBallSwap()
{
  int r1, c1, r2, c2;
  int maxSize = 0, scoreFindIndex, largestUnscorable;
  int colorToSwap;
  vector<int> sizes = *(d->Get_Sizes());
  for (int i = 0; i < r * c; i++)
  {
    if (goals[i] == 0)
    {
      continue;
    }
    if (board[i] != '*' && goals[i] == 1)
    {

      if (sizes[d->Find(i)] >= mss)
      {
        if (goalMap.find(d->Find(i)) == goalMap.end())
        {
          goalCoordinate.insert(make_pair(d->Find(i), i));
          goalMap.insert(make_pair(d->Find(i), sizes[d->Find(i)]));
        }
      }
      else
      {
        if (toSmallMap.find(d->Find(i)) == toSmallMap.end())
        {
          toSmallMap.insert(make_pair(d->Find(i), sizes[d->Find(i)]));
        }
      }
    }
  }
  //it first will attempt to score the largest set that is in a goal position.
  if (!goalMap.empty())
  {
    for (itr = goalMap.begin(); itr != goalMap.end(); itr++)
    {
      if (maxSize < itr->second)
      {
        maxSize = itr->second;
        scoreFindIndex = itr->first;
      }
    }
    for (itrThe2nd = goalCoordinate.begin(); itrThe2nd != goalCoordinate.end(); itrThe2nd++)
    {
      if (itrThe2nd->first == scoreFindIndex)
      {
        cout << "SCORE " << itrThe2nd->second / c << " " << itrThe2nd->second % c << "\n";
      }
    }
  }
  //if it cant score then we check for the largest unscorable set.
  else if (!toSmallMap.empty())
  { 
    bool canSwap = false;
    bool canSwap2 = false;
    int swap1Index;
    for (itr = toSmallMap.begin(); itr != toSmallMap.end(); itr++) // toSmallMap<find(i), int size>
    {
      toSmallSizeOrder.insert(make_pair(itr->second, itr->first)); // toSmallSizeOrder<int size, find(i)> //this way we can sort
    }

    for (itrThe3rd = toSmallSizeOrder.begin(); itrThe3rd != toSmallSizeOrder.end(); itrThe3rd++)
    {
      toSmallVectorFindI.push_back(itrThe3rd->second); //a vector of find(i) in order of the size.
    }
    //We loop through the unscorable sets from largest to smallest size until a swap can be made.
    if (toSmallVectorFindI.size() != 0)
    {
      for (int vectLoop = toSmallVectorFindI.size() - 1; vectLoop >= 0; vectLoop--)
      {
        //reset the 2 swap bools to false, and set the largestunscorable to find(i) of the set we trying to swap to
        //set the color we are looking for as the same color at find(i), We then loop through the board to find a tile that 
        //is the desired color and isnt already part of the set we are trying to add it onto. If so then the first set of
        //coordinates is equal to the the tile
        canSwap = false; 
        canSwap2 = false; 
        largestUnscorable = toSmallVectorFindI[vectLoop];
        colorToSwap = board[toSmallVectorFindI[vectLoop]];
        for (int i = 0; i < r * c; i++)
        {
          if (colorToSwap == board[i] && d->Find(i) != largestUnscorable)
          {
            r1 = i / c;
            c1 = i % c;
            swap1Index = i;
            canSwap = true;
            break;
          }
        }
        
        //We then begin to search for the 2nd set of coordinates we attempt to swap with. to do this
        //we loop though the board and try to find filled adjacent tiles next to the desired set we are trying to increase.
        //first if we are at an index in the set then we look to the right to see if we its filled by a tile not in the set
        for (int i = 0; i < r * c; i++)
        {
          if (d->Find(i) == largestUnscorable)
          {
            if(i % c != c - 1 ){
              if(d->Find(i + 1) != largestUnscorable && (i % c) != (c - 1) && board[i] != '*' && board[i] != '.'){
                r2 = ((i + 1) / c);
                c2 = ((i+1) % c);
              }
            }
            continue;
          }
          //we then check if the index to the right or below is part of the set. if so and the current index is is filled but 
          //has a different color then the 2nd coordinates have been found.
          if (i != (r * c) - 1)
          {
            if (d->Find(i + 1) == largestUnscorable && (i % c) != (c - 1) && board[i] != '*' && board[i] != '.' && r1 != i /c && c1 != i % c && board[i] != colorToSwap) //&& board[i] != colorToSwap
            {
              r2 = (i / c); // if the cell to the right is equal to find(i) (largest unscorable), isnt on the right most column, and it
              c2 = (i % c); // isnt empty then we found the swap
              canSwap2 = true;
              break;
            }
          }
          if ((i + c) < (r * c))
          {
            if (d->Find(i + c) == largestUnscorable && (i + c) < (r * c) && board[i] != '*' && board[i] != '.' && r1 != i /c && c1 != i % c && board[i] != colorToSwap) //&& board[i] != colorToSwap
            {
              r2 = (i / c); // if the cell below is equal to find(i), not below the bottom row,  and isnt empty then we found swap
              c2 = (i % c);
              canSwap2 = true;
              break;
            }
          }
        }
        //if we found both a first and second set of coordinates then we call swap on them
        //if not we loop again onto the next set seeing if there is a possible swap. if we run out of
        //sets then the first 2 tiles that are different colors will be swapped.
        if (canSwap == true && canSwap2 == true)
        {
          cout << "SWAP " << r1 << " " << c1 << " " << r2 << " " << c2;
          break;
        }
        if (vectLoop == 0)
        {
          canSwap = false;
          canSwap2 = false;
        }
        if (canSwap == false && canSwap2 == false)
        {
          int s1index;
          for (int i = 0; i < r * c; i++)
          {
            if (board[i] != '*' && board[i] != '.')
            {
              r1 = i / c;
              c1 = i % c;
              s1index = i;
              break;
            }
          }
          for (int i = s1index + 1; i < r * c; i++)
          {
            if (board[i] != '*' && board[i] != '.' && (board[i] != board[s1index] || i > 40))
            {
              r2 = i / c;
              c2 = i % c;
              break;
            }
          }
          cout << "SWAP " << r1 << " " << c1 << " " << r2 << " " << c2;
          break;
        }
      }
    }
  }
  //if no tiles are in the scoring then we swap the first 2 differeing colors and call swap.
  else
  {
    int s1index;
    for (int i = 0; i < (r * c); i++)
    {
      if (board[i] != '*' && board[i] != '.')
      {
        r1 = i / c;
        c1 = i % c;
        s1index = i;
        break;
      }
    }
    for (int i = s1index + 1; i < (r * c); i++)
    {
      if (board[i] != '*' && board[i] != '.' && (board[i] != board[s1index] || i > 40))
      {
        r2 = i / c;
        c2 = i % c;
        break;
      }
    }
    cout << "SWAP " << r1 << " " << c1 << " " << r2 << " " << c2;
  }
}

//to improve this, I could attempt a color hiarchy to swap to the goal sets rather than size, could also instead
//of swapping the 2 random tiles i could find the largest set and try to add onto it even if it isnt in the scoring set.
int main(int argc, char **argv)
{
  Superball *s;

  s = new Superball(argc, argv);
  for (int i = 0; i < s->board.size(); i++)
  {
    s->unionizeTheBoardForce(i);
  }
  s->analyzeSuperBallSwap();
}


//listening to Paranoid - Black Sabbath
