#include "graphNodeStorage.h"
#include <stdlib.h>

/**
 * Checks if a GraphNode is contained in the GraphNodeStorage.
 *
 * @param graphNodeStorage The GraphNodeStorage to search in.
 * @param graphNode The GraphNode to check for.
 */
bool graphNodeStorageContainsNode(const GraphNodeStorage* graphNodeStorage,
                                  const GraphNode* graphNode) {
  GraphNodeStorageNode* currNode = *graphNodeStorage;

  while (currNode) {
    if (currNode->i == graphNode->i && currNode->s == graphNode->s) {
      return true;
    }
    currNode = currNode->next;
  }

  return false;
}

/**
 * Adds a GraphNode to the GraphNodeStorage, if it is not already present.
 *
 * @param graphNodeStorage A pointer to the GraphNodeStorage to which the GraphNode will be added.
 * @param graphNode A pointer to the GraphNode to be added.
 */
static void graphNodeStorageAddNode(GraphNodeStorage* graphNodeStorage, GraphNode* graphNode) {
  if (graphNode && !graphNodeStorageContainsNode(graphNodeStorage, graphNode)) {
    GraphNodeStorageNode* storageNode = malloc(sizeof(GraphNodeStorageNode));

    storageNode->i = graphNode->i;
    storageNode->s = graphNode->s;
    storageNode->next = *graphNodeStorage;

    *graphNodeStorage = storageNode;
  }
}

/**
 * Connects two GraphNodes in the GraphNodeStorage.
 *
 * @param graphNodeStorage The GraphNodeStorage to connect the nodes in.
 * @param fromNode The GraphNode from which the edge will be directed.
 * @param toNode The GraphNode to which the edge will be directed.
 */
void graphNodeStorageConnectNodes(GraphNode* fromNode, GraphNode* toNode) {
  graphNodeStorageAddNode(&(fromNode->outgoing), toNode);
  graphNodeStorageAddNode(&(toNode->incoming), fromNode);
}

/**
 * Deletes a GraphNodeStorage and frees the memory allocated for it.
 *
 * @param graphNodeStorage A pointer to the GraphNodeStorage to be deleted.
 */
void graphNodeStorageDelete(GraphNodeStorage* graphNodeStorage) {
  if (graphNodeStorage) {
    GraphNodeStorageNode* currNode = *graphNodeStorage;

    while (currNode) {
      GraphNodeStorageNode* nextNode = currNode->next;
      free(currNode);
      currNode = nextNode;
    }

    *graphNodeStorage = NULL;
  }
}

/**
 * Returns the number of nodes in the graph node storage.
 *
 * @param graphNodeStorage The graph node storage.
 */
uint32_t graphNodeStorageGetNNodes(const GraphNodeStorage* graphNodeStorage) {
  uint32_t nNodes = 0;
  GraphNodeStorageNode* currNode = *graphNodeStorage;

  while (currNode) {
    nNodes++;
    currNode = currNode->next;
  }

  return nNodes;
}