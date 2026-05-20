/*Connor Riley
COSC 594 HW2
linear space end gap free alignment algorithm 
*/
#include <algorithm>
#include <fstream>
#include <iostream>
using namespace std;
int main(int argc, char* argv[]) {
  // if files are passed theyu are used, otherwise default to
  // human and drosophila sequences
  string file1 = (argc == 3) ? argv[1] : "neander_sample.fasta";
  string file2 = (argc == 3) ? argv[2] : "human_mito.fasta";
  // open and error check files
  ifstream fin1;
  fin1.open(file1);
  if (!fin1.good()) {
    perror("Error opening drosophila.fasta\n");
    return -1;
  }
  ifstream fin2;
  fin2.open(file2);
  if (!fin2.good()) {
    perror("Error opening second file\n");
    return -1;
  }
  // read line by line, skipping the header
  string seq1 = "";
  string seq2 = "";
  string line;
  bool skip = true;
  while (getline(fin1, line)) {
    if (skip) {
      skip = false;
      continue;
    }
    seq1 += line;
  }
  skip = true;
  while (getline(fin2, line)) {
    if (skip) {
      skip = false;
      continue;
    }
    seq2 += line;
  }
  fin1.close();
  fin2.close();

  int size_seq1 = seq1.size();
  int size_seq2 = seq2.size();
  //only need the previous and current row. intialize them all to 0
  vector<int> prev(size_seq1+1, 0);
  vector<int> current(size_seq1+1, 0);
  
  //scoring system
  int match = 2;
  int mismatch_penalty = -1;
  int gap_penalty = -2;
  int diag_score;
  int horz_score;
  int vert_score;
  int max_score = 0;
  //loop through row calculating score based on the previous and current rows
  for (int i = 1; i <= size_seq2; i++) {
    for (int j = 1; j <= size_seq1; j++) {
      int diag = prev[j - 1];
      int left = current[j - 1] ;
      int up = prev[j];

      diag_score = diag + (seq1[j-1] == seq2[i-1] ? match : mismatch_penalty);
      horz_score = gap_penalty + left;
      vert_score = gap_penalty + up;
      current[j] = max({diag_score, horz_score, vert_score});
      if(current[j] > max_score && (j == size_seq1 || i == size_seq2)){
        max_score = current[j];
        //cout << max_score << "\n";
      }
    }
    //swap the vectors and clear the current one
    swap(prev, current);
    fill(current.begin(), current.end(), 0);

  }
  cout << max_score << "\n";

}