#ifndef BAD_MEMORY_ALG_H
#define BAD_MEMORY_ALG_H

#include "graphNode.h"
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

bool badMemoryAlgorithm(IntervalSet* inputIntervalSet, const bool printExtraInfo);
bool badMemoryDepthFirst(IntervalSet* inputIntervalSet, const bool printExtraInfo);

#endif // BAD_MEMORY_ALG_H