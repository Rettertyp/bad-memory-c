#ifndef TEST_H
#define TEST_H

#include "jsonPrinter.h"
#include <stdbool.h>
#include <stdint.h>

RunInfo testSimpleYes(const uint32_t nIntervals);
RunInfo testSimpleNo(const uint32_t nIntervals);
RunInfo testMaxWitnessesYes(const uint32_t nIntervals);
RunInfo testMaxWitnessesNo(const uint32_t nIntervals);
RunInfo testMaxGroupWitnessesYes(const uint32_t nIntervals);
RunInfo testMaxGroupWitnessesNo(const uint32_t nIntervals);
RunInfo testHardYesAmountVersion(const uint32_t nIntervals);
RunInfo testHardNoAmountVersion(const uint32_t nIntervals);
RunInfo testAllFull(const uint32_t nIntervals);
RunInfo testDepthFirstSimpleYes(const uint32_t nIntervals);
RunInfo testDepthFirstSimpleNo(const uint32_t nIntervals);
RunInfo testDepthFirstMaxWitnessesYes(const uint32_t nIntervals);
RunInfo testDepthFirstMaxWitnessesNo(const uint32_t nIntervals);
RunInfo testDepthFirstMaxGroupWitnessesYes(const uint32_t nIntervals);
RunInfo testDepthFirstMaxGroupWitnessesNo(const uint32_t nIntervals);
RunInfo testDepthFirstHardYesAmountVersion(const uint32_t nIntervals);
RunInfo testDepthFirstHardNoAmountVersion(const uint32_t nIntervals);
RunInfo testDepthFirstAllFull(const uint32_t nIntervals);
RunInfo testParallelizedSimpleYes(const uint32_t nIntervals);
RunInfo testParallelizedSimpleNo(const uint32_t nIntervals);
RunInfo testParallelizedMaxWitnessesYes(const uint32_t nIntervals);
RunInfo testParallelizedMaxWitnessesNo(const uint32_t nIntervals);
RunInfo testParallelizedMaxGroupWitnessesYes(const uint32_t nIntervals);
RunInfo testParallelizedMaxGroupWitnessesNo(const uint32_t nIntervals);
RunInfo testParallelizedHardYesAmountVersion(const uint32_t nIntervals);
RunInfo testParallelizedHardNoAmountVersion(const uint32_t nIntervals);
RunInfo testParallelizedAllFull(const uint32_t nIntervals);
void testRunAll(const uint32_t nIntervals);
void testRunYes(const uint32_t nIntervals);
void testRunAllDepthFirst(const uint32_t nIntervals);
void testRunAllParallelized(const uint32_t nIntervals);
void testSameSimpleInstances(const uint32_t nIntervals, const uint32_t nIterations);
void testAmountInstances(const uint32_t nIntervals);

#endif // TEST_H
