#include "interval.h"
#include <stdlib.h>

/**
 * Creates a new interval with the given bottom and top values.
 *
 * @param bottom The bottom value of the interval.
 * @param top The top value of the interval.
 * @return A pointer to the newly created interval.
 */
Interval* createInterval(int bottom, int top) {
  Interval* interval = malloc(sizeof(Interval));

  if (interval) {
    interval->bottom = bottom;
    interval->top = top;
  }

  return interval;
}

/**
 * Checks if the given interval contains the specified value.
 *
 * @param interval A pointer to the interval to check.
 * @param value The value to check for containment.
 * @return true if the interval contains the value, false otherwise.
 */
bool contains(Interval* interval, int value) {
  return interval->bottom <= value && value <= interval->top;
}

/**
 * Checks if the top value of the interval is greater than the specified value.
 *
 * @param interval A pointer to the interval to check.
 * @param value The value to compare against the top value of the interval.
 * @return true if the top value of the interval is greater than the specified
 * value, false otherwise.
 */
bool greaterThan(Interval* interval, int value) {
  return interval->top > value;
}

/**
 * Checks if the bottom value of the interval is less than the specified value.
 *
 * @param interval A pointer to the interval to check.
 * @param value The value to compare against the bottom value of the interval.
 * @return true if the bottom value of the interval is less than the specified
 * value, false otherwise.
 */
bool lessThan(Interval* interval, int value) {
  return interval->bottom < value;
}