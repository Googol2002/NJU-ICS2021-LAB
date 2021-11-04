#include "asm.h"

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
  uint64_t s = 0;
  uint64_t c1 = 0x5555555555555555, c2 = 0x3333333333333333, c3 = 0x0F0F0F0F0F0F0F0F, 
    c4 = 0x00FF00FF00FF00FF, c5 = 0x0000FFFF0000FFFF, c6 = 0x00000000FFFFFFFF;
  //有效在%0中
  __asm__(
  "movq %2, %0;"\
  "shrq $1, %2;"\
  "andq %3, %2;"\
  "andq %3, %0;"\
  "addq %2, %0;"\
  "movq %0, %2;"\

  "shrq $2, %2;"\
  "andq %4, %2;"\
  "andq %4, %0;"\
  "addq %2, %0;"\
  "movq %0, %2;"\

  "shrq $4 ,%2;"\
  "andq %5, %2;"\
  "andq %5, %0;"\
  "addq %2, %0;"\
  "movq %0, %2;"\

  "shrq $8, %2;"\
  "andq %6, %2;"\
  "andq %6, %0;"\
  "addq %2, %0;"\
  "movq %0, %2;"\

  "shrq $16, %2;"\
  "andq %7, %2;"\
  "andq %7, %0;"\
  "addq %2, %0;"\
  "movq %0, %2;"\

  "shrq $32, %2;"\
  "andq %8, %2;"\
  "andq %8, %0;"\
  "addq %2, %0;"\
  "movq %0, %2;":
  "=r" (s):
  "0" (s), "r" (x), "m"(c1), "m"(c2), "m"(c3), "m"(c4), "m"(c5), "m"(c6)
  );

  return s;
}

void *asm_memcpy(void *dest, const void *src, size_t n) {
  __asm__(
    "mov $0, %%rsi;"\
    "1:;"\
    "movb (%0, %%rsi), %%al;"\
    "movb %%al, (%1, %%rsi);"\
    "inc %%rsi;"\
    "cmp %%rsi, %2;"\
    "jg 1b":
    :
    "r"(src), "r"(dest), "r"(n):
    "memory", "rsi", "rax"
  );
  
  return dest;
}

int asm_setjmp(asm_jmp_buf env) {
  
   asm (
    "push %%rbp;"
    "mov %%rsp, %%rbp;"
    "mov %%rdi, %%rax;" //rax <- env
    "mov %%rcx, (%%rax);"
    "mov %%rdx, 8(%%rax);"
    "mov %%rbx, 16(%%rax);"
    "mov %%rsi, 24(%%rax);"
    "mov (%%rbp), %%rdi;"
    "mov %%rdi, 32(%%rax);"
    "lea 16(%%rbp), %%rdi;"
    "mov %%rdi, 40(%%rax);"
    "mov 8(%%rbp), %%rdi;"
    "mov %%rdi, 48(%%rax);"
    "xor %%rax, %%rax;"
    "pop %%rbp;"
    :
    :
    :
  );
  return 0;
  
  return 0;
}

void asm_longjmp(asm_jmp_buf env, int val) {
  asm(
    "mov %%rsi, %%rax;"
    "mov (%%rdi), %%rcx;"
    "mov 8(%%rdi), %%rdx;"
    "mov 16(%%rdi), %%rbx;"
    "mov 24(%%rdi), %%rsi;"
    "mov 32(%%rdi), %%rbp;"
    "mov 40(%%rdi), %%rsp;"
    "mov 48(%%rdi), %%rdi;"
    "jmp *%%rdi;"
    :
    :
    :
  );
}
