#ifndef MARK_STORAGE_H
#define MARK_STORAGE_H

#include "intervalSet.h"
#include <stdbool.h>

typedef struct MarkNode {
  IntervalSet* intervalSet;
  struct MarkNode* next;
} MarkNode;

typedef MarkNode* MarkStorage;

void markStorageAddSet(MarkStorage* markStorage, IntervalSet* intervalSet);
void markStorageDelete(MarkStorage* markStorage);
bool markStorageIsMarked(MarkStorage* markStorage, IntervalSet* intervalSet);

#endif // MARK_STORAGE_H