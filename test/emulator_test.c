#include "emulator.h"
#include "unity.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/*
 * ALU PROCESSING
 */
void test_outputs_zero(void) {
  uint16_t output_zero = ZX_BIT + ZY_BIT + F_BIT;
  hack_val actual = calculate(output_zero, 1, 2);
  TEST_ASSERT_EQUAL(0, actual);
}

void test_outputs_one(void) {
  uint16_t inst = ZX_BIT + NX_BIT + ZY_BIT + NY_BIT + F_BIT + NO_BIT;
  hack_val actual = calculate(inst, 1, 2);
  TEST_ASSERT_EQUAL(1, actual);
}

void test_outputs_minus_one(void) {
  uint16_t inst = ZX_BIT + NX_BIT + ZY_BIT + F_BIT;
  hack_val actual = calculate(inst, 1, 2);
  TEST_ASSERT_EQUAL(-1, actual);
}

void test_outputs_x(void) {
  uint16_t inst = ZY_BIT + NY_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(3, actual);
}

void test_outputs_y(void) {
  uint16_t inst = ZX_BIT + NX_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(5, actual);
}

void test_outputs_not_x(void) {
  uint16_t inst = ZY_BIT + NY_BIT + NO_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(~3, actual);
}

void test_outputs_not_y(void) {
  uint16_t inst = ZX_BIT + NX_BIT + NO_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(~5, actual);
}

void test_outputs_negative_x(void) {
  uint16_t inst = ZY_BIT + NY_BIT + F_BIT + NO_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(-3, actual);
}

void test_outputs_negative_y(void) {
  uint16_t inst = ZX_BIT + NX_BIT + F_BIT + NO_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(-5, actual);
}

void test_outputs_x_plus_one(void) {
  uint16_t inst = NX_BIT + ZY_BIT + NY_BIT + F_BIT + NO_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(4, actual);
}

void test_outputs_y_plus_one(void) {
  uint16_t inst = ZX_BIT + NX_BIT + NY_BIT + F_BIT + NO_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(6, actual);
}

void test_outputs_x_minus_one(void) {
  uint16_t inst = ZY_BIT + NY_BIT + F_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(2, actual);
}

void test_outputs_y_minus_one(void) {
  uint16_t inst = ZX_BIT + NX_BIT + F_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(4, actual);
}

void test_outputs_x_plus_y(void) {
  uint16_t inst = F_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(8, actual);
}

void test_outputs_x_minus_y(void) {
  uint16_t inst = NX_BIT + F_BIT + NO_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(-2, actual);
}

void test_outputs_y_minus_x(void) {
  uint16_t inst = NY_BIT + F_BIT + NO_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(2, actual);
}

void test_outputs_x_and_y(void) {
  uint16_t inst = 0;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(3 & 5, actual);
}

void test_outputs_x_or_y(void) {
  uint16_t inst = NX_BIT + NY_BIT + NO_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(3 | 5, actual);
}

/*
 * Jumps
 */

void test_increment_pc_by_one_if_no_jump(void) {
  uint16_t inst = 0;
  hack_val actual = program_counter(inst, 5, 10, 24);
  TEST_ASSERT_EQUAL(11, actual);
}

void test_unconditional_jump(void) {
  uint16_t inst = JMP;
  hack_val actual = program_counter(inst, 0, 5, 24);
  TEST_ASSERT_EQUAL(24, actual);
}

void test_jump_gt_0(void) {
  uint16_t inst = JGE;
  hack_val actual = program_counter(inst, 5, 25, 30);
  TEST_ASSERT_EQUAL(30, actual);
}

void test_no_jmp_gt_0(void) {
  uint16_t inst = JGT;
  hack_val actual = program_counter(inst, -5, 25, 30);
  TEST_ASSERT_EQUAL(26, actual);
}

void test_jump_eq_0(void) {
  uint16_t inst = JEQ;
  hack_val actual = program_counter(inst, 0, 4, 24);
  TEST_ASSERT_EQUAL(24, actual);
}

void test_no_jump_eq_0(void) {
  uint16_t inst = JEQ;
  hack_val actual = program_counter(inst, -1, 4, 24);
  TEST_ASSERT_EQUAL(5, actual);
}

void test_jump_ge_0(void) {
  uint16_t inst = 3;
  hack_val actual = program_counter(inst, 0, 4, 24);
  TEST_ASSERT_EQUAL(24, actual);
}

void test_jump_lt_0(void) {
  uint16_t inst = 4;
  hack_val actual = program_counter(inst, -1, 4, 24);
  TEST_ASSERT_EQUAL(24, actual);
}

void test_jump_ne_0(void) {
  uint16_t inst = 5;
  hack_val actual = program_counter(inst, -1, 4, 24);
  TEST_ASSERT_EQUAL(24, actual);
}

void test_jump_le_0(void) {
  uint16_t inst = 6;
  hack_val actual = program_counter(inst, -1, 4, 24);
  TEST_ASSERT_EQUAL(24, actual);
}

/*
 * Process instruction
 */

void test_set_a_register(void) {
  state *state = new_state();
  uint16_t inst = 5;
  process_inst(state, inst);
  TEST_ASSERT_EQUAL(5, state->a_register);
}

void test_output_a_to_d(void) {
  state *state = new_state();
  state->a_register = 10;
  uint16_t inst = 0x8000 + ZX_BIT + NX_BIT + DEST_D_BIT;
  process_inst(state, inst);
  TEST_ASSERT_EQUAL(10, state->d_register);
}

void test_output_a_to_m(void) {
  state *state = new_state();
  state->a_register = 10;
  uint16_t inst = 0x8000 + ZX_BIT + NX_BIT + DEST_M_BIT;
  process_inst(state, inst);
  TEST_ASSERT_EQUAL(10, state->ram[10]);
}

void test_output_d_to_m(void) {
  state *state = new_state();
  state->a_register = 10;
  state->d_register = 20;
  uint16_t inst = 0x8000 + ZY_BIT + NY_BIT + DEST_M_BIT;
  process_inst(state, inst);
  TEST_ASSERT_EQUAL(20, state->ram[10]);
}

void test_unconditional_jump_updates_pc(void) {
  state *state = new_state();
  state->a_register = 10;
  uint16_t inst = 0x8000 + JMP;
  process_inst(state, inst);
  TEST_ASSERT_EQUAL(10, state->pc);
}

/*
 * Render screen
 * */

void test_render_empty_row(void) {
  hack_val map[] = {0};
  char buf[18] = {0};
  FILE *screen = fmemopen(buf, 18, "w+");
  render(map, 16, 1, screen);
  fclose(screen);
  TEST_ASSERT_EQUAL_STRING("                \n", buf);
}

void test_render_last_pixel_single_row(void) {
  hack_val map[] = {1};
  char buf[18] = {0};
  FILE *screen = fmemopen(buf, 18, "w+");
  render(map, 16, 1, screen);
  fclose(screen);
  TEST_ASSERT_EQUAL_STRING("               @\n", buf);
}

void test_render_first_half_of_pixels_single_row(void) {
  hack_val map[] = {0xFF00};
  char buf[18] = {0};
  FILE *screen = fmemopen(buf, 18, "w+");
  render(map, 16, 1, screen);
  fclose(screen);
  TEST_ASSERT_EQUAL_STRING("@@@@@@@@        \n", buf);
}

void test_render_upper_row(void) {
  hack_val map[] = {0xFFFF, 0};
  char buf[35] = {0};
  FILE *screen = fmemopen(buf, 35, "w+");
  render(map, 16, 2, screen);
  fclose(screen);
  TEST_ASSERT_EQUAL_STRING("@@@@@@@@@@@@@@@@\n"
                           "                \n",
                           buf);
}

void test_render_offset_pointer(void) {
  hack_val map[10] = {0};
  map[8] = 0xFFFF;
  map[9] = 0;
  char buf[35] = {0};
  FILE *screen = fmemopen(buf, 35, "w+");
  render(&(map[8]), 16, 2, screen);
  fclose(screen);
  TEST_ASSERT_EQUAL_STRING("@@@@@@@@@@@@@@@@\n"
                           "                \n",
                           buf);
}

void test_m_equals_d(void) {
  state *state = new_state();
  state->d_register = 100;
  state->a_register = 0;
  uint16_t inst = 0xE308;
  process_inst(state, inst);
  TEST_ASSERT_EQUAL(100, state->ram[0]);
}
