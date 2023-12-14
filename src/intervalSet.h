#ifndef INTERVAL_SET_H
#define INTERVAL_SET_H

#include "interval.h"
#include "stack.h"

/**
 * @brief Represents a set of intervals.
 *
 * The IntervalSet struct contains a stack, which is used to store the
 * intervals, the length of the interval set, and an array of Interval structs.
 */
typedef struct IntervalSet {
  Stack stack;         /** The stack used to store the intervals */
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

#endif // INTERVAL_SET_H