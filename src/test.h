#ifndef TEST_H
#define TEST_H

#include <stdbool.h>
#include <stdint.h>

bool measureTime(const uint32_t nInstances, const uint32_t nIntervals,
                 bool (*test)(const uint32_t, const uint32_t));
bool testSimpleYes(const uint32_t nInstances, const uint32_t nIntervals);
bool testSimpleNo(const uint32_t nInstances, const uint32_t nIntervals);
bool testMaxWhitnessesYes(const uint32_t nInstances, const uint32_t nIntervals);
bool testMaxWhitnessesNo(const uint32_t nInstances, const uint32_t nIntervals);
bool testAllFull(const uint32_t nInstances, const uint32_t nIntervals);
bool testRunAll(const uint32_t nInstances, const uint32_t nIntervals);
bool testRunYes(const uint32_t nIntervals);

#endif // TEST_H
