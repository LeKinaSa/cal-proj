//
// Created by daniel on 15/05/2020.
//

#ifndef BRANCH_AND_BOUND_H
#define BRANCH_AND_BOUND_H

#include <vector>

std::vector<int>
branchAndBound(const std::vector<std::vector<float>> &adjMatrix, const std::vector<float> &scores, float cost);


#endif // BRANCH_AND_BOUND_H
