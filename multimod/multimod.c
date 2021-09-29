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

uint64_t power_mod(uint64_t x, uint64_t exp, uint64_t m){
    if (exp == 0){
      return mod(x, m);
    }
    
    uint64_t h128 = x >> (64 - exp);
    uint64_t l128 = x << exp;
    uint64_t valid = valid_bits(m);
    uint64_t mask = UINT64_MAX;

    for (int r = 128 - valid; r >= 0; r--){
        int l = r + valid - 1; // 这就是最左端下标
        uint64_t buffer = 0;
        // 读数
        if (r >= 64){
            buffer = (h128 >> (r - 64));
        }else if (l <= 63){
            buffer = ((l128 >> (r - 0)) | (h128  << (64 - r)));
        }else {
            buffer = ((h128  << (64 - r)) | (l128 >> r));
        }
        //求解
        if (buffer >= m){
            buffer -= m;
        }
        //覆写
        if (r >= 64){
            h128 &= ~(mask << (r - 64));
            h128 |= (buffer << (r - 64));
        }else if (l <= 63){
            l128 &= ~(mask << r);
            h128 &= ~(mask >> (64 - r));
            l128 |= (buffer << r);
            h128 |= (buffer >> (64 - r));
        }else {
            h128 &= ~(mask >> (64 - r));
            l128 &= ~(mask << r);
            h128 |= (buffer >> (64 - r));
            l128 |= (buffer << r);
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
