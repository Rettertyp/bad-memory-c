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

IntervalSet* createBlankIntervalSet(Interval intervals[], int length);
void deleteIntervalSet(IntervalSet* intervalSet);
bool isDominatedBy(IntervalSet* thisSet, IntervalSet* otherSet);
void sortByBottom(IntervalSet* intervalSet);
void printIntervalSet(IntervalSet* intervalSet);
int countGreaterI(IntervalSet* intervalSet, int i);
int countContainingI(IntervalSet* intervalSet, int i);
IntervalSet* getWithoutFirstGIncludingI(IntervalSet* intervalSet, int i, int g);
IntervalSet* getLowestPart(IntervalSet* intervalSet);

#endif // INTERVAL_SET_H