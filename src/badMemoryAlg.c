#include "badMemoryAlg.h"
// #define DEBUG_PRINT 1
#include "debug.h"
#include "jsonPrinter.h"
#include "markStorage.h"
#include <stdint.h>
#include <stdlib.h>

/**
 * @brief Represents the status of an assignment operation.
 */
typedef enum StatusCode {
  SUCCESS = 0,    /** The assignment was successful. */
  ERROR_evtl = 1, /** There were insufficiently many intervals to build the group. */
  ERROR_defn = 2  /** The resulting set is not a valid part-solution.*/
} StatusCode;

/**
 * @brief Represents the result of an assignment operation.
 *
 * This struct intervalContains a pointer to an IntervalSet and an error code.
 * The IntervalSet represents a set of intervals, while the error code
 * indicates the success or failure of the assignment operation.
 */
typedef struct AssignRes {
  IntervalSet* intervalSet; /** Pointer to the IntervalSet */
  StatusCode statusCode;    /** Status code indicating the success or failure of the
                     assignment operation */
} AssignRes;

uint32_t nGroupsBuilt = 0;
uint32_t nGroupsKept = 0;
uint32_t nSteps = 0;

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
static AssignRes assign(const IntervalSet* intervalSet, const uint32_t groupSize) {
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
static AssignRes assignRest(const IntervalSet* intervalSet, const uint32_t groupSize,
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
 * @param markStorage The mark storage of the current graph node.
 * @param directPredNode The direct predecessor node of the current node, used for in/out edges.
 */
static void backtrack(GraphNode* predNode, GraphNode* currNode, IntervalSet* intervalSet,
                      Stack otherStack, GraphNode* directPredNode) {
  nSteps++;
  Stack currStack = stackCopy(otherStack);

  // mark the current interval set
  markStorageAddSet(&(currNode->markStorage), intervalSet);

  const uint32_t nLowestPartGEqI =
      intervalSetCountLowestPartGreaterEqualJ(intervalSet, currNode->i);

  const uint32_t rest = currNode->s - predNode->s - nLowestPartGEqI;

  IntervalSet* inverseLowestPart =
      intervalSetGetInverseLowestPartGreaterEqualJ(intervalSet, currNode->i);

  AssignRes assignRes = assignRest(inverseLowestPart, currNode->i, rest);

  intervalSetDelete(inverseLowestPart);

  switch (assignRes.statusCode) {
  case SUCCESS:
    graphNodeAddIntervalSet(currNode, assignRes.intervalSet);
    graphNodeStorageConnectNodes(directPredNode, currNode);
    graphNodePrintDetailed(currNode);
    nGroupsBuilt++;
    break;

  case ERROR_evtl:
    GraphNode* nextPredNode = stackPop(&currStack);

    if (nextPredNode) {
      IntervalSetNode* currIntSetNode = nextPredNode->intervalSets;

      while (currIntSetNode) {
        IntervalSet* currSet = currIntSetNode->set;

        if (!markStorageIsMarked(&(currNode->markStorage), currSet) &&
            stackEquals(currSet->stack, currStack)) {
          backtrack(nextPredNode, currNode, currSet, currStack, directPredNode);
        }

        currIntSetNode = currIntSetNode->next;
      }
    }
    break;

  default:
    break;
  }

  stackDelete(&currStack);
}

/**
 * Initializes the graph nodes and the base cases of the bad memory algorithm. Resets the metrics.
 *
 * @param inputIntervalSet The input IntervalSet to be processed.
 * @param n The number of graph nodes.
 * @return Returns a pointer to the array of graph nodes.
 */
static GraphNode** initializeGraphNodes(IntervalSet* inputIntervalSet, const uint32_t n) {
  // reset the metrics
  nGroupsBuilt = 0;
  nGroupsKept = 0;
  nSteps = 0;

  // Sort the intervals by their bottom value in descending order
  intervalSetSortByBottom(inputIntervalSet);

  debug_print("The sorted input interval set is:\n");
  intervalSetPrint(inputIntervalSet);
  debug_print("\n");

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
      nSteps++;
      nGroupsBuilt++;
    }
  }

  debug_print("\nGoing into the main loop...\n");

  return graphNodes;
}

/**
 * Frees the memory allocated for the graph nodes and their corresponding intervalSets.
 *
 * @param graphNodes The graph nodes to be freed.
 * @param n The number of graph nodes.
 */
static void freeGraphNodes(GraphNode** graphNodes, const uint32_t n) {
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
}

/**
 * Prints the metrics of the bad memory algorithm.
 */
static RunInfo computeMetrics(GraphNode** graphNodes, const uint32_t n, bool solutionFound,
                              char* description) {
  uint32_t nSolutions = 0;
  uint32_t nUsedNodes = 0;
  uint32_t nOutgoingEdges = 0;
  uint32_t nIncomingEdges = 0;
  uint32_t maxOutgoingEdges = 0;
  uint32_t maxIncomingEdges = 0;
  uint32_t nMarkedSets = 0;
  uint32_t maxSetsPerNode = 0;
  uint32_t minSetsPerNode = UINT32_MAX;

  // count the number of solutions
  for (uint32_t i = 1; i <= n; i++) {
    GraphNode* currNode = getGraphNode(graphNodes, i, n);
    nSolutions += graphNodeGetNIntervalSets(currNode);
  }

  // count the number of groups kept
  for (uint32_t i = 1; i <= n; i++) {
    for (uint32_t j = 1; j <= n; j++) {
      GraphNode* currNode = getGraphNode(graphNodes, i, j);
      uint32_t nIntervalSets = graphNodeGetNIntervalSets(currNode);
      nGroupsKept += nIntervalSets;
      if (nIntervalSets > 0) {
        nUsedNodes++;
      }

      // count the number of outgoing edges
      uint32_t nOutgoingEdgesCurr = graphNodeStorageGetNNodes(&(currNode->outgoing));
      nOutgoingEdges += nOutgoingEdgesCurr;
      maxOutgoingEdges = __max(maxOutgoingEdges, nOutgoingEdgesCurr);

      // count the number of incoming edges
      uint32_t nIncomingEdgesCurr = graphNodeStorageGetNNodes(&(currNode->incoming));
      nIncomingEdges += nIncomingEdgesCurr;
      maxIncomingEdges = __max(maxIncomingEdges, nIncomingEdgesCurr);

      // count the number of marked sets
      nMarkedSets += markStorageCount(&(currNode->markStorage));

      // count the number of sets per node
      maxSetsPerNode = __max(maxSetsPerNode, nIntervalSets);
      minSetsPerNode = __min(minSetsPerNode, nIntervalSets);
    }
  }

  printf("\nnGroupsBuilt: %d\n", nGroupsBuilt);
  printf("nGroupsKept: %d\n", nGroupsKept);
  printf("nSolutions: %d\n", nSolutions);
  printf("nSteps: %d\n", nSteps);
  printf("nUsedNodes: %d\n", nUsedNodes);
  printf("Average outgoing edges: %.2f\n", (float)nOutgoingEdges / nUsedNodes);
  printf("Average incoming edges: %.2f\n", (float)nIncomingEdges / nUsedNodes);
  printf("Max outgoing edges: %d\n", maxOutgoingEdges);
  printf("Max incoming edges: %d\n", maxIncomingEdges);
  printf("nEdges: %d\n", nOutgoingEdges);
  printf("nMarkedSets: %d\n", nMarkedSets);
  printf("Max sets per node: %d\n", maxSetsPerNode);
  printf("Min sets per node: %d\n", minSetsPerNode);

  return (RunInfo){description,
                   solutionFound,
                   n,
                   nGroupsBuilt,
                   nGroupsKept,
                   nSolutions,
                   nSteps,
                   nUsedNodes,
                   nOutgoingEdges,
                   nIncomingEdges,
                   maxOutgoingEdges,
                   maxIncomingEdges,
                   nOutgoingEdges,
                   nMarkedSets,
                   maxSetsPerNode,
                   minSetsPerNode,
                   0,
                   NULL};
}

/**
 * The main function of the bad memory algorithm.
 *
 * @param intervalSet The input IntervalSet to be processed.
 * @return Returns true if there is a solution, false otherwise.
 */
RunInfo badMemoryAlgorithm(IntervalSet* inputIntervalSet, char* description) {
  const uint32_t n = intervalSetCountIntervals(inputIntervalSet);
  GraphNode** graphNodes = initializeGraphNodes(inputIntervalSet, n);

  // main loop of the algorithm
  for (uint32_t i = n; i > 0; i--) {
    for (uint32_t s = i + 1; s <= n; s++) {
      GraphNode* currNode = getGraphNode(graphNodes, i, s);
      debug_print("\ncurrNode: ");
      graphNodePrintDetailed(currNode);

      const uint32_t s_ = s - i;
      for (uint32_t i_ = i; i_ <= n; i_++) {
        GraphNode* predNode = getGraphNode(graphNodes, i_, s_);

        graphNodePrintDetailed(predNode);

        // iterate over all the interval sets in the graph node
        IntervalSetNode* currIntSetNode = predNode->intervalSets;
        while (currIntSetNode) {
          nSteps++;
          IntervalSet* currSet = currIntSetNode->set;

          IntervalSet* lowestPart = intervalSetGetLowestPart(currSet);

          // try to build the current group
          AssignRes assignRes = assign(lowestPart, i);

          intervalSetDelete(lowestPart);

          switch (assignRes.statusCode) {
          case SUCCESS:
            // if the assignment was successful, add the new interval set to the graph
            // node
            graphNodeAddIntervalSet(currNode, assignRes.intervalSet);
            graphNodeStorageConnectNodes(predNode, currNode);
            stackPush(&(assignRes.intervalSet->stack), predNode);

            graphNodePrintDetailed(currNode);
            nGroupsBuilt++;
            break;

          case ERROR_evtl:
            backtrack(predNode, currNode, currSet, currSet->stack, predNode);
            break;

          default:
            break;
          }

          currIntSetNode = currIntSetNode->next;
        }
      }

      graphNodeRemoveDominatedSets(currNode);
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

  RunInfo runInfo = computeMetrics(graphNodes, n, solutionFound, description);

  freeGraphNodes(graphNodes, n);

  return runInfo;
}

static bool buildSetsDepthFirstRecursive(GraphNode** graphNodes, const uint32_t n,
                                         GraphNode* predNode, IntervalSet* currSet);

/**
 * Checks if the rest set is dominated by one of the other sets in its graph node. If not, adds it
 * and recursively calls the function on it.
 *
 * @param graphNodes The graph nodes to be processed.
 * @param n The size of each dimension of the graph.
 * @param i The i-value of the current graph node.
 * @param s The s-value of the current graph node.
 * @param currNode The current graph node.
 * @param currSet The current interval set.
 * @return Returns true if a solution has been found, false otherwise.
 */
static bool buildAndCallRecursive(GraphNode** graphNodes, const uint32_t n, GraphNode* currNode,
                                  GraphNode* predNode, IntervalSet* currSet) {
  nGroupsBuilt++;
  // check if the rest set is dominated by one of the other sets in its graph node. If not, add
  // it and recursively call the function on it
  if (graphNodeSetShouldBeAdded(currNode, currSet)) {
    graphNodeAddIntervalSet(currNode, currSet);
    graphNodeStorageConnectNodes(predNode, currNode);
    graphNodePrintDetailed(currNode);

    // recursively call the function
    return buildSetsDepthFirstRecursive(graphNodes, n, currNode, currSet);
  } else {
    intervalSetDelete(currSet);
    return false;
  }
}

/**
 * Backtrack function of the depth-first variant of the bad memory algorithm.
 *
 * @param graphNodes The graph nodes to be processed.
 * @param predNode The predecessor node of the current node.
 * @param currNode The current node.
 * @param intervalSet The current interval set.
 * @param otherStack The current stack.
 * @param n The size of each dimension of the graph.
 * @return Returns true if a solution has been found, false otherwise.
 */
static bool backtrackDepthFirst(GraphNode** graphNodes, GraphNode* predNode, GraphNode* currNode,
                                IntervalSet* intervalSet, Stack otherStack, const uint32_t n,
                                GraphNode* directPredNode) {
  nSteps++;
  Stack currStack = stackCopy(otherStack);

  // mark the current interval set in the mark storage of the current graph node
  markStorageAddSet(&(currNode->markStorage), intervalSet);

  const uint32_t nLowestPartGEqI =
      intervalSetCountLowestPartGreaterEqualJ(intervalSet, currNode->i);

  const uint32_t rest = currNode->s - predNode->s - nLowestPartGEqI;

  IntervalSet* inverseLowestPart =
      intervalSetGetInverseLowestPartGreaterEqualJ(intervalSet, currNode->i);

  AssignRes assignRes = assignRest(inverseLowestPart, currNode->i, rest);

  intervalSetDelete(inverseLowestPart);

  bool solutionFound = false;

  switch (assignRes.statusCode) {
  case SUCCESS:
    solutionFound &=
        buildAndCallRecursive(graphNodes, n, currNode, directPredNode, assignRes.intervalSet);
    break;

  case ERROR_evtl:
    GraphNode* nextPredNode = stackPop(&currStack);

    if (nextPredNode) {
      IntervalSetNode* currIntSetNode = nextPredNode->intervalSets;

      while (currIntSetNode) {
        IntervalSet* currSet = currIntSetNode->set;

        if (!markStorageIsMarked(&(currNode->markStorage), currSet) &&
            stackEquals(currSet->stack, currStack) &&
            backtrackDepthFirst(graphNodes, nextPredNode, currNode, currSet, currStack, n,
                                directPredNode)) {
          solutionFound = true;
          break;
        }

        currIntSetNode = currIntSetNode->next;
      }
    }
    break;

  default:
    break;
  }

  stackDelete(&currStack);
  return solutionFound;
}

/**
 * Tries to recursively build all possible groups of intervals. Main function of the depth-first
 * variant of the bad memory algorithm.
 *
 * @param graphNodes The graph nodes to be processed.
 * @param n The number of graph nodes.
 * @param i The i-value of the current graph node.
 * @param s The s-value of the current graph node.
 * @param currSet The current interval set.
 * @return Returns true if a solution has been found, false otherwise.
 */
static bool buildSetsDepthFirstRecursive(GraphNode** graphNodes, const uint32_t n,
                                         GraphNode* predNode, IntervalSet* currSet) {
  const uint32_t i = predNode->i;
  const uint32_t s = predNode->s;

  // if a solution has been found, return true
  if (s == n) {
    return true;
  }

  bool solutionFound = false;

  // try to build every possible group
  for (uint32_t j = __min(i, n - s); j > 0; j--) {
    nSteps++;

    GraphNode* currNode = getGraphNode(graphNodes, j, s + j);

    // try to build the group of size j
    IntervalSet* lowestPart = intervalSetGetLowestPart(currSet);
    AssignRes assignRes = assign(lowestPart, j);
    intervalSetDelete(lowestPart);

    switch (assignRes.statusCode) {
    case SUCCESS:
      solutionFound |=
          buildAndCallRecursive(graphNodes, n, currNode, predNode, assignRes.intervalSet);
      break;

    case ERROR_evtl:
      solutionFound |=
          backtrackDepthFirst(graphNodes, predNode, currNode, currSet, currSet->stack, n, predNode);
      break;

    default:
      break;
    }

    if (solutionFound) {
      break;
    }
  }

  return solutionFound;
}

/**
 * Alternative implementation of the bad memory algorithm using a depth-first approach.
 *
 * @param inputIntervalSet The input IntervalSet to be processed.
 * @param printAllMetrics If true, prints all the metrics of the algorithm.
 * @return Returns true if there is a solution, false otherwise.
 */
RunInfo badMemoryDepthFirst(IntervalSet* inputIntervalSet, char* description) {
  const uint32_t n = intervalSetCountIntervals(inputIntervalSet);
  GraphNode** graphNodes = initializeGraphNodes(inputIntervalSet, n);

  bool solutionFound = false;
  // main loop of the algorithm
  for (uint32_t i = n; i >= 1; i--) {
    GraphNode* currNode = getGraphNode(graphNodes, i, i);
    IntervalSetNode* currIntSetNode = currNode->intervalSets;
    if (currIntSetNode &&
        buildSetsDepthFirstRecursive(graphNodes, n, currNode, currIntSetNode->set)) {
      solutionFound = true;
      break;
    }
  }

  RunInfo runInfo = computeMetrics(graphNodes, n, solutionFound, description);

  freeGraphNodes(graphNodes, n);

  return runInfo;
}