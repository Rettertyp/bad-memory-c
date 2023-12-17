#include "instanceGen.h"
#include "debug.h"
#include <stdlib.h>
#include <time.h>

/**
 * Initializes the random number generator.
 * This function sets the seed for the random number generator based on the current time.
 */
void instanceInitRandom() { srand(time(NULL)); }

/**
 * Generates a random number between the given minimum and maximum values.
 *
 * @param min The minimum value of the range.
 * @param max The maximum value of the range.
 * @return The randomly generated number.
 */
static int getRandomNumber(const int min, const int max) {
  return (rand() % (max - min + 1)) + min;
}

/**
 * Returns an Interval containing the given integer.
 *
 * @param i The integer to be contained in the Interval.
 * @return The Interval containing the given integer.
 */
static Interval getIntervalContainingI(const int i) {
  int start = getRandomNumber(__max(1, i / 2), i);
  int end = getRandomNumber(i, 2 * i);

  return (Interval){start, end};
}

/**
 * Generates a random simple instance of the GAI problem with a solution.
 *
 * @param n The number of intervals in the instance.
 */
IntervalSet* instanceSimpleYes(const int n) {
  // generate random numbers that add up to n
  int groups[n];
  int sum = 0;
  int nGroups = 0;

  for (int i = 0; i < n; i++) {
    groups[i] = getRandomNumber(1, n - sum);
    sum += groups[i];
    nGroups++;

    if (sum == n) {
      break;
    }
  }

  // print the groups
  debug_print("Groups: ");
  for (int i = 0; i < nGroups; i++) {
    debug_print("%d ", groups[i]);
  }
  debug_print("\n");

  // generate the intervals
  Interval intervals[n];

  int currGroup = 0;
  int currInterval = 0;
  for (int i = 0; i < nGroups; i++) {
    for (int j = 0; j < (currGroup = groups[i]); j++) {
      intervals[currInterval++] = getIntervalContainingI(currGroup);
    }
  }

  // print the intervals
  debug_print("Intervals: ");
  for (int i = 0; i < n; i++) {
    debug_print("[%d, %d] ", intervals[i].bottom, intervals[i].top);
  }
  debug_print("\n");

  return intervalSetCreateBlank(intervals, n);
}