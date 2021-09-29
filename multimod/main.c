#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

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

int main() {
  test(123, 456, 789);
  // test(123, 456, -1ULL);
  // test(-2ULL, 2, -1ULL);
  // test(2, -2ULL, -1ULL);
  //定位错误
  test(3, -2ULL, -1ULL);
  // test(-2ULL, -2ULL, -1ULL); // should be 1

  uint64_t a = UINT64_MAX / 3, b = UINT64_MAX / 2, m = 10071;
  // test_add(a, b, m);
  // test_power(a, 5, m);
  // test_power(123, 2, m);
}
