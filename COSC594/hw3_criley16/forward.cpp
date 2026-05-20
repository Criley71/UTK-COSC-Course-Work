/* Connor Riley HW3
implements the forward algorithm using dishonest casino model
*/
#include <fstream>
#include <iostream>
#include <math.h>
#include <time.h>
#include <vector>
using namespace std;

void forward(string file);

int main() {
  forward("casino.benchmark1.txt");
  forward("casino.benchmark2.txt");
  return 0;
}

void forward(string file) {
  ifstream fin(file); // read in file and error check
  if (!fin.good()) {
    perror("error");
    exit(1);
  }
  string buffer = "";
  string dice_rolls;
  while (getline(fin, buffer)) {
    dice_rolls += buffer; // read line by line
  }
  fin.close();

  int num_rolls = dice_rolls.size();
  vector<double> fair;        // holds fair probabilities
  vector<double> load;        // holds loaded probabilities
  fair.resize(1 + num_rolls); // need 1 more index for starting state
  load.resize(1 + num_rolls);
  fair[0] = 1; // starts in fair always
  load[0] = 0;
  double sixth = 1.0 / 6.0;
  string check = "";
  vector<double> fair_chances = {sixth, sixth, sixth, sixth, sixth, sixth};
  vector<double> load_chances = {.1, .1, .1, .1, .1, .5};
  for (int i = 1; i <= num_rolls; i++) {
    int roll = dice_rolls[i - 1] - '0';               // gets the index of a roll
    double fair_roll_chance = fair_chances[roll - 1]; // gets fair roll chance
    double load_roll_chance = load_chances[roll - 1]; // gets loaded roll chance
    // fair = (prev_fair * .95 + prev_load * .1) * fair_roll_chance   .1 chance to swap to fair and .95 chance to stay fair
    fair[i] = (fair[i - 1] * .95 + load[i - 1] * .1) * fair_roll_chance;
    // loaded = (prev_fair * .05 + prev_load * .9) * load_roll_chance .05 chance to swap to loaded and .9 chance to stay loaded
    load[i] = (fair[i - 1] * .05 + load[i - 1] * .9) * load_roll_chance;
    // whichever is greater select that roll
    if (fair[i] > load[i]) {
      check += 'F';
    } else {
      check += 'L';
    }
  }
  // probability is the sum of last last index of loaded and fair rolls
  double probability = fair[num_rolls] + load[num_rolls];
  cout << "Probability of sequence: " << probability << "\n";
  // cout << check << "\n";
}