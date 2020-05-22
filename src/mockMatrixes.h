//
// Created by daniel on 22/05/2020.
//

#ifndef CAL_PROJ_MOCKMATRIXES_H
#define CAL_PROJ_MOCKMATRIXES_H

#include <vector>

std::vector<std::vector<float>> randomMatrix(float minX, float maxX, float minY, float maxY, int size, float budget);
std::vector<float> randomScores(int size);

#endif //CAL_PROJ_MOCKMATRIXES_H
