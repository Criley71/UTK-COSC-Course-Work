// Challenge 06: Palindrome Permutation
//Connor Riley
//2/13/23 COSC302 Dr.Emrich
//This program takes in a word and determines if it is 
//a palindrome
#include <iostream>
#include <unordered_map>
#include <string>
#include <algorithm>
using namespace std;

bool isPalindrome(string word) {
    //we make an unordered map to store each character and amount of times
    //it appears in the word. we first though delete all white space from the word.
    unordered_map<char, int> letterCount;
    unordered_map<char, int>::iterator itr;
    word.erase(remove(word.begin(), word.end(), ' '), word.end());
    //We then loop through the word by checking for the amount of 
    //times the first letter appears then removing all instances of the letter.
    //as well as counting each occurance.
    //we do this in a loop until the word has no more letters.
    //We do check if the letter is a letter by checking the int value of the char
    //on the ascii table. 
    while (word.length() != 0) {
        char letter = word[0];
        if ((int)letter < 65) {
            word.erase(remove(word.begin(), word.end(), letter), word.end());
            letter = word[0];
        }
        int occur = 0;
        for (int i = 0; i < word.length(); i++) {
            if (word[i] == letter) {
                occur++;
            }
        }
        //for a word to be a palindrome all only 1 or 0 letters can appear
        //an odd amount of times. So when storing the values into the map we
        //can mod the occurences by 2 as it really only matters if it is odd.
        occur %= 2;
        letterCount.insert(make_pair(letter, occur));
        word.erase(remove(word.begin(), word.end(), letter), word.end());
    }
    //we loop through the map, adding each occurence value together
    //and if the value is greater than 1 it is not a palindrome.
    //if its a 0 or 1 then it is.
    int paliCheck = 0;
    for (itr = letterCount.begin(); itr != letterCount.end(); itr++) {
        paliCheck += itr->second;
    }
    if (paliCheck > 1) {
        return false;
    }
    return true;
}
int main(int argc, char* argv[]) {
    //Take in input, and run the palindrome check to determine if its a palindrome
    //and print the result
    string wordInput;
    while (getline(cin, wordInput)) {
        if (isPalindrome(wordInput) == true) {
            cout << "\"" << wordInput << "\" is a palindrome permutation\n";
        }
        else {
            cout << "\"" << wordInput << "\" is not a palindrome permutation\n";
        }
    }
    return (0);
}

