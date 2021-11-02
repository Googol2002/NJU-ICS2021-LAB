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
    //恢复
    "leave;"\
    "popq %%rcx;"\
    
    "xorq %%rax, %%rax;"\
    "jmpq	*%rcx"::
  );

  return 0;
}

void asm_longjmp(asm_jmp_buf env, int val) {
  __asm__(
    "movl %1, %%eax;"\
    // 恢复rbp
    "movq (%0), %%rbp;"\
    // 恢复rbx
    "movq 8(%0), %%rbx;"\
    // 恢复r12-r15
    "movq 32(%0), %%r12;"\
    "movq 40(%0), %%r13;"\
    "movq 48(%0), %%r14;"\
    "movq 56(%0), %%r15;"\
    // 取rip
    "movq 16(%0), %%r10;"\
    // 恢复rsp，在最后恢复是考虑到red zone的原因
    "movq 24(%0), %%rsp;"\
    //恢复rip
    "jmp *%%r10;":
    :"D" (env), "S" (val)
    //将env强制在rdi中，val强制存在rsi中
  );
}
