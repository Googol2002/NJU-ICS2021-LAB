#include <stdint.h>

uint64_t valid_bits(uint64_t n){
  uint64_t valid;

  for (valid = 0; n; ++valid)
    n = n >> 1;

  return valid;
}

uint64_t mod(uint64_t x, uint64_t m){
  uint64_t valid = valid_bits(m);

  
}




uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
  return (a * b) % m; // buggy
}
