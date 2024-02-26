#include "badMemoryAlg.h"
#define DEBUG_PRINT 1
#include "debug.h"
#include "instanceGen.h"
#include "test.h"

#define N_INTERVALS 1000

#pragma GCC diagnostic ignored "-Wunused-parameter"
int main(int argc, char** argv) {
  testRunAll(N_INTERVALS);
  testRunAllDepthFirst(N_INTERVALS);
  return 0;
}