#include "asm.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

int main() {
  asm_jmp_buf buf1, buf2;
  int a = 1, b = 2, c = 3, d = 4, e = 5, i = 9;
  int f = 1, g = 2, h = 3, j = 4, k = 5, l = 9;
  char src[32] = "12345678";
  char dst[32];
  char arr1[] = "abcdefghijk";
  int r = asm_setjmp(buf1);
  char arr2[] = "abcdefghijk";
  if (r == 0) {
    assert(asm_add(1234, 5678) == 6912);
    assert(asm_popcnt(0x0123456789abcdefULL) == 32);
    
    asm_memcpy(dst, src, 9);
    if (asm_setjmp(buf2)){
      
    }
    asm_longjmp(buf1, 123);
  } else {
    assert(r == 123);
    assert(strcmp(src, dst) == 0);
    assert(a == 1);
    assert(b == 2);
    assert(c == 3);
    assert(d == 4);
    assert(e == 5);
    i--;
    if (i > 0){
      asm_longjmp(buf2, 1);
    }
    assert(strcmp(arr1, arr2) == 0);

    printf("PASSED.\n");
  }
}

// asm_jmp_buf my_jum_buf;
// void fun_fun()
// {
//     printf("Enter fun_fun ...\n");
//     asm_longjmp(my_jum_buf, 8);//如下代码不会被执行
//     printf("fun_fun::can't see");
// }

// void fun()
// {
//     fun_fun();
// }

// int main()
// {
//     int ret;
//     if((ret = asm_setjmp(my_jum_buf), ret))
//     {
//         printf("Main: return after calling longjmp, ret = %d.\n", ret);
//     }
//     else
//     {
//         printf("Main: first time return from setjmp, ret = %d\n", ret);
//         fun();
//     }
//     return 0;
// }