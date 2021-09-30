#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>

uint64_t multimod(uint64_t, uint64_t, uint64_t);
uint64_t add_mod(uint64_t x, uint64_t y, uint64_t m);
uint64_t power_mod(uint64_t x, uint64_t exp, uint64_t m);

void test(uint64_t a, uint64_t b, uint64_t m) {
  #define U64 "%" PRIu64
  printf(U64 " * " U64 " mod " U64 " = " U64 "\n", a, b, m, multimod(a, b, m));
}

void test_add(uint64_t a, uint64_t b, uint64_t m) {
  #define U64 "%" PRIu64
  printf(U64 " + " U64 " mod " U64 " = " U64 "\n", a, b, m, add_mod(a, b, m));
}

void test_power(uint64_t a, uint64_t i, uint64_t m) {
  #define U64 "%" PRIu64
  printf(U64 " * 2^" U64 " mod " U64 " = " U64 "\n", a, i, m, power_mod(a, i, m));
}

void test_multimod();

int main() {
  // test(123, 456, 789);
  // test(123, 456, -1ULL);
  // test(-2ULL, 2, -1ULL);
  // test(2, -2ULL, -1ULL);
  //定位错误
  // // test(3, -2ULL, -1ULL);
  // test(0xFFFFFFFF, -2ULL, -1ULL);
  // test(0x2FFFFFFFF, -2ULL, -1ULL);
  // test(-2ULL, -2ULL, -1ULL); // should be 1

  test_multimod();
  // test_add(a, b, m);
  // test_power(a, 5, m);
  // test_power(123, 2, m);
}

#define TEST_CMD_P_PATH "./testcase.txt"
#define ARRLEN(x) (sizeof(x) / sizeof(x[0]))
void test_multimod(){
  int counter = 0;
  char buffer[65535];
  FILE *fp = fopen(TEST_CMD_P_PATH, "r");
  assert(fp != NULL);

  char* input = fgets(buffer, ARRLEN(buffer), fp);  
  #define U64 "%" PRIu64
  while (input != NULL){
    uint64_t a, b, m, ans;
    _Bool success = 0;
    sscanf(input, U64 " " U64 " " U64 " " U64, &a, &b, &m, &ans);
    printf("Testing " U64 " times " U64 " mod " U64 " ...\n", a, b, m);
    uint64_t result = multimod(a, b, m);
    assert(result == ans);
    input = fgets(buffer, ARRLEN(buffer), fp);
    ++counter;
  }

  printf("通过%d个测试样例 \n", counter);
}