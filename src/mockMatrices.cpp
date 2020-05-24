#include "mockMatrices.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

class RandomPoint {
public:
    float x;
    float y;
    RandomPoint(float minX, float maxX, float minY, float maxY) {
        x = minX + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(maxX-minX)));
        y = minY + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(maxY-minY)));
    }

    float distanceTo(const RandomPoint &p) {
        float dx = p.x - x;
        float dy = p.y - y;
        return sqrt(dx*dx + dy*dy);
    }
};

std::vector<std::vector<float>> randomMatrix(float minX, float maxX, float minY, float maxY, int size, float budget) {
    RandomPoint start(minX, maxX, minY, maxY);
    RandomPoint finish(minX, maxX, minY, maxY);

    //Make sure that the distance from start to finish is within budget
    while (start.distanceTo(finish) > budget) {
        finish = RandomPoint(minX, maxX, minY, maxY);
    }

    vector<RandomPoint> points;
    for (int i = 1; i < size; i++) {
        points.push_back(RandomPoint(minX, maxX, minY, maxY));
    }

    vector<vector<float>> res(size, vector<float>(size, 0));

    for (int from = 0; from < size; from++) {
        for (int to = 0; to < size; to++) {
            if (from == to) {
                res[from][to] = 0;
            } else if (from == 0) {
                res[from][to] = start.distanceTo(points[to - 1]);
            } else if (to == 0) {
                res[from][to] = points[from - 1].distanceTo(finish);
            } else {
                res[from][to] = points[from - 1].distanceTo(points[to - 1]);
            }
        }
    }

    return res;
}

std::vector<float> randomScores(int size) {
    vector<float> res;
    for (int i = 1; i < size; i++) {
        res.push_back(static_cast <float> (rand()) / static_cast <float> (RAND_MAX)); // Random value from 0 to 1
    }
    return res;
}
