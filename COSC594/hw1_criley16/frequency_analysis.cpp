/*Connor Riley
UTK COSC 594
HW1 frequency_analysis.cpp
Takes in a fasta file as a command line argument and
outputs the individual nucleotide frequency as well as
the dinucleotide frequency
*/

#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
using namespace std;

int main(int argc, char *argv[]) {
  string file;
  // error check for apporpriate usage and that the file is accessible
  if (argc != 2) {
    cout << "No input file detected, will use lambda.fasta by default\nFor "
            "custom input USAGE: ./frequency_analysis sequence.fasta\n\n";
    file = "lambda.fasta";
  } else {
    file = argv[1];
  }

  ifstream fin(file);
  if (!fin.good()) {
    perror("Error can not open ");
    cout << argv[1] << "\n";
    return -1;
  }
  // read line by line, skipping the first header line
  string line;
  string sequence = "";
  bool skip = true;
  while (getline(fin, line)) {
    if (skip) {
      skip = false;
      continue;
    }
    sequence += line;
  }
  // cout << sequence << "\n";
  fin.close();
  // nucleotides will be in a double array that is indexed by the ascii value
  // of the nucleotide's letter
  // dinucleotides is just a map of all combinations as keys and
  // the counts as values
  array<double, 256> nucleotides;
  map<string, double> dinucleotides;

  nucleotides['A'] = 0;
  nucleotides['T'] = 0;
  nucleotides['C'] = 0;
  nucleotides['G'] = 0;
  nucleotides['N'] = 0;
  dinucleotides["AA"] = 0;
  dinucleotides["AT"] = 0;
  dinucleotides["AG"] = 0;
  dinucleotides["AC"] = 0;
  dinucleotides["TA"] = 0;
  dinucleotides["TT"] = 0;
  dinucleotides["TG"] = 0;
  dinucleotides["TC"] = 0;
  dinucleotides["GA"] = 0;
  dinucleotides["GT"] = 0;
  dinucleotides["GG"] = 0;
  dinucleotides["GC"] = 0;
  dinucleotides["CA"] = 0;
  dinucleotides["CT"] = 0;
  dinucleotides["CG"] = 0;
  dinucleotides["CC"] = 0;
  // sliding window vars
  int left = 0;
  int right = 1;
  // counting
  while (right < static_cast<int>(sequence.length())) {
    string combine = "";
    combine += sequence[left];
    combine += sequence[right];
    nucleotides[sequence[left]] += 1;
    dinucleotides[combine] += 1;
    left++;
    right++;
  }
  // need to get the left as the loop stops once
  // right exceeds the size of the string
  nucleotides[sequence[left]] += 1;
  // calculate sums of dinucleotides and nucleotides
  int din_sum = 0;
  for (auto itr = dinucleotides.begin(); itr != dinucleotides.end(); itr++) {
    din_sum += itr->second;
  }
  nucleotides['A'] += nucleotides['N'];  // convert any N's to A's
  int nuc_sum =
      nucleotides['A'] + nucleotides['T'] + nucleotides['C'] + nucleotides['G'];

  // divide each count by the amount of nucleotides or dinucleotides
  cout << "Total amount of nucleotides:   " << nuc_sum << "\n";
  cout << "Total amount of dinucleotides: " << din_sum << "\n";
  cout << "PROBABILITIES of " << file << ":\n\n";
  cout << "Nucleotide Frequencies:\n";
  cout << "A:  " << setprecision(15) << nucleotides['A'] / nuc_sum << '\n';
  cout << "C:  " << setprecision(15) << nucleotides['C'] / nuc_sum << '\n';
  cout << "G:  " << setprecision(15) << nucleotides['G'] / nuc_sum << '\n';
  cout << "T:  " << setprecision(15) << nucleotides['T'] / nuc_sum << '\n';
  cout << "\nDinucleotide Frequencies:\n";
  cout << "AA: " << setprecision(15) << dinucleotides["AA"] / din_sum << "\n";
  cout << "AC: " << setprecision(15) << dinucleotides["AC"] / din_sum << "\n";
  cout << "AG: " << setprecision(15) << dinucleotides["AG"] / din_sum << "\n";
  cout << "AT: " << setprecision(15) << dinucleotides["AT"] / din_sum << "\n";
  cout << "CA: " << setprecision(15) << dinucleotides["CA"] / din_sum << "\n";
  cout << "CC: " << setprecision(15) << dinucleotides["CC"] / din_sum << "\n";
  cout << "CG: " << setprecision(15) << dinucleotides["CG"] / din_sum << "\n";
  cout << "CT: " << setprecision(15) << dinucleotides["CT"] / din_sum << "\n";
  cout << "GA: " << setprecision(15) << dinucleotides["GA"] / din_sum << "\n";
  cout << "GC: " << setprecision(15) << dinucleotides["GC"] / din_sum << "\n";
  cout << "GG: " << setprecision(15) << dinucleotides["GG"] / din_sum << "\n";
  cout << "GT: " << setprecision(15) << dinucleotides["GT"] / din_sum << "\n";
  cout << "TA: " << setprecision(15) << dinucleotides["TA"] / din_sum << "\n";
  cout << "TC: " << setprecision(15) << dinucleotides["TC"] / din_sum << "\n";
  cout << "TG: " << setprecision(15) << dinucleotides["TG"] / din_sum << "\n";
  cout << "TT: " << setprecision(15) << dinucleotides["TT"] / din_sum << "\n";

  return 0;
}