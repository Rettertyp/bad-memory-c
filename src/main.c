#include "badMemoryAlg.h"
#define DEBUG_PRINT 1
#include "debug.h"
#include "instanceGen.h"
#include "test.h"
#include <stdlib.h>

#pragma GCC diagnostic ignored "-Wunused-parameter"
int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Usage: ./badMemAlg <n_intervals>\n");
    return 1;
  }

  const uint32_t N_INTERVALS = atoi(argv[1]);

  testRunAll(N_INTERVALS);
  testRunAllDepthFirst(N_INTERVALS);
  return 0;
}