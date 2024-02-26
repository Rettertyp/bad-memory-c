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
 * @param nIntervals The number of intervals in each instance.
 * @param test The test to execute.
 * @return True if the test was succesful, false otherwise.
 */
void measureTime(const uint32_t nIntervals, RunInfo (*test)(const uint32_t)) {
  clock_t start, end;

  start = clock();
  RunInfo runInfo = test(nIntervals);
  end = clock();
  double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
  debug_print("Test took %.3f seconds to execute \n\n", cpu_time_used);

  // save the results to a json file
  jsonPrinterSaveToFile(runInfo, cpu_time_used);

  runInfoDelete(&(runInfo));
}

/**
 * Logs a failed yes instance for debugging purposes.
 *
 * @param instance The instance to log.
 * @param i The instance index.
 */
static void logFailedYesInstance(IntervalSet* instance) {
  debug_print("Instance has no solution, but should.\n");
  if (instance->length == 0) {
    debug_print("[]");
  } else
    for (uint32_t i = 0; i < instance->length; i++) {
      debug_print("[%d, %d, %d], ", instance->intervals[i].bottom, instance->intervals[i].top,
                  instance->intervals[i].amount);
    }

  debug_print("\n");
}

/**
 * Logs a failed no instance for debugging purposes.
 *
 * @param instance The instance to log.
 * @param i The instance index.
 */
static void logFailedNoInstance(IntervalSet* instance) {
  debug_print("Instance has a solution, but shouldn't.\n");
  if (instance->length == 0) {
    debug_print("[]");
  } else
    for (uint32_t i = 0; i < instance->length; i++) {
      debug_print("[%d, %d, %d], ", instance->intervals[i].bottom, instance->intervals[i].top,
                  instance->intervals[i].amount);
    }

  debug_print("\n");
}

/**
 * Gets an instance generator that generates yes instances and tests the bad memory algorithm on it.
 *
 * @param nIntervals The number of intervals in each instance.
 * @param instanceGenerator The instance generator to use.
 * @param solverAlgorithm The solver algorithm to use, either badMemoryAlgorithm or
 * badMemoryDepthFirst.
 * @return True if the test was succesful, false otherwise.
 */
static RunInfo testYes(const uint32_t nIntervals, InstanceInfo (*instanceGenerator)(const uint32_t),
                       RunInfo (*solverAlgorithm)(IntervalSet*, char*), char* description) {
  instanceInitRandom();

  InstanceInfo instance = instanceGenerator(nIntervals);
  RunInfo runInfo = solverAlgorithm(instance.intervalSet, description);

  runInfo.metadataLength = instance.metadataLength;
  runInfo.metadata = instance.metadata;

  if (!runInfo.solutionFound) {
    logFailedYesInstance(instance.intervalSet);
  }

  intervalSetDelete(instance.intervalSet);

  return runInfo;
}

/**
 * Gets an instance generator that generates no instances and tests the bad memory algorithm on it.
 *
 * @param nIntervals The number of intervals in each instance.
 * @param instanceGenerator The instance generator to use.
 * @param solverAlgorithm The solver algorithm to use, either badMemoryAlgorithm or
 * badMemoryDepthFirst.
 * @return True if the test was succesful, false otherwise.
 */
static RunInfo testNo(const uint32_t nIntervals, InstanceInfo (*instanceGenerator)(const uint32_t),
                      RunInfo (*solverAlgorithm)(IntervalSet*, char*), char* description) {
  instanceInitRandom();

  InstanceInfo instance = instanceGenerator(nIntervals);
  RunInfo runInfo = solverAlgorithm(instance.intervalSet, description);

  runInfo.metadataLength = instance.metadataLength;
  runInfo.metadata = instance.metadata;

  if (runInfo.solutionFound) {
    logFailedNoInstance(instance.intervalSet);
  }

  intervalSetDelete(instance.intervalSet);

  return runInfo;
}

/**
 * Tests the bad memory algorithm on a set of simple yes instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return True if the test was succesful, false otherwise.
 */
RunInfo testSimpleYes(const uint32_t nIntervals) {
  debug_print("Testing simple yes instance.\n");

  return testYes(nIntervals, instanceSimpleYes, badMemoryAlgorithm, "SimpleYesBreadthFirst");
}

/**
 * Tests the bad memory algorithm on a set of simple no instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return True if the test was succesful, false otherwise.
 */
RunInfo testSimpleNo(const uint32_t nIntervals) {
  debug_print("Testing simple no instance.\n");

  return testNo(nIntervals, instanceSimpleNo, badMemoryAlgorithm, "SimpleNoBreadthFirst");
}

/**
 * Tests the bad memory algorithm on a set of hard whitness instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return True if the test was succesful, false otherwise.
 */
RunInfo testMaxWhitnessesYes(const uint32_t nIntervals) {
  debug_print("Testing max whitness yes instance.\n");

  return testYes(nIntervals, instanceMaxWhitnessesYes, badMemoryAlgorithm,
                 "MaxWhitnessesYesBreadthFirst");
}

/**
 * Tests the bad memory algorithm on a set of hard whitness no instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return True if the test was succesful, false otherwise.
 */
RunInfo testMaxWhitnessesNo(const uint32_t nIntervals) {
  debug_print("Testing max whitness no instance.\n");

  return testNo(nIntervals, instanceMaxWhitnessesNo, badMemoryAlgorithm,
                "MaxWhitnessesNoBreadthFirst");
}

/**
 * Tests the bad memory algorithm on a set of whitness yes instances that try to maximize the number
 * of groups built.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return True if the test was succesful, false otherwise.
 */
RunInfo testMaxGroupWhitnessesYes(const uint32_t nIntervals) {
  debug_print("Testing max group whitness yes instance.\n");

  return testYes(nIntervals, instanceMaxGroupWhitnessesYes, badMemoryAlgorithm,
                 "MaxGroupWhitnessesYesBreadthFirst");
}

/**
 * Tests the bad memory algorithm on a set of whitness no instances that try to maximize the number
 * of groups built.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return True if the test was succesful, false otherwise.
 */
RunInfo testMaxGroupWhitnessesNo(const uint32_t nIntervals) {
  debug_print("Testing max group whitness no instance.\n");

  return testNo(nIntervals, instanceMaxGroupWhitnessesNo, badMemoryAlgorithm,
                "MaxGroupWhitnessesNoBreadthFirst");
}

/**
 * Tests the bad memory algorithm on a set of hard yes amount version instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return True if the test was succesful, false otherwise.
 */
RunInfo testHardYesAmountVersion(const uint32_t nIntervals) {
  debug_print("Testing hard yes amount version instance.\n");

  return testYes(nIntervals, instanceHardYesAmountVersion, badMemoryAlgorithm,
                 "HardYesAmountVersionBreadthFirst");
}

/**
 * Tests the bad memory algorithm on a set of hard no amount version instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return True if the test was succesful, false otherwise.
 */
RunInfo testHardNoAmountVersion(const uint32_t nIntervals) {
  debug_print("Testing hard no amount version instance.\n");

  return testNo(nIntervals, instanceHardNoAmountVersion, badMemoryAlgorithm,
                "HardNoAmountVersionBreadthFirst");
}

/**
 * Tests the bad memory algorithm on a set of all full instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return True if the test was succesful, false otherwise.
 */
RunInfo testAllFull(const uint32_t nIntervals) {
  debug_print("Testing all full instance.\n");

  return testYes(nIntervals, instanceAllFull, badMemoryAlgorithm, "AllFullBreadthFirst");
}

/**
 * Tests the depth-first variant of the bad memory algorithm on a set of simple yes instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return True if the test was succesful, false otherwise.
 */
RunInfo testDepthFirstSimpleYes(const uint32_t nIntervals) {
  debug_print("Testing depth-first simple yes instance.\n");

  return testYes(nIntervals, instanceSimpleYes, badMemoryDepthFirst, "SimpleYesDepthFirst");
}

/**
 * Tests the depth-first variant of the bad memory algorithm on a set of simple no instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return True if the test was succesful, false otherwise.
 */
RunInfo testDepthFirstSimpleNo(const uint32_t nIntervals) {
  debug_print("Testing depth-first simple no instance.\n");

  return testNo(nIntervals, instanceSimpleNo, badMemoryDepthFirst, "SimpleNoDepthFirst");
}

/**
 * Tests the depth-first variant of the bad memory algorithm on a set of hard whitness instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return True if the test was succesful, false otherwise.
 */
RunInfo testDepthFirstMaxWhitnessesYes(const uint32_t nIntervals) {
  debug_print("Testing depth-first max whitness yes instance.\n");

  return testYes(nIntervals, instanceMaxWhitnessesYes, badMemoryDepthFirst,
                 "MaxWhitnessesYesDepthFirst");
}

/**
 * Tests the depth-first variant of the bad memory algorithm on a set of hard whitness no instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return True if the test was succesful, false otherwise.
 */
RunInfo testDepthFirstMaxWhitnessesNo(const uint32_t nIntervals) {
  debug_print("Testing depth-first max whitness no instance.\n");

  return testNo(nIntervals, instanceMaxWhitnessesNo, badMemoryDepthFirst,
                "MaxWhitnessesNoDepthFirst");
}

/**
 * Tests the depth-first variant of the bad memory algorithm on a set of whitness yes instances that
 * try to maximize the number of groups built.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return True if the test was succesful, false otherwise.
 */
RunInfo testDepthFirstMaxGroupWhitnessesYes(const uint32_t nIntervals) {
  debug_print("Testing depth-first max group whitness yes instance.\n");

  return testYes(nIntervals, instanceMaxGroupWhitnessesYes, badMemoryDepthFirst,
                 "MaxGroupWhitnessesYesDepthFirst");
}

/**
 * Tests the depth-first variant of the bad memory algorithm on a set of whitness no instances that
 * try to maximize the number of groups built.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return True if the test was succesful, false otherwise.
 */
RunInfo testDepthFirstMaxGroupWhitnessesNo(const uint32_t nIntervals) {
  debug_print("Testing depth-first max group whitness no instance.\n");

  return testNo(nIntervals, instanceMaxGroupWhitnessesNo, badMemoryDepthFirst,
                "MaxGroupWhitnessesNoDepthFirst");
}

/**
 * Tests the depth-first variant of the bad memory algorithm on a set of hard yes amount version
 * instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return True if the test was succesful, false otherwise.
 */
RunInfo testDepthFirstHardYesAmountVersion(const uint32_t nIntervals) {
  debug_print("Testing depth-first hard yes amount version instance.\n");

  return testYes(nIntervals, instanceHardYesAmountVersion, badMemoryDepthFirst,
                 "HardYesAmountVersionDepthFirst");
}

/**
 * Tests the depth-first variant of the bad memory algorithm on a set of hard no amount version
 * instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return True if the test was succesful, false otherwise.
 */
RunInfo testDepthFirstHardNoAmountVersion(const uint32_t nIntervals) {
  debug_print("Testing depth-first hard no amount version instance.\n");

  return testNo(nIntervals, instanceHardNoAmountVersion, badMemoryDepthFirst,
                "HardNoAmountVersionDepthFirst");
}

/**
 * Tests the depth-first variant of the bad memory algorithm on a set of all full instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return True if the test was successful, false otherwise.
 */
RunInfo testDepthFirstAllFull(const uint32_t nIntervals) {
  debug_print("Testing depth-first all full instance.\n");

  return testYes(nIntervals, instanceAllFull, badMemoryDepthFirst, "AllFullDepthFirst");
}

/**
 * Runs all tests and logs the time it took to execute each test.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return True if all tests were succesful, false otherwise.
 */
void testRunAll(const uint32_t nIntervals) {

  measureTime(nIntervals, testSimpleYes);
  measureTime(nIntervals, testSimpleNo);
  measureTime(nIntervals, testMaxWhitnessesYes);
  measureTime(nIntervals, testMaxWhitnessesNo);
  measureTime(nIntervals, testMaxGroupWhitnessesYes);
  measureTime(nIntervals, testMaxGroupWhitnessesNo);
  measureTime(nIntervals, testAllFull);

  debug_print("All tests completed.\n");
}

/**
 * Runs 1 instance of all yes tests and logs the time it took to execute each test.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return True if all tests were succesful, false otherwise.
 */
void testRunYes(const uint32_t nIntervals) {

  measureTime(nIntervals, testSimpleYes);
  measureTime(nIntervals, testMaxWhitnessesYes);
  measureTime(nIntervals, testMaxGroupWhitnessesYes);
  measureTime(nIntervals, testAllFull);
  // success &= measureTime(1, nIntervals, testHardYesAmountVersion);

  debug_print("All yes tests completed.\n");
}

/**
 * Runs all tests for the depth-first variant and logs the time it took to execute each test.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return True if all tests were succesful, false otherwise.
 */
void testRunAllDepthFirst(const uint32_t nIntervals) {

  measureTime(nIntervals, testDepthFirstSimpleYes);
  measureTime(nIntervals, testDepthFirstSimpleNo);
  measureTime(nIntervals, testDepthFirstMaxWhitnessesYes);
  measureTime(nIntervals, testDepthFirstMaxWhitnessesNo);
  measureTime(nIntervals, testDepthFirstMaxGroupWhitnessesYes);
  measureTime(nIntervals, testDepthFirstMaxGroupWhitnessesNo);
  measureTime(nIntervals, testDepthFirstAllFull);

  debug_print("All depth-first tests completed.\n");
}