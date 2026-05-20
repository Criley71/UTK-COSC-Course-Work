// Lab 6: Is it prime?
// Connor Riley 10/18/22
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <map>
using namespace std;
//this lab takes in input and reports back if it is prime or not
//and stores all primes between 0 and that number in a vector
//for each inputted number larger than the last it expands the vector
//if its smaller then it searches through the vector for if it is prime
int main(int argc, char *argv[])
{
	int primeIn, maxNum = 0, oldMax = 0;
	vector<int> primeNums;
	vector<int>::iterator it;
	vector<int> primeCheck;
	map <int, int> primeMap;
	//run as long as inputs are given
	while (cin >> primeIn)
	{
		//if the new input is larger than the last then resize the vector and set the
		//previous max as old max to start iteratoring from that number
		if(primeIn > maxNum){
			oldMax = maxNum;
			maxNum = primeIn;
			primeCheck.resize(maxNum + 1, 0);
		}
		//checks for primes by finding all factors between the number 2 and the input's square root
		//if it reaches the square root with no factors the prime is added to the vector
		//other wise they are not added
		for(int i = oldMax + 1; i <= maxNum +1; i++){
			int counter = 0;
			for(int j = 2; j <= sqrt(i); j++){
				if(i % j == 0){
					counter = 1;
				}
			}
			if(counter == 0){

				primeNums.push_back(i);	

			}
		}
		//using an iterator it searches through the vector and if it finds the prime
		//then it outputs prime, if it reaches the end of the vector then it is not found
		//and not prime
		it = find(primeNums.begin(), primeNums.end(), primeIn);
		if(it != primeNums.end()){
			cout << "prime\n";
		}
		else{
			cout << "not prime\n";
		}

	}

	return 0;

}
