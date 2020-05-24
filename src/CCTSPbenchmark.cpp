//
// Created by daniel on 22/05/2020.
//

#include <iostream>
#include "CCTSPbenchmark.h"
#include "mockMatrixes.h"
#include "branchAndBound.h"
#include "nearestNeighbour.h"

using namespace std;

void benchmarkCCTSP() {
    const float BUDGET = 75;

    for (int n = 100; n < 500; n++) {
        auto matrix = randomMatrix(0, 100, 0, 100, n, BUDGET);
        auto score  = randomScores(n);
        cout << "N: " << n << endl;
        auto result = branchAndBound(matrix, score, BUDGET);
        cout << "Result size: " << result.size() << endl;
        auto Hresult = nearestNeighbour(matrix, score, BUDGET);
        cout << "HResult size: " << Hresult.size() << endl;

    }
}
