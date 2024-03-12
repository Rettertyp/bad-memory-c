#ifndef TEST_H
#define TEST_H

#include "jsonPrinter.h"
#include <stdbool.h>
#include <stdint.h>

RunInfo testSimpleYes(const uint32_t nIntervals);
RunInfo testSimpleNo(const uint32_t nIntervals);
RunInfo testMaxWhitnessesYes(const uint32_t nIntervals);
RunInfo testMaxWhitnessesNo(const uint32_t nIntervals);
RunInfo testMaxGroupWhitnessesYes(const uint32_t nIntervals);
RunInfo testMaxGroupWhitnessesNo(const uint32_t nIntervals);
RunInfo testHardYesAmountVersion(const uint32_t nIntervals);
RunInfo testHardNoAmountVersion(const uint32_t nIntervals);
RunInfo testAllFull(const uint32_t nIntervals);
RunInfo testDepthFirstSimpleYes(const uint32_t nIntervals);
RunInfo testDepthFirstSimpleNo(const uint32_t nIntervals);
RunInfo testDepthFirstMaxWhitnessesYes(const uint32_t nIntervals);
RunInfo testDepthFirstMaxWhitnessesNo(const uint32_t nIntervals);
RunInfo testDepthFirstMaxGroupWhitnessesYes(const uint32_t nIntervals);
RunInfo testDepthFirstMaxGroupWhitnessesNo(const uint32_t nIntervals);
RunInfo testDepthFirstHardYesAmountVersion(const uint32_t nIntervals);
RunInfo testDepthFirstHardNoAmountVersion(const uint32_t nIntervals);
RunInfo testDepthFirstAllFull(const uint32_t nIntervals);
RunInfo testParallelizedSimpleYes(const uint32_t nIntervals);
RunInfo testParallelizedSimpleNo(const uint32_t nIntervals);
RunInfo testParallelizedMaxWhitnessesYes(const uint32_t nIntervals);
RunInfo testParallelizedMaxWhitnessesNo(const uint32_t nIntervals);
RunInfo testParallelizedMaxGroupWhitnessesYes(const uint32_t nIntervals);
RunInfo testParallelizedMaxGroupWhitnessesNo(const uint32_t nIntervals);
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
