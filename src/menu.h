#ifndef MENU_H
#define MENU_H

#include "Graph.h"
#include "parsing.h"
#include "branchAndBound.h"
#include "nearestNeighbour.h"

#include <string>
#include <vector>

enum MenuType {
    MAIN_MENU,
    CALCULATE_TRIP_MENU,
    MAP_MENU,
    ALGORITHM_MENU,
    PREFERENCES_MENU,
    EXIT_MENU
};

enum CityMap {
    GRID_4X4,
    GRID_8X8,
    GRID_16X16,
    REPORT
};

enum ReductionStepAlgorithm {
    DIJKSTRA,
    FLOYD_WARSHALL
};

enum CCTSPStepAlgorithm {
    BRANCH_AND_BOUND,
    NEAREST_NEIGHBOUR
};

namespace menu {
    const std::string SEPARATOR = "----------------------------------------";
    const std::string INPUT = " -> ";

    enum OPTION {
        EXIT,
        BACK,
        NONE
    };

    void menuLoop();
    int optionsMenu(const std::string & title, const std::vector<std::string> & options, OPTION option);
    float getFloatValue();

    MenuType mainMenu();
    MenuType calculateTripMenu(const std::vector<float>& preferences,
            const ReductionStepAlgorithm & reductionStepAlgorithm, const CCTSPStepAlgorithm & cctspStepAlgorithm,
            const CityMap & map);
    MenuType mapsMenu(CityMap & map);
    MenuType algorithmsMenu(ReductionStepAlgorithm & reductionStepAlgorithm, CCTSPStepAlgorithm & cctspStepAlgorithm);
    MenuType preferencesMenu(std::vector<float> & preferences);

    template <class T>
    void showPath(const std::vector<Vertex<T>*> & path);

    void showPathOnGraphViewer(const std::vector<Vertex<char>*> & path,
            const std::vector<Vertex<char>*>& pointsOfInterest);
    void showPathOnGraphViewer(const std::vector<Vertex<PosInfo>*>& path,
            const std::vector<Vertex<PosInfo>*>& pointsOfInterest);

    unsigned int selectVertex(const Graph<PosInfo>& graph);

    float getBudget();
    std::vector<float> calculateScores(const std::vector<POICategory> & pointsOfInterestCategories,
            const std::vector<float>& preferences);
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
            if (v->getPath() != nullptr)
                pathFragment.insert(pathFragment.begin(), v);

            v = v->getPath();
        } while (v != nullptr);

        path.insert(path.end(), pathFragment.begin(), pathFragment.end());

        graph.dijkstraShortestPath(poi->getInfo());
    }

    pathFragment.clear();
    Vertex<T>* finishPtr = graph.findVertex(finish);

    do {
        // We insert at the beginning to invert the order of the vertices
        if (finishPtr->getPath() != nullptr)
            pathFragment.insert(pathFragment.begin(), finishPtr);

        finishPtr = finishPtr->getPath();
    } while (finishPtr != nullptr);


    path.insert(path.end(), pathFragment.begin(), pathFragment.end());

    return path;
}

template<class T>
std::vector<Vertex<T>*> mmpMethod(
        Graph<T> & graph,
        const std::vector<Vertex<T>*>& pointsOfInterest,
        const std::vector<float>& scores,
        const T& start,
        const T& finish,
        float budget,
        const ReductionStepAlgorithm & reductionStepAlgorithm,
        const CCTSPStepAlgorithm & cctspStepAlgorithm
) {
    graph.dijkstraShortestPath(start);

    Vertex<T>* finishPtr = graph.findVertex(finish);

    // Check if there is a solution (a path from start to finish with cost no greater than budget)
    if (finishPtr != nullptr) {
        if (finishPtr->getDist() > budget) {
            std::cout << "There isn't a path from start to finish with cost no greater than the budget." << std::endl;
            return std::vector<Vertex<T> *>();
        }
    }
    else {
        return std::vector<Vertex<T>*>();
    }

    Vertex<T>* startPtr = graph.findVertex(start);

    std::vector<std::vector<float>> adj;
    switch (reductionStepAlgorithm) {
        case DIJKSTRA:
            adj = graph.generateAdjacencyMatrixWithDijkstra(pointsOfInterest, startPtr, finishPtr);
            break;
        case FLOYD_WARSHALL:
            adj = graph.generateAdjacencyMatrixWithFloydWarshall(pointsOfInterest, startPtr, finishPtr);
            break;
        default:
            break;
    }

    std::vector<int> tspPath;
    switch (cctspStepAlgorithm) {
        case BRANCH_AND_BOUND:
            tspPath = branchAndBound(adj, scores, budget);
            break;
        case NEAREST_NEIGHBOUR:
            tspPath = nearestNeighbour(adj, scores, budget);
            break;
        default:
            break;
    }

    return reconstructPath(graph, start, finish, adj, pointsOfInterest, tspPath);
}

template <class T>
void menu::showPath(const std::vector<Vertex<T>*> & path) {
    menu::optionsMenu("Best Path", {}, menu::NONE);

    for (int index = 0; index < path.size(); ++index) {
        if (index > 0) {
            std::cout << " - ";
        }
        std::cout << path[index]->getInfo();
    }
    std::cout << std::endl;
}

#endif // MENU_H
