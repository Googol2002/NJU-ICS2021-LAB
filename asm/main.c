#include "asm.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

int main() {
  asm_jmp_buf buf;
  int r = asm_setjmp(buf);
  int a = 1, b = 2, c = 3, d = 4, e = 5;
  if (r == 0) {
    assert(asm_add(1234, 5678) == 6912);
    assert(asm_popcnt(0x0123456789abcdefULL) == 32);
    
    char src[32] = "12345678";
    char dst[32];

    asm_memcpy(dst, src, 9);
    assert(strcmp(src, dst) == 0);

    asm_longjmp(buf, 123);
  } else {
    assert(r == 123);
    assert(a == 1);
    assert(b == 2);
    assert(c == 3);
    assert(d == 4);
    assert(e == 5);

    printf("PASSED.\n");
  }
}
