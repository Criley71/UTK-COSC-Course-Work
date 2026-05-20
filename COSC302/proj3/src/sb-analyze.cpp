/* sb-analyze.cpp
 * Connor Riley
 * 2023-03-08
 * This program returns the possible scoring sets in the SuperBall Game
 * it does it through making disjoint sets of tiles on the game board 
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
  //added onto the superball class is the disjoint set, the maps/iterators and the 
  //unionizeTheBoardForce and analyzeSuperBall functions
  Superball(int argc, char **argv);
  Disjoint_Set *d;
  int r;
  int c;
  int mss;
  int empty;
  vector<int> board;
  vector<int> goals;
  vector<int> colors;
  vector<int> goalScoreIndex;
  map<int, int> goalCoordinate;
  map<int, int> goalMap;
  map<int, int>::iterator itr;
  map<int, int>::iterator itr2;
  void unionizeTheBoardForce(int index);
  void analyzeSuperball();
};

void usage(const char *s)
{
  fprintf(stderr, "usage: sb-read rows cols min-score-size colors\n");
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
  //Here i create d as a disjoint set and initialize it to the size of the board, r * c
  //initialize puts all individual tiles in there own set so union can be called on them
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

/**
 * unionizeTheBoardForce is a function that takes in the index of a tile on the board.
 * its purpose is to go through the board and make unions of sets that are touching
 * It does so by iterating through the board to the right and down. If a tile to the
 * right or below are the same color union is called on them.  
 */
void Superball::unionizeTheBoardForce(int index)
{
  int right = 0;
  int down = 0;
  int curr = board[index];
  //prevents from checking the right most column from looking to the right to union
  if ((index % c) != (c - 1))
  {
    right = board[index + 1];
  }
  //prevents the bottom row from looking downward to union
  if ((index + c) < r * c)
  {
    down = board[index + c];
  }
  //iterate through the board and call union if the same color, and arent already part of the same set
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
 * analyzeSuperball is a function that will print out all the possible scoring sets 
 * It does so by iterating though the board, checking if the set is in a goal position and larger 
 * than the mininum scoring size. if so the set is put into 2 maps, one storing the goal index and the other
 * storing the sets size. Then will print out the scoring sets if there are any.
 */
void Superball::analyzeSuperball()
{
  vector<int> sizes = *(d->Get_Sizes());
  for (int i = 0; i < r * c; i++)
  {
    if (goals[i] == 0)
    {
      continue;
    }
    if (board[i] != '*')
    {

      if (sizes[d->Find(i)] >= mss)
      {

        if (goalMap.find(d->Find(i)) == goalMap.end())
        {
          goalCoordinate.insert(make_pair(d->Find(i), i));//setid, index
          goalMap.insert(make_pair(d->Find(i), sizes[d->Find(i)]));//setid, size
        }
      }
    }
  }
  cout << "Scoring sets:\n";
  if (!goalMap.empty())
  {
    for (itr = goalMap.begin(); itr != goalMap.end(); itr++)
    {
      if(itr->second < 10){
      cout << "Size:  " << itr->second << "  Char: " << (char)board[itr->first] << "  Scoring Cell: ";
    }
    else{
      cout << "Size: " << itr->second << "  Char: " << (char)board[itr->first] << "  Scoring Cell: ";
    }
      for(itr2 = goalCoordinate.begin(); itr2 != goalCoordinate.end(); itr2++){
        if(itr2->first == itr->first){
          cout << itr2->second / c << "," << itr2->second % c << "\n";
        }
      }
    }
  }
}
//run union on the whole board, and call analyze to print the possible scoring sets
int main(int argc, char **argv)
{
  Superball *s;

  s = new Superball(argc, argv);
  for (int i = 0; i < s->board.size(); i++)
  {
    s->unionizeTheBoardForce(i);
  }
  s->analyzeSuperball();
  
  return 0;
}
