#include "asm.h"
#include <string.h>

int64_t asm_add(int64_t a, int64_t b) {
  int64_t result = 0;
  __asm__(
  "movq %1, %0;"\
  "addq %2, %0":
  "=r" (result):
  "m" (a), "m" (b)
  );
  return result;
}

int asm_popcnt(uint64_t x) {
  int s = 0;
  //有效在%0中
  __asm__(
  "movq %2, %0;"\
  "shrq %2, $1;"\
  "andq %2, %3;"\
  "andq %0, %3;"\
  "addq %2, %0;"\
  "movq %0, %2;"\

  "shrq %2, $2;"\
  "andq %2, %4;"\
  "andq %0, %4;"\
  "addq %2, %0;"\
  "movq %0, %2;"\

  "shrq %2, $4;"\
  "andq %2, %5;"\
  "andq %0, %5;"\
  "addq %2, %0;"\
  "movq %0, %2;"\

  "shrq %2, $8;"\
  "andq %2, %6;"\
  "andq %0, %6;"\
  "addq %2, %0;"\
  "movq %0, %2;"\

  "shrq %2, $16;"\
  "andq %2, %7;"\
  "andq %0, %7;"\
  "addq %2, %0;"\
  "movq %0, %2;"\

  "shrq %2, $32;"\
  "andq %2, %8;"\
  "andq %0, %8;"\
  "addq %2, %0;"\
  "movq %0, %2;"
  : 
  "=r" (s):
  "0" (s), "r" (x), "n"(0x5555555555555555), "n"(0x3333333333333333), "n"(0x0F0F0F0F0F0F0F0F),
   "n"(0x00FF00FF00FF00FF), "n"(0x0000FFFF0000FFFF), "n"(0x00000000FFFFFFFF)
  );
  
  // for (int i = 0; i < 64; i++) {
  //   if ((x >> i) & 1) s++;
  // }
  return s;
}

void *asm_memcpy(void *dest, const void *src, size_t n) {
  return memcpy(dest, src, n);
}

int asm_setjmp(asm_jmp_buf env) {
  return setjmp(env);
}

void asm_longjmp(asm_jmp_buf env, int val) {
  longjmp(env, val);
}
