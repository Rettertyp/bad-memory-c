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
 * The IntervalSet struct contains a stack of GraphNodes, the length of the interval set,
 * and an array of Interval structs.
 */
typedef struct IntervalSet {
  Stack stack;         /** The stack of the set */
  int length;          /** The length of the interval set */
  Interval* intervals; /** The array of Interval structs */
} IntervalSet;

IntervalSet* createBlankIntervalSet(const Interval intervals[], const int length);
void deleteIntervalSet(IntervalSet* intervalSet);
bool isDominatedBy(const IntervalSet* thisSet, const IntervalSet* otherSet);
void sortByBottom(IntervalSet* intervalSet);
void printIntervalSet(const IntervalSet* intervalSet);
int countGreaterI(const IntervalSet* intervalSet, const int i);
int countContainingI(const IntervalSet* intervalSet, const int i);
IntervalSet* getWithoutFirstGIncludingI(const IntervalSet* intervalSet, const int i,
                                        int g);
IntervalSet* getLowestPart(const IntervalSet* intervalSet);

#endif // INTERVAL_SET_H