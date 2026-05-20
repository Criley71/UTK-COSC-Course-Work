// Challenge 09: sequence alignment
// Connor Riley 
// 4/16/2023
// COSC302 Dr.Emrich
// this program implements the Needleman-Wunsch algorithm to calculate the matching 
// sequences between 2 dna strands;
#include <iostream>
#include <vector>
#include <string>
using namespace std;

//simple function that takes 2 chars, the 2 that will be compared and returns 
//1 if they match and -1 if they dont. 
int scoreDiag(char s1, char s2){
    if (s1 == s2) return 1;
    else return -1;
}

//we first read in the 2 DNA strands, and then make our sequence alighnment matrix, 
//a 2-d vector. we fill the top row and left column with the gap scoring values.
//we then iterate through the matrix comparing the scores of the diagnol, up and left value.
//we then pick the largest number between the 3 and fill in that spot in the matrix. 
//the final alignment score is the bottom right corner of the matrix.
int main(int argc, char *argv[]) {
    string DNA1, DNA2;
    int s1Legnth, s2length;
    int gapPen = -1;
   
    while(cin >> DNA1 >> DNA2){
        s1Legnth = DNA1.length();
        s2length = DNA2.length();
        vector<vector<int>> sequenceMatrix(s1Legnth + 1, vector<int>(s2length+1));
        for(int i = 0; i <= s1Legnth; i++){
            sequenceMatrix[i][0] = gapPen * i;
        }
        for(int i = 0; i <= s2length; i++){
            sequenceMatrix[0][i] = gapPen * i;
        }

        for(int i = 1; i <= s1Legnth; i++){
            for(int j = 1; j <=s2length; j++){
                int diag = sequenceMatrix[i-1][j-1] + scoreDiag(DNA1[i-1], DNA2[j-1]);
                int up = sequenceMatrix[i-1][j] + gapPen;
                int left = sequenceMatrix[i][j-1] + gapPen;
                sequenceMatrix[i][j] = max(diag, max(up, left));
            }
        }
        cout << sequenceMatrix[s1Legnth][s2length] << "\n";
    }

    return 0;
}



//listening to Melophobia - Cage The Elephant