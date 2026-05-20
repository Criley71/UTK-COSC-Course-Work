#pragma once

#include "decode.h"
#include <cstdint>
#include <iostream>

enum AluCommands {
   ALU_ADD,
   ALU_SUB,
   ALU_MUL,
   ALU_DIV,
   ALU_REM,
   ALU_SLL,
   ALU_SRL,
   ALU_SRA,
   ALU_AND,
   ALU_OR,
   ALU_XOR
};

struct ExecuteOut {
    int64_t result;
    uint8_t n, z, c, v;
};

std::ostream &operator<<(std::ostream &out, const ExecuteOut &eo);
