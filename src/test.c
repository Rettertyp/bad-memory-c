#include "test.h"
#include "badMemoryAlg.h"
#define DEBUG_PRINT 1
#include "debug.h"
#include "instanceGen.h"
#include <unistd.h>

/**
 * Determines whether the given interval should be logged.
 *
 * @param nIntervals The total number of intervals.
 * @param i The current interval index.
 * @return True if the interval should be logged, false otherwise.
 */
bool shouldBeLogged(const unsigned int nIntervals, const unsigned int i) {
  return nIntervals > 200 || !(i % 100);
}

/**
 * Tests the bad memory algorithm on a set of simple yes instances.
 *
 * @param nInstances The number of instances to test.
 * @param nIntervals The number of intervals in each instance.
 * @return True if the test was succesful, false otherwise.
 */
bool testSimpleYes(const unsigned int nInstances, const unsigned int nIntervals) {
  debug_print("Testing simple yes instances.\n");

  bool success = true;

  instanceInitRandom();

  for (unsigned int i = 0; i < nInstances; i++) {
    if (shouldBeLogged(nIntervals, i)) {
      debug_print("Running instance %d\n", i + 1);
    }

    IntervalSet* instance = instanceSimpleYes(nIntervals);
    bool hasSolution = badMemoryAlgorithm(instance);

    if (!hasSolution) {
      success = false;

      debug_print("Instance %d has no solution, but should.\n", i);
      if (instance->length == 0) {
        debug_print("()");
      } else
        for (unsigned int i = 0; i < instance->length; i++) {
          debug_print("(%d, %d), ", instance->intervals[i].bottom, instance->intervals[i].top);
        }

      debug_print("\n");
    }

    intervalSetDelete(instance);
  }

  debug_print("Done.\n\n");

  return success;
}

/**
 * Tests the bad memory algorithm on a set of simple no instances.
 *
 * @param nInstances The number of instances to test.
 * @param nIntervals The number of intervals in each instance.
 * @return True if the test was succesful, false otherwise.
 */
bool testSimpleNo(const unsigned int nInstances, const unsigned int nIntervals) {
  debug_print("Testing simple no instances.\n");

  bool success = true;

  instanceInitRandom();

  for (unsigned int i = 0; i < nInstances; i++) {
    if (shouldBeLogged(nIntervals, i)) {
      debug_print("Running instance %d\n", i + 1);
    }

    IntervalSet* instance = instanceSimpleNo(nIntervals);
    bool hasSolution = badMemoryAlgorithm(instance);

    if (hasSolution) {
      success = false;

      debug_print("Instance %d has a solution, but shouldn't.\n", i);
      if (instance->length == 0) {
        debug_print("()");
      } else
        for (unsigned int i = 0; i < instance->length; i++) {
          debug_print("(%d, %d), ", instance->intervals[i].bottom, instance->intervals[i].top);
        }

      debug_print("\n");
    }

    intervalSetDelete(instance);
  }

  debug_print("Done.\n\n");

  return success;
}

/**
 * Tests the bad memory algorithm on a set of hard whitness instances.
 *
 * @param nInstances The number of instances to test.
 * @param nIntervals The number of intervals in each instance.
 * @return True if the test was succesful, false otherwise.
 */
bool testHardYes(const unsigned int nInstances, const unsigned int nIntervals) {
  debug_print("Testing hard whitness instances.\n");

  bool success = true;

  instanceInitRandom();

  for (unsigned int i = 0; i < nInstances; i++) {
    if (shouldBeLogged(nIntervals, i)) {
      debug_print("Running instance %d\n", i + 1);
    }

    IntervalSet* instance = instanceHardYes(nIntervals);
    bool hasSolution = badMemoryAlgorithm(instance);

    if (!hasSolution) {
      success = false;
    }

    intervalSetDelete(instance);
  }

  debug_print("Done.\n\n");

  return success;
}
