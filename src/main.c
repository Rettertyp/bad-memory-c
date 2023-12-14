#include "badMemoryAlg.h"
#include <stdio.h>

#pragma GCC diagnostic ignored "-Wunused-parameter"
int main(int argc, char** argv) {

  Interval intervals1[] = {{1, 3}, {1, 3}, {2, 2}};

  IntervalSet* intervalSet1 = createBlankIntervalSet(intervals1, 3);

  badMemoryAlgorithm(intervalSet1);

  deleteIntervalSet(intervalSet1);

  return 0;
}