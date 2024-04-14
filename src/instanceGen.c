#include "instanceGen.h"
// #define DEBUG_PRINT 1
#include "debug.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
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
 * Returns a random Interval containing the given integer.
 * The bottom of the Interval is a random number between 1 and i/2, and the top is a random number
 * between i and 2i (or n, if 2i > n).
 *
 * @param i The integer to be contained in the Interval.
 * @param n The maximum value of the Interval.
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
 * This function populates an array of intervals with intervals that contain the specified groups.
 *
 * @param intervals The array of intervals to be populated.
 * @param groups The array of groups to be checked.
 * @param nGroups The number of groups in the array.
 * @param n The maximum value of the intervals.
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
 * Creates an IntervalSet from an array of Intervals and frees the memory of the array. Then packs
 * it into an InstanceInfo.
 *
 * @param intervals The array of Intervals.
 * @param n The number of Intervals in the array.
 * @param name The name type of the instance.
 * @param metadata The metadata of the instance.
 * @param metadataLength The length of the metadata.
 * @return The created IntervalSet.
 */
static InstanceInfo createIntervalSetAndFree(Interval* intervals, const uint32_t n, char* name,
                                             uint32_t* metadata, const uint32_t metadataLength) {
  IntervalSet* instance = intervalSetCreateBlank(intervals, n);

  free(intervals);

  InstanceInfo instanceInfo = {
      .intervalSet = instance, .metadataLength = metadataLength, .metadata = metadata};

  strncpy(instanceInfo.name, name, sizeof(instanceInfo.name) - 1);
  instanceInfo.name[sizeof(instanceInfo.name) - 1] = '\0';

  return instanceInfo;
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

  return createIntervalSetAndFree(intervals, n, "SimpleYes", groups, nGroups);
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

  return createIntervalSetAndFree(intervals, n, "SimpleNo", groups, nGroups);
}

/**
 * Calculates the size of the witness based on the given start and end values.
 *
 * @param start The starting value.
 * @param end The ending value.
 * @return The size of the witness.
 */
static uint32_t calcWitnessSize(const uint32_t start, const uint32_t end) {
  if (start >= end) {
    return 0;
  }

  return 3 * (end - 1);
}

/**
 * Calculates the optimal end values to build witnesses based on the given number of intervals.
 *
 * @param endValues The array to store the ending values of the witnesses.
 * @param n The number of intervals.
 * @param witnessWidth The width of the witnesses.
 * @return The number of witnesses that were computed.
 */
static uint32_t calcEndValues(uint32_t endValues[], const uint32_t n, const uint32_t witnessWidth) {
  uint32_t currEnd = 3;
  uint32_t sum = 0;
  uint32_t nextWitnessSize = calcWitnessSize(3, currEnd + witnessWidth);
  uint32_t i = 0;

  // add witnesses until no witness fits in the remaining space
  while (sum + nextWitnessSize < n) {
    currEnd += witnessWidth;
    sum += nextWitnessSize;
    endValues[i++] = currEnd;

    nextWitnessSize = calcWitnessSize(3, currEnd + witnessWidth);
  }

  // try moving the witnesses to the left by one up to three times
  bool spaceLeft = i > 0;
  while (spaceLeft) {
    // check if the witnesses could be moved to the left by one
    for (int32_t j = i - 1; j >= 0; j--) {
      // when moving a witness to the left, the sum of intervals increases by 3
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
 * Adds a witness to the given array of intervals.
 *
 * @param intervals The array of intervals to be populated.
 * @param start The starting value of the witness.
 * @param end The ending value of the witness.
 * @param i The index of the array on which to start adding the witness.
 * @return The index of the array after adding the witness.
 */
static uint32_t getWitness(Interval intervals[], const uint32_t start, const uint32_t end,
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

  // add the key group that turns the combination into a witness
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
 * @returns The index of the array after adding the intervals.
 */
static uint32_t fillRemainingSpace(Interval intervals[], uint32_t i, const uint32_t n) {
  while (i < n) {
    intervals[i++] = (Interval){1, 1, 1};
  }

  return i;
}

/**
 * Generates as many witnesses as possible for the given number of intervals.
 *
 * @param intervals The array of intervals to be populated.
 * @param n The length of the array.
 * @returns The number of witnesses that were computed.
 */
static uint32_t getMaxNumWitnesses(Interval intervals[], const uint32_t n) {
  uint32_t witnessWidth = 4;
  uint32_t i = 0;
  uint32_t start = __min(3, n);
  uint32_t* endValues = malloc(sizeof(uint32_t) * (n / 4));
  uint32_t endLenght = calcEndValues(endValues, n, witnessWidth);

  uint32_t nWitnesses = 0;

  // add all witnesses that were computed in calcEndValues
  for (uint32_t endI = 0; endI < endLenght; endI++) {
    i = getWitness(intervals, start, endValues[endI], i);
    nWitnesses++;
    start += witnessWidth / 2;
  }

  // fill the remaining space with "[1, 1]" intervals
  fillRemainingSpace(intervals, i, n);

  debug_print("Number of witnesses: %d\n", nWitnesses);

  return nWitnesses;
}

/**
 * Generates a hard yes instance of the GAI problem, consisting of as many witnesses as possible.
 *
 * @param n The number of intervals in the instance.
 * @return The generated instance.
 */
InstanceInfo instanceMaxWitnessesYes(const uint32_t n) {
  Interval* intervals = malloc(sizeof(Interval) * n);

  uint32_t nWitnesses = getMaxNumWitnesses(intervals, n);

  printIntervals(intervals, n);

  uint32_t* metadata = malloc(sizeof(uint32_t));
  *metadata = nWitnesses;

  return createIntervalSetAndFree(intervals, n, "MaxWitnessesYes", metadata, 1);
}

/**
 * Generates a hard no instance of the GAI problem, consisting of as many witnesses as possible.
 *
 * @param n The number of intervals in the instance.
 * @return The generated instance.
 */
InstanceInfo instanceMaxWitnessesNo(const uint32_t n) {
  Interval* intervals = malloc(sizeof(Interval) * n);

  uint32_t nWitnesses = getMaxNumWitnesses(intervals, n - 1);

  // add the last group that makes the solution impossible
  addImpossibleGroup(intervals, n);

  printIntervals(intervals, n);

  uint32_t* metadata = malloc(sizeof(uint32_t));
  *metadata = nWitnesses;

  return createIntervalSetAndFree(intervals, n, "MaxWitnessesNo", metadata, 1);
}

/**
 * Generates witnesses that maximize the number of groups the algorithm will build.
 *
 * @param intervals The array of intervals to be populated.
 * @param n The length of the array.
 * @returns The number of witnesses that were computed.
 */
static uint32_t getMaxGroupWitnesses(Interval intervals[], const uint32_t n) {
  int32_t witnessWidth = 6;
  uint32_t i = 0;
  int32_t start = __min(3, n);
  int32_t end = 2 * sqrt(n);
  uint32_t nextWitnessSize = 0;
  uint32_t nWitnesses = 0;
  // add witnesses until no witness fits in the remaining space
  while (i < n) {
    nextWitnessSize = calcWitnessSize(start, end);
    // if the next witness does not fit in the remaining space, shrink the end value
    while (i + nextWitnessSize >= n) {
      end--;
      nextWitnessSize = calcWitnessSize(start, end);
    }
    // if no witnesses can be added anymore , add "[1 , 1]" intervals to fill the remaining space
    if (end - start < witnessWidth) {
      i = fillRemainingSpace(intervals, i, n);
    }
    // if there is space for a witness , add it
    else if (i + nextWitnessSize < n) {
      nWitnesses++;
      i = getWitness(intervals, start, end, i);
      start += witnessWidth / 2;
      end -= witnessWidth / 2;
    }
  }

  debug_print("Number of witnesses: %d\n", nWitnesses);

  return nWitnesses;
}

/**
 * Generate a hard yes instance of the GAI problem, trying to maximize the number of groups the
 * algorithm will build.
 *
 * @param n The number of intervals in the instance.
 * @return The generated instance.
 */
InstanceInfo instanceMaxGroupWitnessesYes(const uint32_t n) {
  Interval* intervals = malloc(sizeof(Interval) * n);

  uint32_t nWitnesses = getMaxGroupWitnesses(intervals, n);

  printIntervals(intervals, n);

  uint32_t* metadata = malloc(sizeof(uint32_t));
  *metadata = nWitnesses;

  return createIntervalSetAndFree(intervals, n, "MaxGroupWitnessesYes", metadata, 1);
}

/**
 * Generate a hard no instance of the GAI problem, trying to maximize the number of groups the
 * algorithm will build.
 *
 * @param n The number of intervals in the instance.
 * @return The generated instance.
 */
InstanceInfo instanceMaxGroupWitnessesNo(const uint32_t n) {
  Interval* intervals = malloc(sizeof(Interval) * n);

  uint32_t nWitnesses = getMaxGroupWitnesses(intervals, n - 1);

  // add the last group that makes the solution impossible
  addImpossibleGroup(intervals, n);

  printIntervals(intervals, n);

  uint32_t* metadata = malloc(sizeof(uint32_t));
  *metadata = nWitnesses;

  return createIntervalSetAndFree(intervals, n, "MaxGroupWitnessesNo", metadata, 1);
}

/**
 * Adds a especially hard witness to the given array of intervals, that makes use of the "amount"
 * key of Intervals to make the hardest possible instance
 *
 * @param intervals The array of intervals to be populated.
 * @param start The starting value of the witness.
 * @param end The ending value of the witness.
 * @param i The index of the array on which to start adding the witness.
 * @returns The index of the array after adding the witness.
 */
static uint32_t getWitnessAmountVersion(Interval intervals[], const uint32_t start,
                                        const uint32_t end, uint32_t i) {
  const uint32_t nIntervalsPerGroup = end - 1;

  // add the top interval
  intervals[i++] = (Interval){start, end, 1};

  // add the middle group of intervals
  intervals[i++] = (Interval){start + 1, end - 1, nIntervalsPerGroup};

  // add the bottom group of interval
  intervals[i++] = (Interval){start + 2, end - 2, nIntervalsPerGroup - 1};

  // add the key group that turns the combination into a witness
  intervals[i++] = (Interval){end - 1, end, nIntervalsPerGroup};

  return i;
}

/**
 * Generates as many amount-witnesses as possible for the given number of interval-objects.
 *
 * @param intervals The array of intervals to be populated.
 * @param n The length of the array.
 * @returns The number of witnesses that were computed.
 */
static uint32_t getWitnessesAmountVersion(Interval intervals[], const uint32_t n) {
  uint32_t i = 0;
  uint32_t start = __min(3, n);
  uint32_t end = n;
  uint32_t witnessSize = 4;
  // only used to print the number of witnesses
  uint32_t nWitnesses = 0;

  // add witnesses until the next witness does not fit in the remaining space
  while (i < n) {
    // if the next witness does not fit in the remaining space, add "[1, 1]" intervals to fill the
    // remaining space
    if (i + witnessSize >= n || start + witnessSize > end) {
      i = fillRemainingSpace(intervals, i, n);
    }

    // if there is space for a witness, add it
    else {
      nWitnesses++;
      i = getWitnessAmountVersion(intervals, start, end, i);
      start += 2;
      end -= 2;
    }
  }

  debug_print("Number of witnesses: %d\n", nWitnesses);

  return nWitnesses;
}

/**
 * Generates a hard yes instance of the GAI problem, consisting of as many witnesses as possible,
 * using the amount key.
 *
 * @param n The number of interval-objects in the instance.
 * @return The generated instance.
 */
InstanceInfo instanceHardYesAmountVersion(const uint32_t n) {
  Interval* intervals = malloc(sizeof(Interval) * n);

  uint32_t nWitnesses = getWitnessesAmountVersion(intervals, n);

  printIntervals(intervals, n);

  uint32_t* metadata = malloc(sizeof(uint32_t));
  *metadata = nWitnesses;

  return createIntervalSetAndFree(intervals, n, "HardYesAmountVersion", metadata, 1);
}

/**
 * Generates a hard no instance of the GAI problem, consisting of as many witnesses as possible,
 * using the amount key.
 *
 * @param n The number of interval-objects in the instance.
 * @return The generated instance.
 */
InstanceInfo instanceHardNoAmountVersion(const uint32_t n) {
  Interval* intervals = malloc(sizeof(Interval) * n);

  uint32_t nWitnesses = getWitnessesAmountVersion(intervals, n - 1);

  // add the last group that makes the solution impossible
  addImpossibleGroup(intervals, n);

  printIntervals(intervals, n);

  uint32_t* metadata = malloc(sizeof(uint32_t));
  *metadata = nWitnesses;

  return createIntervalSetAndFree(intervals, n, "HardNoAmountVersion", metadata, 1);
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

  return createIntervalSetAndFree(intervals, n, "AllFull", NULL, 0);
}