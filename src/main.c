#include "badMemoryAlg.h"
#define DEBUG_PRINT 1
#include "debug.h"
#include "instanceGen.h"

#define N_INSTANCES 1000
#define N_INTERVALS 100

#pragma GCC diagnostic ignored "-Wunused-parameter"
int main(int argc, char** argv) {

  /* // print the value of the DEBUG_PRINT macro
  debug_print("DEBUG_PRINT: %d\n", DEBUG_PRINT);

  Interval intervals[] = {
      {5, 5}, {4, 9}, {4, 6}, {4, 7}, {2, 9}, {2, 2}, {2, 4}, {1, 1}, {1, 2}, {1, 1},
  };

  IntervalSet* intervalSet = intervalSetCreateBlank(intervals, 10);

  bool hasRes = badMemoryAlgorithm(intervalSet);

  if (hasRes) {
    debug_print("The interval set has a solution.\n");
  } else {
    debug_print("The interval set does not have a solution.\n");
  }

  intervalSetDelete(intervalSet); */

  instanceInitRandom();

  for (unsigned int i = 0; i < N_INSTANCES; i++) {
    if (!(i % 100)) {
      debug_print("Instance %d\n", i);
    }

    IntervalSet* instance = instanceSimpleYes(N_INTERVALS);
    bool hasSolution = badMemoryAlgorithm(instance);

    if (!hasSolution) {
      debug_print("Instance %d has no solution.\n", i);
      if (instance->length == 0) {
        debug_print("()");
      } else
        for (unsigned int i = 0; i < instance->length; i++) {
          debug_print("(%d, %d), ", instance->intervals[i].bottom, instance->intervals[i].top);
        }

      debug_print("\n");
    }

    intervalSetDelete(instance);
  }

  debug_print("Done.\n");

  return 0;
}