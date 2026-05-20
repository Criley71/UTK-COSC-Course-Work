#pragma once

#include <cstdint>
#include <iostream>

enum OpcodeCategory {
    LOAD,
    STORE,
    BRANCH,
    JALR,
    JAL,
    OP_IMM,
    OP,
    AUIPC,
    LUI,
    OP_IMM_32,
    OP_32,
    SYSTEM,
    UNIMPL
};

struct DecodeOut {
    OpcodeCategory op;  // The category (given by the opcode)
    uint8_t rd;         // The destination register NUMBER (not value)
    uint8_t funct3;     // The 3-bit extended opcode (if available)
    uint8_t funct7;     // The 7-bit extended opcode (if available)
    int64_t offset;     // Offsets for BRANCH and STORE and SHAMT for SLLI/SRLI/SRAI
    int64_t left_val;   // typically the value of rs1
    int64_t right_val;  // typically the value of rs2 or immediate
};

std::ostream &operator<<(std::ostream &out, const DecodeOut &fo);
