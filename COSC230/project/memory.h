#pragma once

#include "execute.h"

#include <iostream>

struct MemoryOut {
    int64_t value;
};

std::ostream &operator<<(std::ostream &out, const MemoryOut &mo);
