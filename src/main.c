#include "badMemoryAlg.h"
#include <stdio.h>

#pragma GCC diagnostic ignored "-Wunused-parameter"
int main(int argc, char** argv) {

  Interval intervals1[] = {{1, 3}, {1, 3}, {2, 2}};

  IntervalSet* intervalSet1 = createBlankIntervalSet(intervals1, 3);

  bool hasRes = badMemoryAlgorithm(intervalSet1);

  if (hasRes) {
    printf("The interval set has a solution.\n");
  } else {
    printf("The interval set does not have a solution.\n");
  }

  deleteIntervalSet(intervalSet1);

  return 0;
}