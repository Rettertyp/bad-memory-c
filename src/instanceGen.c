#include "instanceGen.h"
// #define DEBUG_PRINT 1
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
static Interval getIntervalContainingI(const uint32_t i, const uint32_t n) {
  uint32_t start = getRandomNumber(__max(1, i / 2), i);
  uint32_t end = getRandomNumber(i, __min(n, i * 2));

  return (Interval){start, end, 1};
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
                                    const uint32_t nGroups, const uint32_t n) {
  uint32_t currGroup = 0;
  uint32_t currInterval = 0;
  for (uint32_t i = 0; i < nGroups; i++) {
    currGroup = groups[i];

    for (uint32_t j = 0; j < currGroup; j++) {
      intervals[currInterval++] = getIntervalContainingI(currGroup, n);
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
static void printIntArray(uint32_t groups[], uint32_t n) {
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
    debug_print("[%d, %d, %d] ", intervals[i].bottom, intervals[i].top, intervals[i].amount);
  }
  debug_print("\n");
}

/**
 * Creates an IntervalSet from an array of Intervals and frees the memory of the array.
 *
 * @param intervals The array of Intervals.
 * @param n The number of Intervals in the array.
 * @return The created IntervalSet.
 */
static InstanceInfo createIntervalSetAndFree(Interval* intervals, const uint32_t n,
                                             uint32_t* metadata, const uint32_t metadataLength) {
  IntervalSet* instance = intervalSetCreateBlank(intervals, n);

  free(intervals);

  return (InstanceInfo){instance, metadataLength, metadata};
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
      intervals[n - 1] = (Interval){i, i, 1};
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
 * @return The generated instance.
 */
InstanceInfo instanceSimpleYes(const uint32_t n) {
  uint32_t* groups = malloc(sizeof(uint32_t) * n);

  // generate random numbers that add up to n
  uint32_t nGroups = getRandomGroups(groups, n);

  printIntArray(groups, nGroups);

  // generate the intervals
  Interval* intervals = malloc(sizeof(Interval) * n);

  getIntervalsContainingI(intervals, groups, nGroups, n);

  printIntervals(intervals, n);

  return createIntervalSetAndFree(intervals, n, groups, nGroups);
}

/**
 * Generates a random simple instance of the GAI problem without a solution.
 *
 * @param n The number of intervals in the instance.
 */
InstanceInfo instanceSimpleNo(const uint32_t n) {
  uint32_t* groups = malloc(sizeof(uint32_t) * (n - 1));

  // make groups that add up to n-1, to be able to add the last group that makes the solution
  // impossible
  uint32_t nGroups = getRandomGroups(groups, n - 1);

  printIntArray(groups, nGroups);

  Interval* intervals = malloc(sizeof(Interval) * n);

  getIntervalsContainingI(intervals, groups, nGroups, n - 1);

  // add the last group that makes the solution impossible
  addImpossibleGroup(intervals, n);

  printIntervals(intervals, n);

  return createIntervalSetAndFree(intervals, n, groups, nGroups);
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

  return 3 * (end - 1);
}

/**
 * Calculates the optimal end value to build whitnesses based on the given number of intervals.
 *
 * @param n The number of intervals.
 * @return The ending value of the whitness.
 */
static uint32_t calcEndValues(uint32_t endValues[], const uint32_t n,
                              const uint32_t whitnessWidth) {
  uint32_t currEnd = 3;
  uint32_t sum = 0;
  uint32_t nextWhitnessSize = calcWhitnessSize(3, currEnd + whitnessWidth);
  uint32_t i = 0;

  // add whitnesses until no whitness fits in the remaining space
  while (sum + nextWhitnessSize < n) {
    currEnd += whitnessWidth;
    sum += nextWhitnessSize;
    endValues[i++] = currEnd;

    nextWhitnessSize = calcWhitnessSize(3, currEnd + whitnessWidth);
  }

  // try moving the whitnesses to the left by one up to three times
  bool spaceLeft = i > 0;
  while (spaceLeft) {
    // check if the whitnesses could be moved to the left by one
    for (int32_t j = i - 1; j >= 0; j--) {
      // when moving a whitness to the left, the sum of intervals increases by 3
      uint32_t newSum = sum + 3;
      if (newSum < n) {
        sum = newSum;
        endValues[j]++;
      } else {
        spaceLeft = false;
        break;
      }
    }
  }

  printIntArray(endValues, i);

  return i;
}

/**
 * Adds a whitness to the given array of intervals.
 *
 * @param intervals The array of intervals to be populated.
 * @param start The starting value of the whitness.
 * @param end The ending value of the whitness.
 * @param i The index of the array on which to start adding the whitness.
 * @return The index of the array after adding the whitness.
 */
static uint32_t getWhitness(Interval intervals[], const uint32_t start, const uint32_t end,
                            uint32_t i) {
  const uint32_t nIntervalsPerGroup = end - 1;

  // add the top interval
  intervals[i++] = (Interval){start, end, 1};

  // add the middle group of intervals
  for (uint32_t j = 0; j < nIntervalsPerGroup; j++) {
    intervals[i++] = (Interval){start + 1, end - 1, 1};
  }

  // add the bottom group of interval
  for (uint32_t j = 0; j < nIntervalsPerGroup - 1; j++) {
    intervals[i++] = (Interval){start + 2, end - 2, 1};
  }

  // add the key group that turns the combination into a whitness
  for (uint32_t j = 0; j < nIntervalsPerGroup; j++) {
    intervals[i++] = (Interval){end - 1, end, 1};
  }

  return i;
}

/**
 * Fills the remaining space in the given array of intervals with "[1, 1]" intervals.
 *
 * @param intervals The array of intervals to be populated.
 * @param i The index of the array on which to start adding the intervals.
 * @param n The length of the array.
 */
static uint32_t fillRemainingSpace(Interval intervals[], uint32_t i, const uint32_t n) {
  while (i < n) {
    intervals[i++] = (Interval){1, 1, 1};
  }

  return i;
}

/**
 * Generates as many whitnesses as possible for the given number of intervals.
 *
 * @param intervals The array of intervals to be populated.
 * @param n The length of the array.
 */
static uint32_t getMaxNumWhitnesses(Interval intervals[], const uint32_t n) {
  uint32_t whitnessWidth = 4;
  uint32_t i = 0;
  uint32_t start = __min(3, n);
  uint32_t* endValues = malloc(sizeof(uint32_t) * (n / 4));
  uint32_t endLenght = calcEndValues(endValues, n, whitnessWidth);

  uint32_t nWhitnesses = 0;

  // add all whitnesses that were computed in calcEndValues
  for (uint32_t endI = 0; endI < endLenght; endI++) {
    i = getWhitness(intervals, start, endValues[endI], i);
    nWhitnesses++;
    start += whitnessWidth / 2;
  }

  // fill the remaining space with "[1, 1]" intervals
  i = fillRemainingSpace(intervals, i, n);

  debug_print("Number of whitnesses: %d\n", nWhitnesses);

  return nWhitnesses;
}

/**
 * Generates a hard yes instance of the GAI problem, consisting of as many whitnesses as possible.
 *
 * @param n The number of intervals in the instance.
 * @return The generated instance.
 */
InstanceInfo instanceMaxWhitnessesYes(const uint32_t n) {
  Interval* intervals = malloc(sizeof(Interval) * n);

  uint32_t nWhitnesses = getMaxNumWhitnesses(intervals, n);

  printIntervals(intervals, n);

  uint32_t* metadata = malloc(sizeof(uint32_t));
  *metadata = nWhitnesses;

  return createIntervalSetAndFree(intervals, n, metadata, 1);
}

/**
 * Generates a hard no instance of the GAI problem, consisting of as many whitnesses as possible.
 *
 * @param n The number of intervals in the instance.
 * @return The generated instance.
 */
InstanceInfo instanceMaxWhitnessesNo(const uint32_t n) {
  Interval* intervals = malloc(sizeof(Interval) * n);

  uint32_t nWhitnesses = getMaxNumWhitnesses(intervals, n - 1);

  // add the last group that makes the solution impossible
  addImpossibleGroup(intervals, n);

  printIntervals(intervals, n);

  uint32_t* metadata = malloc(sizeof(uint32_t));
  *metadata = nWhitnesses;

  return createIntervalSetAndFree(intervals, n, metadata, 1);
}

/**
 * Generates whitnesses that maximize the number of groups the algorithm will build.
 *
 * @param intervals The array of intervals to be populated.
 * @param n The length of the array.
 */
static uint32_t getMaxGroupWhitnesses(Interval intervals[], const uint32_t n) {
  int32_t whitnessWidth = 6;
  uint32_t i = 0;
  int32_t start = __min(3, n);
  int32_t end = 2 * sqrt(n);
  uint32_t nextWhitnessSize = 0;
  uint32_t nWhitnesses = 0;
  // add whitnesses until no whitness fits in the remaining space
  while (i < n) {
    nextWhitnessSize = calcWhitnessSize(start, end);
    // if the next whitness does not fit in the remaining space , shrink the end value
    while (i + nextWhitnessSize >= n) {
      end--;
      nextWhitnessSize = calcWhitnessSize(start, end);
    }
    // if no whitnesses can be added anymore , add "[1 , 1]" intervals to fill the remaining space
    if (end - start < whitnessWidth) {
      i = fillRemainingSpace(intervals, i, n);
    }
    // if there is space for a whitness , add it
    else if (i + nextWhitnessSize < n) {
      nWhitnesses++;
      i = getWhitness(intervals, start, end, i);
      start += whitnessWidth / 2;
      end -= whitnessWidth / 2;
    }
  }

  debug_print("Number of whitnesses: %d\n", nWhitnesses);

  return nWhitnesses;
}

/**
 * Generate a hard yes instance of the GAI problem, trying to maximize the number of groups the
 * algorithm will build.
 *
 * @param n The number of intervals in the instance.
 * @return The generated instance.
 */
InstanceInfo instanceMaxGroupWhitnessesYes(const uint32_t n) {
  Interval* intervals = malloc(sizeof(Interval) * n);

  uint32_t nWhitnesses = getMaxGroupWhitnesses(intervals, n);

  printIntervals(intervals, n);

  uint32_t* metadata = malloc(sizeof(uint32_t));
  *metadata = nWhitnesses;

  return createIntervalSetAndFree(intervals, n, metadata, 1);
}

/**
 * Generate a hard no instance of the GAI problem, trying to maximize the number of groups the
 * algorithm will build.
 *
 * @param n The number of intervals in the instance.
 * @return The generated instance.
 */
InstanceInfo instanceMaxGroupWhitnessesNo(const uint32_t n) {
  Interval* intervals = malloc(sizeof(Interval) * n);

  uint32_t nWhitnesses = getMaxGroupWhitnesses(intervals, n - 1);

  // add the last group that makes the solution impossible
  addImpossibleGroup(intervals, n);

  printIntervals(intervals, n);

  uint32_t* metadata = malloc(sizeof(uint32_t));
  *metadata = nWhitnesses;

  return createIntervalSetAndFree(intervals, n, metadata, 1);
}

/**
 * Adds a especially hard whitness to the given array of intervals, that makes use of the "amount"
 * key of Intervals to make the hardest possible instance
 *
 * @param intervals The array of intervals to be populated.
 * @param start The starting value of the whitness.
 * @param end The ending value of the whitness.
 * @param i The index of the array on which to start adding the whitness.
 */
static uint32_t getWhitnessAmountVersion(Interval intervals[], const uint32_t start,
                                         const uint32_t end, uint32_t i) {
  const uint32_t nIntervalsPerGroup = end - 1;

  // add the top interval
  intervals[i++] = (Interval){start, end, 1};

  // add the middle group of intervals
  intervals[i++] = (Interval){start + 1, end - 1, nIntervalsPerGroup};

  // add the bottom group of interval
  intervals[i++] = (Interval){start + 2, end - 2, nIntervalsPerGroup - 1};

  // add the key group that turns the combination into a whitness
  intervals[i++] = (Interval){end - 1, end, nIntervalsPerGroup};

  return i;
}

/**
 * Generates as many amount-whitnesses as possible for the given number of interval-objects.
 *
 * @param intervals The array of intervals to be populated.
 * @param n The length of the array.
 */
static uint32_t getWhitnessesAmountVersion(Interval intervals[], const uint32_t n) {
  uint32_t i = 0;
  uint32_t start = __min(3, n);
  uint32_t end = n;
  uint32_t whitnessSize = 4;
  // only used to print the number of whitnesses
  uint32_t nWhitnesses = 0;

  // add whitnesses until the next whitness does not fit in the remaining space
  while (i < n) {
    // if the next whitness does not fit in the remaining space, add "[1, 1]" intervals to fill the
    // remaining space
    if (i + whitnessSize >= n || start + whitnessSize >= end) {
      while (i < n) {
        intervals[i++] = (Interval){1, 1, 1};
      }
    }

    // if there is space for a whitness, add it
    else {
      nWhitnesses++;
      i = getWhitnessAmountVersion(intervals, start, end, i);
      start += 2;
      end -= 2;
    }
  }

  debug_print("Number of whitnesses: %d\n", nWhitnesses);

  return nWhitnesses;
}

/**
 * Generates a hard yes instance of the GAI problem, consisting of as many whitnesses as possible,
 * using the amount key.
 *
 * @param n The number of interval-objects in the instance.
 * @return The generated instance.
 */
InstanceInfo instanceHardYesAmountVersion(const uint32_t n) {
  Interval* intervals = malloc(sizeof(Interval) * n);

  uint32_t nWhitnesses = getWhitnessesAmountVersion(intervals, n);

  printIntervals(intervals, n);

  uint32_t* metadata = malloc(sizeof(uint32_t));
  *metadata = nWhitnesses;

  return createIntervalSetAndFree(intervals, n, metadata, 1);
}

/**
 * Generates a hard no instance of the GAI problem, consisting of as many whitnesses as possible,
 * using the amount key.
 *
 * @param n The number of interval-objects in the instance.
 * @return The generated instance.
 */
InstanceInfo instanceHardNoAmountVersion(const uint32_t n) {
  Interval* intervals = malloc(sizeof(Interval) * n);

  uint32_t nWhitnesses = getWhitnessesAmountVersion(intervals, n - 1);

  // add the last group that makes the solution impossible
  addImpossibleGroup(intervals, n);

  printIntervals(intervals, n);

  uint32_t* metadata = malloc(sizeof(uint32_t));
  *metadata = nWhitnesses;

  return createIntervalSetAndFree(intervals, n, metadata, 1);
}

/**
 * Generates an instance of the GAI problem, consisting only of intervals [1, n].
 *
 * @param n The number of intervals in the instance.
 * @return The generated instance.
 */
InstanceInfo instanceAllFull(const uint32_t n) {
  Interval* intervals = malloc(sizeof(Interval) * n);

  for (uint32_t i = 0; i < n; i++) {
    intervals[i] = (Interval){1, n, 1};
  }

  return createIntervalSetAndFree(intervals, n, NULL, 0);
}