#ifndef BAD_MEMORY_ALG_H
#define BAD_MEMORY_ALG_H

#include "graphNode.h"
#include "jsonPrinter.h"
#include <stdlib.h>

RunInfo badMemoryAlgorithm(IntervalSet* inputIntervalSet);
RunInfo badMemoryDepthFirst(IntervalSet* inputIntervalSet);
RunInfo badMemoryAlgorithmParallelized(IntervalSet* inputIntervalSet);

#endif // BAD_MEMORY_ALG_H