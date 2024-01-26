#ifndef INSTANCEGEN_H
#define INSTANCEGEN_H

#include "intervalSet.h"
#include <stdint.h>

void instanceInitRandom();
IntervalSet* instanceSimpleYes(const uint32_t n);
IntervalSet* instanceSimpleNo(const uint32_t n);
IntervalSet* instanceMaxWhitnessesYes(const uint32_t n);
IntervalSet* instanceMaxWhitnessesNo(const uint32_t n);
IntervalSet* instanceMaxGroupWhitnessesYes(const uint32_t n);
IntervalSet* instanceMaxGroupWhitnessesNo(const uint32_t n);
IntervalSet* instanceHardYesAmountVersion(const uint32_t n);
IntervalSet* instanceHardNoAmountVersion(const uint32_t n);
IntervalSet* instanceAllFull(const uint32_t n);

#endif // INSTANCEGEN_H