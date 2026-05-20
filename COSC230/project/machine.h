#pragma once

#include <cstdint>
#include <iostream>

#include "decode.h"
#include "execute.h"
#include "fetch.h"
#include "memory.h"

constexpr int MEM_SIZE = 1 << 18;
constexpr int NUM_REGS = 32;

class Machine {
    FetchOut mFO;    // Result of the fetch() method.
    DecodeOut mDO;   // Result of decode() method.
    ExecuteOut mEO;  // Result of execute() method.
    MemoryOut mMO;   // Result of memory() method.
    // The writeback stage does not have an intermediate output

    char *mMemory;            // The memory.
    int mMemorySize;          // The size of the memory (should be MEM_SIZE)
    int64_t mPC;              // The program counter
    int64_t mRegs[NUM_REGS];  // The register file
public:
    // written in machine.cpp
    Machine(char *mem, int size);
    int64_t get_pc() const;
    void set_pc(int64_t to);
    int64_t get_xreg(int which) const;
    void set_xreg(int which, int64_t value);

    // written in fetch.cpp
    void fetch();
    const FetchOut &fetch_out() const;

    // written in decode.cpp
    void decode();
    const DecodeOut &decode_out() const;

    // written in execute.cpp
    void execute();
    const ExecuteOut &execute_out() const;

    // written in memory.cpp
    void memory();
    const MemoryOut &memory_out() const;

    // written in writeback.cpp
    void writeback();
};

// Read from the internal memory
// Usage:
//   int myintval = memory_read<int>(memory_pointer, 0); // Read the first 4 bytes
//   char mycharval = memory_read<char>(memory_pointer, 8); // Read byte index 8
template <typename T>
T memory_read(const char *mem, int64_t address)
{
    return *reinterpret_cast<const T *>(mem + address);
}

// Write to the internal memory
// Usage:
//   memory_write<int>(memory_pointer, 0, 0xdeadbeef); // Set bytes 0, 1, 2, 3 to 0xdeadbeef
//   memory_write<char>(memory_pointer, 8, 0xff);      // Set byte index 8 to 0xff
template <typename T>
void memory_write(char *mem, int64_t address, T value)
{
    *reinterpret_cast<T *>(mem + address) = value;
}

// Sign extend from the bit index `index` of the given value `value`.
int64_t sign_extend(int64_t value, int8_t index);
