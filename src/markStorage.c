#include "markStorage.h"
#include "debug.h"
#include <stdlib.h>

/**
 * Adds an IntervalSet to the MarkStorage.
 *
 * @param markStorage The MarkStorage to add the IntervalSet to.
 * @param intervalSet The IntervalSet to be added.
 */
void markStorageAddSet(MarkStorage* markStorage, IntervalSet* intervalSet) {
  if (intervalSet) {
    MarkNode* markNode = malloc(sizeof(MarkNode));

    markNode->intervalSet = intervalSet;
    markNode->next = *markStorage;

    *markStorage = markNode;
  }
}

/**
 * Deletes a MarkStorage and frees the memory allocated for it. Does not delete
 * the IntervalSets contained in the MarkStorage.
 *
 * @param markStorage A pointer to the MarkStorage to be deleted.
 */
void markStorageDelete(MarkStorage* markStorage) {
  if (markStorage) {
    MarkNode* currNode = *markStorage;

    while (currNode) {
      MarkNode* nextNode = currNode->next;
      free(currNode);
      currNode = nextNode;
    }

    *markStorage = NULL;
  }
}

/**
 * Checks if a given interval set is marked in the mark storage.
 *
 * @param markStorage The mark storage to search in.
 * @param intervalSet The interval set to check for marking.
 * @return True if the interval set is marked, false otherwise.
 */
bool markStorageIsMarked(MarkStorage* markStorage, IntervalSet* intervalSet) {
  MarkNode* currNode = *markStorage;

  while (currNode) {
    if (currNode->intervalSet == intervalSet) {
      return true;
    }
    currNode = currNode->next;
  }

  return false;
}

/**
 * Prints the mark storage to the console for debugging purposes.
 *
 * @param markStorage The mark storage to print.
 */
void markStoragePrint(const MarkStorage* markStorage) {
  MarkNode* currNode = *markStorage;

  while (currNode) {
    printf("Pointer to interval set: %p\n", (void*)currNode->intervalSet);
    currNode = currNode->next;
  }
}