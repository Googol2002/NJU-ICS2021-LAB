#include <stdint.h>
#include <assert.h>
#include <stdio.h>

#define LENGTH64 (sizeof(uint64_t) * 8)

uint64_t valid_bits(uint64_t n){
  uint64_t valid;

  for (valid = 0; n; ++valid)
    n = n >> 1;

  return valid;
}

uint64_t power_mod(uint64_t x, uint64_t exp, uint64_t m);
uint64_t ONE = 1;

//x只能处理在2^64-1范围内的数
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
  if (t < x || t < y){// t + 2 ^ 64溢出
    //(~m) + 1 相当于 2 ^ 64 - m
    return mod(mod(t, m) +  mod((~m) + 1, m), m);
  }else {
    return mod(t, m);
  }

}

#define LHS(x, t) ((t >= 64 || t < 0) ? 0 : x << t)
#define RHS(x, t) ((t >= 64 || t < 0) ? 0 : x >> t)

uint64_t power_mod(uint64_t x, uint64_t exp, uint64_t m){
    if (exp == 0){
      return mod(x, m);
    }
    
    uint64_t c = 0;
    uint64_t h128 = x >> (64 - exp);
    uint64_t l128 = x << exp;
    uint64_t valid = valid_bits(m);
    uint64_t mask = UINT64_MAX;

    for (int r = 128 - valid; r >= 0; r--){
        int l = r + valid - 1; // 这就是最左端下标
        uint64_t buffer = 0;
        _Bool flag = 0;
        // 读数
        if (r >= 64){
            buffer = RHS(h128, (r - 64));
            c = RHS(h128, (r - 64 + 64)) & 1;
        }else if (l <= 63){
            buffer = (RHS(l128, (r - 0)) | LHS(h128, (64 - r)));
            c = RHS(h128, (r - 64 + 64)) & 1;
        }else {
            buffer = (LHS(h128, (64 - r)) | RHS(l128, r));
            c = RHS(h128, (r)) & 1;
        }
        
        if (c == 1){//t + 2^64 的情况
          if (buffer >= m){
            printf("l: %d r %d", l, r);
            //assert(0);
          }
          buffer += (~m + 1);
          flag = 1;
        }
        if (buffer >= m){//t 情况
            buffer -= m;
        }
        //覆写
        if (r >= 64){
            h128 &= ~LHS(mask, (r - 64));
            h128 |= LHS(buffer, (r - 64));
            if (flag){
              h128 &= ~LHS(ONE, (r));
            }
        }else if (l <= 63){
            l128 &= ~LHS(mask, r);
            h128 &= ~RHS(mask, (64 - r));
            l128 |= LHS(buffer, r);
            h128 |= RHS(buffer, (64 - r));
            if (flag){
              h128 &= ~LHS(ONE, (r));
            }
        }else {
            h128 &= ~RHS(mask, (64 - r));
            l128 &= ~LHS(mask, r);
            h128 |= RHS(buffer, (64 - r));
            l128 |= LHS(buffer, r);
            if (flag){
              h128 &= ~LHS(ONE, (r));
            }
        }
    }

    return l128;
}



uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
  uint64_t result = 0;
  
  for (int i = 0; i < 64; i++){
    uint64_t sign = (a >> i) & 1;
    if (sign == 1){
      result = add_mod(result, power_mod(b, i, m), m);
    }
  }
  return result;
}
