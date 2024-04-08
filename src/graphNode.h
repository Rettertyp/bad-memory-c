#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

#include "graphNodeStorage.h"
#include "intervalSet.h"
#include "markStorage.h"
#include <stdint.h>

typedef struct IntervalSet IntervalSet;
typedef struct MarkNode MarkNode;
typedef MarkNode* MarkStorage;
typedef struct GraphNodeStorageNode GraphNodeStorageNode;
typedef GraphNodeStorageNode* GraphNodeStorage;

/**
 * @brief Structure representing a node in a linked list of interval sets.
 */
typedef struct IntervalSetNode {
  IntervalSet* set;             /** Pointer to the interval set. */
  struct IntervalSetNode* next; /** Pointer to the next interval set node in the linked list. */
} IntervalSetNode;

/**
 * @brief Structure representing a data-node in the dynamic program.
 */
typedef struct GraphNode {
  uint32_t i;                    /** The i-value of the graph node. */
  uint32_t s;                    /** The s-value of the graph node. */
  IntervalSetNode* intervalSets; /** Pointer to the linked list of interval
                                    sets associated with the graph node. */
  MarkStorage markStorage;       /** Mark storage associated with the graph node. */
  GraphNodeStorage incoming;     /** The incoming edges of the graph node. */
  GraphNodeStorage outgoing;     /** The outgoing edges of the graph node. */
} GraphNode;

GraphNode graphNodeCreate(const uint32_t i, const uint32_t s);
void graphNodeDelete(GraphNode* graphNode);
void graphNodeAddIntervalSet(GraphNode* graphNode, IntervalSet* intervalSet);
bool graphNodeSetShouldBeAdded(const GraphNode* graphNode, const IntervalSet* intervalSet);
void graphNodeRemoveDominatedSets(GraphNode* graphNode);
uint32_t graphNodeGetNIntervalSets(const GraphNode* graphNode);
void graphNodePrint(const GraphNode* graphNode);
void graphNodePrintDetailed(const GraphNode* graphNode);
GraphNode* getGraphNode(GraphNode** graphNodes, const uint32_t i, const uint32_t s);
typedef bool (*CompareFunc)(int32_t, int32_t);
bool greater(int32_t a, int32_t b);
bool less(int32_t a, int32_t b);
int32_t graphNodeGetPathLengthBackwards(GraphNode** graphNodes, const uint32_t n,
                                        CompareFunc compare, int32_t initialValue);

#endif // GRAPH_NODE_H