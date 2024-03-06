#include "jsonPrinter.h"
#include <json-c/json.h>
#include <stdio.h>
#include <unistd.h>

#define FILE_DESC_LENGTH 100

/**
 * Frees the memory used by a RunInfo.
 *
 * @param runInfo The RunInfo to free.
 */
void runInfoDelete(RunInfo* runInfo) {
  if (runInfo->metadata) {
    free(runInfo->metadata);
    runInfo->metadata = NULL;
  }
}

/**
 * Adds the contents of a RunInfo to a json_object. Also calculates and adds average edge grades.
 *
 * @param runInfo The RunInfo to add to the json_object.
 * @param jobj The json_object to add the RunInfo to.
 */
static void addRunInfoToJson(RunInfo* runInfo, json_object* jobj) {
  json_object_object_add(jobj, "description", json_object_new_string(runInfo->description));
  json_object_object_add(jobj, "solutionFound", json_object_new_boolean(runInfo->solutionFound));
  json_object_object_add(jobj, "nIntervals", json_object_new_int(runInfo->nIntervals));
  json_object_object_add(jobj, "nGroupsBuilt", json_object_new_int(runInfo->nGroupsBuilt));
  json_object_object_add(jobj, "nGroupsKept", json_object_new_int(runInfo->nGroupsKept));
  json_object_object_add(jobj, "nSolutions", json_object_new_int(runInfo->nSolutions));
  json_object_object_add(jobj, "nSteps", json_object_new_int(runInfo->nSteps));
  json_object_object_add(jobj, "nUsedNodes", json_object_new_int(runInfo->nUsedNodes));
  json_object_object_add(jobj, "avgOutgoingEdges",
                         json_object_new_double(runInfo->avgOutgoingEdges));
  json_object_object_add(jobj, "avgIncomingEdges",
                         json_object_new_double(runInfo->avgIncomingEdges));
  json_object_object_add(jobj, "maxOutgoingEdges", json_object_new_int(runInfo->maxOutgoingEdges));
  json_object_object_add(jobj, "maxIncomingEdges", json_object_new_int(runInfo->maxIncomingEdges));
  json_object_object_add(jobj, "nEdges", json_object_new_int(runInfo->nEdges));
  json_object_object_add(jobj, "nMarkedSets", json_object_new_int(runInfo->nMarkedSets));
  json_object_object_add(jobj, "maxSetsPerNode", json_object_new_int(runInfo->maxSetsPerNode));
  json_object_object_add(jobj, "longestPath", json_object_new_int(runInfo->longestPath));
  json_object_object_add(jobj, "shortestPath", json_object_new_int(runInfo->shortestPath));
  json_object_object_add(jobj, "runTime", json_object_new_double(runInfo->runTime));
  // add metadata
  json_object_object_add(jobj, "metadata", json_object_new_array_ext(runInfo->metadataLength));
  json_object* metadataArray = json_object_object_get(jobj, "metadata");
  for (uint32_t i = 0; i < runInfo->metadataLength; i++) {
    json_object_array_put_idx(metadataArray, i, json_object_new_int(runInfo->metadata[i]));
  }
}

/**
 * Saves a json_object to a file.
 *
 * @param jobj The json_object to save to the file.
 * @param fileDesc The description of the file to save the json_object to.
 */
static void jsonPrinterSaveToFile(json_object* jobj, char fileDesc[100]) {
  char filename[100];

  // check if the file already exists, if so, increase the number in the filename
  int i = 1;
  do {
    sprintf(filename, "results/%s_%d.json", fileDesc, i);
    i++;
  } while (access(filename, F_OK) != -1);

  FILE* file = fopen(filename, "w");

  if (file) {
    fprintf(file, "%s", json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_PRETTY));
    fclose(file);
  } else {
    printf("Error: Could not open file %s for writing.\n", filename);
  }
}

/**
 * Saves the contents of a RunInfo to a json file.
 *
 * @param runInfo The RunInfo to save to the json file.
 */
void jsonPrinterPrint(RunInfo runInfo) {
  json_object* jobj = json_object_new_object();

  addRunInfoToJson(&runInfo, jobj);

  char fileDesc[FILE_DESC_LENGTH];
  snprintf(fileDesc, FILE_DESC_LENGTH, "%s_%d", runInfo.description, runInfo.nIntervals);

  jsonPrinterSaveToFile(jobj, fileDesc);

  // free the json object
  json_object_put(jobj);
}

/**
 * Saves the contents of an array of RunInfo to a json file.
 *
 * @param breadthFirstRunInfo The RunInfo for the breadth first algorithm.
 * @param depthFirstRunInfo The RunInfo for the depth first algorithm.
 * @param parallelizedRunInfo The RunInfo for the parallelized algorithm.
 */
void jsonPrinterPrintArray(RunInfo breadthFirstRunInfo, RunInfo depthFirstRunInfo,
                           RunInfo parallelizedRunInfo) {
  json_object* jobj = json_object_new_array_ext(2);

  // add breadth first information
  json_object* breadthFirstJson = json_object_new_object();
  addRunInfoToJson(&breadthFirstRunInfo, breadthFirstJson);
  json_object_array_put_idx(jobj, 0, breadthFirstJson);

  // add depth first information
  json_object* depthFirstJson = json_object_new_object();
  addRunInfoToJson(&depthFirstRunInfo, depthFirstJson);
  json_object_array_put_idx(jobj, 1, depthFirstJson);

  // add parallel information
  json_object* parallelJson = json_object_new_object();
  addRunInfoToJson(&parallelizedRunInfo, parallelJson);
  json_object_array_put_idx(jobj, 2, parallelJson);

  char fileDesc[FILE_DESC_LENGTH];
  snprintf(fileDesc, FILE_DESC_LENGTH, "%s_&_%s_%d", breadthFirstRunInfo.description,
           depthFirstRunInfo.description, breadthFirstRunInfo.nIntervals);

  jsonPrinterSaveToFile(jobj, fileDesc);

  // free the json object
  json_object_put(jobj);
}
