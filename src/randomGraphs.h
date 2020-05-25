#ifndef RANDOM_GRAPHS_H
#define RANDOM_GRAPHS_H

#include "Graph.h"
#include <vector>

/**
 * @brief Generates a random graph with a given amount of vertices, that should, in theory, be relatively sparse.
 *
 * @param graph
 * @param pointsOfInterest
 * @param numVertices
 */
void generateRandomGraph(Graph<int>& graph, std::vector<Vertex<int>*>& pointsOfInterest, int numVertices);

void testReductionStepAlgorithms();


#endif // RANDOM_GRAPHS_H
