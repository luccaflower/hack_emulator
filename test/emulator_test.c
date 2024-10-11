#include "emulator.h"
#include "unity.h"
#include <assert.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

/*
 * ALU PROCESSING
 */
void test_outputs_zero(void) {
  hack_val output_zero = ZX_BIT + ZY_BIT + F_BIT;
  hack_val actual = calculate(output_zero, 1, 2);
  TEST_ASSERT_EQUAL(0, actual);
}

void test_outputs_one(void) {
  hack_val inst = ZX_BIT + NX_BIT + ZY_BIT + NY_BIT + F_BIT + NO_BIT;
  hack_val actual = calculate(inst, 1, 2);
  TEST_ASSERT_EQUAL(1, actual);
}

void test_outputs_minus_one(void) {
  hack_val inst = ZX_BIT + NX_BIT + ZY_BIT + F_BIT;
  hack_val actual = calculate(inst, 1, 2);
  TEST_ASSERT_EQUAL(-1, actual);
}

void test_outputs_x(void) {
  hack_val inst = ZY_BIT + NY_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(3, actual);
}

void test_outputs_y(void) {
  hack_val inst = ZX_BIT + NX_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(5, actual);
}

void test_outputs_not_x(void) {
  hack_val inst = ZY_BIT + NY_BIT + NO_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(~3, actual);
}

void test_outputs_not_y(void) {
  hack_val inst = ZX_BIT + NX_BIT + NO_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(~5, actual);
}

void test_outputs_negative_x(void) {
  hack_val inst = ZY_BIT + NY_BIT + F_BIT + NO_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(-3, actual);
}

void test_outputs_negative_y(void) {
  hack_val inst = ZX_BIT + NX_BIT + F_BIT + NO_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(-5, actual);
}

void test_outputs_x_plus_one(void) {
  hack_val inst = NX_BIT + ZY_BIT + NY_BIT + F_BIT + NO_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(4, actual);
}

void test_outputs_y_plus_one(void) {
  hack_val inst = ZX_BIT + NX_BIT + NY_BIT + F_BIT + NO_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(6, actual);
}

void test_outputs_x_minus_one(void) {
  hack_val inst = ZY_BIT + NY_BIT + F_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(2, actual);
}

void test_outputs_y_minus_one(void) {
  hack_val inst = ZX_BIT + NX_BIT + F_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(4, actual);
}

void test_outputs_x_plus_y(void) {
  hack_val inst = F_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(8, actual);
}

void test_outputs_x_minus_y(void) {
  hack_val inst = NX_BIT + F_BIT + NO_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(-2, actual);
}

void test_outputs_y_minus_x(void) {
  hack_val inst = NY_BIT + F_BIT + NO_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(2, actual);
}

void test_outputs_x_and_y(void) {
  hack_val inst = 0;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(3 & 5, actual);
}

void test_outputs_x_or_y(void) {
  hack_val inst = NX_BIT + NY_BIT + NO_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(3 | 5, actual);
}

/*
 * Jumps
 */

void test_increment_pc_by_one_if_no_jump(void) {
  hack_val inst = 0;
  hack_val actual = program_counter(inst, 5, 10, 24);
  TEST_ASSERT_EQUAL(11, actual);
}

void test_unconditional_jump(void) {
  hack_val inst = 7;
  hack_val actual = program_counter(inst, 0, 5, 24);
  TEST_ASSERT_EQUAL(24, actual);
}

void test_jump_gt_0(void) {
  hack_val inst = 1;
  hack_val actual = program_counter(inst, 5, 25, 30);
  TEST_ASSERT_EQUAL(30, actual);
}

void test_no_jmp_gt_0(void) {
  hack_val inst = 1;
  hack_val actual = program_counter(inst, -5, 25, 30);
  TEST_ASSERT_EQUAL(26, actual);
}

void test_jump_eq_0(void) {
  hack_val inst = 2;
  hack_val actual = program_counter(inst, 0, 4, 24);
  TEST_ASSERT_EQUAL(24, actual);
}

void test_jump_ge_0(void) {
  hack_val inst = 3;
  hack_val actual = program_counter(inst, 0, 4, 24);
  TEST_ASSERT_EQUAL(24, actual);
}

void test_jump_lt_0(void) {
  hack_val inst = 4;
  hack_val actual = program_counter(inst, -1, 4, 24);
  TEST_ASSERT_EQUAL(24, actual);
}

void test_jump_ne_0(void) {
  hack_val inst = 5;
  hack_val actual = program_counter(inst, -1, 4, 24);
  TEST_ASSERT_EQUAL(24, actual);
}

void test_jump_le_0(void) {
  hack_val inst = 6;
  hack_val actual = program_counter(inst, -1, 4, 24);
  TEST_ASSERT_EQUAL(24, actual);
}

/*
 * Select y input to ALU
 */

void test_select_a_for_alu(void) {
  const hack_val inst = 0x1000;
  hack_val a = 5;
  hack_val m = 10;
  hack_val *actual = select_a_or_m(inst, &a, &m);
  TEST_ASSERT_EQUAL(5, *actual);
}

void test_select_m_for_alu(void) {
  const hack_val inst = 0;
  hack_val a = 5;
  hack_val m = 10;
  hack_val *actual = select_a_or_m(inst, &a, &m);
  TEST_ASSERT_EQUAL(10, *actual);
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

void test_output_to_d(void) {
  state *state = new_state();
  state->a_register = 10;
  uint16_t inst = 0x8000 + ZX_BIT + NX_BIT + A_VAL_BIT + DEST_D_BIT;
  process_inst(state, inst);
  TEST_ASSERT_EQUAL(10, state->d_register);
}

void test_output_to_m(void) {
  state *state = new_state();
  state->a_register = 10;
  uint16_t inst = 0x8000 + ZX_BIT + NX_BIT + A_VAL_BIT + DEST_M_BIT;
  process_inst(state, inst);
  TEST_ASSERT_EQUAL(10, state->ram[10]);
}
