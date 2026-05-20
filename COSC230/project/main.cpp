//Connor Riley 10/20/22 Main.cpp
//This program opens and reads in an instruction  file as binary
//and converts it hexadecimal form.
#include <iostream>
#include <fstream>
#include "machine.h"
using namespace std;

int main(int argc, char *argv[])
{
    char *mem    = nullptr;
    int64_t size = 0;
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <object file>\n";
        return -1;
    }
    // TODO: write the code that error checks and reads the file
    // given by the command line argument.
	//
	// First we open the file as binary and at the end of the file.
	// this way we can get the size by just using tellg. We make sure the file
	// opened and if not the program prints an error and exits. we set our memory pointer
	// to a dynamically stored char array the size of our total memory. we set the size
	// of the file and seek back to the beginning and use ifstream read to store the file
	// data into our mem array. We then close the file.
    ifstream fin(argv[1], ios::binary | ios::ate);
    if(!fin.is_open()){
        perror(argv[1]);
        return -1;
    }

	mem = new char[MEM_SIZE];	
    size = fin.tellg();
	if(size > MEM_SIZE){
		return -1;		
	} 
    fin.seekg(0, fin.beg);
    fin.read(mem, size);
	
	fin.close();

//we set the current program to 0 using set_pc() to set where the
//machin will be reading from to start. We delete our mem array at the end 
//to free space.
    Machine mach(mem, MEM_SIZE);
	mach.set_pc(0);
    while (mach.get_pc() < size) {
        mach.fetch();
        cout << mach.fetch_out() << '\n';
        mach.decode();
        cout << mach.decode_out() << '\n';
        mach.set_pc(mach.get_pc() + 4);
        // TODO: For each stage, you will add to this
        // to see the intermediate results.
    }

    // TODO: Free any memory associated with mem
	delete[] mem;
	return 0;
}

