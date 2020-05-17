#include <iostream>

#include <vector>
#include "branchAndBound.h"

using namespace std;

vector<vector<float>> getArticleMatrix () {
    vector<vector<float>> res(5 , vector<float>());
    res[0] = {0, 3,  6, 8, 11};
    res[1] = {3, 0,  6, 9, 10};
    res[2] = {6, 10, 0, 4, 7};
    res[3] = {2, 10, 5, 0, 3};
    res[4] = {2, 8,  8, 3, 0};

    return res;
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
