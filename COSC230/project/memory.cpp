#include "machine.h"

#include "memory.h"

#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

void Machine::memory() {
    if (mDO.op == STORE) {
        switch (mDO.funct3) {
            case 0b000: // SB
                memory_write<uint8_t>(mMemory, mEO.result, mDO.right_val);
            break;
            // TODO: Finish here SH, SW, and SD
            default:
                cerr << "[MEMORY: STORE]: Invalid funct3: " << mDO.funct3 << '\n';
            break;
        }
    }
    else if (mDO.op == LOAD) {
        switch (mDO.funct3) {
            case 0b000: // LB
                mMO.value = memory_read<int8_t>(mMemory, mEO.result);
            break;
            // TODO: Finish here LBU, LH, LHU, LW, LWU, and LD
            default:
                cerr << "[MEMORY: LOAD]: Invalid funct3: " << mDO.funct3 << '\n';
            break;
        }
    }
    else {
        // If this is not a LOAD or STORE, then this stage just copies
        // the ALU result.
        mMO.value = mEO.result;
    }
}


const MemoryOut &Machine::memory_out() const {
    return mMO;
}

ostream &operator<<(ostream &out, const MemoryOut &mo) {
    ostringstream sout;
    sout << "0x" << hex << right << setfill('0') << setw(16) << mo.value;
    return out << sout.str();
}
