#ifndef BAD_MEMORY_ALG_H
#define BAD_MEMORY_ALG_H

#include "graphNode.h"

/**
 * @brief Represents the result of an assignment operation.
 *
 * This struct contains a pointer to an IntervalSet and an error code.
 * The IntervalSet represents a set of intervals, while the error code
 * indicates the success or failure of the assignment operation.
 */
typedef struct AssignRes {
  IntervalSet* intervalSet; /** Pointer to the IntervalSet */
  int statusCode;           /** Status code indicating the success or failure of the
                            assignment operation */
} AssignRes;

bool badMemoryAlgorithm(IntervalSet* intervalSet);
AssignRes assign(IntervalSet* intervalSet, int groupSize);

#endif // BAD_MEMORY_ALG_H