#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

#include "intervalSet.h"

/**
 * @brief Structure representing a node in a graph.
 */
typedef struct GraphNode {
  int i;                         /** The identifier of the graph node. */
  int s;                         /** The size of the graph node. */
  IntervalSetNode* intervalSets; /** Pointer to the linked list of interval
                                    sets associated with the graph node. */
} GraphNode;

/**
 * @brief Structure representing a node in a linked list of interval sets.
 */
typedef struct IntervalSetNode {
  IntervalSet* set; /** Pointer to the interval set. */
  struct IntervalSetNode*
      next; /** Pointer to the next interval set node in the linked list. */
} IntervalSetNode;

GraphNode createGraphNode(int i, int s);
void deleteGraphNode(GraphNode* graphNode);
void addIntervalSet(GraphNode* graphNode, IntervalSet* intervalSet);
void removeDominatedSets(GraphNode* graphNode);
int getNumberOfIntervalSets(GraphNode* graphNode);
void printGraphNode(GraphNode* graphNode);

#endif // GRAPH_NODE_H