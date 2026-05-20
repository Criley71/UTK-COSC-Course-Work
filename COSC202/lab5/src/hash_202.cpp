#include "hash_202.hpp"
#include <cstdio>	// printf
#include <iostream> // >>
#include <sstream>	// istringstream
#include <vector>
using namespace std;
istringstream ss;
int last7(string hexString)
{
	unsigned int finalKey;
	string sevenKey;
	if (hexString.length() >= 7)
	{
		sevenKey = hexString.substr(hexString.size() - 7);
	}
	else
	{
		sevenKey = hexString;
	}
	stringstream ss;
	ss.clear();
	ss << hex << sevenKey;
	ss >> finalKey;
	return finalKey;
}

int Xor(string hexString){
	int finalKey = 0;
	int size = hexString.length();
	string temp;
	unsigned int decInt;
	if(size <= 7){
		stringstream ss;
		ss.clear();
		temp = hexString;
		ss << hex << temp;
		ss >> decInt;
		return decInt;
	}
	else{ 
		if(size % 7 == 0){
			for(int i = 0; i < size/7; i++){

				temp = hexString.substr(i*7, 7);
				stringstream ss;
				ss.clear();
				ss << hex << temp;
				ss >> decInt;
				finalKey ^= decInt;
				decInt = 0;
			}
			return finalKey;
		}
		else if(size % 7 != 0){
			for(int i = 0; i < (size/7) + 1; i++){
				if(size - (i*7) <= 7){
					temp = hexString.substr(i*7);	
				}else{
					temp = hexString.substr(i*7, 7);
				}
				stringstream ss;
				ss.clear();
				ss << hex << temp;
				ss >> decInt;
				finalKey ^= decInt;
				decInt = 0;
			}
			return finalKey;
		}
	}	
}
// DATA MEMBERS
/* Obviously, you can use these however you want, but here is how I used them.  You may
   not modify or add to this header file, so you need to be able to implement the lab with
   just these member variables. */

// std::vector <std::string> Keys;   // The hash table of keys.
// std::vector <std::string> Vals;   // The Vals[i] is the val associated with Keys[i]
// size_t Nkeys;                     // This is the number of keys that have been inserted.
// int Fxn;                          // The hash function.  I have 0 = "Xor" and 1 = "Last7".
// int Coll;                         // The collision resolution strategy.  0 = Linear. 1 = Double
// size_t Nprobes;                   // When I call Find(), I set this value to the number of probes.
// END DATA MEMBERS

/* You must call Set_Up() to set up the table before using it.  This method
   takes the hash table size, a name of a hash function (either "Last7" or "XOR"), and
   the name of a collision resolution strategy (either "Linear" or "Double").

   You should error check your input, and look for the following errors, in the following
   order.  If you see one of these errors, return the string specified:

   - Table already set up:                "Hash table already set up"
   - Bad table size:                      "Bad table size"
   - Bad hash function:                   "Bad hash function"
   - Bad collision resolution strategy:   "Bad collision resolution strategy" */
string Hash_202::Set_Up(size_t table_size, const string &fxn, const string &collision)
{
	if (Keys.size() != 0)
	{
		return "Hash table already set up";
	}
	else if(table_size < 0)
	{
		return "Bad table size";
	}
	else if (fxn != "Last7" &&  fxn != "XOR")
	{
		//		return "Bad hash function";
	}
	else if (collision != "Linear" && collision != "Double")
	{
		return "Bad collision resolution strategy";
	}
	// check if table already set up
	// check for bad table size
	// check for bad hash function
	// check for bad collision resolution strategy

	// resize table (Keys & Vals vector) + initialize Nkeys to 0

	Keys.resize(table_size, "");
	Vals.resize(table_size, "");
	Nkeys = 0;
	if (fxn == "Last7")
	{
		Fxn = 1;
	}
	else if (fxn == "XOR")
	{
		Fxn = 0;
	}

	if (collision == "Linear")
	{
		Coll = 0;
	}
	else if (collision == "Double")
	{
		Coll = 1;
	}
	

	/*
	   (void)table_size;
	   (void)fxn;
	   (void)collision;
	   */
	return "";
}

/* Add() adds the given key/val pair to the hash table.  If successful, it should return
   an empty string.  Add should test for the following errors, in this order,
   and return the given strings:

   - Table not set up:                "Hash table not set up"
   - Empty string for the key:        "Empty key"
   - Key not composed of hex digits:  "Bad key (not all hex digits)"
   - Empty string for the val:        "Empty val"
   - The hash table is full:          "Hash table full"
   - Cannot insert key:               "Cannot insert key"
   - The key is already in the table  "Key already in the table"
   */
string Hash_202::Add(const string &key, const string &val)
{
	// check if table not set up
	// check if empty string for key
	// check if key not composed of hex digits
	// check if empty string for val
	// check if hash table is full
	// check if cannot insert key
	// check if key is already in table

	int hexKey = 0;
	int index = 0;
	int probes = 0;
	if (Keys.size() == 0)
	{
		return "Hash table not set up";
	}
	else if (key == "")
	{
		return "Empty key";
	}
	for(int i = 0; i < key.size(); i++){
		if(!isxdigit(key[i])){
			return "Bad key (not all hex digits)";
		}
	}
	if (val == "")
	{
		return "Empty val";
	}
	else if (Keys.size() == Nkeys)
	{
		return "Hash table full";
	}
	// get index
	if (Fxn == 1)
	{
		index = last7(key) % Keys.size();
	}
	else if (Fxn == 0)
	{
		index = Xor(key) % Keys.size();
	}
	// get probe increment
	// linear probing
	if (Coll == 0)
	{
		while (Vals[index] != "" && Keys.at(index) != key)
		{
			index = index + 1;
			index = index % Keys.size();
			probes++;
			if(Keys.at(index) == key){
				return "Key already in the table";
			}
			if (probes == Keys.size())
			{
				return "Cannot insert key";
			}
		}
		//		index = index % Keys.size();
		//	Keys[index] = key;
		//	Vals[index] = val;
		Keys.insert(Keys.begin() + index, key);
		Vals.insert(Vals.begin() + index, val);
	}
	// double probing
	// fxn you are using is index, fxn not being used % tablesize is increment, if increment is 0 add 1
	if (Coll == 1)
	{
		int doubleIndex, doubleIncrement;
		if (Fxn == 0)
		{
			doubleIndex = Xor(key) % Keys.size();
			doubleIncrement = last7(key) % Keys.size();
			if(doubleIncrement == 0){
				doubleIncrement = 1;
			}
			while (Vals[doubleIndex] != "" && Keys.at(doubleIndex) != key)
			{
				doubleIndex += doubleIncrement;
				doubleIndex = doubleIndex %  Keys.size();
				probes++;
				if(Keys.at(doubleIndex) == key){
					return "Key already in the table";
				}
				if (probes == Keys.size())
				{
					return "Cannot insert key";
				}
			}
			doubleIndex = doubleIndex % Keys.size();
			Keys[doubleIndex] = key;
			Vals[doubleIndex] = val;

		}

		else if (Fxn == 1)
		{
			doubleIndex = last7(key) % Keys.size();
			doubleIncrement = Xor(key) % Keys.size();
			if(doubleIncrement == 0){
				doubleIncrement = 1;
			}
			while (Keys[doubleIndex] != "" && Keys[doubleIndex] != key)
			{
				doubleIndex += doubleIncrement;
				doubleIndex = doubleIndex %  Keys.size();
				probes++;
				if(Keys.at(doubleIndex) == key){
					return "Key already in the table";
				}
				if (probes == Keys.size())
				{
					return "Cannot insert key";
				}
			}			
			doubleIndex = doubleIndex % Keys.size();
			Keys[doubleIndex] = key;
			Vals[doubleIndex] = val;
		}	
	}	
	// handle collisions

	// increment Nkeys
	Nkeys++;
	(void)key;
	(void)val;

	return "";
}

/* Find() returns the val associated with the given key.  If the hash table has not been
   set up yet, or if the key is not in the hash table, or if the key is not composed of
   all hex digits, it should return an empty string.

   Find() is not const, because it sets the variable Nprobes to equal the number of
   probes that it took to find the answer. */
string Hash_202::Find(const string &key)
{
	int index = 0;
	int probe = 0;
	Nprobes = probe;
	// check if hash table not set up
	// check if key is not composed of all hex digits
	if(Keys.size() <= 0){
		return "";
	}
	for(int i = 0; i < key.size(); i++){
		if(!isxdigit(key[i])){
			return "";
		}
	}

	// get index
	if(Fxn == 0){
		index = Xor(key) % Keys.size();
	}
	else if(Fxn == 1){
		index = last7(key) % Keys.size();
	}

	if(Coll == 0){

		while(Keys.at(index) != key && Vals.at(index) != ""){
			probe++;
			index = (index +  1) % Keys.size();
			if(probe == Keys.size()){
				return "";
			}
		}
		Nprobes = probe;
		return Vals[index];

	}
	else if(Coll == 1){
		int doubleIndex, doubleIncrement;
		if(Fxn == 0){
			doubleIndex = index;
			doubleIncrement = last7(key) % Keys.size();
			if(doubleIncrement == 0){
				doubleIncrement += 1;
			}
			while(Keys[doubleIndex] != key){
				doubleIndex += doubleIncrement;
				doubleIndex = doubleIndex % Keys.size();
				probe++;
				if(probe == Keys.size()){
					return "";
				}
			} 
			Nprobes = probe;
			return Vals[doubleIndex];
		}
		else if(Fxn == 1){
			doubleIndex = index;
			doubleIncrement = Xor(key) % Keys.size();
			if(doubleIncrement == 0){
				doubleIncrement = 1;
			}
			while(Keys[doubleIndex] != key){
				doubleIndex += doubleIncrement;
				doubleIndex = doubleIndex % Keys.size();
				probe++;
				if(probe == Keys.size()){
					return "";
				}
			}
			Nprobes = probe;
			return Vals[doubleIndex];
		}		

	}
	// get probe incremen
	// find val
	// val = ""
	// for count in keys:
	//	if Keys.at((index + (count * probe)) % Keys.size()) == key):
	//		val = Vals.at((index + (count * probe)) % Vals.size())
	//		break
	// Nprobes = count

	(void)key;
}

/* Print() prints all non-empty slots in the hash table, one per line.
   The index should be printed first, right justified and padded to five characters.
   Then a space, the key, a space, and the val. This should do nothing if the hash
   table has not been set up yet. */
void Hash_202::Print() const
{
	// if table has been set up
	if (Keys.size() != 0)
	{
		for (unsigned long int i = 0; i < Keys.size(); i++)
		{
			// print if not empty
			if (Keys.at(i) != "")
				printf("%5lu %s %s\n", i, Keys.at(i).c_str(), Vals.at(i).c_str());
		}
	}
}

/* Total_Probes() should traverse the hash table, and for every key, caculcate how many probes
   it takes find that key.  It should return the total number of probes.  It should
   return 0 if the hash table has not been set up yet.  It is not const, because it
   uses Find() to find the number of probes for each key. */
size_t Hash_202::Total_Probes()
{
	size_t sum = 0;

	if (Keys.size() != 0)
	{
		for (unsigned long int i = 0; i < Keys.size(); i++)
		{
			if (Keys.at(i) != "")
			{ // don't include empty strings
				Find(Keys.at(i));
				sum += Nprobes;
			}
		}
	}

	return sum; // error: sum = 0 if table not setup
}




