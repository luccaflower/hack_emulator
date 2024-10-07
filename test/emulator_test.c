#include "emulator.h"
#include "unity.h"
#include <stdbool.h>
#include <stdint.h>

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
