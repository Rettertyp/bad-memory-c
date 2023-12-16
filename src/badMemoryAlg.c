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

  printf("The sorted input interval set is:\n");
  printIntervalSet(inputIntervalSet);
  printf("\n");

  const int n = inputIntervalSet->length;
  GraphNode graphNodes[n][n];
  GraphNode* graphNodesPtr = (GraphNode*)graphNodes;

  // Initialize empty graphNodes
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      *(getGraphNode(graphNodesPtr, i, j, n)) = createGraphNode(i, j);
    }
  }

  printf("Initializing base cases...\n");

  // initialize the base cases
  for (int i = 1; i <= n; i++) {
    AssignRes assignRes = assign(inputIntervalSet, i);
    if (assignRes.statusCode == SUCCESS) {
      addIntervalSet(getGraphNode(graphNodesPtr, i, i, n), assignRes.intervalSet);
      printGraphNodeDetailed(getGraphNode(graphNodesPtr, i, i, n));
    }
  }

  printf("\nGoing into the main loop...\n");

  // main loop of the algorithm
  for (int i = n; i > 0; i--) {
    for (int s = i + 1; s <= n; s++) {
      GraphNode* currNode = getGraphNode(graphNodesPtr, i, s, n);
      printf("\ncurrNode: ");
      printGraphNode(currNode);
      const int s_ = s - i;
      for (int i_ = i; i_ <= s_; i_++) {

        printGraphNode(getGraphNode(graphNodesPtr, i_, s_, n));

        // iterate over all the interval sets in the graph node
        IntervalSetNode* node = getGraphNode(graphNodesPtr, i_, s_, n)->intervalSets;
        while (node != NULL) {

          IntervalSet* lowestPart = getLowestPart(node->set);

          // try to build the current group
          AssignRes assignRes = assign(lowestPart, i);

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

  printf("\nChecking if there is a solution...\n");

  // check if there is a solution
  for (int i = 1; i <= n; i++) {
    GraphNode* currNode = getGraphNode(graphNodesPtr, i, n, n);
    if (getNumberOfIntervalSets(currNode) > 0) {
      printGraphNodeDetailed(currNode);
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
AssignRes assign(const IntervalSet* intervalSet, const int groupSize) {
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
