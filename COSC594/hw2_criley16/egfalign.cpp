#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;
int main(int argc, char* argv[]) {
  // if files are passed they are used, otherwise default to
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
  // make MxN matrix, initialize first row and column to zero
  int size_seq1 = seq1.size();
  int size_seq2 = seq2.size();
  vector<vector<int>> align_matrix;
  align_matrix.resize(size_seq1 + 1);
  for (int i = 0; i < size_seq1 + 1; i++) {
    align_matrix[i].resize(size_seq2 + 1, 0);
  }
  int match = 2;
  int mismatch_penalty = -1;
  int gap_penalty = -2;
  int diag_score;
  int horz_score;
  int vert_score;
  // check for the max value in final row or column and save coordinates
  pair<int, int> max_coord = pair<int, int>(0, 0);
  for (int i = 1; i <= size_seq1; i++) {
    for (int j = 1; j <= size_seq2; j++) {
      diag_score = align_matrix[i - 1][j - 1] + (seq1[i - 1] == seq2[j - 1] ? match : mismatch_penalty);
      horz_score = align_matrix[i][j - 1] + gap_penalty;
      vert_score = align_matrix[i - 1][j] + gap_penalty;
      align_matrix[i][j] = max({diag_score, horz_score, vert_score, 0});
      if (align_matrix[i][j] > align_matrix[max_coord.first][max_coord.second] && (i == size_seq1 || j == size_seq2)) {
        // cout << "New Max "<< align_matrix[i][j] <<" at  (" << i << ", " << j << ")\n";
        max_coord = pair<int, int>(i, j);
      }
    }
  }
  int max_score = align_matrix[max_coord.first][max_coord.second];
  string align_seq1 = "";
  string align_seq2 = "";
  int i = max_coord.first;
  int j = max_coord.second;
  // work our way back from max coordinates until we reach top left
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
        match_count += 1;
      } else if (seq1_substr[j] == '-' || seq2_substr[j] == '-') {
        match_string += "\033[33m-\033[0m";  // if gap match print yellow '-'
        gap_count += 1;
      } else {
        match_string += "\033[31mX\033[0m";  // if mismatch print red X
        mm_count += 1;
      }
    }

    cout << "Neanderthal: " << seq1_substr << "\n";
    cout << "MATCH:       " << match_string << "\n";
    cout << "Human Mito:  " << seq2_substr << "\n\n";
  }
  // print stats
  cout << "Alignment Score: " << max_score << "\n";
  cout << "The sequence is " << align_seq1.size() << " nucleotides long\n";
  cout << "Number of matches:    " << match_count << "/" << align_seq1.size() << " (" << (float(match_count) / align_seq1.size()) * 100 << "%)\n";
  cout << "Number of gaps:       " << gap_count << "/" << align_seq1.size() << " (" << (float(gap_count) / align_seq1.size()) * 100 << "%)\n";
  cout << "Number of mismatches: " << mm_count << "/" << align_seq1.size() << " (" << (float(mm_count) / align_seq1.size()) * 100 << "%)\n";
}