#include "interval.h"
#include "debug.h"
#include <stdlib.h>

/**
 * Checks if the given interval intervalContains the specified value.
 *
 * @param interval A pointer to the interval to check.
 * @param value The value to check for containment.
 * @return true if the interval intervalContains the value, false otherwise.
 */
bool intervalContains(const Interval* interval, const uint32_t value) {
  return interval->bottom <= value && value <= interval->top;
}

/**
 * Checks if the bottom value of the given interval is greater than the specified value.
 *
 * @param interval The interval to check.
 * @param value The value to compare with the bottom value of the interval.
 * @return true if the bottom value is greater than the specified value, false otherwise.
 */
bool intervalGreaterThan(const Interval* interval, const uint32_t value) {
  return interval->bottom > value;
}

/**
 * Checks if the bottom value of the given interval is greater than or equal to the specified value.
 *
 * @param interval The interval to compare.
 * @param value The value to compare against.
 * @return True if the bottom value is greater than or equal to the specified value, false
 * otherwise.
 */
bool intervalGreaterEqual(const Interval* interval, const uint32_t value) {
  return value <= interval->top;
}

/**
 * Checks if the top value of the given interval is less than the specified value.
 *
 * @param interval The interval to check.
 * @param value The value to compare with the top value of the interval.
 * @return true if the top value is less than the specified value, false otherwise.
 */
bool intervalLessThan(const Interval* interval, const uint32_t value) {
  return interval->top < value;
}