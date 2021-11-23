#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>

#define N 10000000

#define TRUE 0
#define FALSE 1

static bool is_prime[N] = {0};
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

  for (int i = 2; i <= n; i++) {
    if(is_prime[i]){
      int product = i * i;
      if (product > n){
        break;
      }
      for (int j = product; j <= n; j += i) {
        is_prime[j] = false;
      }
    }
      
  }

  int *p = primes;
  for (int i = 2; i <= n; i++)
    if (is_prime[i]) {
      *p++ = i;
    }
  *p = 0;
  return primes;
}
