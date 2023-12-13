#include "badMemoryAlg.h"
#include <stdio.h>

#pragma GCC diagnostic ignored "-Wunused-parameter"
int main(int argc, char** argv) {

  Interval intervals1[] = {{1, 3}, {2, 4}, {3, 5}, {4, 9}, {5, 20}};

  IntervalSet* intervalSet1 = createIntervalSet(intervals1, 5);

  badMemoryAlgorithm(intervalSet1);

  deleteIntervalSet(intervalSet1);

  return 0;
}