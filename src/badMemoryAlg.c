#include "badMemoryAlg.h"
#include <stdio.h>

/**
 * The main function of the bad memory algorithm.
 *
 * @param intervalSet The input IntervalSet to be processed.
 * @return Returns true if there is a solution, false otherwise.
 */
bool badMemoryAlgorithm(IntervalSet* inputIntervalSet) {
  // Sort the intervals by their bottom value in descending order
  sortByBottom(inputIntervalSet);

  printIntervalSet(inputIntervalSet);

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
      printGraphNodeDetailed(&(graphNodes[i][i]));
    }
  }

  // main loop of the algorithm
  for (int i = n - 1; i >= 0; i--) {
    for (int s = i + 1; s < n; s++) {
      GraphNode* currNode = &(graphNodes[i][s]);
      for (int i_ = i; i_ < n; i_++) {
        int s_ = s - i;

        // iterate over all the interval sets in the graph node
        IntervalSetNode* node = graphNodes[i_][s_].intervalSets;
        while (node != NULL) {

          IntervalSet* lowestPart = getLowestPart(node->set);

          // try to build the current group
          AssignRes assignRes = assign(lowestPart, i + 1);

          if (assignRes.statusCode == SUCCESS) {
            // if the assignment was successful, add the new interval set to the graph
            // node
            addIntervalSet(currNode, assignRes.intervalSet);
            push(&(assignRes.intervalSet->stack), currNode);

            printGraphNodeDetailed(currNode);
          } else if (assignRes.statusCode == ERROR_evtl) {
            // #TODO: implement backtrack()
          }

          deleteIntervalSet(lowestPart);

          node = node->next;
        }
      }

      removeDominatedSets(currNode);
    }
  }

  // check if there is a solution
  for (int i = 0; i < n; i++) {
    GraphNode* currNode = &(graphNodes[i][n - 1]);
    printGraphNodeDetailed(currNode);
    if (getNumberOfIntervalSets(currNode) > 0) {
      return true;
    }
  }

  return false;
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
    return (AssignRes){NULL, ERROR_evtl};
  }

  if (countContainingI(intervalSet, groupSize) < groupSize) {
    return (AssignRes){NULL, ERROR_defn};
  }

  printf("The group %d can be built.\n", groupSize);

  return (AssignRes){getWithoutFirstGIncludingI(intervalSet, groupSize, groupSize),
                     SUCCESS};
}