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
