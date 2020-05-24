#ifndef MENU_H
#define MENU_H

#include "Graph.h"
#include "parsing.h"
#include "branchAndBound.h"
#include "nearestNeighbour.h"
#include "graph_viewer/graphviewer.h"

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

enum MAP {
    PORTO,
    AVEIRO
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

    template <class T>
    void menuLoop(const std::vector<Graph<T>> & graphs, const std::vector<Vertex<T>*> & pointsOfInterest, const std::vector<POICategory> & pointsOfInterestCategories);
    int optionsMenu(const std::string & title, const std::vector<std::string> & options, OPTION option);
    float getFloatValue();

    MenuType mainMenu();
    template <class T>
    MenuType calculateTripMenu(const std::vector<Graph<T>> & graph, std::vector<float> preferences,
                               const std::vector<Vertex<T>*> & pointsOfInterest, const std::vector<POICategory> & pointsOfInterestCategories,
                               ReductionStepAlgorithm & reductionStepAlgorithm, CCTSPStepAlgorithm & cctspStepAlgorithm, MAP & map);
    MenuType mapsMenu(MAP & map);
    MenuType algorithmsMenu(ReductionStepAlgorithm & reductionStepAlgorithm, CCTSPStepAlgorithm & cctspStepAlgorithm);
    MenuType preferencesMenu(std::vector<float> & preferences);

    template <class T>
    std::vector<Vertex<T>*> mmpMethod(
            Graph<T> & graph,
            const std::vector<Vertex<T>*>& pointsOfInterest,
            const std::vector<float>& scores,
            const T& start,
            const T& finish,
            float budget,
            const ReductionStepAlgorithm & reductionStepAlgorithm,
            const CCTSPStepAlgorithm & cctspStepAlgorithm
    );

    template <class T>
    void showPath(const std::vector<Vertex<T>*> & path);

    void showPathOnGraphViewer(const std::vector<Vertex<char>*> & path);
    void showPathOnGraphViewer(const std::vector<Vertex<VertexInfo>*> & path);

    template <class T>
    T selectStart(const Graph<T> & graph);
    template <class T>
    T selectFinish(const Graph<T> & graph);
    float getBudget();
    std::vector<float> calculateScores(const std::vector<POICategory> & pointsOfInterestCategories, const std::vector<float> preferences);
}


template<class T>
std::vector<Vertex<T>*> menu::mmpMethod(
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

template<class T>
T menu::selectStart(const Graph<T> & graph) {
    int answer = optionsMenu("Select Start Vertex", graph.getStringList(), menu::BACK);
    return (graph.getVertexSet().at(answer - 1))->getInfo();
}

template<class T>
T menu::selectFinish(const Graph<T> & graph) {
    int answer = optionsMenu("Select Finish Vertex", graph.getStringList(), menu::BACK);
    return (graph.getVertexSet().at(answer - 1))->getInfo();
}

template <class T>
void menu::showPath(const std::vector<Vertex<T>*> & path) {
    menu::optionsMenu("Best Path", {}, menu::NONE);
    std::cout << (path[0])->getInfo();
    for (int index = 1; index < path.size(); ++ index) {
        std::cout << " - " << (path[index])->getInfo();
    }
    std::cout << std::endl;
}

template<class T>
MenuType menu::calculateTripMenu(const std::vector<Graph<T>> & graphs, std::vector<float> preferences,
                                 const std::vector<Vertex<T>*> & pointsOfInterest, const std::vector<POICategory> & pointsOfInterestCategories,
                                 ReductionStepAlgorithm & reductionStepAlgorithm, CCTSPStepAlgorithm & cctspStepAlgorithm, MAP & map) {

    Graph<T> graph = graphs[map];
    T start, finish;

    try {
        start = menu::selectStart(graph);
        finish = menu::selectFinish(graph);
    }
    catch (const std::out_of_range& outOfRange) {
        return MAIN_MENU;
    }
    float budget = menu::getBudget();

    std::vector<float> scores = menu::calculateScores(pointsOfInterestCategories, preferences);
    std::vector<Vertex<T>*> path = menu::mmpMethod(graph, pointsOfInterest, scores, start, finish, budget, reductionStepAlgorithm, cctspStepAlgorithm);
    //showPath(path);
    showPathOnGraphViewer(path);
    optionsMenu("", {}, BACK);
    return MAIN_MENU;
}

template <class T>
void menu::menuLoop(const std::vector<Graph<T>> & graphs, const std::vector<Vertex<T>*> & pointsOfInterest, const std::vector<POICategory> & pointsOfInterestCategories) {
    std::vector<float> preferences = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    MAP map = PORTO;
    ReductionStepAlgorithm reductionStepAlgorithm = DIJKSTRA;
    CCTSPStepAlgorithm cctspStepAlgorithm = BRANCH_AND_BOUND;

    MenuType menu = MAIN_MENU;
    while (menu != EXIT_MENU) {
        switch (menu) {
            case MAIN_MENU:
                menu = menu::mainMenu();
                break;
            case CALCULATE_TRIP_MENU:
                menu = menu::calculateTripMenu(graphs, preferences, pointsOfInterest, pointsOfInterestCategories,
                                         reductionStepAlgorithm, cctspStepAlgorithm, map);
                break;
            case MAP_MENU:
                menu = menu::mapsMenu(map);
                break;
            case ALGORITHM_MENU:
                menu = menu::algorithmsMenu(reductionStepAlgorithm, cctspStepAlgorithm);
                break;
            case PREFERENCES_MENU:
                menu = menu::preferencesMenu(preferences);
                break;
            default:
                break;
        }
    }
}


#endif // MENU_H
