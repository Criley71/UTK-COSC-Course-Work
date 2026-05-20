/*Connor Riley
COSC 594 Fall 2025 HW2
This implements a global alignment of 2 dna sequences
using the needleman wunsch algorithm
*/
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char* argv[]) {
  // if files are passed they are used, otherwise default to
  // human and drosophila sequences
  string file1 = (argc == 3) ? argv[1] : "drosophila.fasta";
  string file2 = (argc == 3) ? argv[2] : "human.fasta";
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

  // seq1 = "GTCGACGCA";
  // seq2 = "GATTACA";

  // The matrix will be the size of seq1 + 1 by the size of seq 2 + 1
  // initialize all values to zero
  int size_seq1 = seq1.size();
  int size_seq2 = seq2.size();
  vector<vector<int>> align_matrix;
  align_matrix.resize(size_seq1 + 1);
  for (int i = 0; i < size_seq1 + 1; i++) {
    align_matrix[i].resize(size_seq2 + 1, 0);
  }
  // scoring system
  // initialize first column and row to index * gap_penalty
  int match = 2;
  int mismatch_penalty = -1;
  int gap_penalty = -2;
  for (int i = 0; i <= size_seq1; i++) {
    align_matrix[i][0] = i * gap_penalty;
  }
  for (int i = 0; i <= size_seq2; i++) {
    align_matrix[0][i] = i * gap_penalty;
  }

  // align_matrix[0].size() = size_seq1+1 (seq1 will be horizontal)
  // align_matrix.size() = size_seq2+1 (seq2 will be vertical)

  // loop through the matrix checking each cell for optimal scoring between
  // missmatching, matching or having a gap.
  int diag_score;
  int horz_score;
  int vert_score;
  for (int i = 1; i <= size_seq1; i++) {
    for (int j = 1; j <= size_seq2; j++) {
      diag_score = align_matrix[i - 1][j - 1] + (seq1[i - 1] == seq2[j - 1] ? match : mismatch_penalty);
      horz_score = align_matrix[i][j - 1] + gap_penalty;
      vert_score = align_matrix[i - 1][j] + gap_penalty;
      align_matrix[i][j] = max({diag_score, horz_score, vert_score});
    }
  }
  // start bottom right and check if diag + match equals current cells value
  // or if the left or top cell + gap equals the current cells value
  // if so this is part of the optimal alignment
  // if its a diag then no gap is added, just the 2 current values in the sequences
  // if its up then seq 2 gets a gap
  // if its left then seq 1 gets a gap
  string align_seq1 = "";
  string align_seq2 = "";
  int i = size_seq1;
  int j = size_seq2;

  while (i > 0 || j > 0) {
    if (i > 0 && j > 0) {
      diag_score = align_matrix[i - 1][j - 1] + (seq1[i - 1] == seq2[j - 1] ? match : mismatch_penalty);
      if (align_matrix[i][j] == diag_score) {
        align_seq1 = seq1[i - 1] + align_seq1;
        align_seq2 = seq2[j - 1] + align_seq2;
        i -= 1;
        j -= 1;
        continue;
      }
    }
    if (i > 0) {
      vert_score = align_matrix[i - 1][j] + gap_penalty;
      if (align_matrix[i][j] == vert_score) {
        align_seq1 = seq1[i - 1] + align_seq1;
        align_seq2 = "-" + align_seq2;
        i -= 1;
        continue;
      }
    }
    if (j > 0) {
      horz_score = align_matrix[i][j - 1] + gap_penalty;
      if (align_matrix[i][j] == horz_score) {
        align_seq1 = "-" + align_seq1;
        align_seq2 = seq2[j - 1] + align_seq2;
        j -= 1;
        continue;
      }
    }
    break;
  }
  int width = 100;
  double match_count = 0;
  double gap_count = 0;
  double mm_count = 0;
  // loop through 100 characters at a time, to show alignment beyween the 2 sequences
  for (int i = 0; i < static_cast<int>(align_seq1.length()); i += width) {
    string seq1_substr = align_seq1.substr(i, width);
    string seq2_substr = align_seq2.substr(i, width);

    string match_string = "";
    for (int j = 0; j < static_cast<int>(seq1_substr.size()); j++) {
      if (seq1_substr[j] == seq2_substr[j] && seq1_substr[j] != '-') {
        if (seq1_substr[j] == '\0') continue;
        match_string += "\033[32m|\033[0m";  // if match print green |
        match_count+=1;
      } else if (seq1_substr[j] == '-' || seq2_substr[j] == '-') {
        match_string += "\033[33m-\033[0m";  // if gap match print yellow '-'
        gap_count += 1;
      } else {
        match_string += "\033[31mX\033[0m";  // if mismatch print red X
        mm_count += 1;
      }
    }

    cout << "Droso: " << seq1_substr << "\n";
    cout << "MATCH: " << match_string << "\n";
    cout << "Human: " << seq2_substr << "\n\n";
  }
  // score is in the bottom right of the matrix
  // print out statistics
  cout << "Alignment Score: " << align_matrix[size_seq1][size_seq2] << "\n";
  cout << "Number of matches:    " << match_count << "/" << align_seq1.size()<< " (" << ((match_count)/align_seq1.size()) *100 << "%)\n";
  cout << "Number of gaps:       " << gap_count << "/" << align_seq1.size() << " (" << ((gap_count)/align_seq1.size()) *100 << "%)\n";
  cout << "Number of mismatches: " << mm_count << "/" << align_seq1.size() << " (" << ((mm_count)/align_seq1.size()) *100 << "%)\n";
}