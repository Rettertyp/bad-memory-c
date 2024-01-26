#include "badMemoryAlg.h"
#define DEBUG_PRINT 1
#include "debug.h"
#include "instanceGen.h"
#include "test.h"

#define N_INSTANCES 1000
#define N_INTERVALS 100

#pragma GCC diagnostic ignored "-Wunused-parameter"
int main(int argc, char** argv) {
  // testRunAll(N_INSTANCES, N_INTERVALS);
  testRunYes(1000);
  return 0;
}