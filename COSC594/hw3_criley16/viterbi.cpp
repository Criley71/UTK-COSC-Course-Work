/*Connor Riley HW3
Implements the viterbi algorithm
for dishonest casino
*/
#include <fstream>
#include <iostream>
#include <math.h>
#include <time.h>
#include <vector>
using namespace std;

void viterbi(string file, string out) {
  // read in file and error check
  ifstream fin(file);
  if (!fin.good()) {
    perror("error");
    exit(1);
  }
  string buffer = "";
  string dice_rolls;
  while (getline(fin, buffer)) {
    dice_rolls += buffer;
  }
  fin.close();

  int num_rolls = dice_rolls.size();
  vector<pair<double, string>> fair;   // double is the calculated value.
  vector<pair<double, string>> loaded; // string will say if the previous was fair or loaded
  fair.resize(1 + num_rolls);          // need extra index for start values
  loaded.resize(1 + num_rolls);
  fair[0] = {1.0, "start"};
  loaded[0] = {0.0, "start"};
  double sixth = 1.0 / 6.0;
  vector<double> fair_chances = {sixth, sixth, sixth, sixth, sixth, sixth};
  vector<double> loaded_chances = {.1, .1, .1, .1, .1, .5};
  for (int i = 1; i <= num_rolls; i++) {
    int roll = dice_rolls[i - 1] - '0';               // get roll value
    double fair_roll_chance = fair_chances[roll - 1]; // get chance of roll for each dice type
    double loaded_roll_chance = loaded_chances[roll - 1];

    double fair_to_fair = fair[i - 1].first * .95 * fair_roll_chance;    // chance for fair to fair
    double loaded_to_fair = loaded[i - 1].first * .1 * fair_roll_chance; // chance for loaded to fair
    if (fair_to_fair > loaded_to_fair) {
      fair[i] = {fair_to_fair, "fair"}; // if fair was greater than keep fair and say it came from fair
    } else {
      fair[i] = {loaded_to_fair, "load"};// else loaded was greater and set loaded as previous
    }
    //do same thing for loaded
    double loaded_to_loaded = loaded[i - 1].first * .9 * loaded_roll_chance;
    double fair_to_loaded = fair[i - 1].first * .05 * loaded_roll_chance;
    if (loaded_to_loaded > fair_to_loaded) {
      loaded[i] = {loaded_to_loaded, "load"};
    } else {
      loaded[i] = {fair_to_loaded, "fair"};
    }
  }
  //traceback
  string trace_back = "";
  string state = "";
  if (loaded[num_rolls].first > fair[num_rolls].first) {
    state = "load";
  } else {
    state = "fair";
  }
  //just work backwards
  for (int i = num_rolls; i > 0; i--) {
    if (state == "fair") {
      trace_back = 'F' + trace_back;
      state = fair[i].second;
    } else {
      trace_back = 'L' + trace_back;
      state = loaded[i].second;
    }
  }
  //output to file
  ofstream fout(out);
  if (!fout.good()) {
    perror("Error");
    return;
  }
  fout << dice_rolls << "\n"
       << trace_back;
}

int main() {
  viterbi("casino.benchmark1.txt", "viterbi.1.txt");
  viterbi("casino.benchmark2.txt", "viterbi.2.txt");
  return 0;
}