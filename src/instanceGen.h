#ifndef INSTANCEGEN_H
#define INSTANCEGEN_H

#include "intervalSet.h"

void instanceInitRandom();
IntervalSet* instanceSimpleYes(const unsigned int n);
IntervalSet* instanceSimpleNo(const unsigned int n);

#endif // INSTANCEGEN_H