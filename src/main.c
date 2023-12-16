#include "badMemoryAlg.h"
#include "debug.h"
#include <stdio.h>

#pragma GCC diagnostic ignored "-Wunused-parameter"
int main(int argc, char** argv) {

  // print the value of the DEBUG_PRINT macro
  debug_print("DEBUG_PRINT: %d\n", DEBUG_PRINT);

  Interval intervals1[] = {
      {4, 7}, {4, 7}, {4, 7}, {4, 5}, {1, 3}, {3, 5}, {2, 6}, {1, 5}, {1, 5}, {1, 5},
  };

  IntervalSet* intervalSet1 = createBlankIntervalSet(intervals1, 10);

  bool hasRes = badMemoryAlgorithm(intervalSet1);

  if (hasRes) {
    debug_print("The interval set has a solution.\n");
  } else {
    debug_print("The interval set does not have a solution.\n");
  }

  deleteIntervalSet(intervalSet1);

  return 0;
}