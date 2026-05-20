#include "machine.h"
#include "execute.h"

#include <iostream>
#include <sstream>

using namespace std;

ExecuteOut alu(AluCommands cmd, int64_t left, int64_t right) {
    ExecuteOut ret;
    switch (cmd) {
        case ALU_ADD:
            ret.result = left + right;
        break;
        case ALU_SUB:
            ret.result = left - right;
        break;
        // TODO: Finish the commands here.
    }
    // Now that we have the result, determine the flags.
    uint8_t sign_left = (left >> 63) & 1;
    uint8_t sign_right = (right >> 63) & 1;
    uint8_t sign_result = (ret.result >> 63) & 1;
    ret.z = !ret.result;
    ret.n = sign_result;
    ret.v = ~sign_left & ~sign_right & sign_result |
             sign_left & sign_right & ~sign_result;
    ret.c = (ret.result > left) || (ret.result > right);
    return ret;
}

void Machine::execute() {
   AluCommands cmd;
   // Most instructions will follow left/right
   // but some won't, so we need these:
   int64_t op_left = mDO.left_val;
   int64_t op_right = mDO.right_val; 
   
   if (mDO.op == BRANCH) {
      // A branch needs to subtract the operands
      cmd = ALU_SUB;
   }
   else if (mDO.op == LOAD || mDO.op == STORE) {
      // For loads and stores, we need to add the
      // offset with the base register.
      cmd = ALU_ADD;
   }
   else if (mDO.op == OP) {
      // We can't tell which ALU command to use until
      // we read the funct3 and funct7
      switch (mDO.funct3) {
         case 0b000: // ADD or SUB
             if (mDO.funct7 == 0) {
                cmd = ALU_ADD;
             }
             else if (mDO.funct7 == 32) {
                cmd = ALU_SUB;
             }
         break;
         // TODO: Finish the rest of the OP functions here.
      }
   }
   else if (mDO.op == OP_32) {
       op_left = sign_extend(op_left, 31);
       op_right = sign_extend(op_right, 31);
       // TODO: You still need to determine the ALU command
   }
   else if (mDO.op == OP_IMM) {
       // TODO: Look and see which ALU op needs to be executed.
   }
   else if (mDO.op == OP_IMM_32) { 
       op_left = sign_extend(op_left, 31);
       op_right = sign_extend(op_right, 31);
       // This is just like OP_IMM except we truncated
       // the left and right ops.
    //    TODO: Still need the ALU op
   }
   else if (mDO.op == JALR) {
       // JALR has an offset and a register value that
       // need to be added together.
       cmd = ALU_ADD;
   }
   mEO = alu(cmd, op_left, op_right);
}

const ExecuteOut &Machine::execute_out() const {
    return mEO;
}

ostream &operator<<(ostream &out, const ExecuteOut &eo) {
    ostringstream sout;
    sout << "Result: " << eo.result << " [NZCV]: " 
            << (uint32_t)eo.n 
            << (uint32_t)eo.z 
            << (uint32_t)eo.c
            << (uint32_t)eo.v;
    return out << sout.str();
}
