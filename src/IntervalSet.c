#include "intervalSet.h"
// #define DEBUG_PRINT 1
#include "debug.h"
#include <stdlib.h>

/**
 * Function pointer type for Interval comparison functions.
 */
typedef bool (*ComparisonFunc)(const Interval*, const uint32_t);

/**
 * Creates a new IntervalSet object.
 *
 * @param intervals An array of Interval objects.
 * @param length The length of the intervals array.
 * @param stack The Stack object associated with the IntervalSet.
 * @return A pointer to the newly created IntervalSet object, or NULL if memory allocation
 * fails.
 */
static IntervalSet* intervalSetCreate(const Interval intervals[], const uint32_t length,
                                      const Stack stack) {
  IntervalSet* intervalSet = malloc(sizeof(IntervalSet) + sizeof(Interval) * length);

  if (intervalSet) {
    intervalSet->stack = stackCopy(stack);
    intervalSet->length = length;

    // copy the intervals into the new array
    for (uint32_t i = 0; i < length; i++) {
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
IntervalSet* intervalSetCreateBlank(const Interval intervals[], const uint32_t length) {
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
 * Counts the number of intervals in the given IntervalSet.
 *
 * @param intervalSet The IntervalSet to count intervals from.
 * @return The number of intervals in the IntervalSet.
 */
uint32_t intervalSetCountIntervals(const IntervalSet* intervalSet) {
  uint32_t count = 0;
  for (uint32_t i = 0; i < intervalSet->length; i++) {
    count += intervalSet->intervals[i].amount;
  }
  return count;
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
  if (intervalSetCountIntervals(thisSet) != intervalSetCountIntervals(otherSet)) {
    return false;
  }

  uint32_t thisI = 0, thisJ = 0, otherI = 0, otherJ = 0;
  while (thisI < thisSet->length) {
    if (!(otherSet->intervals[otherI].bottom <= thisSet->intervals[thisI].bottom)) {
      return false;
    }

    // J counts how many intervals at position I have been checked
    thisJ++;
    otherJ++;
    if (thisJ == thisSet->intervals[thisI].amount) {
      thisI++;
      thisJ = 0;
    }
    if (otherJ == otherSet->intervals[otherI].amount) {
      otherI++;
      otherJ = 0;
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
static void heapify(Interval intervals[], const uint32_t n, const uint32_t i) {
  uint32_t smallest = i;
  uint32_t left = 2 * i + 1;
  uint32_t right = 2 * i + 2;

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
static void heapSort(Interval intervals[], const uint32_t n) {
  // go over all nodes that are not leaves (turns array into heap)
  for (int32_t i = n / 2 - 1; i >= 0; i--) {
    heapify(intervals, n, i);
  }

  // successively remove all leaves from back to front
  for (int32_t i = n - 1; i >= 0; i--) {
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
    debug_print("[]");
  } else
    for (uint32_t i = 0; i < intervalSet->length; i++) {
      debug_print("[%d, %d, %d], ", intervalSet->intervals[i].bottom, intervalSet->intervals[i].top,
                  intervalSet->intervals[i].amount);
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
static uint32_t intervalSetCount(const IntervalSet* intervalSet, const uint32_t i,
                                 ComparisonFunc compFunc) {
  uint32_t count = 0;

  for (uint32_t j = 0; j < intervalSet->length; j++) {
    if (compFunc(&(intervalSet->intervals[j]), i)) {
      count += intervalSet->intervals[j].amount;
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
uint32_t intervalSetCountGreaterI(const IntervalSet* intervalSet, const uint32_t i) {
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
uint32_t intervalSetCountContainingI(const IntervalSet* intervalSet, const uint32_t i) {
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
static uint32_t intervalSetCountGreaterEqualI(const IntervalSet* intervalSet, const uint32_t i) {
  return intervalSetCount(intervalSet, i, intervalGreaterEqual);
}

/**
 * Frees an array of intervals.
 *
 * @param intervals The Interval-array to free.
 */
static void freeIntervals(Interval intervals[]) {
  free(intervals);
  intervals = NULL;
}

/**
 * Returns the first interval in the given interval set that contains the specified value.
 *
 * @param intervalSet The interval set to search in.
 * @param i The value to search for.
 * @return A pointer to the first interval that contains the specified value, or NULL if
 * no such interval is found.
 */
static Interval* intervalSetGetFirstContainingI(IntervalSet* intervalSet, const uint32_t i) {
  for (uint32_t j = 0; j < intervalSet->length; j++) {
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
IntervalSet* intervalSetGetWithoutFirstGIncludingI(const IntervalSet* intervalSet, const uint32_t i,
                                                   const uint32_t g) {
  Interval* intervals = malloc(sizeof(Interval) * intervalSet->length);

  // counts the number of interval-objects that have been added to the new set
  int32_t lengthCounter = -1;

  uint32_t nAssigned = 0;
  for (uint32_t outerK = 0; outerK < intervalSet->length; outerK++) {
    // computed once, since it is the same for all intervals in the outer loop
    const Interval* currInterval = &(intervalSet->intervals[outerK]);
    bool intervalContainsI = intervalContains(currInterval, i);
    bool hasBeenAdded = false;

    for (uint32_t innerK = 0; innerK < currInterval->amount; innerK++) {
      if (nAssigned < g && intervalContainsI) {
        nAssigned++;
      } else if (hasBeenAdded) {
        // if the interval has already been added to the new set, just increment the amount
        intervals[lengthCounter].amount++;
      } else {
        // if the interval has not been added to the new set yet, add it now
        intervals[++lengthCounter] = *currInterval;
        intervals[lengthCounter].amount = 1;
        hasBeenAdded = true;
      }
    }
  }

  IntervalSet* res = intervalSetCreate(intervals, lengthCounter + 1, intervalSet->stack);

  freeIntervals(intervals);

  return res;
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
static IntervalSet* getLessThanIRightOfB(const IntervalSet* intervalSet, const uint32_t i,
                                         const uint32_t b) {
  // choosing intervalSet.length as upper bound
  Interval* intervals = malloc(sizeof(Interval) * intervalSet->length);

  uint32_t nChosen = 0;
  for (uint32_t j = 0; j < intervalSet->length; j++) {
    const Interval* currInterval = &(intervalSet->intervals[j]);
    if (intervalLessThan(currInterval, i) && currInterval->bottom >= b) {
      intervals[nChosen++] = *currInterval;
    }
  }

  IntervalSet* res = intervalSetCreate(intervals, nChosen, intervalSet->stack);

  freeIntervals(intervals);

  return res;
}

/**
 * Retrieves the lowest part of the given interval set.
 *
 * @param intervalSet The interval set to retrieve the lowest part from.
 * @return The lowest part of the interval set.
 */
IntervalSet* intervalSetGetLowestPart(IntervalSet* intervalSet) {
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
static uint32_t countLessThanIRightOfBGreaterEqualJ(const IntervalSet* intervalSet,
                                                    const uint32_t i, const uint32_t b,
                                                    const uint32_t j) {
  uint32_t nChosen = 0;
  for (uint32_t k = 0; k < intervalSet->length; k++) {
    const Interval* currInterval = &(intervalSet->intervals[k]);
    if (intervalLessThan(currInterval, i) && currInterval->bottom >= b &&
        intervalGreaterEqual(currInterval, j)) {
      nChosen += currInterval->amount;
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
uint32_t intervalSetCountLowestPartGreaterEqualJ(IntervalSet* intervalSet, const uint32_t j) {
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
                                                             const uint32_t i, const uint32_t b,
                                                             const uint32_t j) {
  // choosing intervalSet.length as upper bound
  Interval* intervals = malloc(sizeof(Interval) * intervalSet->length);

  uint32_t nChosen = 0;
  for (uint32_t k = 0; k < intervalSet->length; k++) {
    const Interval* currInterval = &(intervalSet->intervals[k]);
    if (!(intervalLessThan(currInterval, i) && currInterval->bottom >= b &&
          intervalGreaterEqual(currInterval, j))) {
      intervals[nChosen++] = *currInterval;
    }
  }

  IntervalSet* res = intervalSetCreate(intervals, nChosen, intervalSet->stack);

  freeIntervals(intervals);

  return res;
}

/**
 * Retrieves the inverse of the lowest part greater than or equal to j from the given interval set.
 *
 * @param intervalSet The interval set to retrieve the inverse of.
 * @param j The value to compare against the lowest part of the intervals.
 * @return The resulting interval set containing the inverse.
 */
IntervalSet* intervalSetGetInverseLowestPartGreaterEqualJ(IntervalSet* intervalSet,
                                                          const uint32_t j) {
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