#include "badMemoryAlg.h"
#include "debug.h"
#include "markStorage.h"
#include <stdint.h>
#include <stdlib.h>

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
AssignRes assign(const IntervalSet* intervalSet, const uint32_t groupSize) {
  if (intervalSetCountGreaterI(intervalSet, groupSize) > 0) {
    return (AssignRes){NULL, ERROR_defn};
  }

  if (intervalSetCountContainingI(intervalSet, groupSize) < groupSize) {
    return (AssignRes){NULL, ERROR_evtl};
  }

  debug_print("The group %d can be built.\n", groupSize);

  return (AssignRes){intervalSetGetWithoutFirstGIncludingI(intervalSet, groupSize, groupSize),
                     SUCCESS};
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
AssignRes assignRest(const IntervalSet* intervalSet, const uint32_t groupSize,
                     const uint32_t rest) {
  if (intervalSetCountGreaterI(intervalSet, groupSize) > 0) {
    return (AssignRes){NULL, ERROR_defn};
  }

  if (intervalSetCountContainingI(intervalSet, groupSize) < rest) {
    return (AssignRes){NULL, ERROR_evtl};
  }

  debug_print("The group %d can be built.\n", groupSize);

  return (AssignRes){intervalSetGetWithoutFirstGIncludingI(intervalSet, groupSize, rest), SUCCESS};
}

/**
 * The backtrack function of the bad memory algorithm.
 *
 * @param predNode The predecessor node of the current node.
 * @param currNode The current node.
 * @param intervalSet The current interval set.
 * @param currStack The current stack.
 */
void backtrack(GraphNode* predNode, GraphNode* currNode, IntervalSet* intervalSet, Stack otherStack,
               MarkStorage* markStorage) {
  Stack currStack = stackCopy(otherStack);

  // mark the current interval set
  markStorageAddSet(markStorage, intervalSet);

  int32_t nLowestPartGEqI = intervalSetCountLowestPartGreaterEqualJ(intervalSet, currNode->i);

  const uint32_t rest = currNode->s - predNode->s - nLowestPartGEqI;

  IntervalSet* inverseLowestPart =
      intervalSetGetInverseLowestPartGreaterEqualJ(intervalSet, currNode->i);

  AssignRes assignRes = assignRest(inverseLowestPart, currNode->i, rest);

  intervalSetDelete(inverseLowestPart);

  if (assignRes.statusCode == SUCCESS) {
    graphNodeAddIntervalSet(currNode, assignRes.intervalSet);

  } else if (assignRes.statusCode == ERROR_evtl) {

    GraphNode* nextPredNode = stackPop(&currStack);

    if (nextPredNode) {

      IntervalSetNode* currIntSetNode = nextPredNode->intervalSets;
      while (currIntSetNode) {
        IntervalSet* currSet = currIntSetNode->set;

        if (!markStorageIsMarked(markStorage, currSet) && stackEquals(currSet->stack, currStack)) {

          backtrack(nextPredNode, currNode, currSet, currStack, markStorage);
        }

        currIntSetNode = currIntSetNode->next;
      }
    }
  }

  stackDelete(&currStack);
}

/**
 * The main function of the bad memory algorithm.
 *
 * @param intervalSet The input IntervalSet to be processed.
 * @return Returns true if there is a solution, false otherwise.
 */
bool badMemoryAlgorithm(IntervalSet* inputIntervalSet) {
  // Sort the intervals by their bottom value in descending order
  intervalSetSortByBottom(inputIntervalSet);

  debug_print("The sorted input interval set is:\n");
  intervalSetPrint(inputIntervalSet);
  debug_print("\n");

  const uint32_t n = inputIntervalSet->length;

  // Initialize the graph nodes on the heap
  GraphNode** graphNodes = malloc(n * sizeof(GraphNode*));
  for (uint32_t i = 0; i < n; i++) {
    graphNodes[i] = malloc(n * sizeof(GraphNode));
  }

  // populate the graph nodes
  for (uint32_t i = 1; i <= n; i++) {
    for (uint32_t j = 1; j <= n; j++) {
      *(getGraphNode(graphNodes, i, j)) = graphNodeCreate(i, j);
    }
  }

  debug_print("Initializing base cases...\n");

  // initialize the base cases
  for (uint32_t i = 1; i <= n; i++) {
    AssignRes assignRes = assign(inputIntervalSet, i);
    if (assignRes.statusCode == SUCCESS) {
      GraphNode* currNode = getGraphNode(graphNodes, i, i);
      graphNodeAddIntervalSet(currNode, assignRes.intervalSet);
      graphNodePrintDetailed(currNode);
    }
  }

  debug_print("\nGoing into the main loop...\n");

  // main loop of the algorithm
  for (uint32_t i = n; i > 0; i--) {
    for (uint32_t s = i + 1; s <= n; s++) {
      GraphNode* currNode = getGraphNode(graphNodes, i, s);
      debug_print("\ncurrNode: ");
      graphNodePrintDetailed(currNode);

      // create a mark storage
      MarkStorage markStorage = NULL;

      const uint32_t s_ = s - i;
      for (uint32_t i_ = i; i_ <= n; i_++) {
        GraphNode* predNode = getGraphNode(graphNodes, i_, s_);

        graphNodePrintDetailed(predNode);

        // iterate over all the interval sets in the graph node
        IntervalSetNode* currIntSetNode = predNode->intervalSets;
        while (currIntSetNode) {
          IntervalSet* currSet = currIntSetNode->set;

          IntervalSet* lowestPart = intervalSetGetLowestPart(currSet);

          // try to build the current group
          AssignRes assignRes = assign(lowestPart, i);

          intervalSetDelete(lowestPart);

          if (assignRes.statusCode == SUCCESS) {
            // if the assignment was successful, add the new interval set to the graph
            // node
            graphNodeAddIntervalSet(currNode, assignRes.intervalSet);
            stackPush(&(assignRes.intervalSet->stack), predNode);

            graphNodePrintDetailed(currNode);

          } else if (assignRes.statusCode == ERROR_evtl) {

            backtrack(predNode, currNode, currSet, currSet->stack, &markStorage);
          }

          currIntSetNode = currIntSetNode->next;
        }
      }

      graphNodeRemoveDominatedSets(currNode);

      // free the memory allocated for the mark storage
      markStorageDelete(&markStorage);
    }
  }

  debug_print("\nChecking if there is a solution...\n");

  bool solutionFound = false;

  // check if there is a solution
  for (uint32_t i = 1; i <= n; i++) {
    GraphNode* currNode = getGraphNode(graphNodes, i, n);
    if (graphNodeGetNIntervalSets(currNode) > 0) {
      debug_print("Solution found!\n");
      graphNodePrintDetailed(currNode);
      solutionFound = true;
      break;
    }
  }

  // free the memory allocated for the graph nodes and their corresponding intervalSets
  for (uint32_t i = 1; i <= n; i++) {
    for (uint32_t j = 1; j <= n; j++) {
      graphNodeDelete(getGraphNode(graphNodes, i, j));
    }

    free(graphNodes[i - 1]);
    graphNodes[i - 1] = NULL;
  }

  free(graphNodes);
  graphNodes = NULL;

  return solutionFound;
}