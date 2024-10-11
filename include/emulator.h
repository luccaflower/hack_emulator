#include <stdint.h>

typedef struct state state;

typedef int16_t hack_val;

void program_loop(uint16_t *instructions);

struct state {
  hack_val program_counter;
  hack_val a_register;
  hack_val d_register;
  hack_val *ram;
  uint16_t pc;
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
hack_val calculate(const uint16_t inst, hack_val x, hack_val y);

hack_val program_counter(const uint16_t inst, const hack_val alu_out,
                         uint16_t program_counter, const hack_val a_reg);

#define A_VAL_BIT 0x1000

hack_val *select_a_or_m(const uint16_t inst, hack_val *restrict a,
                        hack_val *restrict m);
void render(hack_val *screen, uint16_t width, uint16_t height);
