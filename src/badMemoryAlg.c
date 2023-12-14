#include "badMemoryAlg.h"
#include <stdio.h>

/**
 * The main function of the bad memory algorithm.
 *
 * @param intervalSet The input IntervalSet to be processed.
 * @return Returns true if there is a solution, false otherwise.
 */
bool badMemoryAlgorithm(IntervalSet* inputIntervalSet) {
  int n = inputIntervalSet->length;
  GraphNode graphNodes[n][n];

  // Initialize empty graphNodes
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      graphNodes[i][j] = createGraphNode(i + 1, j + 1);
      printGraphNode(&(graphNodes[i][j]));
    }
  }

  // initialize the base cases
  for (int i = 0; i < n; i++) {
    AssignRes assignRes = assign(inputIntervalSet, i + 1);
    printf("assignRes.statusCode: %d\n", assignRes.statusCode);
    if (assignRes.statusCode == 0) {
      addIntervalSet(&(graphNodes[i][i]), assignRes.intervalSet);
      printGraphNode(&(graphNodes[i][i]));
    }
  }

  return true;
}

/**
 * Tries to build the group of size groupSize. If it is not possible to build
 * the group, returns NULL and sets the errorCode field of the AssignRes struct.
 *
 * @param intervalSet The input IntervalSet to be processed.
 * @param groupSize The number of intervals to be assigned.
 * @return Returns a pointer to an AssignRes struct containing the result of the
 * assignment operation. The errorCode field of the AssignRes struct indicates
 * the success or failure of the assignment operation.
 */
AssignRes assign(IntervalSet* intervalSet, int groupSize) {
  if (countGreaterI(intervalSet, groupSize) > 0) {
    return (AssignRes){NULL, 1};
  }

  if (countContainingI(intervalSet, groupSize) < groupSize) {
    return (AssignRes){NULL, 2};
  }

  printf("The group %d can be built.\n", groupSize);

  return (AssignRes){getWithoutFirstGIncludingI(intervalSet, groupSize, groupSize), 0};
}