#include "asm.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

// int main() {
//   asm_jmp_buf buf1, buf2;
//   int a = 1, b = 2, c = 3, d = 4, e = 5, i = 9;
//   int f = 1, g = 2, h = 3, j = 4, k = 5, l = 9;
//   char src[32] = "12345678";
//   char dst[32];
//   char arr1[] = "abcdefghijk";
//   int r = asm_setjmp(buf1);
//   char arr2[] = "abcdefghijk";
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
//     assert(strcmp(arr1, arr2) == 0);

//     printf("PASSED.\n");
//   }
// }

static asm_jmp_buf buf;

void second() {
    printf("second\n");         // prints
    asm_longjmp(buf,1);             // jumps back to where setjmp was called - making setjmp now return 1
}

void first() {
    second();
    printf("first\n");          // does not print
}

int main() {   
    if (!asm_setjmp(buf))
        first();                // when executed, setjmp returned 0
    else                        // when longjmp jumps back, setjmp returns 1
        printf("main\n");       // prints

    return 0;
}