#ifndef GRAPH_NODE_STORAGE_H
#define GRAPH_NODE_STORAGE_H

#include "graphNode.h"
#include <stdint.h>

// forward declaration of GraphNode
typedef struct GraphNode GraphNode;

typedef struct GraphNodeStorageNode {
  uint32_t i;
  uint32_t s;
  struct GraphNodeStorageNode* next;
} GraphNodeStorageNode;

typedef GraphNodeStorageNode* GraphNodeStorage;

void graphNodeStorageConnectNodes(GraphNode* fromNode, GraphNode* toNode);
void graphNodeStorageDelete(GraphNodeStorage* graphNodeStorage);
uint32_t graphNodeStorageGetNNodes(const GraphNodeStorage* graphNodeStorage);

#endif // GRAPH_NODE_STORAGE_H