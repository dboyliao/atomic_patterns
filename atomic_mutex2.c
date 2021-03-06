#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include "op_atomic.h"

a_uint32_t thread_count = 0;
static atomic_bool lock[SLOT] = {};
extern uint64_t counts[];
ATOMIC_HACK_DECLARE
extern uint64_t count;

void setup(int num_threads) {}

void* test(void *arg)
{
  uint64_t bound = *(uint64_t*)arg;
  bool expected_bool = false;
  uint32_t tid = atomic_fetch_add_explicit(&thread_count, 1,
                                           memory_order_relaxed);
  uint32_t slot = tid % SLOT;
  for (uint64_t i = 0; i < bound; i++)
    {
      while (!atomic_compare_exchange_strong_explicit(
        &lock[slot], &expected_bool, true,
        memory_order_acquire,
        memory_order_relaxed))
        {
          expected_bool = false;
          ATOMIC_HACK_OP;
        }
      counts[slot]++;
      atomic_store_explicit(&lock[slot], false, memory_order_release);
    }
  return NULL;
}
