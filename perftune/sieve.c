#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>

#define N 10000000

static int is_prime[N] = {0};
static int primes[N];

#define ABS(x) (x > 0 ? x : -x)

// static uint16_t my_sqrt(uint32_t n){
//   double guess = n;
//   while (ABS(guess * guess - n) >= 0.25){
//     guess = (guess + n / guess) / 2.0;
//   }

//   return (uint16_t)guess + 1;
// }


int *sieve(int n) {
  assert(n + 1 < N);

  for (int i = 2; i <= (n); i++) {//O(n^2)变为O(n^{3/2})
    for (int j = i + i; j <= n; j += i) {
      is_prime[j] = 1; // 用1表示false，0默认为true
    }
  }

  int *p = primes;
  for (int i = 2; i <= n; i++)//O(n)
    if (!is_prime[i]) {// 0默认为true
      *p++ = i;
    }
  *p = 0;
  return primes;
}
