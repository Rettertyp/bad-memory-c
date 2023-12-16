#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

#include "intervalSet.h"

// forward declaration of IntervalSet
typedef struct IntervalSet IntervalSet;

/**
 * @brief Structure representing a node in a linked list of interval sets.
 */
typedef struct IntervalSetNode {
  IntervalSet* set; /** Pointer to the interval set. */
  struct IntervalSetNode*
      next; /** Pointer to the next interval set node in the linked list. */
} IntervalSetNode;

/**
 * @brief Structure representing a data-node in the dynamic program.
 */
typedef struct GraphNode {
  int i;                         /** The i-value of the graph node. */
  int s;                         /** The s-value of the graph node. */
  IntervalSetNode* intervalSets; /** Pointer to the linked list of interval
                                    sets associated with the graph node. */
} GraphNode;

GraphNode createGraphNode(const int i, const int s);
void deleteGraphNode(GraphNode* graphNode);
void addIntervalSet(GraphNode* graphNode, IntervalSet* intervalSet);
void removeDominatedSets(GraphNode* graphNode);
int getNumberOfIntervalSets(const GraphNode* graphNode);
void printGraphNode(const GraphNode* graphNode);
void printGraphNodeDetailed(const GraphNode* graphNode);

#endif // GRAPH_NODE_H