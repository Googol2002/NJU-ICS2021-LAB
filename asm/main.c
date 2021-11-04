#include "asm.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

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

static void first();
static void second();

/* Use a file scoped static variable for the exception stack so we can access
 * it anywhere within this translation unit. */
static asm_jmp_buf exception_env;
static int exception_type;

int main(void) {
    char* volatile mem_buffer = NULL;

    if (asm_setjmp(exception_env)) {
        // if we get here there was an exception
        printf("first failed, exception type: %d\n", exception_type);
    } else {
        // Run code that may signal failure via longjmp.
        puts("calling first");
        first();

        mem_buffer = malloc(300); // allocate a resource
        printf("%s\n", strcpy(mem_buffer, "first succeeded")); // not reached
    }

    free(mem_buffer); // NULL can be passed to free, no operation is performed

    return 0;
}

static void first() {
    asm_jmp_buf my_env;

    puts("entering first"); // reached

    memcpy(my_env, exception_env, sizeof my_env);

    switch (asm_setjmp(exception_env)) {
        case 3: // if we get here there was an exception.
            puts("second failed, exception type: 3; remapping to type 1");
            exception_type = 1;

        default: // fall through
            memcpy(exception_env, my_env, sizeof exception_env); // restore exception stack
            asm_longjmp(exception_env, exception_type); // continue handling the exception

        case 0: // normal, desired operation
            puts("calling second"); // reached 
            second();
            puts("second succeeded"); // not reached
    }

    memcpy(exception_env, my_env, sizeof exception_env); // restore exception stack

    puts("leaving first"); // never reached
}

static void second() {
    puts("entering second" ); // reached

    exception_type = 3;
    asm_longjmp(exception_env, exception_type); // declare that the program has failed

    puts("leaving second"); // not reached
}

// asm_jmp_buf mainTask, childTask;

// void call_with_cushion();
// void child();

// int main() {
//     if (!asm_setjmp(mainTask)) {
//         call_with_cushion(); // child never returns, yield
//     } // execution resumes after this "}" after first time that child yields

//     while (1) {
//         printf("Parent\n");
        
//         if (!asm_setjmp(mainTask))
//             asm_longjmp(childTask, 1); // yield - note that this is undefined under C99
//     }
// }

// void call_with_cushion() {
//     char space[1000]; // Reserve enough space for main to run
//     space[999] = 1; // Do not optimize array out of existence
//     child();
// }

// void child() {
//     while (1) {
//         printf("Child loop begin\n");
        
//         if (!asm_setjmp(childTask))
//             asm_longjmp(mainTask, 1); // yield - invalidates childTask in C99

//         printf("Child loop end\n");

//         if (!asm_setjmp(childTask))
//             asm_longjmp(mainTask, 1); // yield - invalidates childTask in C99
//     }

//     /* Don't return. Instead we should set a flag to indicate that main()
//        should stop yielding to us and then longjmp(mainTask, 1) */
// }