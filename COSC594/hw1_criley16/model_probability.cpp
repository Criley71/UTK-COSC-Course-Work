/*Connor Riley
UTK COSC 594
HW1 model_probability.cpp
Computes the log probability on the neandrathal
DNA sequence using a multinomial and
third-order markov model of the human mitochondria sequence
*/
#include <math.h>
#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
  // error check to readin the human mitochondria
  // dna sequence from the human_mito.fasta file
  ifstream fin("human_mito.fasta");
  if (!fin.good()) {
    perror("Error can not open file\n");
    return -1;
  }
  // read line by line skipping the first line as it
  // is the header
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
  // cout << sequence;
  fin.close();

  // begin multinomial model calculations
  array<double, 256> nucleotides;
  nucleotides['A'] = 0;
  nucleotides['T'] = 0;
  nucleotides['C'] = 0;
  nucleotides['G'] = 0;
  nucleotides['N'] = 0;
  // loop through the sequence and tally each nucleotide
  for (int i = 0; i < static_cast<int>(sequence.length()); i++) {
    nucleotides[sequence[i]] += 1;
  }
  // get the probabilities of each nucleotide by dividing by total amount of
  // nucleotides
  nucleotides['A'] += nucleotides['N'];  // convert any N's to A's
  int nuc_sum = nucleotides['A'] + nucleotides['T'] + nucleotides['C'] + nucleotides['G'];
  double a_prob = nucleotides['A'] / nuc_sum;
  double t_prob = nucleotides['T'] / nuc_sum;
  double c_prob = nucleotides['C'] / nuc_sum;
  double g_prob = nucleotides['G'] / nuc_sum;

  // open and error check the neadnderthal dna sequence from
  // neander_sample.fasta, error check
  fin.open("neander_sample.fasta");
  if (!fin.good()) {
    perror("Error can not open neander_sample.fasta\n");
    return -1;
  }
  // read in the neanderthal sequence the same way we did above
  line = "";
  string neander_sequence = "";
  skip = true;
  while (getline(fin, line)) {
    if (skip) {
      skip = false;
      continue;
    }
    neander_sequence += line;
  }
  fin.close();
  // cout << neander_sequence << "\n";

  // multinomial model is the sum of the log(probability_of_nucelotide)
  // for every nucleotide in the sequence.
  double prob = 0;
  for (int i = 0; i < static_cast<int>(neander_sequence.length()); i++) {
    switch (neander_sequence[i]) {
      case 'A':
        prob += log(a_prob);
        break;
      case 'C':
        prob += log(c_prob);
        break;
      case 'G':
        prob += log(g_prob);
        break;
      case 'T':
        prob += log(t_prob);
        break;
      default:
        prob += log(a_prob);  // if nucletide is N then add A's probability
        break;
    }
  }
  // expected value: -452.662312232692
  // precision set to match length of Piazza output
  cout << "Neanderthal Multinomial Model log-probability:        " << setprecision(15) << prob << "\n";
  // end multinomial model

  // begin 3rd order markov model
  // third order markov model uses the probability of a nucleotide appearing
  // dependent on the 3 preceding it. read through the sequence using a 4 point
  // sliding window and tallied it using a 4d matrix array
  int first = 0;
  int second = 1;
  int third = 2;
  int curr = 3;
  double markov_array[4][4][4][4] = {0};
  string markov_sequence = "";
  for (int i = 0; i < static_cast<int>(sequence.length()); i++) {
    // first switch statement used since CS102
    // i convert the sequence to where:
    // A->A, C->B, G->C, T->D
    // this is because I index the matrix by subtracting 'A'
    // from the converted sequence index (keeps range from 0 to 3)
    switch (sequence[i]) {
      case 'A':
        markov_sequence += 'A';
        break;
      case 'C':
        markov_sequence += 'B';
        break;
      case 'G':
        markov_sequence += 'C';
        break;
      case 'T':
        markov_sequence += 'D';
        break;
      default:
        markov_sequence += 'A';
        break;
    }
  }
  //  cout << markov_sequence << "\n";
  // loop through the entire sequence and tally each set of 4
  // markov_sequence[0][1][2][3] = ACGT
  while (curr < static_cast<int>(markov_sequence.length())) {
    markov_array[markov_sequence[first] - 'A'][markov_sequence[second] - 'A'][markov_sequence[third] - 'A'][markov_sequence[curr] - 'A'] += 1;
    first = second;
    second = third;
    third = curr;
    curr += 1;
  }

  // I now calculate the probability of each nucleotide appearing
  // after each combination of the 3 preceding nucleotides.
  // this is then stored in a 4d matrix
  // so the probability of A appearing after ATG depends
  // on the amount of C's, G's, T's and A's appearing after ATG
  double markov_prob_array[4][4][4][4] = {0};
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      for (int k = 0; k < 4; k++) {
        for (int m = 0; m < 4; m++) {
          int order_sum = markov_array[i][j][k][0] + markov_array[i][j][k][1] + markov_array[i][j][k][2] + markov_array[i][j][k][3];
          if (order_sum == 0) break;
          markov_prob_array[i][j][k][m] = markov_array[i][j][k][m] / order_sum;
        }
      }
    }
  }
  // convert the sequence to ABCD again for indexing the array
  string markov_neander_seq = "";
  for (int i = 0; i < static_cast<int>(neander_sequence.length()); i++) {
    switch (neander_sequence[i]) {
      case 'A':
        markov_neander_seq += 'A';
        break;
      case 'C':
        markov_neander_seq += 'B';
        break;
      case 'G':
        markov_neander_seq += 'C';
        break;
      case 'T':
        markov_neander_seq += 'D';
        break;
      default:
        markov_neander_seq += 'A';
        break;
    }
  }
  double markov_prob = 0;
  first = 0;
  second = 1;
  third = 2;
  curr = 3;
  // add in the log of probability of the first 3 nucleotides
  // as they do not have 3 preceding nucleotides
  for (int i = 0; i < 3; i++) {
    switch (neander_sequence[i]) {
      case 'A':
        markov_prob += log(a_prob);
        break;
      case 'C':
        markov_prob += log(c_prob);
        break;
      case 'G':
        markov_prob += log(g_prob);
        break;
      case 'T':
        markov_prob += log(t_prob);
        break;
      default:
        markov_prob += log(a_prob);
        break;
    }
  }
  //begin sliding window on the neanderthal sequence
  while (curr < static_cast<int>(markov_neander_seq.length())) {
    markov_prob += log(markov_prob_array[markov_neander_seq[first] - 'A'][markov_neander_seq[second] - 'A'][markov_neander_seq[third] - 'A'][markov_neander_seq[curr] - 'A']);
    first = second;
    second = third;
    third = curr;
    curr += 1;
  }
  // cout << markov_sequence.length() << "\n";
  // cout << mar_sum << "\n";
  // expected value: -451.796605782161
  // set to match length of the Piazza output 
  cout << "Neanderthal Third-Order Markov Model log-probability: " << setprecision(15) << markov_prob << "\n";
}