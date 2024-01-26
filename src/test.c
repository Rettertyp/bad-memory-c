#include "test.h"
#include "badMemoryAlg.h"
#define DEBUG_PRINT 1
#include "debug.h"
#include "instanceGen.h"
#include <time.h>
#include <unistd.h>

/**
 * Measures the time it takes to execute the given test.
 *
 * @param nInstances The number of instances to test.
 * @param nIntervals The number of intervals in each instance.
 * @param test The test to execute.
 * @return True if the test was succesful, false otherwise.
 */
bool measureTime(const uint32_t nInstances, const uint32_t nIntervals,
                 bool (*test)(const uint32_t, const uint32_t)) {
  clock_t start, end;

  start = clock();
  bool success = test(nInstances, nIntervals);
  end = clock();
  double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
  debug_print("Test took %.3f seconds to execute \n\n", cpu_time_used);

  return success;
}

/**
 * Determines whether the given interval should be logged.
 *
 * @param nIntervals The total number of intervals.
 * @param i The current interval index.
 * @return True if the interval should be logged, false otherwise.
 */
static bool shouldBeLogged(const uint32_t nIntervals, const uint32_t i) {
  return nIntervals > 200 || !(i % 100);
}

/**
 * Logs the given interval if it should be logged.
 *
 * @param nIntervals The total number of intervals.
 * @param i The current interval index.
 */
static void logTestStatusIfNecessary(const uint32_t nIntervals, const uint32_t i) {
  if (shouldBeLogged(nIntervals, i)) {
    debug_print("Running instance %d\n", i + 1);
  }
}

/**
 * Logs a failed yes instance for debugging purposes.
 *
 * @param instance The instance to log.
 * @param i The instance index.
 */
static void logFailedYesInstance(IntervalSet* instance, const uint32_t i) {
  debug_print("Instance %d has no solution, but should.\n", i);
  if (instance->length == 0) {
    debug_print("()");
  } else
    for (uint32_t i = 0; i < instance->length; i++) {
      debug_print("(%d, %d), ", instance->intervals[i].bottom, instance->intervals[i].top);
    }

  debug_print("\n");
}

/**
 * Logs a failed no instance for debugging purposes.
 *
 * @param instance The instance to log.
 * @param i The instance index.
 */
static void logFailedNoInstance(IntervalSet* instance, const uint32_t i) {
  debug_print("Instance %d has a solution, but shouldn't.\n", i);
  if (instance->length == 0) {
    debug_print("()");
  } else
    for (uint32_t i = 0; i < instance->length; i++) {
      debug_print("(%d, %d), ", instance->intervals[i].bottom, instance->intervals[i].top);
    }

  debug_print("\n");
}

/**
 * Gets an instance generator that generates yes instances and tests the bad memory algorithm on it.
 *
 * @param nInstances The number of instances to test.
 * @param nIntervals The number of intervals in each instance.
 * @param instanceGenerator The instance generator to use.
 * @return True if the test was succesful, false otherwise.
 */
static bool testYes(const uint32_t nInstances, const uint32_t nIntervals,
                    IntervalSet* (*instanceGenerator)(const uint32_t)) {
  instanceInitRandom();

  bool success = true;

  for (uint32_t i = 0; i < nInstances; i++) {
    logTestStatusIfNecessary(nIntervals, i);

    IntervalSet* instance = instanceGenerator(nIntervals);
    bool hasSolution = badMemoryAlgorithm(instance, nInstances == 1);

    if (!hasSolution) {
      success = false;

      logFailedYesInstance(instance, i);
      break;
    }

    intervalSetDelete(instance);
  }

  return success;
}

/**
 * Gets an instance generator that generates no instances and tests the bad memory algorithm on it.
 *
 * @param nInstances The number of instances to test.
 * @param nIntervals The number of intervals in each instance.
 * @param instanceGenerator The instance generator to use.
 * @return True if the test was succesful, false otherwise.
 */
static bool testNo(const uint32_t nInstances, const uint32_t nIntervals,
                   IntervalSet* (*instanceGenerator)(const uint32_t)) {
  bool success = true;

  for (uint32_t i = 0; i < nInstances; i++) {
    logTestStatusIfNecessary(nIntervals, i);

    IntervalSet* instance = instanceGenerator(nIntervals);
    bool hasSolution = badMemoryAlgorithm(instance, nInstances == 1);

    if (hasSolution) {
      success = false;

      logFailedNoInstance(instance, i);
      break;
    }

    intervalSetDelete(instance);
  }

  return success;
}

/**
 * Tests the bad memory algorithm on a set of simple yes instances.
 *
 * @param nInstances The number of instances to test.
 * @param nIntervals The number of intervals in each instance.
 * @return True if the test was succesful, false otherwise.
 */
bool testSimpleYes(const uint32_t nInstances, const uint32_t nIntervals) {
  debug_print("Testing simple yes instances.\n");

  return testYes(nInstances, nIntervals, instanceSimpleYes);
}

/**
 * Tests the bad memory algorithm on a set of simple no instances.
 *
 * @param nInstances The number of instances to test.
 * @param nIntervals The number of intervals in each instance.
 * @return True if the test was succesful, false otherwise.
 */
bool testSimpleNo(const uint32_t nInstances, const uint32_t nIntervals) {
  debug_print("Testing simple no instances.\n");

  return testNo(nInstances, nIntervals, instanceSimpleNo);
}

/**
 * Tests the bad memory algorithm on a set of hard whitness instances.
 *
 * @param nInstances The number of instances to test.
 * @param nIntervals The number of intervals in each instance.
 * @return True if the test was succesful, false otherwise.
 */
bool testMaxWhitnessesYes(const uint32_t nInstances, const uint32_t nIntervals) {
  debug_print("Testing max whitness yes instances.\n");

  return testYes(nInstances, nIntervals, instanceMaxWhitnessesYes);
}

/**
 * Tests the bad memory algorithm on a set of hard whitness no instances.
 *
 * @param nInstances The number of instances to test.
 * @param nIntervals The number of intervals in each instance.
 * @return True if the test was succesful, false otherwise.
 */
bool testMaxWhitnessesNo(const uint32_t nInstances, const uint32_t nIntervals) {
  debug_print("Testing max whitness no instances.\n");

  return testNo(nInstances, nIntervals, instanceMaxWhitnessesNo);
}

/**
 * Tests the bad memory algorithm on a set of whitness yes instances that try to maximize the number
 * of groups built.
 *
 * @param nInstances The number of instances to test.
 * @param nIntervals The number of intervals in each instance.
 * @return True if the test was succesful, false otherwise.
 */
bool testMaxGroupWhitnessesYes(const uint32_t nInstances, const uint32_t nIntervals) {
  debug_print("Testing max group whitness yes instances.\n");

  return testYes(nInstances, nIntervals, instanceMaxGroupWhitnessesYes);
}

/**
 * Tests the bad memory algorithm on a set of whitness no instances that try to maximize the number
 * of groups built.
 *
 * @param nInstances The number of instances to test.
 * @param nIntervals The number of intervals in each instance.
 * @return True if the test was succesful, false otherwise.
 */
bool testMaxGroupWhitnessesNo(const uint32_t nInstances, const uint32_t nIntervals) {
  debug_print("Testing max group whitness no instances.\n");

  return testNo(nInstances, nIntervals, instanceMaxGroupWhitnessesNo);
}

/**
 * Tests the bad memory algorithm on a set of hard yes amount version instances.
 *
 * @param nInstances The number of instances to test.
 * @param nIntervals The number of intervals in each instance.
 * @return True if the test was succesful, false otherwise.
 */
bool testHardYesAmountVersion(const uint32_t nInstances, const uint32_t nIntervals) {
  debug_print("Testing hard yes amount version instances.\n");

  return testYes(nInstances, nIntervals, instanceHardYesAmountVersion);
}

/**
 * Tests the bad memory algorithm on a set of hard no amount version instances.
 *
 * @param nInstances The number of instances to test.
 * @param nIntervals The number of intervals in each instance.
 * @return True if the test was succesful, false otherwise.
 */
bool testHardNoAmountVersion(const uint32_t nInstances, const uint32_t nIntervals) {
  debug_print("Testing hard no amount version instances.\n");

  return testNo(nInstances, nIntervals, instanceHardNoAmountVersion);
}

/**
 * Tests the bad memory algorithm on a set of all full instances.
 *
 * @param nInstances The number of instances to test.
 * @param nIntervals The number of intervals in each instance.
 * @return True if the test was succesful, false otherwise.
 */
bool testAllFull(const uint32_t nInstances, const uint32_t nIntervals) {
  debug_print("Testing all full instances.\n");

  return testYes(nInstances, nIntervals, instanceAllFull);
}

/**
 * Runs all tests and logs the time it took to execute each test.
 *
 * @param nInstances The number of instances to test.
 * @param nIntervals The number of intervals in each instance.
 * @return True if all tests were succesful, false otherwise.
 */
bool testRunAll(const uint32_t nInstances, const uint32_t nIntervals) {
  bool success = true;

  success &= measureTime(nInstances, nIntervals, testSimpleYes);
  success &= measureTime(nInstances, nIntervals, testSimpleNo);
  success &= measureTime(nInstances, nIntervals, testMaxWhitnessesYes);
  success &= measureTime(nInstances, nIntervals, testMaxWhitnessesNo);
  success &= measureTime(nInstances, nIntervals, testMaxGroupWhitnessesYes);
  success &= measureTime(nInstances, nIntervals, testMaxGroupWhitnessesNo);
  success &= measureTime(nInstances, nIntervals, testAllFull);

  debug_print(success ? "All tests passed.\n" : "Some tests failed.\n");

  return success;
}

/**
 * Runs 1 instance of all yes tests and logs the time it took to execute each test.
 *
 * @param nInstances The number of instances to test.
 * @param nIntervals The number of intervals in each instance.
 * @return True if all tests were succesful, false otherwise.
 */
bool testRunYes(const uint32_t nIntervals) {
  bool success = true;

  success &= measureTime(1, nIntervals, testSimpleYes);
  success &= measureTime(1, nIntervals, testMaxWhitnessesYes);
  success &= measureTime(1, nIntervals, testMaxGroupWhitnessesYes);
  success &= measureTime(1, nIntervals, testAllFull);
  // success &= measureTime(1, nIntervals, testHardYesAmountVersion);

  debug_print(success ? "All tests passed.\n" : "Some tests failed.\n");

  return success;
}