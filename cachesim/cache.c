#include "common.h"
#include <inttypes.h>

void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);

static uint64_t cycle_cnt = 0;

void cycle_increase(int n) { cycle_cnt += n; }

// TODO: implement the following functions

struct CACHE_SLOT{
  uint8_t data[BLOCK_SIZE];
  uintptr_t tag;
  bool valid;
  bool dirty;
};

static struct CACHE_SLOT *cache_slot;
static int cache_total_size_width, cache_associativity_width, cache_group_width, block_number_mask;
uintptr_t inner_addr_mask, group_number_mask, tag_mask;

static inline uintptr_t extract_inner_addr(uintptr_t addr){
  return addr & inner_addr_mask;
}

static inline uintptr_t extract_group_number(uintptr_t addr){
  return (addr & group_number_mask) >> BLOCK_WIDTH;
}

static inline uintptr_t extract_tag(uintptr_t addr){
  return (addr & tag_mask) >> (BLOCK_WIDTH + cache_group_width);
}

static inline uintptr_t map_to_cache_addr(uintptr_t addr, uint32_t i){
  return (extract_group_number(addr) << cache_associativity_width) + i;
}

static inline uintptr_t extract_block_number(uintptr_t addr){
  return addr & block_number_mask >> BLOCK_WIDTH;
}

static inline uintptr_t construct_block_number(uintptr_t tag, uint32_t index){
  return tag << cache_group_width | index;
}

static inline uint32_t choose(uint32_t n) { return rand() % n; }

static void write_back(uintptr_t addr, uint32_t index){
  struct CACHE_SLOT* target_cache = &cache_slot[map_to_cache_addr(addr, index)];
  mem_write(construct_block_number(target_cache->tag, index), target_cache->data);
  target_cache->dirty = false;
}

static void read_from(uintptr_t addr, uint32_t index){
  struct CACHE_SLOT* target_cache = &cache_slot[map_to_cache_addr(addr, index)];
  mem_read(extract_block_number(addr), target_cache->data);
  target_cache->valid = true;
}

uint32_t cache_read(uintptr_t addr) {
  for (int i = map_to_cache_addr(addr, 0);
    i < map_to_cache_addr(addr, exp2(cache_associativity_width)); ++i){

    if (cache_slot[i].valid && cache_slot[i].tag == extract_tag(addr)){
        return * ((uint32_t*) &cache_slot[i].data[extract_inner_addr(addr)]);
    }
  }

  // not hit
  uint32_t index = choose(exp2(cache_associativity_width));

  struct CACHE_SLOT* target_cache = &cache_slot[map_to_cache_addr(addr, index)];
  if (target_cache->dirty){
    write_back(addr, index);
  }

  read_from(addr, index);

  return * ((uint32_t*) &target_cache->data[extract_inner_addr(addr)]);
}

void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
  bool hit = false;
  uint32_t index = -1;

  for (int i = map_to_cache_addr(addr, 0);
    i < map_to_cache_addr(addr, exp2(cache_associativity_width)); ++i){
    if (cache_slot[i].valid && cache_slot[i].tag == extract_tag(addr)){
        hit = true;
        index = i;
    }
  }

  struct CACHE_SLOT* target_cache;
  if (!hit){
    index = choose(exp2(cache_associativity_width));
    target_cache = &cache_slot[map_to_cache_addr(addr, index)];
    
    if (target_cache->valid){
      if (target_cache->dirty){
        write_back(addr, index);
      }
      read_from(addr, index);
    }else {
      read_from(addr, index);
    }
  }else {
    target_cache = &cache_slot[map_to_cache_addr(addr, index)];
  }

  uint32_t *data_target = (uint32_t *)(&target_cache->data[extract_inner_addr(addr)]);
  *data_target &= (~wmask);
  *data_target |= (data & wmask);
  target_cache->dirty = true;
}

void init_cache(int total_size_width, int associativity_width) {
  cache_total_size_width = total_size_width;
  cache_associativity_width = associativity_width;
  //(总大小 - 块大小) - 路数 = 组数
  cache_group_width = cache_total_size_width - BLOCK_WIDTH - associativity_width;
  
  inner_addr_mask = mask_with_len(BLOCK_WIDTH);
  group_number_mask = mask_with_len(cache_group_width) << BLOCK_WIDTH;
  tag_mask = mask_with_len(MEM_SIZE - BLOCK_WIDTH - cache_group_width) 
    << (BLOCK_WIDTH + cache_group_width);
  block_number_mask = mask_with_len(MEM_SIZE - BLOCK_WIDTH);

  // exp2(cache_total_size_width) 为 cache全部数据区大小
  cache_slot = calloc(exp2(cache_total_size_width) / BLOCK_SIZE, sizeof(struct CACHE_SLOT));
  assert(cache_slot);
}

void display_statistic(void) {
}
