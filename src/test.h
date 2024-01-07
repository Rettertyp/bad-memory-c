#ifndef TEST_H
#define TEST_H

#include <stdbool.h>
#include <stdint.h>

bool testSimpleYes(const uint32_t nInstances, const uint32_t nIntervals);
bool testSimpleNo(const uint32_t nInstances, const uint32_t nIntervals);
bool testHardYes(const uint32_t nInstances, const uint32_t nIntervals);
bool testHardNo(const uint32_t nInstances, const uint32_t nIntervals);

#endif // TEST_H
