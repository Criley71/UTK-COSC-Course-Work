#include <iomanip>
#include <sstream>

#include "machine.h"
using namespace std;

static constexpr int OPCODE_ROWS = 2 * 2;
static constexpr int OPCODE_COLS = 2 * 2 * 2;
static constexpr uint32_t OPCODE_ROW_MASK = OPCODE_ROWS - 1;
static constexpr uint32_t OPCODE_COL_MASK = OPCODE_COLS - 1;

// Opcode map from the RISC-V opcode table.
static constexpr OpcodeCategory OPCODE_MAP[OPCODE_ROWS][OPCODE_COLS] = {
 // First row (inst[6:5] = 0b00)
    {LOAD,   UNIMPL, UNIMPL, UNIMPL, OP_IMM, AUIPC,  OP_IMM_32, UNIMPL},
 // Second row (inst[6:5] = 0b01)
    {STORE,  UNIMPL, UNIMPL, UNIMPL, OP,     LUI,    OP_32,     UNIMPL},
 // Third row (inst[6:5] = 0b10)
    {UNIMPL, UNIMPL, UNIMPL, UNIMPL, UNIMPL, UNIMPL, UNIMPL,    UNIMPL},
 // Fourth row (inst[6:5] = 0b11)
    {BRANCH, JALR,   UNIMPL, JAL,    SYSTEM, UNIMPL, UNIMPL,    UNIMPL}
};

// Decode helper functions
static void decode_b(const Machine &mach, DecodeOut &dec)
{
    const FetchOut &fo = mach.fetch_out();

    dec.funct3         = (fo.instruction >> 12) & 7;
    // get_xreg truncates for us, so no need to mask the bits
    dec.left_val       = mach.get_xreg(fo.instruction >> 15);  
    dec.right_val      = mach.get_xreg(fo.instruction >> 20);

    // Offset has a sign bit at index 12
    dec.offset =
        sign_extend((((fo.instruction >> 31) & 1) << 12) | (((fo.instruction >> 25) & 0x3f) << 5) |
                        (((fo.instruction >> 8) & 0xf) << 1) | (((fo.instruction >> 7) & 1) << 11),
                    12);
    dec.rd = 0;
}
static void decode_r(const Machine &mach, DecodeOut &dec)
{
    // TODO: Decode R type here
    const FetchOut &fo = mach.fetch_out();

    dec.funct3 = (fo.instruction >> 12) & 7;
    dec.left_val = mach.get_xreg(fo.instruction >> 15);
    dec.right_val = mach.get_xreg(fo.instruction >> 20);
    dec.funct7 = fo.instruction >> 25;
    dec.rd = (fo.instruction >> 7) & 0x1F;
    
}
static void decode_s(const Machine &mach, DecodeOut &dec)
{
    const FetchOut &fo = mach.fetch_out();

    dec.funct3 = (fo.instruction >> 12) & 7;
    dec.left_val = mach.get_xreg(fo.instruction >> 15);
    dec.right_val = mach.get_xreg(fo.instruction >> 20);
    dec.offset = (((fo.instruction >> 31) & 1) << 5) | (((fo.instruction >> 7) & 0x1F));
    dec.rd = 0;

    // TODO: Decode S type here
}
static void decode_j(const Machine &mach, DecodeOut &dec)
{
    // TODO: Decode J type here
    const FetchOut &fo = mach.fetch_out();

    dec.rd = (fo.instruction >> 7) &0x1F;
    dec.right_val = sign_extend((((fo.instruction >> 31 ) & 1) << 20 ) | (((fo.instruction >> 21) & 0x3FF) << 1) | (((fo.instruction >> 20) & 1) << 11) | (((fo.instruction >> 12) & 0x7FF)) << 12, 20);
}
static void decode_u(const Machine &mach, DecodeOut &dec)
{
    // TODO: Decode U type here
    const FetchOut &fo = mach.fetch_out();
    dec.rd = (fo.instruction >> 7) & 0x1F;
    dec.right_val = sign_extend((((fo.instruction >> 12) & 0xFFFFF) << 12), 12);
}
static void decode_i(const Machine &mach, DecodeOut &dec)
{
    // TODO: Decode I type here
    const FetchOut &fo = mach.fetch_out();
    dec.offset = sign_extend((((fo.instruction >> 31) & 1) << 11), 11);
    dec.funct3 = (fo.instruction >> 12) & 7;
    dec.left_val = mach.get_xreg(fo.instruction >> 15);
    dec.rd = (fo.instruction >> 7) & 0x1F;

}

static constexpr OpcodeCategory find_opcode_category(uint32_t instruction)
{
    uint8_t opcode_map_row = (instruction >> 5) & OPCODE_ROW_MASK;
    uint8_t opcode_map_col = (instruction >> 2) & OPCODE_COL_MASK;
    return OPCODE_MAP[opcode_map_row][opcode_map_col];
}

void Machine::decode()
{
    uint8_t inst_size = mFO.instruction & 3;
    if (inst_size != 3) {
        cerr << "[DECODE] Invalid instruction (not a 32-bit instruction).\n";
        return;
    }
    mDO.op = find_opcode_category(mFO.instruction);
    switch (mDO.op) {
        case LOAD:
        case JALR:
        case OP_IMM:
        case OP_IMM_32:
            decode_i(*this, mDO);
            break;
        case STORE:
            decode_s(*this, mDO);
            break;
        case BRANCH:
            decode_b(*this, mDO);
            break;
        case JAL:
            decode_j(*this, mDO);
            break;
        case AUIPC:
        case LUI:
            decode_u(*this, mDO);
            break;
        case OP:
        case OP_32:
            decode_r(*this, mDO);
            break;
        case UNIMPL:
            cerr << "Unimplemented op type: " << mDO.op << '\n';
            break;
        default:
            cerr << "Invalid op type: " << mDO.op << '\n';
            break;
    }
}

const DecodeOut &Machine::decode_out() const
{
    return mDO;
}

// Debug output
std::ostream &operator<<(std::ostream &out, const DecodeOut &dec)
{
    ostringstream sout;
    sout << "Operation: ";
    switch (dec.op) {
        case LUI:
            sout << "LUI";
            break;
        case AUIPC:
            sout << "AUIPC";
            break;
        case LOAD:
            sout << "LOAD";
            break;
        case STORE:
            sout << "STORE";
            break;
        case OP_IMM:
            sout << "OPIMM";
            break;
        case OP_IMM_32:
            sout << "OPIMM32";
            break;
        case OP:
            sout << "OP";
            break;
        case OP_32:
            sout << "OP32";
            break;
        case BRANCH:
            sout << "BRANCH";
            break;
        case JALR:
            sout << "JALR";
            break;
        case JAL:
            sout << "JAL";
            break;
        case SYSTEM:
            sout << "SYSTEM";
            break;
        case UNIMPL:
            sout << "NOT-IMPLEMENTED";
            break;
    }
    sout << '\n';
    sout << "RD       : " << (uint32_t)dec.rd << '\n';
    sout << "funct3   : " << (uint32_t)dec.funct3 << '\n';
    sout << "funct7   : " << (uint32_t)dec.funct7 << '\n';
    sout << "offset   : " << dec.offset << '\n';
    sout << "left     : " << dec.left_val << '\n';
    sout << "right    : " << dec.right_val;
    return out << sout.str();
}
