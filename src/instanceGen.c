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
static unsigned int getRandomNumber(const unsigned int min, const unsigned int max) {
  return (rand() % (max - min + 1)) + min;
}

/**
 * Returns an Interval containing the given integer.
 *
 * @param i The integer to be contained in the Interval.
 * @return The Interval containing the given integer.
 */
static Interval getIntervalContainingI(const unsigned int i) {
  unsigned int start = getRandomNumber(__max(1, i / 2), i);
  unsigned int end = getRandomNumber(i, 2 * i);

  return (Interval){start, end};
}

/**
 * Generates a set of groups that add up to n.
 *
 * @param groups An array to store the generated groups (gets mutated)
 * @param n The total number of the groups should add up to.
 * @return The number of groups generated.
 */
static unsigned int getRandomGroups(unsigned int groups[], const unsigned int n) {
  unsigned int sum = 0;
  unsigned int nGroups = 0;

  for (unsigned int i = 0; i < n; i++) {
    groups[i] = getRandomNumber(1, n - sum);
    sum += groups[i];
    nGroups++;

    if (sum == n) {
      break;
    }
  }

  return nGroups;
}

/**
 * @brief Generates intervals containing the given groups.
 *
 * This function populates an array of intervals with intervals that contain the specified groups.
 *
 * @param intervals The array of intervals to be populated.
 * @param groups The array of groups to be checked.
 * @param nGroups The number of groups in the array.
 */
static void getIntervalsContainingI(Interval intervals[], const unsigned int groups[],
                                    const unsigned int nGroups) {
  unsigned int currGroup = 0;
  unsigned int currInterval = 0;
  for (unsigned int i = 0; i < nGroups; i++) {
    for (unsigned int j = 0; j < (currGroup = groups[i]); j++) {
      intervals[currInterval++] = getIntervalContainingI(currGroup);
    }
  }
}

/**
 * Prints the elements of an integer array.
 *
 * @param groups The integer array to be printed.
 * @param n The number of elements in the array.
 */
#pragma GCC diagnostic ignored "-Wunused-parameter"
static void printGroups(unsigned int groups[], unsigned int n) {
  debug_print("Groups: ");
  for (unsigned int i = 0; i < n; i++) {
    debug_print("%d ", groups[i]);
  }
  debug_print("\n");
}

/**
 * Prints the intervals in the given array.
 *
 * @param intervals The array of intervals.
 * @param n The number of intervals in the array.
 */
#pragma GCC diagnostic ignored "-Wunused-parameter"
static void printIntervals(Interval intervals[], unsigned int n) {
  debug_print("Intervals: ");
  for (unsigned int i = 0; i < n; i++) {
    debug_print("[%d, %d] ", intervals[i].bottom, intervals[i].top);
  }
  debug_print("\n");
}

/**
 * Frees the memory allocated for groups and intervals.
 *
 * @param groups The pointer to the array of groups.
 * @param intervals The pointer to the array of intervals.
 */
static void freeGroupsAndIntervals(unsigned int groups[], Interval intervals[]) {
  free(groups);
  groups = NULL;
  free(intervals);
  intervals = NULL;
}

/**
 * Generates a random simple instance of the GAI problem with a solution.
 *
 * @param n The number of intervals in the instance.
 */
IntervalSet* instanceSimpleYes(const unsigned int n) {
  // generate random numbers that add up to n
  unsigned int* groups = malloc(sizeof(unsigned int) * n);
  unsigned int nGroups = getRandomGroups(groups, n);

  printGroups(groups, nGroups);

  // generate the intervals
  Interval* intervals = malloc(sizeof(Interval) * n);

  getIntervalsContainingI(intervals, groups, nGroups);

  printIntervals(intervals, n);

  IntervalSet* instance = intervalSetCreateBlank(intervals, n);

  // free allocated memory
  freeGroupsAndIntervals(groups, intervals);

  return instance;
}

/**
 * Generates a random simple instance of the GAI problem without a solution.
 *
 * @param n The number of intervals in the instance.
 */
IntervalSet* instanceSimpleNo(const unsigned int n) {
  // make groups that add up to n-1, to be able to add the last group that makes the solution
  // impossible
  unsigned int* groups = malloc(sizeof(unsigned int) * (n - 1));
  unsigned int nGroups = getRandomGroups(groups, n - 1);

  printGroups(groups, nGroups);

  Interval* intervals = malloc(sizeof(Interval) * n);

  getIntervalsContainingI(intervals, groups, nGroups);

  // add the last group that makes the solution impossible
  for (unsigned int i = 1; i <= n; i++) {
    // check if group i can be built
    unsigned int nIncluding = 0;

    for (unsigned int j = 0; j < n - 1; j++) {
      if (intervalContains(&(intervals[j]), i)) {
        nIncluding++;
      }
    }

    if (nIncluding < i - 1) {
      intervals[n - 1] = (Interval){i, i};
      break;
    }
  }

  if (intervals[n - 1].bottom == 0) {
    printf("Could not find a group that makes the solution impossible.\n");
  }

  printIntervals(intervals, n);

  IntervalSet* instance = intervalSetCreateBlank(intervals, n);

  // free allocated memory
  freeGroupsAndIntervals(groups, intervals);

  return instance;
}