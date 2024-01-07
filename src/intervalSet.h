#ifndef INTERVAL_SET_H
#define INTERVAL_SET_H

#include "interval.h"
#include "stack.h"
#include <stdint.h>

// forward declaration of the Stack
struct StackNode;
typedef struct StackNode* Stack;

/**
 * @brief Represents a set of intervals.
 *
 * The IntervalSet struct intervalContains a stack of GraphNodes, the length of the interval set,
 * and an array of Interval structs.
 */
typedef struct IntervalSet {
  Stack stack;          /** The stack of the set */
  uint32_t length;      /** The length of the interval set */
  Interval intervals[]; /** The array of Interval structs */
} IntervalSet;

IntervalSet* intervalSetCreateBlank(const Interval intervals[], const uint32_t length);
void intervalSetDelete(IntervalSet* intervalSet);
bool intervalSetIsDominatedBy(const IntervalSet* thisSet, const IntervalSet* otherSet);
void intervalSetSortByBottom(IntervalSet* intervalSet);
void intervalSetPrint(const IntervalSet* intervalSet);
uint32_t intervalSetCountGreaterI(const IntervalSet* intervalSet, const uint32_t i);
uint32_t intervalSetCountContainingI(const IntervalSet* intervalSet, const uint32_t i);
IntervalSet* intervalSetGetWithoutFirstGIncludingI(const IntervalSet* intervalSet, const uint32_t i,
                                                   uint32_t g);
IntervalSet* intervalSetGetLowestPart(IntervalSet* intervalSet);
uint32_t intervalSetCountLowestPartGreaterEqualJ(IntervalSet* intervalSet, const uint32_t j);
IntervalSet* intervalSetGetInverseLowestPartGreaterEqualJ(IntervalSet* intervalSet,
                                                          const uint32_t j);

#endif // INTERVAL_SET_H