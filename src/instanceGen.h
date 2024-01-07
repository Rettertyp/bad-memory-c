#ifndef INSTANCEGEN_H
#define INSTANCEGEN_H

#include "intervalSet.h"
#include <stdint.h>

void instanceInitRandom();
IntervalSet* instanceSimpleYes(const uint32_t n);
IntervalSet* instanceSimpleNo(const uint32_t n);
IntervalSet* instanceHardYes(const uint32_t n);
IntervalSet* instanceHardNo(const uint32_t n);

#endif // INSTANCEGEN_H