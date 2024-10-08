#include <stdint.h>

typedef struct state state;

typedef uint16_t c_instruction;
typedef int16_t hack_val;
typedef uint16_t a_val;

struct state {
  a_val program_counter;
  a_val a_register;
  hack_val d_register;
  hack_val *ram;
  a_val pc;
};
state *new_state(void);

#define DEST_M_BIT 0x8
#define DEST_D_BIT 0x10
#define DEST_A_BIT 0x20

void process_inst(state *state, uint16_t inst);

#define KEYBOARD 0x6000
#define SCREEN 0x4000

#define ZX_BIT 0x0800
#define NX_BIT 0x0400
#define ZY_BIT 0x0200
#define NY_BIT 0x0100
#define F_BIT 0x0080
#define NO_BIT 0x0040
hack_val calculate(const c_instruction inst, hack_val x, hack_val y);

a_val program_counter(const c_instruction inst, const hack_val alu_out,
                      a_val program_counter, const a_val a_reg);

#define A_VAL_BIT 0x1000

hack_val *select_a_or_m(const c_instruction inst, hack_val *restrict a,
                        hack_val *restrict m);
