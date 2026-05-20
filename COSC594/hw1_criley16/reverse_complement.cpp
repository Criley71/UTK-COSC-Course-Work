/*Connor Riley
UTK COSC 594
HW1 reverse_complement.cpp
reads the lambda.fasta file and creates a new
fasta file with the reverse complement sequence
*/

#include <array>
#include <fstream>
#include <iostream>
using namespace std;

int main() {
  // open lambda.fasta, make sure it opens
  // other wise error and return
  ifstream fin("lambda.fasta");
  if (!fin.good()) {
    perror("Error opening lambda.fasta\n");
    return 1;
  }

  string line;
  string sequence = "";
  bool skip = true;
  // reads the lambda.fasta line by line
  while (getline(fin, line)) {
    if (skip) {  // skips the header line
      skip = false;
      continue;
    }
    sequence += line + "\n";
  }
  fin.close();
  // open/create lambda.rev.fasta and error check
  ofstream fout("lambda.rev.fasta");
  if (!fout.good()) {
    perror("Error creating lambda.rev.fasta\n");
    return 1;
  }
  // header of file
  fout << ">reversed\n";
  // have an array of 256 chars where the nitrogen base
  // ascii value is converted to an int and is set to the
  // complement base
  array<char, 256> complement;
  complement['A'] = 'T';
  complement['T'] = 'A';
  complement['G'] = 'C';
  complement['C'] = 'G';

  // iterate through the original sequence backwards
  int count = 0;
  for (auto itr = sequence.rbegin(); itr != sequence.rend(); itr++) {
    if (*itr == '\n') continue;  // skip over new line characters
    if (count == 70) {  // every 71 characters (starts at 0) make a new line
      fout << "\n";
      count = 0;
    }

    fout << complement[*itr];
    count++;
  }
  fout << "\n";
  return 0;
}