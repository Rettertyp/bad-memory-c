#include "badMemoryAlg.h"
#define DEBUG_PRINT 1
#include "debug.h"
#include "instanceGen.h"
#include "test.h"

#define N_INSTANCES 1000
#define N_INTERVALS 100

#pragma GCC diagnostic ignored "-Wunused-parameter"
int main(int argc, char** argv) {

  bool success = true;

  success &= testSimpleYes(N_INSTANCES, N_INTERVALS);

  success &= testSimpleNo(N_INSTANCES, N_INTERVALS);

  if (success) {
    debug_print("All tests passed.\n");
  } else {
    debug_print("Some tests failed.\n");
  }

  return 0;
}