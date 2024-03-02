#ifndef JSON_PRINTER_H
#define JSON_PRINTER_H

#include <stdbool.h>
#include <stdint.h>

#define RUN_INFO_DESC_LENGTH 100

typedef struct RunInfo {
  char description[RUN_INFO_DESC_LENGTH];
  bool solutionFound;
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
  uint32_t nMarkedSets;
  uint32_t maxSetsPerNode;
  uint32_t minSetsPerNode;
  double runTime;
  uint32_t metadataLength;
  uint32_t* metadata;
} RunInfo;

void runInfoDelete(RunInfo* runInfo);
void jsonPrinterPrint(RunInfo runInfo);
void jsonPrinterPrintArray(RunInfo breadthFirstRunInfo, RunInfo depthFirstRunInfo);

#endif // JSON_PRINTER_H