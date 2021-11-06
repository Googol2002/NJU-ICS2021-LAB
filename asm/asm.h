#include <stdint.h>
#include <stddef.h>

// typedef uint64_t __my_jmp_buf[8];
// struct __my__jmp_buf_struct{
//     uintptr_t __jmpbuf[8];
// };
// typedef struct __my__jmp_buf_struct my_jmp_buf[1];
typedef uint64_t my_jmp_buf[8];
#define asm_jmp_buf my_jmp_buf

int64_t asm_add(int64_t a, int64_t b);
int     asm_popcnt(uint64_t x);
void   *asm_memcpy(void *dest, const void *src, size_t n);
int     volatile asm_setjmp(asm_jmp_buf env) __attribute__ ((__nothrow__));
void    volatile asm_longjmp(asm_jmp_buf env, int val) __attribute__ ((__nothrow__));
//  __attribute__ ((__noreturn__))