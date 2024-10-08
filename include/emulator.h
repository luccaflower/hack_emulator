#include <stdint.h>
typedef uint16_t c_instruction;
typedef int16_t hack_val;

#define ZX_BIT 0x0800
#define NX_BIT 0x0400
#define ZY_BIT 0x0200
#define NY_BIT 0x0100
#define F_BIT 0x0080
#define NO_BIT 0x0040
hack_val calculate(const c_instruction inst, hack_val x, hack_val y);

typedef uint16_t a_val;
a_val program_counter(const c_instruction inst, const hack_val alu_out,
                      a_val program_counter, const a_val a_reg);

#define A_VAL_BIT 0x1000

hack_val *select_a_or_m(const c_instruction inst, hack_val *restrict a,
                        hack_val *restrict m);
