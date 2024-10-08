#include "emulator.h"
#include "unity.h"
#include <stdbool.h>
#include <stdint.h>

/*
 * ALU PROCESSING
 */
void test_outputs_zero(void) {
  c_instruction output_zero = ZX_BIT + ZY_BIT + F_BIT;
  hack_val actual = calculate(output_zero, 1, 2);
  TEST_ASSERT_EQUAL(0, actual);
}

void test_outputs_one(void) {
  c_instruction inst = ZX_BIT + NX_BIT + ZY_BIT + NY_BIT + F_BIT + NO_BIT;
  hack_val actual = calculate(inst, 1, 2);
  TEST_ASSERT_EQUAL(1, actual);
}

void test_outputs_minus_one(void) {
  c_instruction inst = ZX_BIT + NX_BIT + ZY_BIT + F_BIT;
  hack_val actual = calculate(inst, 1, 2);
  TEST_ASSERT_EQUAL(-1, actual);
}

void test_outputs_x(void) {
  c_instruction inst = ZY_BIT + NY_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(3, actual);
}

void test_outputs_y(void) {
  c_instruction inst = ZX_BIT + NX_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(5, actual);
}

void test_outputs_not_x(void) {
  c_instruction inst = ZY_BIT + NY_BIT + NO_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(~3, actual);
}

void test_outputs_not_y(void) {
  c_instruction inst = ZX_BIT + NX_BIT + NO_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(~5, actual);
}

void test_outputs_negative_x(void) {
  c_instruction inst = ZY_BIT + NY_BIT + F_BIT + NO_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(-3, actual);
}

void test_outputs_negative_y(void) {
  c_instruction inst = ZX_BIT + NX_BIT + F_BIT + NO_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(-5, actual);
}

void test_outputs_x_plus_one(void) {
  c_instruction inst = NX_BIT + ZY_BIT + NY_BIT + F_BIT + NO_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(4, actual);
}

void test_outputs_y_plus_one(void) {
  c_instruction inst = ZX_BIT + NX_BIT + NY_BIT + F_BIT + NO_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(6, actual);
}

void test_outputs_x_minus_one(void) {
  c_instruction inst = ZY_BIT + NY_BIT + F_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(2, actual);
}

void test_outputs_y_minus_one(void) {
  c_instruction inst = ZX_BIT + NX_BIT + F_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(4, actual);
}

void test_outputs_x_plus_y(void) {
  c_instruction inst = F_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(8, actual);
}

void test_outputs_x_minus_y(void) {
  c_instruction inst = NX_BIT + F_BIT + NO_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(-2, actual);
}

void test_outputs_y_minus_x(void) {
  c_instruction inst = NY_BIT + F_BIT + NO_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(2, actual);
}

void test_outputs_x_and_y(void) {
  c_instruction inst = 0;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(3 & 5, actual);
}

void test_outputs_x_or_y(void) {
  c_instruction inst = NX_BIT + NY_BIT + NO_BIT;
  hack_val actual = calculate(inst, 3, 5);
  TEST_ASSERT_EQUAL(3 | 5, actual);
}

/*
 * Jumps
 */

void test_increment_pc_by_one_if_no_jump(void) {
  c_instruction inst = 0;
  a_val actual = program_counter(inst, 5, 10, 24);
  TEST_ASSERT_EQUAL(11, actual);
}

void test_unconditional_jump(void) {
  c_instruction inst = 7;
  a_val actual = program_counter(inst, 0, 5, 24);
  TEST_ASSERT_EQUAL(24, actual);
}

void test_jump_gt_0(void) {
  c_instruction inst = 1;
  a_val actual = program_counter(inst, 5, 25, 30);
  TEST_ASSERT_EQUAL(30, actual);
}

void test_no_jmp_gt_0(void) {
  c_instruction inst = 1;
  a_val actual = program_counter(inst, -5, 25, 30);
  TEST_ASSERT_EQUAL(26, actual);
}

void test_jump_eq_0(void) {
  c_instruction inst = 2;
  a_val actual = program_counter(inst, 0, 4, 24);
  TEST_ASSERT_EQUAL(24, actual);
}

void test_jump_ge_0(void) {
  c_instruction inst = 3;
  a_val actual = program_counter(inst, 0, 4, 24);
  TEST_ASSERT_EQUAL(24, actual);
}

void test_jump_lt_0(void) {
  c_instruction inst = 4;
  a_val actual = program_counter(inst, -1, 4, 24);
  TEST_ASSERT_EQUAL(24, actual);
}

void test_jump_ne_0(void) {
  c_instruction inst = 5;
  a_val actual = program_counter(inst, -1, 4, 24);
  TEST_ASSERT_EQUAL(24, actual);
}

void test_jump_le_0(void) {
  c_instruction inst = 6;
  a_val actual = program_counter(inst, -1, 4, 24);
  TEST_ASSERT_EQUAL(24, actual);
}
