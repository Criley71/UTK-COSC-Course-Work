//Connor Riley 10/18/22
//primes lab
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
//This program takes in a user's number input and finds all primes in between 0 and the
//given number and stores them in a vector and printed
int main(int argc, char *argv[])
{
	//first make sure the user put in a command line arguement and if not tell them usage
	if (argc != 2)
	{
		cout << "Usage: " << argv[0] << " <number>\n";
		return 0;
	}
	//convert the command line argument into an int
	int primeIn = strtol(argv[1], NULL, 10);

	//create a vector of ints to store prime numbers
	//if a user gives a number less than 2 there are no primes so it will do nothing
	vector<int> primeNums;
	if (primeIn < 2)
	{
	}
	//Any number greater than or equal to 2 will then be tested
	//you can test by skipping all evens, and then check factors between 2 and the 
	//test numbers square root. if divisible at all it is not prime, but if it reaches it
	//sqaure root with no whole number factors then we store it in the prime vectors
	else if (primeIn >= 2)
	{
		primeNums.push_back(2);

		for (int i = 3; i <= primeIn; i+=2)
		{
			if(sqrt(i) < 3 && i % 2 != 0){
				primeNums.push_back(i);
				continue;
			}
			for (int j = 2; j <= sqrt(primeIn); j++)
			{
				if (i % j == 0)
				{
					break;
				}
				else if (j + 1 > sqrt(primeIn))
				{
					primeNums.push_back(i);
				}
			}

		}
	}

	//then print out the primes 20 per line
	int counter = 0;
	for (int i = 0; i < primeNums.size(); i++){
		cout << primeNums[i];
		counter+=1;
		if(counter == 20){
			cout << '\n';
		}else if(i == primeNums.size() - 1){

		}
		else{
			cout << ", ";
		}
	}
	return 0;
}


