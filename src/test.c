#include "test.h"
#include "badMemoryAlg.h"
#define DEBUG_PRINT 1
#include "debug.h"
#include "instanceGen.h"
#include <time.h>
#include <unistd.h>

/**
 * Logs a failed yes instance for debugging purposes.
 *
 * @param instance The instance to log.
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
                       RunInfo (*solverAlgorithm)(IntervalSet*)) {
  instanceInitRandom();

  InstanceInfo instance = instanceGenerator(nIntervals);
  RunInfo runInfo = solverAlgorithm(instance.intervalSet);

  runInfo.metadataLength = instance.metadataLength;
  runInfo.metadata = instance.metadata;
  char tmp[RUN_INFO_DESC_LENGTH];
  snprintf(tmp, RUN_INFO_DESC_LENGTH, "%s_%s", instance.name, runInfo.description);
  strncpy(runInfo.description, tmp, RUN_INFO_DESC_LENGTH);

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
                      RunInfo (*solverAlgorithm)(IntervalSet*)) {
  instanceInitRandom();

  InstanceInfo instance = instanceGenerator(nIntervals);
  RunInfo runInfo = solverAlgorithm(instance.intervalSet);

  runInfo.metadataLength = instance.metadataLength;
  runInfo.metadata = instance.metadata;
  char tmp[RUN_INFO_DESC_LENGTH];
  snprintf(tmp, RUN_INFO_DESC_LENGTH, "%s_%s", instance.name, runInfo.description);
  strncpy(runInfo.description, tmp, RUN_INFO_DESC_LENGTH);

  if (runInfo.solutionFound) {
    logFailedNoInstance(instance.intervalSet);
  }

  intervalSetDelete(instance.intervalSet);

  return runInfo;
}

/**
 * Gets an instance and tests the bad memory algorithm on it.
 *
 * @param instance The instance to test.
 * @param solverAlgorithm The solver algorithm to use, either badMemoryAlgorithm or
 * badMemoryDepthFirst.
 * @return The RunInfo of the test.
 */
static RunInfo testGivenInstance(InstanceInfo instance, RunInfo (*solverAlgorithm)(IntervalSet*)) {
  RunInfo runInfo = solverAlgorithm(instance.intervalSet);

  runInfo.metadataLength = instance.metadataLength;
  runInfo.metadata = instance.metadata;
  char tmp[RUN_INFO_DESC_LENGTH];
  snprintf(tmp, RUN_INFO_DESC_LENGTH, "%s_%s", instance.name, runInfo.description);
  strncpy(runInfo.description, tmp, RUN_INFO_DESC_LENGTH);

  return runInfo;
}

/**
 * Measures and saves the time it takes to execute a test.
 *
 * @param nIntervals The number of intervals in each instance.
 * @param test The test to execute.
 */
static void measureTime(const uint32_t nIntervals, RunInfo (*test)(const uint32_t)) {
  clock_t start, end;

  start = clock();
  RunInfo runInfo = test(nIntervals);
  end = clock();

  // set the run time
  runInfo.runTime = ((double)(end - start)) / CLOCKS_PER_SEC;
  debug_print("Test took %.3f seconds to execute \n\n", runInfo.runTime);

  // save the results to a json file
  jsonPrinterPrint(runInfo);

  runInfoDelete(&(runInfo));
}

/**
 * Measures and saves the time it takes to execute all three algorithms on the same instance.
 *
 * @param nIntervals The number of intervals in each instance.
 * @param instanceGenerator The instance generator to use.
 */
static void measureTimeSameInstance(const uint32_t nIntervals,
                                    InstanceInfo (*instanceGenerator)(const uint32_t)) {
  instanceInitRandom();

  InstanceInfo instance = instanceGenerator(nIntervals);
  clock_t start, end;

  start = clock();
  RunInfo breadthFirstRunInfo = testGivenInstance(instance, badMemoryAlgorithm);
  end = clock();
  breadthFirstRunInfo.runTime = ((double)(end - start)) / CLOCKS_PER_SEC;
  debug_print("BreadthFirst took %.3f seconds to execute \n\n", breadthFirstRunInfo.runTime);

  start = clock();
  RunInfo depthFirstRunInfo = testGivenInstance(instance, badMemoryDepthFirst);
  end = clock();
  depthFirstRunInfo.runTime = ((double)(end - start)) / CLOCKS_PER_SEC;
  debug_print("DepthFirst took %.3f seconds to execute \n\n", depthFirstRunInfo.runTime);

  start = clock();
  RunInfo parallelizedRunInfo = testGivenInstance(instance, badMemoryAlgorithmParallelized);
  end = clock();
  parallelizedRunInfo.runTime = ((double)(end - start)) / CLOCKS_PER_SEC;
  debug_print("Parallelized took %.3f seconds to execute \n\n", parallelizedRunInfo.runTime);

  // delete the instance
  intervalSetDelete(instance.intervalSet);

  // save the results to a json file
  jsonPrinterPrintArray(breadthFirstRunInfo, depthFirstRunInfo, parallelizedRunInfo);

  runInfoDelete(&(breadthFirstRunInfo));
  // frees metadata of both runInfos, since it's the same
}

/**
 * Tests the bad memory algorithm on a set of simple yes instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return The RunInfo of the test.
 */
RunInfo testSimpleYes(const uint32_t nIntervals) {
  debug_print("Testing simple yes instance.\n");

  return testYes(nIntervals, instanceSimpleYes, badMemoryAlgorithm);
}

/**
 * Tests the bad memory algorithm on a set of simple no instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return The RunInfo of the test.
 */
RunInfo testSimpleNo(const uint32_t nIntervals) {
  debug_print("Testing simple no instance.\n");

  return testNo(nIntervals, instanceSimpleNo, badMemoryAlgorithm);
}

/**
 * Tests the bad memory algorithm on a set of hard whitness instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return The RunInfo of the test.
 */
RunInfo testMaxWhitnessesYes(const uint32_t nIntervals) {
  debug_print("Testing max whitness yes instance.\n");

  return testYes(nIntervals, instanceMaxWhitnessesYes, badMemoryAlgorithm);
}

/**
 * Tests the bad memory algorithm on a set of hard whitness no instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return The RunInfo of the test.
 */
RunInfo testMaxWhitnessesNo(const uint32_t nIntervals) {
  debug_print("Testing max whitness no instance.\n");

  return testNo(nIntervals, instanceMaxWhitnessesNo, badMemoryAlgorithm);
}

/**
 * Tests the bad memory algorithm on a set of whitness yes instances that try to maximize the number
 * of groups built.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return The RunInfo of the test.
 */
RunInfo testMaxGroupWhitnessesYes(const uint32_t nIntervals) {
  debug_print("Testing max group whitness yes instance.\n");

  return testYes(nIntervals, instanceMaxGroupWhitnessesYes, badMemoryAlgorithm);
}

/**
 * Tests the bad memory algorithm on a set of whitness no instances that try to maximize the number
 * of groups built.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return The RunInfo of the test.
 */
RunInfo testMaxGroupWhitnessesNo(const uint32_t nIntervals) {
  debug_print("Testing max group whitness no instance.\n");

  return testNo(nIntervals, instanceMaxGroupWhitnessesNo, badMemoryAlgorithm);
}

/**
 * Tests the bad memory algorithm on a set of hard yes amount version instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return The RunInfo of the test.
 */
RunInfo testHardYesAmountVersion(const uint32_t nIntervals) {
  debug_print("Testing hard yes amount version instance.\n");

  return testYes(nIntervals, instanceHardYesAmountVersion, badMemoryAlgorithm);
}

/**
 * Tests the bad memory algorithm on a set of hard no amount version instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return The RunInfo of the test.
 */
RunInfo testHardNoAmountVersion(const uint32_t nIntervals) {
  debug_print("Testing hard no amount version instance.\n");

  return testNo(nIntervals, instanceHardNoAmountVersion, badMemoryAlgorithm);
}

/**
 * Tests the bad memory algorithm on a set of all full instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return The RunInfo of the test.
 */
RunInfo testAllFull(const uint32_t nIntervals) {
  debug_print("Testing all full instance.\n");

  return testYes(nIntervals, instanceAllFull, badMemoryAlgorithm);
}

/**
 * Tests the depth-first variant of the bad memory algorithm on a set of simple yes instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return The RunInfo of the test.
 */
RunInfo testDepthFirstSimpleYes(const uint32_t nIntervals) {
  debug_print("Testing depth-first simple yes instance.\n");

  return testYes(nIntervals, instanceSimpleYes, badMemoryDepthFirst);
}

/**
 * Tests the depth-first variant of the bad memory algorithm on a set of simple no instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return The RunInfo of the test.
 */
RunInfo testDepthFirstSimpleNo(const uint32_t nIntervals) {
  debug_print("Testing depth-first simple no instance.\n");

  return testNo(nIntervals, instanceSimpleNo, badMemoryDepthFirst);
}

/**
 * Tests the depth-first variant of the bad memory algorithm on a set of hard whitness instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return The RunInfo of the test.
 */
RunInfo testDepthFirstMaxWhitnessesYes(const uint32_t nIntervals) {
  debug_print("Testing depth-first max whitness yes instance.\n");

  return testYes(nIntervals, instanceMaxWhitnessesYes, badMemoryDepthFirst);
}

/**
 * Tests the depth-first variant of the bad memory algorithm on a set of hard whitness no instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return The RunInfo of the test.
 */
RunInfo testDepthFirstMaxWhitnessesNo(const uint32_t nIntervals) {
  debug_print("Testing depth-first max whitness no instance.\n");

  return testNo(nIntervals, instanceMaxWhitnessesNo, badMemoryDepthFirst);
}

/**
 * Tests the depth-first variant of the bad memory algorithm on a set of whitness yes instances that
 * try to maximize the number of groups built.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return The RunInfo of the test.
 */
RunInfo testDepthFirstMaxGroupWhitnessesYes(const uint32_t nIntervals) {
  debug_print("Testing depth-first max group whitness yes instance.\n");

  return testYes(nIntervals, instanceMaxGroupWhitnessesYes, badMemoryDepthFirst);
}

/**
 * Tests the depth-first variant of the bad memory algorithm on a set of whitness no instances that
 * try to maximize the number of groups built.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return The RunInfo of the test.
 */
RunInfo testDepthFirstMaxGroupWhitnessesNo(const uint32_t nIntervals) {
  debug_print("Testing depth-first max group whitness no instance.\n");

  return testNo(nIntervals, instanceMaxGroupWhitnessesNo, badMemoryDepthFirst);
}

/**
 * Tests the depth-first variant of the bad memory algorithm on a set of hard yes amount version
 * instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return The RunInfo of the test.
 */
RunInfo testDepthFirstHardYesAmountVersion(const uint32_t nIntervals) {
  debug_print("Testing depth-first hard yes amount version instance.\n");

  return testYes(nIntervals, instanceHardYesAmountVersion, badMemoryDepthFirst);
}

/**
 * Tests the depth-first variant of the bad memory algorithm on a set of hard no amount version
 * instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return The RunInfo of the test.
 */
RunInfo testDepthFirstHardNoAmountVersion(const uint32_t nIntervals) {
  debug_print("Testing depth-first hard no amount version instance.\n");

  return testNo(nIntervals, instanceHardNoAmountVersion, badMemoryDepthFirst);
}

/**
 * Tests the depth-first variant of the bad memory algorithm on a set of all full instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return The RunInfo of the test.
 */
RunInfo testDepthFirstAllFull(const uint32_t nIntervals) {
  debug_print("Testing depth-first all full instance.\n");

  return testYes(nIntervals, instanceAllFull, badMemoryDepthFirst);
}

/**
 * Tests the parallelized version of the bad memory algorithm on a set of simple yes instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return The RunInfo of the test.
 */
RunInfo testParallelizedSimpleYes(const uint32_t nIntervals) {
  debug_print("Testing parallelized simple yes instance.\n");

  return testYes(nIntervals, instanceSimpleYes, badMemoryAlgorithmParallelized);
}

/**
 * Tests the parallelized version of the bad memory algorithm on a set of simple no instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return The RunInfo of the test.
 */
RunInfo testParallelizedSimpleNo(const uint32_t nIntervals) {
  debug_print("Testing parallelized simple no instance.\n");

  return testNo(nIntervals, instanceSimpleNo, badMemoryAlgorithmParallelized);
}

/**
 * Tests the parallelized version of the bad memory algorithm on a set of hard whitness instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return The RunInfo of the test.
 */
RunInfo testParallelizedMaxWhitnessesYes(const uint32_t nIntervals) {
  debug_print("Testing parallelized max whitness yes instance.\n");

  return testYes(nIntervals, instanceMaxWhitnessesYes, badMemoryAlgorithmParallelized);
}

/**
 * Tests the parallelized version of the bad memory algorithm on a set of hard whitness no
 * instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return The RunInfo of the test.
 */
RunInfo testParallelizedMaxWhitnessesNo(const uint32_t nIntervals) {
  debug_print("Testing parallelized max whitness no instance.\n");

  return testNo(nIntervals, instanceMaxWhitnessesNo, badMemoryAlgorithmParallelized);
}

/**
 * Tests the parallelized version of the bad memory algorithm on a set of whitness yes instances
 * that try to maximize the number of groups built.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return The RunInfo of the test.
 */
RunInfo testParallelizedMaxGroupWhitnessesYes(const uint32_t nIntervals) {
  debug_print("Testing parallelized max group whitness yes instance.\n");

  return testYes(nIntervals, instanceMaxGroupWhitnessesYes, badMemoryAlgorithmParallelized);
}

/**
 * Tests the parallelized version of the bad memory algorithm on a set of whitness no instances that
 * try to maximize the number of groups built.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return The RunInfo of the test.
 */
RunInfo testParallelizedMaxGroupWhitnessesNo(const uint32_t nIntervals) {
  debug_print("Testing parallelized max group whitness no instance.\n");

  return testNo(nIntervals, instanceMaxGroupWhitnessesNo, badMemoryAlgorithmParallelized);
}

/**
 * Tests the parallelized version of the bad memory algorithm on a set of hard yes amount version
 * instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return The RunInfo of the test.
 */
RunInfo testParallelizedHardYesAmountVersion(const uint32_t nIntervals) {
  debug_print("Testing parallelized hard yes amount version instance.\n");

  return testYes(nIntervals, instanceHardYesAmountVersion, badMemoryAlgorithmParallelized);
}

/**
 * Tests the parallelized version of the bad memory algorithm on a set of hard no amount version
 * instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return The RunInfo of the test.
 */
RunInfo testParallelizedHardNoAmountVersion(const uint32_t nIntervals) {
  debug_print("Testing parallelized hard no amount version instance.\n");

  return testNo(nIntervals, instanceHardNoAmountVersion, badMemoryAlgorithmParallelized);
}

/**
 * Tests the parallelized version of the bad memory algorithm on a set of all full instances.
 *
 * @param nIntervals The number of intervals in each instance.
 * @return The RunInfo of the test.
 */
RunInfo testParallelizedAllFull(const uint32_t nIntervals) {
  debug_print("Testing parallelized all full instance.\n");

  return testYes(nIntervals, instanceAllFull, badMemoryAlgorithmParallelized);
}

/**
 * Runs all tests and logs the time it took to execute each test.
 *
 * @param nIntervals The number of intervals in each instance.
 */
void testRunAll(const uint32_t nIntervals) {

  // measureTime(nIntervals, testSimpleYes);
  // measureTime(nIntervals, testSimpleNo);
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
 */
void testRunAllDepthFirst(const uint32_t nIntervals) {
  // measureTime(nIntervals, testDepthFirstSimpleYes);
  // measureTime(nIntervals, testDepthFirstSimpleNo);
  measureTime(nIntervals, testDepthFirstMaxWhitnessesYes);
  measureTime(nIntervals, testDepthFirstMaxWhitnessesNo);
  measureTime(nIntervals, testDepthFirstMaxGroupWhitnessesYes);
  measureTime(nIntervals, testDepthFirstMaxGroupWhitnessesNo);
  measureTime(nIntervals, testDepthFirstAllFull);

  debug_print("All depth-first tests completed.\n");
}

/**
 * Runs all tests for the parallelized variant and logs the time it took to execute each test.
 *
 * @param nIntervals The number of intervals in each instance.
 */
void testRunAllParallelized(const uint32_t nIntervals) {
  // measureTime(nIntervals, testParallelizedSimpleYes);
  // measureTime(nIntervals, testParallelizedSimpleNo);
  measureTime(nIntervals, testParallelizedMaxWhitnessesYes);
  measureTime(nIntervals, testParallelizedMaxWhitnessesNo);
  measureTime(nIntervals, testParallelizedMaxGroupWhitnessesYes);
  measureTime(nIntervals, testParallelizedMaxGroupWhitnessesNo);
  measureTime(nIntervals, testParallelizedAllFull);

  debug_print("All parallelized tests completed.\n");
}

/**
 * Runs all algorithms on the same instance nIterations times.
 *
 * @param nIntervals The number of intervals in each instance.
 * @param nIterations The number of times to run each test.
 */
void testSameSimpleInstances(const uint32_t nIntervals, const uint32_t nIterations) {
  for (uint32_t i = 0; i < nIterations; i++) {
    measureTimeSameInstance(nIntervals, instanceSimpleYes);
    measureTimeSameInstance(nIntervals, instanceSimpleNo);
  }
}

/**
 * Runs all algorithms on amount instances consisting of nIntervals intervals.
 *
 * @param nIntervals The number of intervals in each instance.
 */
void testAmountInstances(const uint32_t nIntervals) {
  // measureTime(nIntervals, testHardYesAmountVersion);
  measureTime(nIntervals, testHardNoAmountVersion);
  // measureTime(nIntervals, testParallelizedHardYesAmountVersion);
  measureTime(nIntervals, testParallelizedHardNoAmountVersion);
  // measureTime(nIntervals, testDepthFirstHardYesAmountVersion);
  measureTime(nIntervals, testDepthFirstHardNoAmountVersion);
}
