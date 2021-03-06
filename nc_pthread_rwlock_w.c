#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdint.h>
#include "op_atomic.h"

a_uint32_t thread_count = 0;
static pthread_rwlock_t rwlock[4096];

void setup(int num_threads)
{
  for (int i = 0; i < num_threads; i++)
    pthread_rwlock_init(&rwlock[i], NULL);
}

void* test(void *arg)
{
  uint32_t tid = atomic_fetch_add_explicit(&thread_count, 1,
                                           memory_order_relaxed);
  uint64_t bound = *(uint64_t*)arg;
  for (uint64_t i = 0; i < bound; i++)
    {
      pthread_rwlock_wrlock(&rwlock[tid]);
      pthread_rwlock_unlock(&rwlock[tid]);
    }
  return NULL;
}
