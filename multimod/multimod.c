#include <stdint.h>

#define LENGTH64 (sizeof(uint64_t) * 8)

uint64_t valid_bits(uint64_t n){
  uint64_t valid;

  for (valid = 0; n; ++valid)
    n = n >> 1;

  return valid;
}

uint64_t mod(uint64_t x, uint64_t m){
  uint64_t valid = valid_bits(m);

  for (int i = LENGTH64 - valid; i >= 0; --i){
    if (x >= (m << i))
      x -= (m << i);
  }

  return x;
}


uint64_t add_mod(uint64_t x, uint64_t y, uint64_t m){
  uint64_t t = x + y;
  if (t < x || t < y){// 溢出
    return mod(mod(t, m) + mod(UINT64_MAX, m), m);
  }else {
    return mod(t, m);
  }

}

uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
  return (a * b) % m; // buggy
}
