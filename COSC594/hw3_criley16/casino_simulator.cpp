/*Connor Riley HW3 COSC 594
generates a random set of 300 dice rolls
where it may swap to a loaded dice or to fair dice
after every roll
*/
#include <iostream>
#include <random>
#include <time.h>
#include <vector>
#include <fstream>
using namespace std;

int main() {
  srand(time(0));
  double sixth = 1.0 / 6.0;
  vector<char> dice_numbers = {'1', '2', '3', '4', '5', '6'};            // for printing dice roll
  vector<double> fair_dice = {sixth, sixth, sixth, sixth, sixth, sixth}; // chances for fair dice
  vector<double> loaded_dice = {.1, .1, .1, .1, .1, .5};                 // chances for loaded dice
  int rand_fair_dice_roll = rand() % 6;                                  // get numbers 0 through 5
  int rand_load_dice_roll = rand() % 10;                                 // get numbers 0 through 9, 5-9 are a 6-roll
  int swap_roll = rand() % 100;
  bool is_on_fair_dice = true; // start on fair
  string dice_choice = "";
  string dice_out = "";
  ofstream fout("rolls.txt");
  for (int i = 0; i < 300; i++) {
    if (is_on_fair_dice) {
      rand_fair_dice_roll = rand() % 6;
      dice_choice += 'F';                            // add that it was a fair dice
      dice_out += dice_numbers[rand_fair_dice_roll]; // add dice roll to string
    } else {
      rand_load_dice_roll = rand() % 10;
      if (rand_load_dice_roll > 4) { // if greater than 4 then it was a 6
        rand_load_dice_roll = 5;
      }
      dice_choice += 'L';                            // add that it was loaded
      dice_out += dice_numbers[rand_load_dice_roll]; // add the dice roll
    }
    swap_roll = rand() % 100;             // swap chance out of 100
    if (swap_roll < 5) {                  // if 0-4 then we swap as its a .05 chance to swap
      is_on_fair_dice = !is_on_fair_dice; //(i know the example uses .1 for load to fair but the write up says to do this way)
    }
    if ((i % 100 == 0 && i != 0) || i == 299) { //output 100 rolls per line
      cout << dice_choice << "\n";
      cout << dice_out << "\n";
      cout << "\n";
      fout << dice_choice << "\n" << dice_out << "\n\n";
      dice_choice = "";
      dice_out = "";
    }
  }
}