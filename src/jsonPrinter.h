#ifndef JSON_PRINTER_H
#define JSON_PRINTER_H

#include <stdint.h>

typedef struct RunInfo {
  uint32_t nIntervals;
  uint32_t nGroupsBuilt;
  uint32_t nGroupsKept;
  uint32_t nSolutions;
  uint32_t nSteps;
  uint32_t nUsedNodes;
  uint32_t nOutgoingEdges;
  uint32_t nIncomingEdges;
  uint32_t maxOutgoingEdges;
  uint32_t maxIncomingEdges;
  uint32_t nEdges;
  char* description;
} RunInfo;

void jsonPrinterSaveToFile(const char* description, RunInfo runInfo);

#endif // JSON_PRINTER_H