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

/** Reconstructs the full path from the cost-constrained TSP path. */
template<class T>
std::vector<Vertex<T>*> reconstructPath(Graph<T>& graph, T start, T finish,
        const std::vector<std::vector<float>>& adjMatrix, const std::vector<Vertex<T>*>& pointsOfInterest,
        const std::vector<int>& tspPath) {

    std::vector<Vertex<T>*> path, pathFragment;
    path.push_back(graph.findVertex(start));

    graph.dijkstraShortestPath(start);

    for (int i = 1; i < tspPath.size(); ++i) {
        int idx = tspPath.at(i);

        Vertex<T>* poi = pointsOfInterest.at(idx - 1);
        Vertex<T>* v = poi;

        pathFragment.clear();

        do {
            // We insert at the beginning to invert the order of the vertices
            pathFragment.insert(pathFragment.begin(), v);

            v = v->getPath();
        } while (v->getPath() != nullptr);

        path.insert(path.end(), pathFragment.begin(), pathFragment.end());

        graph.dijkstraShortestPath(poi->getInfo());
    }

    pathFragment.clear();
    Vertex<T>* finishPtr = graph.findVertex(finish);

    do {
        // We insert at the beginning to invert the order of the vertices
        pathFragment.insert(pathFragment.begin(), finishPtr);

        finishPtr = finishPtr->getPath();
    } while (finishPtr->getPath() != nullptr);


    path.insert(path.end(), pathFragment.begin(), pathFragment.end());

    return path;
}


void initReportGraph(Graph<char>& graph, std::vector<Vertex<char>*>& pointsOfInterest, std::vector<float>& scores) {
    for (char c = 'a'; c <= 'k'; ++c) {
        graph.addVertex(c);
    }

    graph.addVertex('s');

    graph.addEdge('a', 's', 4.0);
    graph.addEdge('a', 'i', 5.0);
    graph.addEdge('b', 'c', 4.0);
    graph.addEdge('b', 's', 3.0);
    graph.addEdge('b', 'g', 7.0);
    graph.addEdge('c', 'b', 4.0);
    graph.addEdge('c', 'e', 4.0);
    graph.addEdge('d', 's', 2.0);
    graph.addEdge('d', 'e', 3.0);
    graph.addEdge('e', 'd', 3.0);
    graph.addEdge('e', 'i', 3.0);
    graph.addEdge('e', 'k', 2.0);
    graph.addEdge('e', 'j', 2.0);
    graph.addEdge('f', 'h', 1.0);
    graph.addEdge('f', 'j', 1.0);
    graph.addEdge('f', 'k', 2.0);
    graph.addEdge('g', 'c', 2.0);
    graph.addEdge('g', 'h', 3.0);
    graph.addEdge('h', 'f', 2.0);
    graph.addEdge('h', 'g', 2.0);
    graph.addEdge('i', 'e', 2.0);
    graph.addEdge('j', 'e', 2.0);
    graph.addEdge('j', 'f', 2.0);
    graph.addEdge('k', 'f', 3.0);
    graph.addEdge('s', 'a', 1.0);
    graph.addEdge('s', 'b', 3.0);
    graph.addEdge('s', 'c', 5.0);
    graph.addEdge('s', 'd', 3.0);

    pointsOfInterest.push_back(graph.findVertex('b'));
    pointsOfInterest.push_back(graph.findVertex('h'));
    pointsOfInterest.push_back(graph.findVertex('i'));
    pointsOfInterest.push_back(graph.findVertex('j'));

    scores.push_back(1.0);
    scores.push_back(3.0);
    scores.push_back(4.0);
    scores.push_back(2.0);
}


int main() {
    Graph<char> graph;
    std::vector<Vertex<char>*> pois;
    std::vector<float> scores;
    initReportGraph(graph, pois, scores);
    std::vector<POICategory> cat = {HOTEL, HOTEL, HOTEL, HOTEL};
    std::vector<Graph<char>> graphs = {graph};
    menu::menuLoop(graphs, pois, cat);
    /*srand (static_cast <unsigned> (time(0)));
    benchmarkCCTSP();
    auto i = randomMatrix(0, 100, 0, 100, 50, 50);
    srand(time(nullptr));

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
