#include "emulator.h"
hack_val calculate(const c_instruction inst, hack_val x, hack_val y) {
  hack_val result;
  if (ZX_BIT & inst) {
    x = 0;
  }
  if (NX_BIT & inst) {
    x = ~x;
  }
  if (ZY_BIT & inst) {
    y = 0;
  }
  if (NY_BIT & inst) {
    y = ~y;
  }
  if (F_BIT & inst) {
    result = x + y;
  } else {
    result = x & y;
  }
  if (NO_BIT & inst) {
    result = ~result;
  }
  return result;
}

enum JUMP {
  NO_JMP = 0,
  JGT = 1,
  JEQ = 2,
  JGE = 3,
  JLT = 4,
  JNE = 5,
  JLE = 6,
  JMP = 7
};

a_val program_counter(const c_instruction inst, const hack_val alu_out,
                      a_val program_counter, const a_val a_reg) {
  enum JUMP jmp = inst & JMP;
  switch (jmp) {
  case NO_JMP:
    program_counter++;
    break;
  case JGT:
    if (alu_out > 0)
      program_counter = a_reg;
    else
      program_counter++;
    break;
  case JMP:
    program_counter = a_reg;
    break;
  case JEQ:
    if (!alu_out)
      program_counter = a_reg;
    else
      program_counter++;
    break;
  case JGE:
    if (alu_out >= 0)
      program_counter = a_reg;
    else
      program_counter++;
    break;
  case JLT:
    if (alu_out < 0)
      program_counter = a_reg;
    else
      program_counter++;
    break;
  case JNE:
    if (alu_out)
      program_counter = a_reg;
    else
      program_counter++;
    break;
  case JLE:
    if (alu_out <= 0)
      program_counter = a_reg;
    else
      program_counter++;
    break;
  }
  return program_counter;
}
