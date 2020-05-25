#ifndef BRANCH_AND_BOUND_H
#define BRANCH_AND_BOUND_H

#include <vector>

/**
 * @brief Calculates the best route using the branch and bound method.
 * @param adjMatrix     adjacency matrix of the start vertex and the points of interest
 * @param scores        list of scores of the points of interest
 * @param budget        maximum budget
 * @return              list of indices of the matrix corresponding to the visited points
 */
std::vector<int>
branchAndBound(const std::vector<std::vector<float>> &adjMatrix, const std::vector<float> &scores, float budget);


#endif // BRANCH_AND_BOUND_H
