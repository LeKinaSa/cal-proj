//
// Created by daniel on 22/05/2020.
//

#ifndef MOCK_MATRICES_H
#define MOCK_MATRICES_H

#include <vector>

std::vector<std::vector<float>> randomMatrix(float minX, float maxX, float minY, float maxY, int size, float budget);
std::vector<float> randomScores(int size);

#endif // MOCK_MATRICES_H
