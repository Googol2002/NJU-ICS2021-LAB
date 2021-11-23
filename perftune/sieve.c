#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>

#define N 10000000

#define TRUE 0
#define FALSE 1

static bool is_prime[N] = {};
static int primes[N];

#define MIN(x, y) (x > y ? y : x)

// static uint16_t my_sqrt(uint32_t n){
//   double guess = n;
//   while (ABS(guess * guess - n) >= 0.25){
//     guess = (guess + n / guess) / 2.0;
//   }

//   return (uint16_t)guess + 1;
// }


int *sieve(int n) {
  assert(n + 1 < N);
  for (int i = 0; i <= n; i++)
    is_prime[i] = true;
  is_prime[2] = false;

  for (int i = 3; i <= n; i += 2) {
    if(is_prime[i]){
      int product = i * i, step = 2 * i;
      if (product > n){
        break;
      }
      // 对于 i * k (k < i) 应该在 i开始前被筛掉，因为i * k 有因子 k 小于 i
      // i * (i + 2k * i) i必为奇数 i + 2k * i 必为奇数
      for (int j = product; j <= n; j += step) {
        is_prime[j] = false;
      }
    }
  }

  int *p = primes;
  *p++ = 2;
  for (int i = 2; i <= n; i += 2)
    if (is_prime[i]) {
      *p++ = i;
    }
  *p = 0;
  return primes;
}
