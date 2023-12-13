#include "intervalSet.h"
#include <stdio.h>

/**
 * Creates a new IntervalSet object.
 *
 * @param intervals An array of Interval objects.
 * @param length The length of the intervals array.
 * @return A pointer to the newly created IntervalSet object, or NULL if memory
 * allocation fails.
 */
IntervalSet* createIntervalSet(Interval intervals[], int length) {
  IntervalSet* intervalSet = malloc(sizeof(IntervalSet));

  if (intervalSet) {
    intervalSet->stack = NULL;
    intervalSet->length = length;
    intervalSet->intervals = malloc(sizeof(Interval) * length);

    // copy the intervals into the new array
    if (intervalSet->intervals) {
      for (int i = 0; i < length; i++) {
        intervalSet->intervals[i] = intervals[i];
      }
    } else {
      free(intervalSet);
      intervalSet = NULL;
    }
  }

  return intervalSet;
}

/**
 * Deletes the specified IntervalSet and frees the allocated memory.
 *
 * @param intervalSet A pointer to the IntervalSet to be deleted.
 */
void deleteIntervalSet(IntervalSet* intervalSet) {
  if (intervalSet) {
    free(intervalSet->intervals);
    free(intervalSet);
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
bool isDominatedBy(IntervalSet* thisSet, IntervalSet* otherSet) {
  if (thisSet->length != otherSet->length) {
    return false;
  }

  for (int i = 0; i < thisSet->length; i++) {
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
void swap(Interval* a, Interval* b) {
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
void heapify(Interval intervals[], int n, int i) {
  int largest = i;
  int left = 2 * i + 1;
  int right = 2 * i + 2;

  // check if one of the two successors is bigger
  if (left < n && intervals[left].bottom > intervals[largest].bottom) {
    largest = left;
  }

  if (right < n && intervals[right].bottom > intervals[largest].bottom) {
    largest = right;
  }

  // if the left or right was bigger
  if (largest != i) {
    swap(&intervals[i], &intervals[largest]);
    heapify(intervals, n, largest);
  }
}

/**
 * Sorts an array of Interval structs using the heap sort algorithm.
 *
 * @param intervals The array of Interval structs to be sorted.
 * @param n The number of elements in the array.
 */
void heapSort(Interval intervals[], int n) {
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
 * Sorts the intervals in the given IntervalSet by their bottom values.
 *
 * @param intervalSet The IntervalSet to be sorted.
 */
void sortByBottom(IntervalSet* intervalSet) {
  heapSort(intervalSet->intervals, intervalSet->length);
}

/**
 * Prints the intervals in the given IntervalSet.
 *
 * @param intervalSet The IntervalSet to be printed.
 */
void printIntervalSet(IntervalSet* intervalSet) {
  for (int i = 0; i < intervalSet->length; i++) {
    printf("(%d, %d), ", intervalSet->intervals[i].bottom,
           intervalSet->intervals[i].top);
  }

  printf("\n");
}

/**
 * Counts the number of intervals in the given IntervalSet that are greater than
 * the specified value.
 *
 * @param intervalSet The IntervalSet to search in.
 * @param i The value to compare against.
 * @return The count of intervals greater than the specified value.
 */
int countGreaterI(IntervalSet* intervalSet, int i) {
  int count = 0;

  for (int j = 0; j < intervalSet->length; j++) {
    if (greaterThan(&(intervalSet->intervals[j]), i)) {
      count++;
    }
  }

  return count;
}

/**
 * Counts the number of intervals in the given IntervalSet that contain the
 * specified value.
 *
 * @param intervalSet The IntervalSet to search in.
 * @param i The value to check for containment.
 * @return The count of intervals containing the specified value.
 */
int countContainingI(IntervalSet* intervalSet, int i) {
  int count = 0;

  for (int j = 0; j < intervalSet->length; j++) {
    if (contains(&(intervalSet->intervals[j]), i)) {
      count++;
    }
  }

  return count;
}