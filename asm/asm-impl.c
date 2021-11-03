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
  
   __asm__(
    "movq %%rbx, 8(%%rdi);"\
    "movq %%r12, 32(%%rdi);"\
    "movq %%r13, 40(%%rdi);"\
    "movq %%r14, 48(%%rdi);"\
    "movq %%r15, 56(%%rdi);"\
    "leave;"\
    "popq %%rcx;"\
    "movq %%rcx, 16(%%rdi);"\
    "movq %%rbp, 0(%%rdi);"\
    "movq %%rsp, 24(%%rdi);"\
    "xorq %%rax, %%rax;"\
    "jmpq	*%%rcx"::
  );
  
  return 0;
}

void asm_longjmp(asm_jmp_buf env, int val) {
  __asm__(
    "movq %%rsi, %%rax;"\
    "movq 0(%%rdi), %%rbp;"\
    "movq 8(%%rdi), %%rbx;"\
    "movq 32(%%rdi), %%r12;"\
    "movq 40(%%rdi), %%r13;"\
    "movq 48(%%rdi), %%r14;"\
    "movq 56(%%rdi), %%r15;"\
    "movq 16(%%rdi), %%rcx;"\
    "testq	%%rax, %%rax;"\
    "jnz	1f;"\
    "incq	%%rax;"\
    "1:movq 24(%%rdi), %%rsp;"\
    "jmp *%%rcx;":
    :
  );
}
