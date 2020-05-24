//
// Created by daniel on 22/05/2020.
//

#include <iostream>
#include <chrono>
#include "CCTSPbenchmark.h"
#include "mockMatrices.h"
#include "branchAndBound.h"
#include "nearestNeighbour.h"

using namespace std;

float getScore(const vector<float> &score, const vector<int> &solution) {
    if (solution.at(0) != 0) {
        cerr << "First element in solution must be 0" << endl;
        exit(0);
    }

    float res = 0;
    for (int i = 1; i < solution.size(); i++) {
        res += score[solution[i] - 1];
    }
    return res;
}

float getCost(const vector<vector<float>> &matrix, const vector<int> &solution) {
    if (solution.at(0) != 0) {
        cerr << "First element in solution must be 0" << endl;
        exit(0);
    }

    if (solution.size() < 2) {
        return -1;
        cerr << "Size of solution must greater or equal than 2" << endl;
        cerr << "This is only a benchmark restriction" << endl;
        exit(0);
    }


    float res = 0;

    for (int i = 0; i < solution.size() - 1; i++) {
        res += matrix[solution[i]][solution[i + 1]];
    }
    res += matrix[solution[solution.size() - 1]][0]; // Loop back to the start

    return res;
}

void benchmarkCCTSP() {
    const bool CSV = true;

    if (CSV) {
        cout << "Matrix size, "
             << "Result size, Result score, Result loop, Result delta, "
             << "HResult size, HResult score, HResult loop, HResult delta"
             << endl;
    }

    const float BUDGET = 75;
    for (int n = 2; n < 200; n++) {
        auto matrix = randomMatrix(0, 100, 0, 100, n, BUDGET);
        auto score  = randomScores(n);

        if (CSV) {
            cout << n;
        } else {
            cout << "N: " << n << endl;
        }

        auto t1 = std::chrono::high_resolution_clock::now();
        auto result = branchAndBound(matrix, score, BUDGET);
        auto t2 = std::chrono::high_resolution_clock::now();
        auto delta = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

        if (CSV) {
            cout << ", " << result.size()          ;
            cout << ", " << getScore(score, result);
            cout << ", " << getCost(matrix, result);
            cout << ", " << delta                  ;
        } else {
            cout << "Result size: "  << result.size()           << endl;
            cout << "Result score: " << getScore(score, result) << endl;
            cout << "Result loop: "  << getCost(matrix, result) << endl;
            cout << "Result delta: " << delta                   << endl;
        }

        t1 = std::chrono::high_resolution_clock::now();
        auto Hresult = nearestNeighbour(matrix, score, BUDGET);
        t2 = std::chrono::high_resolution_clock::now();
        delta = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

        if (CSV) {
            cout << ", " << Hresult.size()          ;
            cout << ", " << getScore(score, Hresult);
            cout << ", " << getCost(matrix, Hresult);
            cout << ", " << delta                   ;

            cout << endl;
        } else {
            cout << "HResult size: "  << Hresult.size()           << endl;
            cout << "HResult score: " << getScore(score, Hresult) << endl;
            cout << "HResult loop: "  << getCost(matrix, Hresult) << endl;
            cout << "HResult delta: " << delta                    << endl;
        }
    }

    if (CSV) {
        exit(0);
    }
}
