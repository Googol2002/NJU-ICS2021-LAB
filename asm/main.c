#include "asm.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  asm_jmp_buf buf1;
  int r = asm_setjmp(buf1);
  if (r == 0) {
    assert(asm_add(1234, 5678) == 6912);
    assert(asm_popcnt(0x0123456789abcdefULL) == 32);
    
    asm_longjmp(buf1, 123);
  } else {
    assert(r == 123);
    
    printf("PASSED.\n");
  }
}