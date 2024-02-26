#ifndef BAD_MEMORY_ALG_H
#define BAD_MEMORY_ALG_H

#include "graphNode.h"
#include "jsonPrinter.h"
#include <stdlib.h>

RunInfo badMemoryAlgorithm(IntervalSet* inputIntervalSet, char* description);
RunInfo badMemoryDepthFirst(IntervalSet* inputIntervalSet, char* description);

#endif // BAD_MEMORY_ALG_H