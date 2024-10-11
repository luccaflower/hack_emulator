
#include "emulator.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
  FILE *f = fopen(argv[1], "r");
  char buf[18];
  uint16_t instructions[0x8000] = {0};
  uint16_t cursor = 0;
  while (fgets(buf, sizeof buf, f) != NULL) {
    printf("%s", buf);
    if (cursor >= 0x8000) {
      puts("Too many instructions");
      exit(EXIT_FAILURE);
    }
    uint16_t instruction = 0;
    for (uint16_t i = 0; i < 16; ++i) {
      switch (buf[i]) {
      case '0':
        break;
      case '1':
        instruction += 1 << (15 - i);
        break;
      default:
        printf("Unknown char: %c\n", buf[i]);
        exit(EXIT_FAILURE);
      }
    }
    instructions[cursor] = instruction;
    printf("%d\n", instruction);
    cursor++;
  }
  if (feof(f))
    puts("End of file reached");
  else {
    puts("some kinda error");
    exit(EXIT_FAILURE);
  }
  fclose(f);

  program_loop(instructions);
}
