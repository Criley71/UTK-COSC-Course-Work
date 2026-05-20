/*Connor Riley
UTK COSC 594
HW1 markov_generator.cpp
Uses the human_mito.fasta to train a third-order markov
model and then generates a 20000 nucleotide sequence 
with these probabilities
*/
#include <math.h>
#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;
int main(int argc, char* argv[]) {
  // this is just copied from the model_probability.cpp as I am
  // just getting the probabilities for the 3rd order markov
  // New Stuff begins around line 125
  srand(time(0));
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
  //double t_prob = nucleotides['T'] / nuc_sum; unused
  double c_prob = nucleotides['C'] / nuc_sum;
  double g_prob = nucleotides['G'] / nuc_sum;
  // cout << a_prob << "\n";
  // cout << c_prob << "\n";
  // cout << g_prob << "\n";
  // cout << t_prob << "\n";

  int first = 0;
  int second = 1;
  int third = 2;
  int curr = 3;
  double markov_array[4][4][4][4] = {0};
  string markov_sequence = "";
  for (int i = 0; i < static_cast<int>(sequence.length()); i++) {
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
  while (curr < static_cast<int>(sequence.length())) {
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
          // cout << char(char(i)+'A') << char(char(j)+'A') << char(char(k)+'A') << char(char(m)+'A') << ": " << markov_prob_array[i][j][k][m] << "\n";
        }
      }
    }
  }

  // END THIRD ORDER MARKOV CALCULATIONS


  // BEGIN GENERATING SEQUENCE
  // first create/open markov_simulated.fasta
  int nuc_count = 0;
  ofstream fout;
  fout.open("markov_simulated.fasta");
  fout << ">Markov Simulation\n";
  // for the first 3 nucleotides we just pick off of
  // the general probability of each ATCG appearing
  // will generate a random number between 0 and 1 where
  // the ranges look like:
  // 0->a_prob pick A
  // a_prob->(a_prob + c_prob) pick C
  //(a_prob+c_prob)->(a_prob + c_prob + g_prob) pick G
  // (a_prob + c_prob + g_prob)-> 1 pick T
  double choose_a = a_prob;
  double choose_c = a_prob + c_prob;
  double choose_g = a_prob + c_prob + g_prob;
  // else will be choose_t
  double r;
  string generated_seq = "";
  // the generated seq string will be made of ABCD for
  // array indexing purposes while the output to
  // markov_simulated.fasta will be ACGT's
  for (int i = 0; i < 3; i++) {
    r = ((double)rand()) / RAND_MAX;  
    // is this actually the best way to get a random number in c++, I had to
    // look it up and this is what GeeksForGeeks had and it seems a little much
    //https://www.geeksforgeeks.org/cpp/generate-a-random-number-between-0-and-1/
    if (r < choose_a) {
      fout << "A";
      generated_seq += "A";
    } else if (r < choose_c) {
      fout << "C";
      generated_seq += "B";
    } else if (r < choose_g) {
      fout << "G";
      generated_seq += "C";
    } else {
      fout << "T";
      generated_seq += "D";
    }
  }
  nuc_count = 3;
  first = 0;
  second = 1;
  third = 2;
  // cout << markov_sequence;
  // Sliding window that uses the previous 3 to then get the probabilities of each
  // nucleotide based on the 3 preceding nucleotides
  while (nuc_count < 20000) {
    r = ((double)rand()) / RAND_MAX;
    choose_a = markov_prob_array[generated_seq[first] - 'A'][generated_seq[second] - 'A'][generated_seq[third] - 'A'][0];
    choose_c = choose_a + markov_prob_array[generated_seq[first] - 'A'][generated_seq[second] - 'A'][generated_seq[third] - 'A'][1];
    choose_g = choose_c + markov_prob_array[generated_seq[first] - 'A'][generated_seq[second] - 'A'][generated_seq[third] - 'A'][2];
    if (r < choose_a) {
      fout << "A";
      generated_seq += "A";
    } else if (r < choose_c) {
      fout << "C";
      generated_seq += "B";
    } else if (r < choose_g) {
      fout << "G";
      generated_seq += "C";
    } else {
      fout << "T";
      generated_seq += "D";
    }
    nuc_count += 1;
    first = second;
    second = third;
    third += 1;
    if (nuc_count % 70 == 0) fout << "\n";  // every 70 chars we newline
  }
  fout << "\n";  // fasta seems to have new line at end
}