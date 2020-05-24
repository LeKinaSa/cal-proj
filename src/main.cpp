#include "Graph.h"
#include "parsing.h"
#include "branchAndBound.h"
#include "nearestNeighbour.h"
#include "menu.h"

#include <vector>
#include <ctime>
#include <cstdlib>
#include "mockMatrices.h"
#include "CCTSPbenchmark.h"
#include <iostream>

using namespace std;

vector<vector<float>> getArticleMatrix () {
    vector<vector<float>> res(5 , vector<float>());
    res[0] = {0, 3,  6, 8, 11};
    res[1] = {12, 0,  9, 10, 10};
    res[2] = {6, 10, 0, 4, 7};
    res[3] = {2, 10, 5, 0, 3};
    res[4] = {2, 8,  8, 3, 0};

    return res;
}

int main() {
    menu::menuLoop();

    /*srand (static_cast <unsigned> (time(0)));
    benchmarkCCTSP();
    auto i = randomMatrix(0, 100, 0, 100, 50, 50);
    srand(time(nullptr));
    //benchmarkCCTSP();

    Graph<char> graph;
    std::vector<Vertex<char>*> pointsOfInterest;
    std::vector<float> scores;

    initReportGraph(graph, pointsOfInterest, scores);
    std::vector<Vertex<char>*> path = mmpMethod(graph, pointsOfInterest, scores, 's', 'f', 12);

    for (Vertex<char>* v : path) {
        std::cout << v->getInfo() << " ";
    }

    std::cout << std::endl;

    vector<float> a = {0, 1, 3, 4, 2};
    auto aa = branchAndBound(getArticleMatrix(), a, 12);
    auto aaa = nearestNeighbour(getArticleMatrix(), a, 12);*/

    // PARSING RELATED CODE
    /*
    Graph<VertexInfo> graph1;
    std::vector<Vertex<VertexInfo>*> pointsOfInterest1;
    std::vector<float> scores1;

    std::map<POICategory, float> userPreferences = {
            {INFORMATION, 1.0},
            {HOTEL, 1.0},
            {ATTRACTION, 1.0},
            {VIEWPOINT, 1.0},
            {GUEST_HOUSE, 1.0},
            {PICNIC_SITE, 1.0},
            {ARTWORK, 1.0},
            {CAMP_SITE, 1.0},
            {MUSEUM, 1.0},
            {UNSPECIFIED, 0.0}
    };

    parseVertexFile("nodes_lat_lon_porto.txt", graph1);
    parseEdgeFile("edges_porto.txt", graph1);
    parseTagsFile("t03_tags_porto.txt", graph1, pointsOfInterest1, scores1, userPreferences);
    */

    return 0;
}
