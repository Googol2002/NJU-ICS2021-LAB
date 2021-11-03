#include "asm.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

// int main() {
//   asm_jmp_buf buf1, buf2;
//   int a = 1, b = 2, c = 3, d = 4, e = 5, i = 9;
//   char src[32] = "12345678";
//   char dst[32];
//   int r = asm_setjmp(buf1);
//   if (r == 0) {
//     assert(asm_add(1234, 5678) == 6912);
//     assert(asm_popcnt(0x0123456789abcdefULL) == 32);
    
//     asm_memcpy(dst, src, 9);
//     if (asm_setjmp(buf2)){
      
//     }
//     asm_longjmp(buf1, 123);
//   } else {
//     assert(r == 123);
//     assert(strcmp(src, dst) == 0);
//     assert(a == 1);
//     assert(b == 2);
//     assert(c == 3);
//     assert(d == 4);
//     assert(e == 5);
//     i--;
//     if (i > 0){
//       asm_longjmp(buf2, 1);
//     }
//     printf("PASSED.\n");
//   }
// }

#include <stdio.h>
#include <setjmp.h>

//儲存程式跳躍時所需之資訊

// asm_jmp_buf jmpbuffer;

// int fun_a(int v) {
//   int r = v * 2 - 1;
//   if (r < 0) {
//     // 跳躍至 main 函數
//     asm_longjmp(jmpbuffer, 1);
//   }
//   return r;
// }
// int fun_b(int v) {
//   int r = fun_a(v) + 6;
//   if (r > 10) {
//     // 跳躍至 main 函數
//     asm_longjmp(jmpbuffer, 2);
//   }
//   return r;
// }
// int fun_c(int v) {
//   int r = fun_b(v) * 5 - 21;
//   return r;
// }
// int main() {
//   // 設定跳躍目標位置
//   int jmpVal = asm_setjmp(jmpbuffer);
//   if ( jmpVal == 1 ) {
//     printf("fun_a errorn");
//   } else if ( jmpVal == 2 ) {
//     printf("fun_b errorn");
//   } else {  // jmpVal == 0
//     int x = -5;
//     int result = fun_c(x);
//     printf("Result = %dn", result);
//   }
//   return 0;
// }

#include <stdio.h>
#include <setjmp.h>

// 儲存程式跳躍時所需之資訊
jmp_buf jmpbuffer;

int fun_a(int v) {
  int r = v * 2 - 1;
  if (r < 0) {
    // 跳躍至 main 函數
    longjmp(jmpbuffer, 1);
  }
  return r;
}
int fun_b(int v) {
  int r = fun_a(v) + 6;
  if (r > 10) {
    // 跳躍至 main 函數
    longjmp(jmpbuffer, 2);
  }
  return r;
}
int fun_c(int v) {
  int r = fun_b(v) * 5 - 21;
  return r;
}
int main() {
  // 設定跳躍目標位置
  int jmpVal = setjmp(jmpbuffer);
  if ( jmpVal == 1 ) {
    printf("fun_a errorn");
  } else if ( jmpVal == 2 ) {
    printf("fun_b errorn");
  } else {  // jmpVal == 0
    int x = 11;
    int result = fun_c(x);
    printf("Result = %dn", result);
  }
  return 0;
}