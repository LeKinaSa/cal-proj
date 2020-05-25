#include "menu.h"
#include "../lib/graphviewer.h"

#include <iostream>
#include <algorithm>

int menu::optionsMenu(const std::string & title, const std::vector<std::string> & options, OPTION option) {
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
        catch (const std::exception& e) {
            std::cerr << "Invalid int." << std::endl;
            continue;
        }

        if ((answer >= 0) && (answer <= options.size())) {
            break;
        }
    }

    return answer;
}

float menu::getFloatValue() {
    std::string answerStr;
    float answer;

    while (true) {
        std::cout << menu::INPUT;
        getline(std::cin, answerStr);
        try {
            answer = stof(answerStr);
        }
        catch (const std::exception& e) {
            std::cerr << "Invalid float." << std::endl;
            continue;
        }
        break;
    }

    return answer;
}



float menu::getBudget() {
    optionsMenu("Select Budget", {}, menu::NONE);
    float answer = getFloatValue();
    return answer;
}

std::vector<float> menu::calculateScores(const std::vector<POICategory> & pointsOfInterestCategories,
        const std::vector<float>& preferences) {
    std::vector<float> scores;
    for (POICategory category : pointsOfInterestCategories) {
        scores.push_back(preferences[category]);
    }
    return scores;
}


MenuType menu::mainMenu() {
    int answer = optionsMenu("Main Menu", { "Calculate Trip", "Select Map", "Select Algorithm", "Select Preferences" }, menu::EXIT);
    switch (answer) {
        case 0:
            return EXIT_MENU;
        case 1:
            return CALCULATE_TRIP_MENU;
        case 2:
            return MAP_MENU;
        case 3:
            return ALGORITHM_MENU;
        case 4:
            return PREFERENCES_MENU;
        default:
            return MAIN_MENU;
    }
}

MenuType menu::mapsMenu(CityMap & map) {
    const std::vector<std::string> maps = {"Grid 4x4", "Grid 8x8", "Grid 16x16", "Report"};

    int answer = optionsMenu("Select Map", maps, menu::BACK);

    if (answer >= 1 && answer <= 4) {
        map = static_cast<CityMap>(answer - 1);
    }

    return MAIN_MENU;
}

MenuType menu::preferencesMenu(std::vector<float> & preferences) {
    const std::vector<std::string> interestPointsCategories = { "Information", "Hotel", "Attraction", "Viewpoint",
                                                                "Guest House", "Picnic Site","Artwork", "Camp Site",
                                                                "Museum" };

    optionsMenu("Select Preferences", {}, menu::NONE);

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

MenuType menu::algorithmsMenu(ReductionStepAlgorithm & reductionStepAlgorithm, CCTSPStepAlgorithm & cctspStepAlgorithm) {
    int answer = optionsMenu("Select the Reduction Step Algorithm", {"Dijkstra", "Floyd-Warshall"}, menu::BACK);
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
    answer = optionsMenu("Select the CCTSP Step Algorithm", {"Branch and Bound", "Nearest Neighbour"}, menu::BACK);
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

void menu::showPathOnGraphViewer(const std::vector<Vertex<char>*> & path,
        const std::vector<Vertex<char>*>& pointsOfInterest) {
    GraphViewer *gv = new GraphViewer(600, 600, true);
    gv->createWindow(600, 600);
    gv->defineVertexColor(WHITE);

    char c;

    if (!path.empty()) {
        c = path[0]->getInfo();

        gv->addNode(c);
        gv->setVertexLabel(c, std::string(1, c));
        gv->setVertexColor(c, BLUE);
    }

    for (int index = 1; index < path.size(); ++index) {
        c = path[index]->getInfo();

        gv->addNode(c);
        gv->setVertexLabel(c, std::string(1, c));

        gv->addEdge(index - 1, path[index - 1]->getInfo(), c, EdgeType::DIRECTED);

        if (index == path.size() - 1) {
            gv->setVertexColor(c, YELLOW);
        }
    }

    for (auto v : path) {
        if (std::find(pointsOfInterest.begin(), pointsOfInterest.end(), v) != pointsOfInterest.end()) {
            gv->setVertexColor(v->getInfo(), GRAY);
        }
    }

    gv->rearrange();
}

void menu::showPathOnGraphViewer(const std::vector<Vertex<PosInfo>*> & path,
        const std::vector<Vertex<PosInfo>*>& pointsOfInterest) {
    GraphViewer *gv = new GraphViewer(600, 600, false);
    gv->createWindow(600, 600);
    gv->defineVertexColor(WHITE);

    if (!path.empty()) {
        gv->addNode(path[0]->getInfo().getId(), path[0]->getInfo().getX(), path[0]->getInfo().getY());
        gv->setVertexColor(path[0]->getInfo().getId(), BLUE);
    }

    for (int index = 1; index < path.size(); ++ index) {
        gv->addNode(path[index]->getInfo().getId(), path[index]->getInfo().getX(), path[index]->getInfo().getY());
        gv->addEdge(index - 1, path[index - 1]->getInfo().getId(), path[index]->getInfo().getId(), EdgeType::DIRECTED);

        if (index == path.size() - 1) {
            gv->setVertexColor(path[index]->getInfo().getId(), YELLOW);
        }
    }

    for (auto v : path) {
        if (std::find(pointsOfInterest.begin(), pointsOfInterest.end(), v) != pointsOfInterest.end()) {
            gv->setVertexColor(v->getInfo().getId(), GRAY);
        }
    }

    gv->rearrange();
}


unsigned int menu::selectVertex(const Graph<PosInfo>& graph) {
    std::cout << "Enter a vertex id between 0 and " << graph.getVertexSet().size() - 1 << std::endl;

    std::string answerStr;
    unsigned int answer;

    while (true) {
        std::cout << menu::INPUT;
        getline(std::cin, answerStr);
        try {
            answer = stoul(answerStr);
        }
        catch (const std::exception& e) {
            std::cerr << "Invalid unsigned int." << std::endl;
            continue;
        }
        if ((answer >= 0) && (answer < graph.getVertexSet().size() - 1)) {
            break;
        }
    }
    return answer;
}


void menu::menuLoop() {
    std::vector<float> preferences = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    CityMap map = GRID_4X4;
    ReductionStepAlgorithm reductionStepAlgorithm = DIJKSTRA;
    CCTSPStepAlgorithm cctspStepAlgorithm = BRANCH_AND_BOUND;

    MenuType menu = MAIN_MENU;
    while (menu != EXIT_MENU) {
        switch (menu) {
            case MAIN_MENU:
                menu = menu::mainMenu();
                break;
            case CALCULATE_TRIP_MENU:
                menu = menu::calculateTripMenu(preferences, reductionStepAlgorithm, cctspStepAlgorithm, map);
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
    pointsOfInterest.push_back(graph.findVertex('i'));
    pointsOfInterest.push_back(graph.findVertex('j'));
    pointsOfInterest.push_back(graph.findVertex('h'));

    scores.push_back(1.0);
    scores.push_back(3.0);
    scores.push_back(4.0);
    scores.push_back(2.0);
}

MenuType menu::calculateTripMenu(const std::vector<float>& preferences,
                                 const ReductionStepAlgorithm & reductionStepAlgorithm,
                                 const CCTSPStepAlgorithm & cctspStepAlgorithm, const CityMap & map) {

    if (map == REPORT) {
        Graph<char> graph;
        std::vector<Vertex<char>*> pointsOfInterest;
        std::vector<float> scores;

        initReportGraph(graph, pointsOfInterest, scores);

        std::vector<Vertex<char>*> path = mmpMethod(graph, pointsOfInterest, scores, 's', 'f',
                                                    12, reductionStepAlgorithm, cctspStepAlgorithm);

        showPath(path);

        showPathOnGraphViewer(path, pointsOfInterest);
    }
    else {
        Graph<PosInfo> graph;
        std::vector<Vertex<PosInfo>*> pointsOfInterest;
        std::vector<POICategory> categories;

        std::string filePath;

        if (map == GRID_4X4) filePath = "maps/4x4/";
        else if (map == GRID_8X8) filePath = "maps/8x8/";
        else filePath = "maps/16x16/";

        parseVertexFile(filePath + "nodes.txt", graph);
        parseEdgeFile(filePath + "edges.txt", graph, false);
        parseTagsFile(filePath + "tags.txt", graph, pointsOfInterest, categories);

        std::vector<float> scores = calculateScores(categories, preferences);

        optionsMenu("Start vertex", {}, menu::NONE);
        unsigned int start = selectVertex(graph);
        optionsMenu("Finish vertex", {}, menu::NONE);
        unsigned int finish = selectVertex(graph);

        float budget = getBudget();

        std::vector<Vertex<PosInfo>*> path = mmpMethod(graph, pointsOfInterest, scores,
                                                       PosInfo(start), PosInfo(finish), budget, reductionStepAlgorithm, cctspStepAlgorithm);

        showPath(path);

        showPathOnGraphViewer(path, pointsOfInterest);
    }

    optionsMenu("", {}, BACK);
    return MAIN_MENU;
}


