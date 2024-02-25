#include "graphNode.h"
// #define DEBUG_PRINT 1
#include "debug.h"
#include <stdlib.h>

/**
 * Creates a new GraphNode with the specified i, s values and an empty list of
 * IntervalSets and returns it.
 *
 * @param i The value of the 'i' field in the GraphNode.
 * @param s The value of the 's' field in the GraphNode.
 * @return The newly created GraphNode.
 */
GraphNode graphNodeCreate(const uint32_t i, const uint32_t s) {
  return (GraphNode){.i = i, .s = s, .intervalSets = NULL, .markStorage = NULL};
}

/**
 * Deletes a GraphNode and frees the memory allocated for it.
 * Also deletes all associated IntervalSets and frees their memory.
 *
 * @param graphNode A pointer to the GraphNode to be deleted.
 */
void graphNodeDelete(GraphNode* graphNode) {
  if (graphNode) {
    // delete the interval sets in the graph node
    IntervalSetNode* intervalSetNode = graphNode->intervalSets;
    while (intervalSetNode) {
      IntervalSetNode* next = intervalSetNode->next;
      intervalSetDelete(intervalSetNode->set);
      intervalSetNode->set = NULL;
      free(intervalSetNode);
      intervalSetNode = next;
    }
    graphNode->intervalSets = NULL;

    // delete the mark storage
    markStorageDelete(&(graphNode->markStorage));

    // delete the incoming and outgoing edges
    graphNodeStorageDelete(&(graphNode->incoming));
    graphNodeStorageDelete(&(graphNode->outgoing));

    // the graphNode itself does not need to be freed, only the array containing it
  }
}

/**
 * Deletes an IntervalSetNode and frees the memory allocated for it.
 *
 * @param intervalSetNode The IntervalSetNode to be deleted.
 */
static void graphNodeDeleteIntervalSet(IntervalSetNode* intervalSetNode) {
  if (intervalSetNode) {
    intervalSetDelete(intervalSetNode->set);
    intervalSetNode->set = NULL;
    free(intervalSetNode);
    intervalSetNode = NULL;
  }
}

/**
 * Adds an IntervalSet to the specified GraphNode.
 *
 * @param graphNode A pointer to the GraphNode to which the IntervalSet will be
 * added.
 * @param intervalSet A pointer to the IntervalSet to be added.
 */
void graphNodeAddIntervalSet(GraphNode* graphNode, IntervalSet* intervalSet) {
  IntervalSetNode* intervalSetNode = malloc(sizeof(IntervalSetNode));

  if (intervalSetNode) {
    intervalSetNode->set = intervalSet;
    intervalSetNode->next = graphNode->intervalSets;
    graphNode->intervalSets = intervalSetNode;
  }
}

/**
 * Checks whether the specified IntervalSet should be added to the specified GraphNode, or if it is
 * dominated by another IntervalSet in the GraphNode.
 *
 * @param graphNode The GraphNode to which the IntervalSet should be added.
 * @param intervalSet The IntervalSet to be added.
 * @return True if the IntervalSet should be added, false if it is dominated by another IntervalSet
 * in the GraphNode.
 */
bool graphNodeSetShouldBeAdded(const GraphNode* graphNode, const IntervalSet* intervalSet) {
  IntervalSetNode* intervalSetNode = graphNode->intervalSets;

  while (intervalSetNode) {
    if (intervalSetIsDominatedBy(intervalSet, intervalSetNode->set)) {
      return false;
    }
    intervalSetNode = intervalSetNode->next;
  }

  return true;
}

/**
 * Removes dominated sets from the graph node.
 * A set is considered dominated if there exists another set in the graph node
 * where all lower bounds are less than or equal to the lower bounds of the
 * dominated set.
 *
 * @param graphNode The graph node from which to remove dominated sets.
 */
void graphNodeRemoveDominatedSets(GraphNode* graphNode) {
  IntervalSetNode* outerSetNode = graphNode->intervalSets;

  while (outerSetNode) {
    IntervalSetNode* innerSetNode = outerSetNode->next;

    while (innerSetNode) {

      if (intervalSetIsDominatedBy(innerSetNode->set, outerSetNode->set)) {
        // the innerSetNode is dominated by the outerSetNode => remove the innerSetNode

        // get predecessor of innerSetNode
        IntervalSetNode* pred = outerSetNode;
        while (pred->next != innerSetNode) {
          pred = pred->next;
        }

        // remove innerSetNode from list
        IntervalSetNode* next = innerSetNode->next;
        pred->next = next;

        // delete innerSetNode
        graphNodeDeleteIntervalSet(innerSetNode);

        innerSetNode = next;

      } else if (intervalSetIsDominatedBy(outerSetNode->set, innerSetNode->set)) {
        // the outerSetNode is dominated by the innerSetNode => remove the outerSetNode

        IntervalSetNode* next = outerSetNode->next;

        // remove outerSetNode from list
        IntervalSetNode* pred = graphNode->intervalSets;
        if (pred == outerSetNode) {
          graphNode->intervalSets = next;
        } else {
          while (pred->next != outerSetNode) {
            pred = pred->next;
          }
          pred->next = next;
        }

        // delete outerSetNode
        graphNodeDeleteIntervalSet(outerSetNode);

        outerSetNode = next;

        // continue with next outerSetNode
        innerSetNode = outerSetNode->next;

      } else {

        innerSetNode = innerSetNode->next;
      }
    }

    outerSetNode = outerSetNode->next;
  }
}

/**
 * Returns the number of interval sets in the given graph node.
 *
 * @param graphNode The graph node to count the interval sets from.
 * @return The number of interval sets in the graph node.
 */
uint32_t graphNodeGetNIntervalSets(const GraphNode* graphNode) {
  uint32_t count = 0;

  IntervalSetNode* intervalSetNode = graphNode->intervalSets;

  while (intervalSetNode) {
    count++;
    intervalSetNode = intervalSetNode->next;
  }

  return count;
}

/**
 * Prints the details of a GraphNode.
 *
 * @param graphNode The GraphNode to be printed.
 */
#pragma GCC diagnostic ignored "-Wunused-parameter"
void graphNodePrint(const GraphNode* graphNode) {
  debug_print("GraphNode(%d, %d)\n", graphNode->i, graphNode->s);
}

/**
 * Prints the details of a GraphNode and its associated IntervalSets.
 *
 * @param graphNode The GraphNode to be printed.
 */
void graphNodePrintDetailed(const GraphNode* graphNode) {
  debug_print("GraphNode(%d, %d) [\n", graphNode->i, graphNode->s);

  IntervalSetNode* intervalSetNode = graphNode->intervalSets;

  while (intervalSetNode) {
    intervalSetPrint(intervalSetNode->set);
    intervalSetNode = intervalSetNode->next;
  }

  debug_print("]\n");
}

/**
 * Returns a pointer to the specified GraphNode in the 2D array of GraphNodes.
 *
 * @param graphNodes The 2D array of GraphNodes.
 * @param i The row index of the desired GraphNode (1-based index).
 * @param s The column index of the desired GraphNode (1-based index).
 * @param n The size of the square matrix (number of rows/columns).
 * @return A pointer to the specified GraphNode.
 */
GraphNode* getGraphNode(GraphNode** graphNodes, const uint32_t i, const uint32_t s) {
  return &(graphNodes[i - 1][s - 1]);
}