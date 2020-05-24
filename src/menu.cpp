//
// Created by C Martins on 24/05/2020.
//

#include "menu.h"
#include "branchAndBound.h"
#include "nearestNeighbour.h"

#include <iostream>

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

enum OPTION {
        EXIT,
        BACK,
        NONE
    };

int optionsMenu(const std::string & title, const std::vector<std::string> & options, OPTION option) {
    if (title != "") {
        std::cout << menu::SEPARATOR << std::endl;
        std::cout << '\t' << title << std::endl;
    }
    std::cout << menu::SEPARATOR << std::endl;
    for (size_t index = 0; index < options.size(); index ++) {
        std::cout << " [" << index + 1 << "] " << options[index] << std::endl;
    }

    switch (option) {
        case EXIT:
            std::cout << " [0] Exit" << std::endl;
            break;
        case BACK:
            std::cout << " [0] Back" << std::endl;
            break;
        case NONE:
            return 0;
        default:
            return 0;
    }

    std::string answerStr;
    int answer;

    while (true) {
        std::cout << menu::INPUT;
        getline(std::cin, answerStr);
        try {
            answer = stoi(answerStr);
        }
        catch (std::exception e) {
            continue;
        }
        if ((answer >= 0) && (answer <= options.size())) {
            break;
        }
    }
    return answer;
}

float getFloatValue() {
    std::string answerStr;
    float answer;
    while (true) {
        std::cout << menu::INPUT;
        getline(std::cin, answerStr);
        try {
            answer = stof(answerStr);
        }
        catch (std::exception e) {
            continue;
        }
        break;
    }
    return answer;
}

MENU_TYPE mainMenu() {
    int answer = optionsMenu("Main Menu", {"Calculate Trip", "Select Map", "Select Preferences", "Select Algorithm"}, EXIT);
    switch (answer) {
        case 0:
            return EXIT_MENU;
        case 1:
            return CALCULATE_TRIP_MENU;
        case 2:
            return MAP_MENU;
        case 3:
            return PREFERENCES_MENU;
        case 4:
            return ALGORITHM_MENU;
        default:
            return MAIN_MENU;
    }
}

template<class T>
std::vector<Vertex<T>*> mmpMethod(
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
T selectStart(const Graph<T> & graph) {
    int answer = optionsMenu("Select Start Vertex", graph.getStringList(), BACK);
    return (graph.getVertexSet().at(answer - 1))->getInfo();
}

template<class T>
T selectFinish(const Graph<T> & graph) {
    int answer = optionsMenu("Select Finish Vertex", graph.getStringList(), BACK);
    return (graph.getVertexSet().at(answer - 1))->getInfo();
}

float getBudget() {
    optionsMenu("Select Budget", {}, NONE);
    float answer = getFloatValue();
    return answer;
}

std::vector<float> calculateScores(const std::vector<POICategory> & pointsOfInterestCategories, const std::vector<float> preferences) {
    std::vector<float> scores;
    for (POICategory category : pointsOfInterestCategories) {
        scores.push_back(preferences[category]);
    }
    return scores;
}

template <class T>
void showPath(const std::vector<Vertex<T>*> & path) {
    optionsMenu("Best Path", {}, NONE);
    std::cout << (path[0])->getInfo();
    for (int index = 1; index < path.size(); ++ index) {
        std::cout << " - " << (path[index])->getInfo();
    }
    std::cout << std::endl;
    optionsMenu("", {}, BACK);
}

template<class T>
MENU_TYPE calculateTripMenu(const std::vector<Graph<T>> & graph, std::vector<float> preferences,
        const std::vector<Vertex<T>*> & pointsOfInterest, const std::vector<POICategory> & pointsOfInterestCategories,
        REDUCTION_STEP_ALGORITHM & reductionStepAlgorithm, CCTSP_STEP_ALGORITHM & cctspStepAlgorithm, MAP & map) {

    //Graph<Vertex<T>*> graph = graphs[map];

    T start = selectStart(graph);
    T finish = selectFinish(graph);
    float budget = getBudget();

    std::vector<float> scores = calculateScores(pointsOfInterest, pointsOfInterestCategories, preferences);
    std::vector<Vertex<T>*> path = mmpMethod(graph, pointsOfInterest, scores, start, finish, budget, reductionStepAlgorithm, cctspStepAlgorithm);
    showPath(path);
    optionsMenu("", {}, BACK);
    return MAIN_MENU;
}

MENU_TYPE algorithmsMenu(REDUCTION_STEP_ALGORITHM & reductionStepAlgorithm, CCTSP_STEP_ALGORITHM & cctspStepAlgorithm) {
    int answer = optionsMenu("Select the Reduction Step Algorithm", {"Dijkstra", "Floyd-Warshall"}, BACK);
    switch (answer) {
        case 0:
            return MAIN_MENU;
        case 1:
            reductionStepAlgorithm = DIJKSTRA;
            break;
        case 2:
            reductionStepAlgorithm = FLOYD_WARSHALL;
            break;
        default:
            return MAIN_MENU;
    }
    answer = optionsMenu("Select the CCTSP Step Algorithm", {"Branch and Bound", "Nearest Neighbour"}, BACK);
    switch (answer) {
        case 0:
            return MAIN_MENU;
        case 1:
            cctspStepAlgorithm = BRANCH_AND_BOUND;
            break;
        case 2:
            cctspStepAlgorithm = NEAREST_NEIGHBOUR;
            break;
        default:
            return MAIN_MENU;
    }
    return MAIN_MENU;
}

MENU_TYPE mapsMenu(MAP & map) {
    const std::vector<std::string> maps = {"Porto"}; //TODO
    int answer = optionsMenu("Select Map", maps, BACK);
    switch (answer) {
        case 0:
            break;
        case 1:
            map = PORTO;
            break;
        default:
            break;
    }
    return MAIN_MENU;
}

MENU_TYPE preferencesMenu(std::vector<float> & preferences) {
    const std::vector<std::string> interestPointsCategories = { "Information", "Hotel", "Attraction", "Viewpoint",
                                                                "Guest House", "Picnic Site","Artwork", "Camp Site",
                                                                "Museum" };

    optionsMenu("Select Preferences", {}, NONE);

    float answer;
    preferences.clear();
    for (std::string point : interestPointsCategories) {
        std::cout << " " << point << std::endl;
        answer = getFloatValue();
        preferences.push_back(answer);
    }
    preferences.push_back(1.0);
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
                menu = mainMenu();
                break;
            case CALCULATE_TRIP_MENU:
                menu = calculateTripMenu(graphs, preferences, pointsOfInterest, pointsOfInterestCategories,
                        reductionStepAlgorithm, cctspStepAlgorithm, map);
                break;
            case MAP_MENU:
                menu = mapsMenu(map);
                break;
            case ALGORITHM_MENU:
                menu = algorithmsMenu(reductionStepAlgorithm, cctspStepAlgorithm);
                break;
            case PREFERENCES_MENU:
                menu = preferencesMenu(preferences);
                break;
            default:
                break;
        }
    }
}

