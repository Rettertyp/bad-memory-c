#include "jsonPrinter.h"
#include <json-c/json.h>
#include <stdio.h>
#include <unistd.h>

/**
 * Adds the contents of a RunInfo to a json_object. Also calculates and adds average edge grades.
 *
 * @param runInfo The RunInfo to add to the json_object.
 * @param jobj The json_object to add the RunInfo to.
 */
static void addRunInfoToJson(RunInfo* runInfo, json_object* jobj) {
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
  json_object_object_add(jobj, "description", json_object_new_string(runInfo->description));
}

/**
 * Saves the contents of a RunInfo to a json file.
 *
 * @param description A description of the run.
 * @param runInfo The RunInfo to save to the json file.
 */
void jsonPrinterSaveToFile(const char* description, RunInfo runInfo) {
  json_object* jobj = json_object_new_object();

  addRunInfoToJson(&runInfo, jobj);

  char filename[100];

  // check if the file already exists, if so, increase the number in the filename
  int i = 1;
  do {
    sprintf(filename, "results/%s_%d_%d.json", description, runInfo.nIntervals, i);
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