#include "graphNode.h"
#include <stdio.h>

/**
 * Creates a new GraphNode with the specified i, s values and an empty list of
 * IntervalSets and returns it.
 *
 * @param i The value of the 'i' field in the GraphNode.
 * @param s The value of the 's' field in the GraphNode.
 * @return The newly created GraphNode.
 */
GraphNode createGraphNode(int i, int s) {
  GraphNode graphNode;

  graphNode.i = i;
  graphNode.s = s;
  graphNode.intervalSets = NULL;

  return graphNode;
}

/**
 * Deletes a GraphNode and frees the memory allocated for it.
 * Also deletes all associated IntervalSets and frees their memory.
 *
 * @param graphNode A pointer to the GraphNode to be deleted.
 */
void deleteGraphNode(GraphNode* graphNode) {
  if (graphNode) {
    IntervalSetNode* intervalSetNode = graphNode->intervalSets;

    while (intervalSetNode) {
      IntervalSetNode* next = intervalSetNode->next;
      deleteIntervalSet(intervalSetNode->set);
      free(intervalSetNode);
      intervalSetNode = next;
    }

    free(graphNode);
  }
}

/**
 * Adds an IntervalSet to the specified GraphNode.
 *
 * @param graphNode A pointer to the GraphNode to which the IntervalSet will be
 * added.
 * @param intervalSet A pointer to the IntervalSet to be added.
 */
void addIntervalSet(GraphNode* graphNode, IntervalSet* intervalSet) {
  IntervalSetNode* intervalSetNode = malloc(sizeof(IntervalSetNode));

  if (intervalSetNode) {
    intervalSetNode->set = intervalSet;
    intervalSetNode->next = graphNode->intervalSets;
    graphNode->intervalSets = intervalSetNode;
  }
}

/**
 * Removes dominated sets from the graph node.
 * A set is considered dominated if there exists another set in the graph node
 * where all lower bounds are less than or equal to the lower bounds of the
 * dominated set.
 *
 * @param graphNode The graph node from which to remove dominated sets.
 */
void removeDominatedSets(GraphNode* graphNode) {
  IntervalSetNode* outerSetNode = graphNode->intervalSets;

  while (outerSetNode) {
    IntervalSetNode* innerSetNode = outerSetNode->next;

    while (innerSetNode) {
      printf("Comparing:\n");
      printIntervalSet(outerSetNode->set);
      printf("and\n");
      printIntervalSet(innerSetNode->set);

      if (isDominatedBy(innerSetNode->set, outerSetNode->set)) {
        printf("innerSetNode is dominated by outerSetNode\n");

        // get predecessor of innerSetNode
        IntervalSetNode* pred = outerSetNode;
        while (pred->next != innerSetNode) {
          pred = pred->next;
        }
        // remove innerSetNode from list
        IntervalSetNode* next = innerSetNode->next;
        pred->next = next;

        // delete innerSetNode
        deleteIntervalSet(innerSetNode->set);
        free(innerSetNode);
        innerSetNode = next;

      } else if (isDominatedBy(outerSetNode->set, innerSetNode->set)) {
        printf("outerSetNode is dominated by innerSetNode\n");

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
        deleteIntervalSet(outerSetNode->set);
        free(outerSetNode);
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
int getNumberOfIntervalSets(GraphNode* graphNode) {
  int count = 0;

  IntervalSetNode* intervalSetNode = graphNode->intervalSets;

  while (intervalSetNode) {
    count++;
    intervalSetNode = intervalSetNode->next;
  }

  return count;
}

/**
 * Prints the details of a GraphNode and its associated IntervalSets.
 *
 * @param graphNode The GraphNode to be printed.
 */
void printGraphNode(GraphNode* graphNode) {
  printf("GraphNode(%d, %d)\n", graphNode->i, graphNode->s);

  IntervalSetNode* intervalSetNode = graphNode->intervalSets;

  while (intervalSetNode) {
    printIntervalSet(intervalSetNode->set);
    intervalSetNode = intervalSetNode->next;
  }

  printf("\n");
}