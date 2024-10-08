#include "emulator.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

void process_inst(state *state, uint16_t inst) {
  if (inst & 0x8000) {
    hack_val a = state->a_register;
    hack_val x = state->d_register;
    hack_val y = *select_a_or_m(inst, &a, &(state->ram[a]));
    hack_val alu_out = calculate(inst, x, y);
    if (inst & DEST_M_BIT) {
      state->ram[a] = alu_out;
    }
    if (inst & DEST_D_BIT) {
      state->d_register = alu_out;
    }
    if (inst & DEST_A_BIT) {
      assert(alu_out > 0);
      state->a_register = alu_out;
    }
    state->pc = program_counter(inst, alu_out, state->pc, state->a_register);
  } else {
    state->a_register = inst & 0x7FFF;
    state->pc++;
  }
}

state *new_state(void) {
  hack_val *ram = calloc(1, 0x6001 * sizeof(*ram));
  state *s = calloc(1, sizeof(*s));
  s->ram = ram;
  return s;
}

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

hack_val *select_a_or_m(const c_instruction inst, hack_val *restrict a,
                        hack_val *restrict m) {
  if (inst & A_VAL_BIT)
    return a;
  else
    return m;
}

bool is_a_instruction(const uint16_t inst) { return inst & 0x8000; }
