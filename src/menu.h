//
// Created by C Martins on 24/05/2020.
//

#ifndef CAL_PROJ_MENU_H
#define CAL_PROJ_MENU_H

#include "Graph.h"
#include "parsing.h"
#include "branchAndBound.h"
#include "nearestNeighbour.h"

#include <string>
#include <vector>

enum MENU_TYPE {
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
enum REDUCTION_STEP_ALGORITHM {
    DIJKSTRA,
    FLOYD_WARSHALL
};
enum CCTSP_STEP_ALGORITHM {
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

    MENU_TYPE mainMenu();
    template <class T>
    MENU_TYPE calculateTripMenu(const std::vector<Graph<T>> & graph, std::vector<float> preferences,
                                const std::vector<Vertex<T>*> & pointsOfInterest, const std::vector<POICategory> & pointsOfInterestCategories,
                                REDUCTION_STEP_ALGORITHM & reductionStepAlgorithm, CCTSP_STEP_ALGORITHM & cctspStepAlgorithm, MAP & map);
    MENU_TYPE mapsMenu(MAP & map);
    MENU_TYPE algorithmsMenu(REDUCTION_STEP_ALGORITHM & reductionStepAlgorithm, CCTSP_STEP_ALGORITHM & cctspStepAlgorithm);
    MENU_TYPE preferencesMenu(std::vector<float> & preferences);

    template <class T>
    std::vector<Vertex<T>*> mmpMethod(
            Graph<T> & graph,
            const std::vector<Vertex<T>*>& pointsOfInterest,
            const std::vector<float>& scores,
            const T& start,
            const T& finish,
            float budget,
            const REDUCTION_STEP_ALGORITHM & reductionStepAlgorithm,
            const CCTSP_STEP_ALGORITHM & cctspStepAlgorithm
    );

    template <class T>
    void showPath(const std::vector<Vertex<T>*> & path);

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
        const REDUCTION_STEP_ALGORITHM & reductionStepAlgorithm,
        const CCTSP_STEP_ALGORITHM & cctspStepAlgorithm
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
    menu::optionsMenu("", {}, menu::BACK);
}

template<class T>
MENU_TYPE menu::calculateTripMenu(const std::vector<Graph<T>> & graphs, std::vector<float> preferences,
                            const std::vector<Vertex<T>*> & pointsOfInterest, const std::vector<POICategory> & pointsOfInterestCategories,
                            REDUCTION_STEP_ALGORITHM & reductionStepAlgorithm, CCTSP_STEP_ALGORITHM & cctspStepAlgorithm, MAP & map) {

    Graph<T> graph = graphs[map];

    T start = menu::selectStart(graph);
    T finish = menu::selectFinish(graph);
    float budget = menu::getBudget();

    std::vector<float> scores = menu::calculateScores(pointsOfInterestCategories, preferences);
    std::vector<Vertex<T>*> path = menu::mmpMethod(graph, pointsOfInterest, scores, start, finish, budget, reductionStepAlgorithm, cctspStepAlgorithm);
    showPath(path);
    optionsMenu("", {}, BACK);
    return MAIN_MENU;
}

template <class T>
void menu::menuLoop(const std::vector<Graph<T>> & graphs, const std::vector<Vertex<T>*> & pointsOfInterest, const std::vector<POICategory> & pointsOfInterestCategories) {
    std::vector<float> preferences = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    MAP map = PORTO;
    REDUCTION_STEP_ALGORITHM reductionStepAlgorithm = DIJKSTRA;
    CCTSP_STEP_ALGORITHM cctspStepAlgorithm = BRANCH_AND_BOUND;

    MENU_TYPE menu = MAIN_MENU;
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



#endif //CAL_PROJ_MENU_H
