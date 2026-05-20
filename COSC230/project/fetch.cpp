//Connor Riley 10/20/22 Fetch.cpp
//This program is used to read in 4 bytes of memory and store in a structure
#include <iomanip>
#include <iostream>
#include <sstream>

#include "machine.h"
#include "fetch.h"

using namespace std;

void Machine::fetch()
{
    //This function will set the instruction field in the FetchOut struct
	//to the 4 bytes of memory read in from the Memory(mMemory)
	mFO.instruction = memory_read<int32_t>(mMemory, get_pc());
}

const FetchOut &Machine::fetch_out() const
{
    return mFO;
}

std::ostream &operator<<(ostream &out, const FetchOut &fo)
{
    // The code below allows us to cout a FetchOut structure.
    // We can use this to debug our code.
    // FetchOut fo = { 0xdeadbeef };
    // cout << fo << '\n';

    // We use a string stream to avoid changing the ostream by using
    // manipulators.
    std::ostringstream sout;
    sout << "0x" << hex << setfill('0') << right << setw(8) << fo.instruction;
    return out << sout.str();
}
