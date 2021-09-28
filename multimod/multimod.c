#include <stdint.h>

uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
  return (a * b) % m; // buggy
}
