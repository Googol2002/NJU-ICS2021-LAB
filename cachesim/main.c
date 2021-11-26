#include "common.h"
#include <time.h>
#include <unistd.h>

uint32_t cpu_read(uintptr_t addr, int len);
void cpu_write(uintptr_t addr, int len, uint32_t data);
uint32_t cpu_uncache_read(uintptr_t addr, int len);
void cpu_uncache_write(uintptr_t addr, int len, uint32_t data);

void init_mem(void);
void init_cache(int total_size_width, int associativity_width);
void display_statistic(void);

static uint32_t seed;
static char *tracefile;

static void init_rand(uint32_t seed) {
  printf("random seed = %u\n", seed);
  srand(seed);
}

static inline uint32_t choose(uint32_t n) { return rand() % n; }

struct _trace {
  uint32_t addr : 28;
  uint8_t len : 3;
  bool is_write : 1;
};

struct trace {
  struct _trace t;
  uint32_t data;
};

static void trace_exec(struct trace *t, bool is_check) {
  if (t->t.is_write) {
    cpu_write(t->t.addr, t->t.len, t->data);
    if (is_check) {
      cpu_uncache_write(t->t.addr, t->t.len, t->data);
    }
  }
  else {
    uint32_t ret = cpu_read(t->t.addr, t->t.len);
    if (is_check) {
      uint32_t ret_uncache = cpu_uncache_read(t->t.addr, t->t.len);
      assert(ret == ret_uncache);
    }
  }
}

static void random_trace(void) {
  const int choose_len[] = {1, 2, 4};
  struct trace t;

  int i;
  for (i = 0; i < 1000000; i ++) {
    t.t.len = choose_len[ choose(sizeof(choose_len) / sizeof(choose_len[0])) ] ;
    t.t.addr = choose(MEM_SIZE) & ~(t.t.len - 1);
    t.t.is_write = choose(2);
    if (t.t.is_write) t.data = rand();

    trace_exec(&t, true);
  }
}

static void check_diff(void) {
  uintptr_t addr = 0;
  for (addr = 0; addr < MEM_SIZE; addr += 4) {
    uint32_t ret = cpu_read(addr, 4);
    uint32_t ret_uncache = cpu_uncache_read(addr, 4);
    assert(ret == ret_uncache);
  }
}

static void parse_args(int argc, char *argv[]) {
  int o;
  bool has_seed = false;
  char *p;
  while ( (o = getopt(argc, argv, "-r:")) != -1) {
    switch (o) {
      case 'r': seed = strtol(optarg, &p, 0);
                if (!(*optarg != '\0' && *p =='\0')) {
                  printf("invalid seed\n");
                }
                else {
                  has_seed = true;
                }
                break;
      case 1:
                if (tracefile != NULL) printf("too much argument '%s', ignored\n", optarg);
                else tracefile = optarg;
                break;
      default:
                printf("Usage: %s [-r seed] [trace_file]\n", argv[0]);
                assert(0);
    }
  }

  if (!has_seed) {
    seed = time(0);
  }
}

void replay_trace(void) {
  if (tracefile == NULL) {
    random_trace();
    check_diff();
    printf("Random test pass!\n");
    return;
  }

  char cmd[80];
  sprintf(cmd, "bzcat %s", tracefile);
  FILE *fp = popen(cmd, "r");
  assert(fp);

  struct trace t;
  while (fread(&t.t, sizeof(t.t), 1, fp) == 1) {
    // do not care data
    trace_exec(&t, false);
  }

  pclose(fp);
}

int main(int argc, char *argv[]) {
  parse_args(argc, argv);

  init_rand(seed);
  init_mem();

  init_cache(14, 2);

  replay_trace();

  display_statistic();

  return 0;
}
