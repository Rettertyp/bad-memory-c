#include "intervalSet.h"
#include "debug.h"
#include <stdlib.h>

/**
 * Function pointer type for Interval comparison functions.
 */
typedef bool (*ComparisonFunc)(const Interval*, const unsigned int);

/**
 * Creates a new IntervalSet object.
 *
 * @param intervals An array of Interval objects.
 * @param length The length of the intervals array.
 * @param stack The Stack object associated with the IntervalSet.
 * @return A pointer to the newly created IntervalSet object, or NULL if memory allocation
 * fails.
 */
static IntervalSet* intervalSetCreate(const Interval intervals[], const unsigned int length,
                                      const Stack stack) {
  IntervalSet* intervalSet = malloc(sizeof(IntervalSet) + sizeof(Interval) * (length));

  if (intervalSet) {
    intervalSet->stack = stackCopy(stack);
    intervalSet->length = length;
    // set the intervals pointer one cell after the IntervalSet struct
    intervalSet->intervals = (Interval*)(intervalSet + 1);

    // copy the intervals into the new array

    for (unsigned int i = 0; i < length; i++) {
      intervalSet->intervals[i] = intervals[i];
    }
  }

  return intervalSet;
}

/**
 * Creates a new IntervalSet object.
 *
 * @param intervals An array of Interval objects.
 * @param length The length of the intervals array.
 * @return A pointer to the newly created IntervalSet object, or NULL if memory
 * allocation fails.
 */
IntervalSet* intervalSetCreateBlank(const Interval intervals[], const unsigned int length) {
  return intervalSetCreate(intervals, length, NULL);
}

/**
 * Creates a copy of the given IntervalSet.
 *
 * @param intervalSet The IntervalSet to be copied.
 * @return A new IntervalSet that is a copy of the original IntervalSet.
 */
IntervalSet* intervalSetCopy(const IntervalSet* intervalSet) {
  return intervalSetCreate(intervalSet->intervals, intervalSet->length, intervalSet->stack);
}

/**
 * Deletes the specified IntervalSet and frees the allocated memory.
 *
 * @param intervalSet A pointer to the IntervalSet to be deleted.
 */
void intervalSetDelete(IntervalSet* intervalSet) {
  if (intervalSet) {
    stackDelete(&(intervalSet->stack));
    free(intervalSet);
    intervalSet = NULL;
  }
}

/**
 * Determines whether the given IntervalSet is dominated by another IntervalSet.
 *
 * @param thisSet   The IntervalSet to be checked.
 * @param otherSet  The IntervalSet to compare against.
 * @return          Returns true if thisSet is dominated by otherSet, false
 * otherwise.
 */
bool intervalSetIsDominatedBy(const IntervalSet* thisSet, const IntervalSet* otherSet) {
  if (thisSet->length != otherSet->length) {
    return false;
  }

  for (unsigned int i = 0; i < thisSet->length; i++) {
    if (!(otherSet->intervals[i].bottom <= thisSet->intervals[i].bottom)) {
      return false;
    }
  }

  return true;
}

/**
 * Swaps two Interval objects.
 *
 * @param a Pointer to the first Interval object.
 * @param b Pointer to the second Interval object.
 */
static void swap(Interval* a, Interval* b) {
  Interval tmp = *a;
  *a = *b;
  *b = tmp;
}

/**
 * Performs heapify operation on an array of Interval objects.
 *
 * @param intervals The array of Interval objects.
 * @param n The size of the array.
 * @param i The index of the current element.
 */
static void heapify(Interval intervals[], const unsigned int n, const unsigned int i) {
  unsigned int smallest = i;
  unsigned int left = 2 * i + 1;
  unsigned int right = 2 * i + 2;

  // check if one of the two successors is bigger
  if (left < n && intervals[left].bottom < intervals[smallest].bottom) {
    smallest = left;
  }

  if (right < n && intervals[right].bottom < intervals[smallest].bottom) {
    smallest = right;
  }

  // if the left or right was bigger
  if (smallest != i) {
    swap(&intervals[i], &intervals[smallest]);
    heapify(intervals, n, smallest);
  }
}

/**
 * Sorts an array of Interval structs using the heap sort algorithm.
 *
 * @param intervals The array of Interval structs to be sorted.
 * @param n The number of elements in the array.
 */
static void heapSort(Interval intervals[], const unsigned int n) {
  // go over all nodes that are not leaves (turns array into heap)
  for (int i = n / 2 - 1; i >= 0; i--) {
    heapify(intervals, n, i);
  }

  // successively remove all leaves from back to front
  for (int i = n - 1; i >= 0; i--) {
    swap(&intervals[0], &intervals[i]);
    heapify(intervals, i, 0);
  }
}

/**
 * Sorts the intervals in the given IntervalSet by their bottom values in descending
 * order.
 *
 * @param intervalSet The IntervalSet to be sorted.
 */
void intervalSetSortByBottom(IntervalSet* intervalSet) {
  heapSort(intervalSet->intervals, intervalSet->length);
}

/**
 * Prints the intervals in the given IntervalSet.
 *
 * @param intervalSet The IntervalSet to be printed.
 */
void intervalSetPrint(const IntervalSet* intervalSet) {
  if (intervalSet->length == 0) {
    debug_print("()");
  } else
    for (unsigned int i = 0; i < intervalSet->length; i++) {
      debug_print("(%d, %d), ", intervalSet->intervals[i].bottom, intervalSet->intervals[i].top);
    }

  debug_print("\n");
}

/**
 * Counts the number of intervals in the given IntervalSet that satisfy the comparison function.
 *
 * @param intervalSet The IntervalSet to count intervals from.
 * @param i The value to compare intervals with.
 * @param compFunc The comparison function used to compare intervals with the value.
 * @return The number of intervals that satisfy the comparison function.
 */
static unsigned int intervalSetCount(const IntervalSet* intervalSet, const unsigned int i,
                                     ComparisonFunc compFunc) {
  unsigned int count = 0;

  for (unsigned int j = 0; j < intervalSet->length; j++) {
    if (compFunc(&(intervalSet->intervals[j]), i)) {
      count++;
    }
  }

  return count;
}

/**
 * Counts the number of intervals in the given IntervalSet that are greater than
 * the specified value.
 *
 * @param intervalSet The IntervalSet to search in.
 * @param i The value to compare against.
 * @return The count of intervals greater than the specified value.
 */
unsigned int intervalSetCountGreaterI(const IntervalSet* intervalSet, const unsigned int i) {
  return intervalSetCount(intervalSet, i, intervalGreaterThan);
}

/**
 * Counts the number of intervals in the given IntervalSet that contain the
 * specified value.
 *
 * @param intervalSet The IntervalSet to search in.
 * @param i The value to check for containment.
 * @return The count of intervals containing the specified value.
 */
unsigned int intervalSetCountContainingI(const IntervalSet* intervalSet, const unsigned int i) {
  return intervalSetCount(intervalSet, i, intervalContains);
}

/**
 * Counts the number of intervals in the given IntervalSet that are greater than or equal to the
 * specified value.
 *
 * @param intervalSet The IntervalSet to search in.
 * @param i The value to compare against the intervals.
 * @return The count of intervals that are greater than or equal to the specified value.
 */
static unsigned int intervalSetCountGreaterEqualI(const IntervalSet* intervalSet,
                                                  const unsigned int i) {
  return intervalSetCount(intervalSet, i, intervalGreaterEqual);
}

/**
 * Returns the first interval in the given interval set that contains the specified value.
 *
 * @param intervalSet The interval set to search in.
 * @param i The value to search for.
 * @return A pointer to the first interval that contains the specified value, or NULL if
 * no such interval is found.
 */
static Interval* intervalSetGetFirstContainingI(const IntervalSet* intervalSet,
                                                const unsigned int i) {
  for (unsigned int j = 0; j < intervalSet->length; j++) {
    if (intervalContains(&(intervalSet->intervals[j]), i)) {
      return &(intervalSet->intervals[j]);
    }
  }

  return NULL;
}

/**
 * Removes the first 'g' intervals that include 'i' from the given 'intervalSet' and
 * returns a new IntervalSet.
 *
 * @param intervalSet The original IntervalSet from which intervals will be removed.
 * @param i The number the intervals must include.
 * @param g The number of intervals to be removed from the beginning of the IntervalSet.
 * @return A new IntervalSet without the first 'g' intervals that include 'i'.
 */
IntervalSet* intervalSetGetWithoutFirstGIncludingI(const IntervalSet* intervalSet,
                                                   const unsigned int i, const unsigned int g) {
  unsigned int newLength = intervalSet->length - g;
  Interval intervals[newLength];

  unsigned int j = 0;
  unsigned int nAssigned = 0;
  for (unsigned int k = 0; k < intervalSet->length; k++) {
    if (nAssigned < g && intervalContains(&(intervalSet->intervals[k]), i)) {
      nAssigned++;
    } else {
      intervals[j++] = intervalSet->intervals[k];
    }
  }

  return intervalSetCreate(intervals, newLength, intervalSet->stack);
}

/**
 * Retrieves the intervals from the given intervalSet that are right of i and have a
 * bottom value greater than or equal to b.
 *
 * @param intervalSet The intervalSet to retrieve the intervals from.
 * @param i The value the intervals must be right of.
 * @param b The value the intervals must have a greater or equal bottom value than.
 * @return A new IntervalSet containing the retrieved intervals.
 */
static IntervalSet* getLessThanIRightOfB(const IntervalSet* intervalSet, const unsigned int i,
                                         const unsigned int b) {
  // choosing intervalSet.length as upper bound
  Interval intervals[intervalSet->length];

  unsigned int nChosen = 0;
  for (unsigned int j = 0; j < intervalSet->length; j++) {
    const Interval* currInterval = &(intervalSet->intervals[j]);
    if (intervalLessThan(currInterval, i) && currInterval->bottom >= b) {
      intervals[nChosen++] = *currInterval;
    }
  }

  return intervalSetCreate(intervals, nChosen, intervalSet->stack);
}

/**
 * Retrieves the lowest part of the given interval set.
 *
 * @param intervalSet The interval set to retrieve the lowest part from.
 * @return The lowest part of the interval set.
 */
IntervalSet* intervalSetGetLowestPart(const IntervalSet* intervalSet) {
  GraphNode* predNode = stackTop(&(intervalSet->stack));

  // if there is no predecessor, return a copy of the interval set
  if (!predNode) {
    return intervalSetCopy(intervalSet);
  }

  // check if there is an Interval that intervalContains i
  Interval* interval = intervalSetGetFirstContainingI(intervalSet, predNode->i);

  return getLessThanIRightOfB(intervalSet, predNode->i, interval ? interval->bottom : 0);
}

/**
 * Counts the number of intervals in the given `intervalSet` that satisfy the following conditions:
 * - The interval's index is less than `i`.
 * - The interval's bottom value is greater than or equal to `b`.
 * - The interval's index is greater than or equal to `j`.
 *
 * @param intervalSet The interval set to search in.
 * @param i           The upper limit index.
 * @param b           The lower limit bottom value.
 * @param j           The lower limit index.
 * @return            The count of intervals that meet the specified conditions.
 */
static unsigned int countLessThanIRightOfBGreaterEqualJ(const IntervalSet* intervalSet,
                                                        const unsigned int i, const unsigned int b,
                                                        const unsigned int j) {
  unsigned int nChosen = 0;
  for (unsigned int k = 0; k < intervalSet->length; k++) {
    const Interval* currInterval = &(intervalSet->intervals[k]);
    if (intervalLessThan(currInterval, i) && currInterval->bottom >= b &&
        intervalGreaterEqual(currInterval, j)) {
      nChosen++;
    }
  }

  return nChosen;
}

/**
 * Retrieves the lowest part of the IntervalSet that is greater than or equal to a given value.
 *
 * @param intervalSet The IntervalSet to search in.
 * @param j The value to compare against.
 * @return A pointer to the IntervalSet containing the lowest part greater than or equal to j.
 *         If no such IntervalSet exists, a copy of the original IntervalSet is returned.
 */
unsigned int intervalSetCountLowestPartGreaterEqualJ(const IntervalSet* intervalSet,
                                                     const unsigned int j) {
  GraphNode* predNode = stackTop(&(intervalSet->stack));

  // if there is no predecessor, return a copy of the interval set
  if (!predNode) {
    return intervalSetCountGreaterEqualI(intervalSet, j);
  }

  // check if there is an Interval that intervalContains i
  Interval* interval = intervalSetGetFirstContainingI(intervalSet, predNode->i);

  return countLessThanIRightOfBGreaterEqualJ(intervalSet, predNode->i,
                                             interval ? interval->bottom : 0, j);
}

/**
 * Retrieves a new IntervalSet containing intervals from the given IntervalSet
 * that DON'T satisfy the following conditions:
 * - The interval's bottom is greater than or equal to 'b'.
 * - The interval's index is greater than or equal to 'j'.
 * - The interval's index is less than 'i'.
 *
 * @param intervalSet The original IntervalSet.
 * @param i           The upper bound index.
 * @param b           The lower bound value.
 * @param j           The lower bound index.
 * @return            A new IntervalSet containing the desired intervals.
 */
static IntervalSet* getInverseLessThanIRightOfBGreaterEqualJ(const IntervalSet* intervalSet,
                                                             const unsigned int i,
                                                             const unsigned int b,
                                                             const unsigned int j) {
  // choosing intervalSet.length as upper bound
  Interval intervals[intervalSet->length];

  unsigned int nChosen = 0;
  for (unsigned int k = 0; k < intervalSet->length; k++) {
    const Interval* currInterval = &(intervalSet->intervals[k]);
    if (!(intervalLessThan(currInterval, i) && currInterval->bottom >= b &&
          intervalGreaterEqual(currInterval, j))) {
      intervals[nChosen++] = *currInterval;
    }
  }

  return intervalSetCreate(intervals, nChosen, intervalSet->stack);
}

/**
 * Retrieves the inverse of the lowest part greater than or equal to j from the given interval set.
 *
 * @param intervalSet The interval set to retrieve the inverse of.
 * @param j The value to compare against the lowest part of the intervals.
 * @return The resulting interval set containing the inverse.
 */
IntervalSet* intervalSetGetInverseLowestPartGreaterEqualJ(const IntervalSet* intervalSet,
                                                          const unsigned int j) {
  GraphNode* predNode = stackTop(&(intervalSet->stack));

  // if there is no predecessor, return an empty interval set
  if (!predNode) {
    return intervalSetCreateBlank(NULL, 0);
  }

  // check if there is an Interval that intervalContains i
  Interval* interval = intervalSetGetFirstContainingI(intervalSet, predNode->i);

  return getInverseLessThanIRightOfBGreaterEqualJ(intervalSet, predNode->i,
                                                  interval ? interval->bottom : 0, j);
}