#pragma once


struct FetchOut {
    uint32_t instruction;
};
std::ostream &operator<<(std::ostream &out, const FetchOut &fo);
