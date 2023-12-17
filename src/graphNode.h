#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

#include "intervalSet.h"

// forward declaration of IntervalSet
typedef struct IntervalSet IntervalSet;

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
  unsigned int i;                /** The i-value of the graph node. */
  unsigned int s;                /** The s-value of the graph node. */
  IntervalSetNode* intervalSets; /** Pointer to the linked list of interval
                                    sets associated with the graph node. */
} GraphNode;

GraphNode graphNodeCreate(const unsigned int i, const unsigned int s);
void graphNodeDelete(GraphNode* graphNode);
void graphNodeAddIntervalSet(GraphNode* graphNode, IntervalSet* intervalSet);
void graphNodeRemoveDominatedSets(GraphNode* graphNode);
unsigned int graphNodeGetNIntervalSets(const GraphNode* graphNode);
void graphNodePrint(const GraphNode* graphNode);
void graphNodePrintDetailed(const GraphNode* graphNode);
GraphNode* getGraphNode(GraphNode* graphNodes, const unsigned int n, const unsigned int i,
                        const unsigned int s);

#endif // GRAPH_NODE_H