/*
Connor Riley
criley16@vols.utk.edu
*/
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

int main() {
  ifstream fin("small.fasta");
  if (!fin.good()) {
    perror("Error opening file");
  }
  vector<vector<int>> distance_matrix = {
      {0, 0, 0, 0, 0},  // EU980381
      {0, 0, 0, 0, 0},  // EU980380
      {0, 0, 0, 0, 0},  // EU980379
      {0, 0, 0, 0, 0},  // EU980378
      {0, 0, 0, 0, 0}}; // EU980377

  string strand1 = "";
  string strand2 = "";
  string strand3 = "";
  string strand4 = "";
  string strand5 = "";
  string line;
  int header_count = 0;

  while (getline(fin, line)) {
    if (line[0] == '>') {
      header_count += 1;
      continue;
    }
    switch (header_count) {
    case 1:
      strand1 += line;
      break;
    case 2:
      strand2 += line;
      break;
    case 3:
      strand3 += line;
      break;
    case 4:
      strand4 += line;
      break;
    case 5:
      strand5 += line;
      break;
    default:
      break;
    }
  }

  string compare_strand_1;
  string compare_strand_2;
  int diff = 0;
  for (int i = 0; i < static_cast<int>(distance_matrix.size()); i++) {
    switch (i) {
    case 0:
      compare_strand_1 = strand1;
      break;
    case 1:
      compare_strand_1 = strand2;
      break;
    case 2:
      compare_strand_1 = strand3;
      break;
    case 3:
      compare_strand_1 = strand4;
      break;
    case 4:
      compare_strand_1 = strand5;
      break;
    }
    for (int j = i; j < static_cast<int>(distance_matrix[0].size()); j++) {
      diff = 0;
      if (j == i) {
        distance_matrix[i][j] = 0;
        distance_matrix[j][i] = 0;
        continue;
      }
      switch (j) {
      case 0:
        compare_strand_2 = strand1;
        break;
      case 1:
        compare_strand_2 = strand2;
        break;
      case 2:
        compare_strand_2 = strand3;
        break;
      case 3:
        compare_strand_2 = strand4;
        break;
      case 4:
        compare_strand_2 = strand5;
        break;
      }
      for (int k = 0; k < static_cast<int>(compare_strand_1.length()); k++) {
        if (compare_strand_1[k] != compare_strand_2[k]) {
          diff += 1;
        }
      }
      distance_matrix[i][j] = diff;
      distance_matrix[j][i] = diff;
    }
  }
  cout << "\x1B[4m" << "    S-G |S-F |S-E |S-D |S-C |" << "\n";

  for (int i = 0; i < static_cast<int>(distance_matrix.size()); i++) {
    switch (i) {
    case 0:
      cout << "S-G|";
      break;
    case 1:
      cout << "S-F|";
      break;
    case 2:
      cout << "S-E|";
      break;
    case 3:
      cout << "S-D|";
      break;
    case 4:
      cout << "S-C|";
      break;
    }
    for (int j = 0; j < static_cast<int>(distance_matrix[0].size()); j++) {
      cout << setw(3) << setfill(' ') << distance_matrix[i][j] << " |";
    }
    cout << "\n";
  }
  cout << "\x1B[0m";
  // cout << strand1 << "\n\n" << strand2 << "\n\n" << strand3 << "\n\n" << strand4 << "\n\n" << strand5 << "\n";
}