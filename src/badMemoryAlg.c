#include "badMemoryAlg.h"
#include "debug.h"
#include <stdio.h>

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
    return (AssignRes){NULL, ERROR_defn};
  }

  if (countContainingI(intervalSet, groupSize) < groupSize) {
    return (AssignRes){NULL, ERROR_evtl};
  }

  debug_print("The group %d can be built.\n", groupSize);

  return (AssignRes){getWithoutFirstGIncludingI(intervalSet, groupSize, groupSize), SUCCESS};
}

/**
 * Tries to build the group groupSize of size rest. If it is not possible to build the group,
 * returns NULL and sets the errorCode field of the AssignRes struct.
 *
 * @param intervalSet The input IntervalSet to be processed.
 * @param groupSize The number of intervals to be assigned.
 * @param rest The number of intervals to be assigned.
 * @return Returns a pointer to an AssignRes struct containing the result of the assignment
 * operation. The errorCode field of the AssignRes struct indicates the success or failure of
 * the assignment operation.
 */
AssignRes assignRest(const IntervalSet* intervalSet, const int groupSize, const int rest) {
  if (countGreaterI(intervalSet, groupSize) > 0) {
    return (AssignRes){NULL, ERROR_defn};
  }

  if (countContainingI(intervalSet, groupSize) < rest) {
    return (AssignRes){NULL, ERROR_evtl};
  }

  debug_print("The group %d can be built.\n", groupSize);

  return (AssignRes){getWithoutFirstGIncludingI(intervalSet, groupSize, rest), SUCCESS};
}

/**
 * The backtrack function of the bad memory algorithm.
 *
 * @param predNode The predecessor node of the current node.
 * @param currNode The current node.
 * @param intervalSet The current interval set.
 * @param currStack The current stack.
 */
void backtrack(GraphNode* predNode, GraphNode* currNode, IntervalSet* intervalSet,
               Stack currStack) {

  debug_print("Backtracking... predNode: ");
  printGraphNodeDetailed(predNode);
  debug_print("currNode: ");
  printGraphNodeDetailed(currNode);

  int nLowestPartGEqI = countLowestPartGreaterEqualJ(intervalSet, currNode->i);

  const int rest = currNode->s - predNode->s - nLowestPartGEqI;

  IntervalSet* inverseLowestPart = getInverseLowestPartGreaterEqualJ(intervalSet, currNode->i);

  AssignRes assignRes = assignRest(inverseLowestPart, currNode->i, rest);

  deleteIntervalSet(inverseLowestPart);

  if (assignRes.statusCode == SUCCESS) {
    addIntervalSet(currNode, assignRes.intervalSet);
    printGraphNodeDetailed(currNode);

  } else if (assignRes.statusCode == ERROR_evtl) {

    GraphNode* nextPredNode = pop(&currStack);

    if (!nextPredNode) {
      return;
    }

    IntervalSetNode* currIntSetNode = nextPredNode->intervalSets;
    while (currIntSetNode) {
      IntervalSet* currSet = currIntSetNode->set;

      if (stackEquals(currSet->stack, currStack)) {
        backtrack(nextPredNode, currNode, currSet, currStack);
      }

      currIntSetNode = currIntSetNode->next;
    }
  }
}

/**
 * The main function of the bad memory algorithm.
 *
 * @param intervalSet The input IntervalSet to be processed.
 * @return Returns true if there is a solution, false otherwise.
 */
bool badMemoryAlgorithm(IntervalSet* inputIntervalSet) {
  // Sort the intervals by their bottom value in descending order
  sortByBottom(inputIntervalSet);

  debug_print("The sorted input interval set is:\n");
  printIntervalSet(inputIntervalSet);
  debug_print("\n");

  const int n = inputIntervalSet->length;
  GraphNode graphNodes[n][n];
  GraphNode* graphNodesPtr = (GraphNode*)graphNodes;

  // Initialize empty graphNodes
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      *(getGraphNode(graphNodesPtr, i, j, n)) = createGraphNode(i, j);
    }
  }

  debug_print("Initializing base cases...\n");

  // initialize the base cases
  for (int i = 1; i <= n; i++) {
    AssignRes assignRes = assign(inputIntervalSet, i);
    if (assignRes.statusCode == SUCCESS) {
      addIntervalSet(getGraphNode(graphNodesPtr, i, i, n), assignRes.intervalSet);
      printGraphNodeDetailed(getGraphNode(graphNodesPtr, i, i, n));
    }
  }

  debug_print("\nGoing into the main loop...\n");

  // main loop of the algorithm
  for (int i = n; i > 0; i--) {
    for (int s = i + 1; s <= n; s++) {
      GraphNode* currNode = getGraphNode(graphNodesPtr, i, s, n);
      debug_print("\ncurrNode: ");
      printGraphNodeDetailed(currNode);
      const int s_ = s - i;
      for (int i_ = i; i_ <= s_; i_++) {
        GraphNode* predNode = getGraphNode(graphNodesPtr, i_, s_, n);

        printGraphNodeDetailed(predNode);

        // iterate over all the interval sets in the graph node
        IntervalSetNode* currIntSetNode = predNode->intervalSets;
        while (currIntSetNode) {
          IntervalSet* currSet = currIntSetNode->set;

          IntervalSet* lowestPart = getLowestPart(currSet);

          // try to build the current group
          AssignRes assignRes = assign(lowestPart, i);

          deleteIntervalSet(lowestPart);

          if (assignRes.statusCode == SUCCESS) {
            // if the assignment was successful, add the new interval set to the graph
            // node
            addIntervalSet(currNode, assignRes.intervalSet);
            push(&(assignRes.intervalSet->stack), currNode);

            printGraphNodeDetailed(currNode);
          } else if (assignRes.statusCode == ERROR_evtl) {
            debug_print("Assign returned ERROR_evtl.\n");

            Stack stackCopy = copyStack(currSet->stack);

            debug_print("Copied the stack.\n");

            backtrack(predNode, currNode, currSet, stackCopy);
          }

          currIntSetNode = currIntSetNode->next;
        }
      }

      removeDominatedSets(currNode);
    }
  }

  debug_print("\nChecking if there is a solution...\n");

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