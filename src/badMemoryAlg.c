#include "badMemoryAlg.h"

/**
 * @brief Represents the result of an assignment operation.
 *
 * This struct contains a pointer to an IntervalSet and an error code.
 * The IntervalSet represents a set of intervals, while the error code
 * indicates the success or failure of the assignment operation.
 */
typedef struct AssignRes {
  IntervalSet* intervalSet; /** Pointer to the IntervalSet */
  int errorCode; /** Error code indicating the success or failure of the
                 assignment operation */
} AssignRes;

/**
 * The main function of the bad memory algorithm.
 *
 * @param intervalSet The input IntervalSet to be processed.
 * @return Returns true if there is a solution, false otherwise.
 */
bool badMemoryAlgorithm(IntervalSet* intervalSet) {
  int n = intervalSet->length;
  GraphNode graphNodes[n][n];

  // Initialize empty graphNodes
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      graphNodes[i][j] = createGraphNode(i, j);
    }
  }

  // initialize the base cases
  for (int i = 0; i < n; i++) {
    // #TODO
  }
}

/**
 * Tries to build the group of size groupSize. If it is not possible to build
 * the group, returns NULL and sets the errorCode field of the AssignRes struct.
 *
 * @param intervalSet The input IntervalSet to be processed.
 * @param groupSize The number of intervals to be assigned to a single
 * processor.
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
}