#include <string.h>
#include "common.h"

static uint8_t mem[MEM_SIZE];
static uint8_t mem_diff[MEM_SIZE];

void init_mem(void) {
  int i;
  for (i = 0; i < MEM_SIZE; i ++) {
    mem[i] = rand() & 0xff;
  }

  memcpy(mem_diff, mem, MEM_SIZE);
}

void mem_read(uintptr_t block_num, uint8_t *buf) {
  memcpy(buf, mem + (block_num << BLOCK_WIDTH), BLOCK_SIZE);
  cycle_increase(25);
}

void mem_write(uintptr_t block_num, const uint8_t *buf) {
  memcpy(mem + (block_num << BLOCK_WIDTH), buf, BLOCK_SIZE);
  cycle_increase(6);
}

uint32_t mem_uncache_read(uintptr_t addr) {
  uint32_t *p = (void *)mem_diff + (addr & ~0x3);
  return *p;
}

void mem_uncache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
  uint32_t *p = (void *)mem_diff + (addr & ~0x3);
  *p = (*p & ~wmask) | (data & wmask);
}
