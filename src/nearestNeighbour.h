#ifndef NEAREST_NEIGHBOUR_H
#define NEAREST_NEIGHBOUR_H

#include <vector>

/**
 * @brief Calculates an optimal route using the greedy nearest neighbour method. The heuristic used is based on the
 * ratio between a point of interest's score and the cost to visit it.
 * @param adjMatrix     adjacency matrix of the start vertex and the points of interest
 * @param scores        list of scores of the points of interest
 * @param budget        maximum budget
 * @return              list of indices of the matrix corresponding to the visited points
 */
std::vector<int>
nearestNeighbour(const std::vector<std::vector<float>> &adjMatrix, const std::vector<float> &scores, float budget);

#endif // NEAREST_NEIGHBOUR_H
