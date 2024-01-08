#include "instanceGen.h"
#include "debug.h"
#include <math.h>
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
static uint32_t getRandomNumber(const uint32_t min, const uint32_t max) {
  return (rand() % (max - min + 1)) + min;
}

/**
 * Returns an Interval containing the given integer.
 *
 * @param i The integer to be contained in the Interval.
 * @return The Interval containing the given integer.
 */
static Interval getIntervalContainingI(const uint32_t i) {
  uint32_t start = getRandomNumber(__max(1, i / 2), i);
  uint32_t end = getRandomNumber(i, 2 * i);

  return (Interval){start, end};
}

/**
 * Generates a set of groups that add up to n.
 *
 * @param groups An array to store the generated groups (gets mutated)
 * @param n The total number of the groups should add up to.
 * @return The number of groups generated.
 */
static uint32_t getRandomGroups(uint32_t groups[], const uint32_t n) {
  uint32_t sum = 0;
  uint32_t nGroups = 0;

  for (uint32_t i = 0; i < n; i++) {
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
static void getIntervalsContainingI(Interval intervals[], const uint32_t groups[],
                                    const uint32_t nGroups) {
  uint32_t currGroup = 0;
  uint32_t currInterval = 0;
  for (uint32_t i = 0; i < nGroups; i++) {
    currGroup = groups[i];

    for (uint32_t j = 0; j < currGroup; j++) {
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
static void printGroups(uint32_t groups[], uint32_t n) {
  debug_print("Groups: ");
  for (uint32_t i = 0; i < n; i++) {
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
static void printIntervals(Interval intervals[], uint32_t n) {
  debug_print("Intervals: ");
  for (uint32_t i = 0; i < n; i++) {
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
static void freeGroupsAndIntervals(uint32_t groups[], Interval intervals[]) {
  free(groups);
  groups = NULL;
  free(intervals);
  intervals = NULL;
}

/**
 * Creates an IntervalSet from an array of Intervals and frees the memory of the array.
 *
 * @param intervals The array of Intervals.
 * @param n The number of Intervals in the array.
 * @return The created IntervalSet.
 */
static IntervalSet* createIntervalSetAndFree(Interval* intervals, const uint32_t n) {
  IntervalSet* instance = intervalSetCreateBlank(intervals, n);

  free(intervals);

  return instance;
}

/**
 * Creates an interval set and frees the groups and intervals.
 *
 * @param intervals The array of intervals.
 * @param n The number of intervals.
 * @param groups The array of groups.
 * @return The created interval set.
 */
static IntervalSet* createIntervalSetAndFreeGroups(Interval intervals[], uint32_t groups[],
                                                   const uint32_t n) {
  IntervalSet* instance = intervalSetCreateBlank(intervals, n);

  freeGroupsAndIntervals(groups, intervals);

  return instance;
}

/**
 * Adds a group that makes the solution impossible to the given array of n-1 intervals.
 *
 * @param intervals The array of intervals to be populated.
 * @param n The number of intervals in the array.
 */
static void addImpossibleGroup(Interval intervals[], const uint32_t n) {
  for (uint32_t i = 1; i <= n; i++) {
    // check if group i can be built
    uint32_t nIncluding = 0;

    for (uint32_t j = 0; j < n - 1; j++) {
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
}

/**
 * Generates a random simple instance of the GAI problem with a solution.
 *
 * @param n The number of intervals in the instance.
 */
IntervalSet* instanceSimpleYes(const uint32_t n) {
  uint32_t* groups = malloc(sizeof(uint32_t) * n);

  // generate random numbers that add up to n
  uint32_t nGroups = getRandomGroups(groups, n);

  printGroups(groups, nGroups);

  // generate the intervals
  Interval* intervals = malloc(sizeof(Interval) * n);

  getIntervalsContainingI(intervals, groups, nGroups);

  printIntervals(intervals, n);

  return createIntervalSetAndFreeGroups(intervals, groups, n);
}

/**
 * Generates a random simple instance of the GAI problem without a solution.
 *
 * @param n The number of intervals in the instance.
 */
IntervalSet* instanceSimpleNo(const uint32_t n) {
  uint32_t* groups = malloc(sizeof(uint32_t) * (n - 1));

  // make groups that add up to n-1, to be able to add the last group that makes the solution
  // impossible
  uint32_t nGroups = getRandomGroups(groups, n - 1);

  printGroups(groups, nGroups);

  Interval* intervals = malloc(sizeof(Interval) * n);

  getIntervalsContainingI(intervals, groups, nGroups);

  // add the last group that makes the solution impossible
  addImpossibleGroup(intervals, n);

  printIntervals(intervals, n);

  return createIntervalSetAndFreeGroups(intervals, groups, n);
}

/**
 * Calculates the size of the whiteness based on the given start and end values.
 *
 * @param start The starting value.
 * @param end The ending value.
 * @return The size of the whiteness.
 */
static uint32_t calcWhitnessSize(const uint32_t start, const uint32_t end) {
  if (start >= end) {
    return 0;
  }

  return 3 * end - 3;
}

/**
 * Adds a whitness to the given array of intervals.
 *
 * @param intervals The array of intervals to be populated.
 * @param start The starting value of the whitness.
 * @param end The ending value of the whitness.
 * @param i The index of the array on which to start adding the whitness.
 */
static uint32_t getWhitness(Interval intervals[], const uint32_t start, const uint32_t end,
                            uint32_t i) {
  const uint32_t nIntervalsPerGroup = end - 1;

  // add the top interval
  intervals[i++] = (Interval){start, end};

  // add the middle group of intervals
  for (uint32_t j = 0; j < nIntervalsPerGroup; j++) {
    intervals[i++] = (Interval){start + 1, end - 1};
  }

  // add the bottom group of interval
  for (uint32_t j = 0; j < nIntervalsPerGroup - 1; j++) {
    intervals[i++] = (Interval){start + 2, end - 2};
  }

  // add the key group that turns the combination into a whitness
  for (uint32_t j = 0; j < nIntervalsPerGroup; j++) {
    intervals[i++] = (Interval){end - 1, end};
  }

  return i;
}

/**
 * Generates as many whitnesses as possible for the given number of intervals.
 *
 * @param intervals The array of intervals to be populated.
 * @param n The length of the array.
 */
static void getWhitnesses(Interval intervals[], const uint32_t n) {
  uint32_t i = 0;
  uint32_t start = __min(3, n);
  uint32_t end = 2 * sqrt(n);
  uint32_t nextWhitnessSize = 0;
  uint32_t nWhitnesses = 0;

  // add whitnesses until no whitness fits in the remaining space
  while (i < n) {
    nextWhitnessSize = calcWhitnessSize(start, end);

    // if the next whitness does not fit in the remaining space, shrink the end value
    while (i + nextWhitnessSize >= n) {
      debug_print("Next whitness size: %d, start: %d, end: %d\n", nextWhitnessSize, start, end);
      end--;
      nextWhitnessSize = calcWhitnessSize(start, end);
    }

    // if no whitnesses can be added anymore, add "[1, 1]" intervals to fill the remaining space
    if (nextWhitnessSize == 0) {
      while (i < n) {
        intervals[i++] = (Interval){1, 1};
      }
    }

    // if there is space for a whitness, add it
    else if (i + nextWhitnessSize < n) {
      nWhitnesses++;
      i = getWhitness(intervals, start, end, i);
      start += 3;
      end -= 3;
    }
  }

  debug_print("Number of whitnesses: %d\n", nWhitnesses);
}

/**
 * Generates a hard yes instance of the GAI problem, consisting of as many whitnesses as possible.
 *
 * @param n The number of intervals in the instance.
 * @return The generated instance.
 */
IntervalSet* instanceHardYes(const uint32_t n) {
  Interval* intervals = malloc(sizeof(Interval) * n);

  getWhitnesses(intervals, n);

  printIntervals(intervals, n);

  return createIntervalSetAndFree(intervals, n);
}

/**
 * Generates a hard no instance of the GAI problem, consisting of as many whitnesses as possible.
 *
 * @param n The number of intervals in the instance.
 * @return The generated instance.
 */
IntervalSet* instanceHardNo(const uint32_t n) {
  Interval* intervals = malloc(sizeof(Interval) * n);

  getWhitnesses(intervals, n - 1);

  // add the last group that makes the solution impossible
  addImpossibleGroup(intervals, n);

  printIntervals(intervals, n);

  return createIntervalSetAndFree(intervals, n);
}