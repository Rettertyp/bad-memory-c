#include "jsonPrinter.h"
#include <json-c/json.h>
#include <stdio.h>
#include <unistd.h>

/**
 * Frees the memory used by a RunInfo.
 *
 * @param runInfo The RunInfo to free.
 */
void runInfoDelete(RunInfo* runInfo) {
  free(runInfo->metadata);
  runInfo->metadata = NULL;
}

/**
 * Adds the contents of a RunInfo to a json_object. Also calculates and adds average edge grades.
 *
 * @param runInfo The RunInfo to add to the json_object.
 * @param jobj The json_object to add the RunInfo to.
 */
static void addRunInfoToJson(RunInfo* runInfo, double timeUsed, json_object* jobj) {
  json_object_object_add(jobj, "description", json_object_new_string(runInfo->description));
  json_object_object_add(jobj, "solutionFound", json_object_new_boolean(runInfo->solutionFound));
  json_object_object_add(jobj, "nIntervals", json_object_new_int(runInfo->nIntervals));
  json_object_object_add(jobj, "nGroupsBuilt", json_object_new_int(runInfo->nGroupsBuilt));
  json_object_object_add(jobj, "nGroupsKept", json_object_new_int(runInfo->nGroupsKept));
  json_object_object_add(jobj, "nSolutions", json_object_new_int(runInfo->nSolutions));
  json_object_object_add(jobj, "nSteps", json_object_new_int(runInfo->nSteps));
  json_object_object_add(jobj, "nUsedNodes", json_object_new_int(runInfo->nUsedNodes));
  json_object_object_add(jobj, "nOutgoingEdges", json_object_new_int(runInfo->nOutgoingEdges));
  json_object_object_add(jobj, "nIncomingEdges", json_object_new_int(runInfo->nIncomingEdges));
  json_object_object_add(
      jobj, "avgOutgoingEdges",
      json_object_new_double((double)runInfo->nOutgoingEdges / runInfo->nUsedNodes));
  json_object_object_add(
      jobj, "avgIncomingEdges",
      json_object_new_double((double)runInfo->nIncomingEdges / runInfo->nUsedNodes));
  json_object_object_add(jobj, "maxOutgoingEdges", json_object_new_int(runInfo->maxOutgoingEdges));
  json_object_object_add(jobj, "maxIncomingEdges", json_object_new_int(runInfo->maxIncomingEdges));
  json_object_object_add(jobj, "nEdges", json_object_new_int(runInfo->nEdges));
  json_object_object_add(jobj, "nMarkedSets", json_object_new_int(runInfo->nMarkedSets));
  json_object_object_add(jobj, "runTime", json_object_new_double(timeUsed));
  // add metadata
  json_object_object_add(jobj, "metadata", json_object_new_array_ext(runInfo->metadataLength));
  json_object* metadataArray = json_object_object_get(jobj, "metadata");
  for (uint32_t i = 0; i < runInfo->metadataLength; i++) {
    json_object_array_put_idx(metadataArray, i, json_object_new_int(runInfo->metadata[i]));
  }
}

/**
 * Saves the contents of a RunInfo to a json file.
 *
 * @param description A description of the run.
 * @param runInfo The RunInfo to save to the json file.
 */
void jsonPrinterSaveToFile(RunInfo runInfo, double timeUsed) {
  json_object* jobj = json_object_new_object();

  addRunInfoToJson(&runInfo, timeUsed, jobj);

  char filename[100];

  // check if the file already exists, if so, increase the number in the filename
  int i = 1;
  do {
    sprintf(filename, "results/%s_%d_%d.json", runInfo.description, runInfo.nIntervals, i);
    i++;
  } while (access(filename, F_OK) != -1);

  FILE* file = fopen(filename, "w");

  if (file) {
    fprintf(file, "%s", json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_PRETTY));
    fclose(file);
  } else {
    printf("Error: Could not open file %s for writing.\n", filename);
  }

  // free the json object
  json_object_put(jobj);
}