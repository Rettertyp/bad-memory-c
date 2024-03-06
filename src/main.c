#include "badMemoryAlg.h"
#define DEBUG_PRINT 1
#include "debug.h"
#include "instanceGen.h"
#include "test.h"
#include <omp.h>
#include <stdlib.h>

#pragma GCC diagnostic ignored "-Wunused-parameter"
int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Usage: ./badMemAlg <n_intervals>\n");
    return 1;
  }

  const uint32_t N_INTERVALS = atoi(argv[1]);

  // log the number of threads
  printf("Max number of threads: %d\n", omp_get_max_threads());

  testRunAllParallelized(N_INTERVALS);
  testRunAll(N_INTERVALS);
  testSameSimpleInstances(N_INTERVALS, 20);
  testRunAllDepthFirst(N_INTERVALS);

  printf("Done.\n");
  return 0;
}