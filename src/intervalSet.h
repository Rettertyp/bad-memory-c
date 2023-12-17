#ifndef INTERVAL_SET_H
#define INTERVAL_SET_H

#include "interval.h"
#include "stack.h"

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
  Stack stack;         /** The stack of the set */
  int length;          /** The length of the interval set */
  Interval* intervals; /** The array of Interval structs */
} IntervalSet;

IntervalSet* intervalSetCreateBlank(const Interval intervals[], const int length);
void intervalSetDelete(IntervalSet* intervalSet);
bool intervalSetIsDominatedBy(const IntervalSet* thisSet, const IntervalSet* otherSet);
void intervalSetSortByBottom(IntervalSet* intervalSet);
void intervalSetPrint(const IntervalSet* intervalSet);
int intervalSetCountGreaterI(const IntervalSet* intervalSet, const int i);
int intervalSetCountContainingI(const IntervalSet* intervalSet, const int i);
IntervalSet* intervalSetGetWithoutFirstGIncludingI(const IntervalSet* intervalSet, const int i,
                                                   int g);
IntervalSet* intervalSetGetLowestPart(const IntervalSet* intervalSet);
int intervalSetCountLowestPartGreaterEqualJ(const IntervalSet* intervalSet, const int j);
IntervalSet* intervalSetGetInverseLowestPartGreaterEqualJ(const IntervalSet* intervalSet,
                                                          const int j);

#endif // INTERVAL_SET_H