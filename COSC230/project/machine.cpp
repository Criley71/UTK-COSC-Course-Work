#include "machine.h"

Machine::Machine(char *mem, int size)
{
    mMemory     = mem;
    mMemorySize = size;
    // Set the stack pointer (X2/SP) to the bottom
    // of the memory.
    set_xreg(2, size);
}

int64_t Machine::get_pc() const
{
    return mPC;
}

void Machine::set_pc(int64_t to)
{
    mPC = to;
}

int64_t Machine::get_xreg(int which) const
{
    which &= 0x1f;  // Make sure the register number is 0 - 31
    if (which == 0) {
        return 0;  // return 0 if this is the zero register
    }
    return mRegs[which];
}

void Machine::set_xreg(int which, int64_t value)
{
    which &= 0x1f;
    if (which != 0) {
        mRegs[which] = value;
    }
}

int64_t sign_extend(int64_t value, int8_t sign_bit_index) {
    if ((value >> sign_bit_index) & 1) {
        // Sign bit is 1
        return value | (-1UL << sign_bit_index);
    }
    else {
        // Sign bit is 0
        return value & ~(-1UL << sign_bit_index);
    }
}


