#include <iostream>
#include "nearestNeighbour.h"

using namespace std;

std::vector<int>
nearestNeighbour(const std::vector<std::vector<float>> &adjMatrix, const std::vector<float> &scores, float budget) {
    int mSize = adjMatrix.size();
    if (scores.size() != mSize - 1) {
        cerr << "Invalid args" << endl;
        exit(1);
    }
    for (const vector<float> &i : adjMatrix) {
        if (i.size() != mSize) {
            cerr << "Invalid args" << endl;
            exit(1);
        }
    }


    vector<int> path;
    vector<int> unusedVertices;
    float pathCost = 0;
    float score = 0;

    path.reserve(adjMatrix.size());
    unusedVertices.reserve(adjMatrix.size());

    path.push_back(0);
    for (int i = 1; i < adjMatrix.size(); i++) {
        unusedVertices.push_back(i);
    }

    while (!unusedVertices.empty()) {
        float bestRatio = 0;
        int bestUnusedPathIndex = 0;

        for (int i = 0; i < unusedVertices.size(); i++) {
            int index = unusedVertices[i];

            int lastPathIndex = path[path.size() - 1];
            float newCost = pathCost - adjMatrix[lastPathIndex][0]
                            + adjMatrix[lastPathIndex][index] + adjMatrix[index][0];

            float ratio = scores[index - 1] / adjMatrix[lastPathIndex][index];

            if (newCost <= budget && ratio > bestRatio) {
                bestRatio = ratio;
                bestUnusedPathIndex = i;
            }
        }

        if (bestRatio == 0) {
            break;
        } else {
            int index = unusedVertices[bestUnusedPathIndex];

            int lastPathIndex = path[path.size() - 1];
            float newCost = pathCost - adjMatrix[lastPathIndex][0]
                            + adjMatrix[lastPathIndex][index] + adjMatrix[index][0];

            float newScore = score + scores[index - 1];

            pathCost = newCost;
            score    = newScore;
            path.push_back(index);
            unusedVertices.erase(unusedVertices.begin() + bestUnusedPathIndex);
        }
    }

    cout << "Path score: " << score << " | Path cost: " << pathCost << endl;

    return path;
}
