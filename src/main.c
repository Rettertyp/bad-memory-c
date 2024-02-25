#include "badMemoryAlg.h"
#define DEBUG_PRINT 1
#include "debug.h"
#include "instanceGen.h"
#include "test.h"

#define N_INSTANCES 1
#define N_INTERVALS 200

#pragma GCC diagnostic ignored "-Wunused-parameter"
int main(int argc, char** argv) {
  testRunAll(N_INSTANCES, N_INTERVALS);
  // testRunYes(N_INTERVALS);
  testRunAllDepthFirst(N_INSTANCES, N_INTERVALS);
  return 0;
}