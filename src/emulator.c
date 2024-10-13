#include "emulator.h"
#include <assert.h>
#include <bits/types/struct_timeval.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

void print_system_info(state *state) {
  printf("pc: %d\n", state->pc);
  printf("a:%d\n", state->a_register);
  printf("d:%d\n", state->d_register);
  printf("m:%d\n", state->ram[state->a_register]);
  printf("sp: %d\n", state->ram[0]);
}

#define RENDER_FREQ 1000 / 10
void program_loop(uint16_t *instructions) {
  state *state = new_state();
  struct timeval t_start, t_current;
  double elapsed_time;
  gettimeofday(&t_start, NULL);
  while (true) {
    process_inst(state, instructions[state->pc]);
    assert(state->pc <= 0x8000 && "PC out of bounds");
    gettimeofday(&t_current, NULL);
    elapsed_time = (((t_current.tv_sec - t_start.tv_sec) * 1000) +
                    (t_current.tv_usec - t_start.tv_usec) / 1000);
    if (elapsed_time >= RENDER_FREQ) {
      render(&(state->ram[0x4000]), 512, 128, stdout);
      gettimeofday(&t_start, NULL);
      print_system_info(state);
      printf("\e[1;1H\e[2J");
    }
  }
}

void process_inst(state *state, uint16_t inst) {
  if (inst & 0x8000) {
    hack_val a = state->a_register;
    hack_val x = state->d_register;
    hack_val y;

    if (inst & A_VAL_BIT)
      y = state->ram[a];
    else
      y = a;
    hack_val alu_out = calculate(inst, x, y);
    if (inst & DEST_M_BIT) {
      state->ram[a] = alu_out;
    }
    if (inst & DEST_D_BIT) {
      state->d_register = alu_out;
    }
    if (inst & DEST_A_BIT) {
      assert(alu_out >= 0 && "A register cannot be negative");
      state->a_register = alu_out;
    }
    state->pc = program_counter(inst, alu_out, state->pc, state->a_register);
  } else {
    state->a_register = inst;
    state->pc++;
  }
}

state *new_state(void) {
  hack_val *ram = calloc(1, 0x6001 * sizeof(*ram));
  state *s = calloc(1, sizeof(*s));
  s->ram = ram;
  return s;
}

hack_val calculate(const uint16_t inst, hack_val x, hack_val y) {
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

hack_val program_counter(const uint16_t inst, const hack_val alu_out,
                         uint16_t program_counter, const hack_val a_reg) {
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

void render(hack_val *memory_map, uint16_t width, uint16_t height,
            FILE *screen) {
  // (width + newline) * height + null-terminator
  size_t len = (width + 1) * height + 1;
  char *buf = calloc(len, sizeof(*buf));
  assert(buf && "Failed to allocate buffer for rendering");
  size_t buf_cursor = 0;
  size_t words_per_row = width / 16;
  for (size_t y = 0; y < height; y++) {
    for (size_t x = 0; x < words_per_row; x++) {
      uint16_t val = memory_map[words_per_row * y + x];
      for (size_t bit_mask = 0x8000; bit_mask > 0; bit_mask >>= 1) {
        if (val & bit_mask) {
          buf[buf_cursor] = '@';
        } else {
          buf[buf_cursor] = ' ';
        }
        ++buf_cursor;
      }
    }
    buf[buf_cursor] = '\n';
    ++buf_cursor;
  }
  buf[buf_cursor] = 0;
  buf_cursor = 0;
  fprintf(screen, "%s", buf);
  free(buf);
}
