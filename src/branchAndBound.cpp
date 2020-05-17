//
// Created by daniel on 15/05/2020.
//

#include "branchAndBound.h"

#include <utility>
#include <queue>

using namespace std;

// Vertex in the sense of the B&B algorithm solution search tree
class Vertex {
private:
    vector<int> path;
    vector<int> unusedVertices;
    float pathCost;
    float score;
public:
    explicit Vertex(const vector<vector<float>> &adjMatrix)  {
        path.reserve(adjMatrix.size());
        unusedVertices.reserve(adjMatrix.size());

        path.push_back(0);
        for (int i = 1; i < adjMatrix.size(); i++) {
            unusedVertices.push_back(i);
        }
        pathCost = 0;
        score = 0;
    }

    Vertex(vector<int> path, vector<int> unusedVertices, float pathCost, float score) :
    path(std::move(path)), unusedVertices(std::move(unusedVertices)), pathCost(pathCost), score(score) {}

    bool isAtBottom() const {
        return unusedVertices.empty();
    }

    vector<Vertex> getValidChildren(const vector<vector<float>> &adjMatrix, const vector<float> &scores, const float budget) const {
        vector<Vertex> res;
        for (int i = 0; i < unusedVertices.size(); i++) {
            int index = unusedVertices[i];

            int lastPathIndex = path[path.size() - 1];
            float newCost = pathCost - adjMatrix[lastPathIndex][0]
                    + adjMatrix[lastPathIndex][index] + adjMatrix[index][0];

            float newScore = score + scores[index];

            if (newCost <= budget) {
                // copying the vectors
                vector<int> newPath           = this->path;
                vector<int> newUnusedVertices = this->unusedVertices;

                newPath.push_back(index);
                newUnusedVertices.erase(newUnusedVertices.begin() + i);

                res.push_back(Vertex(newPath, newUnusedVertices, newCost, newScore));
            }
        }

        return res;
    }

    bool betterThan(const Vertex &v) const {
        return this->score > v.score;
    }

    const vector<int> &getPath() const {
        return path;
    }
};

vector<int>
branchAndBound(const vector<vector<float>> &adjMatrix, const vector<float> &scores, const float budget) {
    Vertex currentBest = Vertex(adjMatrix);

    queue<Vertex> solutions;
    solutions.push(currentBest);

    while (!solutions.empty()) {
        Vertex candidate = solutions.front();
        solutions.pop();

        if (candidate.betterThan(currentBest)) {
            currentBest = candidate;
        }

        if (candidate.isAtBottom()) {
            break;
        }

        for (Vertex &child : candidate.getValidChildren(adjMatrix, scores, budget)) {
            solutions.push(std::move(child));
        }
    }

    return currentBest.getPath();
}
