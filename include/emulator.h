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
