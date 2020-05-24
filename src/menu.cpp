//
// Created by C Martins on 24/05/2020.
//

#include "menu.h"

#include <iostream>
#include <vector>
#include <string>

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


int optionsMenu(const std::string & title, const std::vector<std::string> & options, bool exit) {
    if (title != "") {
        std::cout << menu::SEPARATOR << std::endl;
        std::cout << '\t' << title << std::endl;
    }
    std::cout << menu::SEPARATOR << std::endl;
    for (size_t index = 0; index < options.size(); index ++) {
        std::cout << " [" << index + 1 << "] " << options[index] << std::endl;
    }
    if (exit) {
        std::cout << " [0] Exit" << std::endl;
    }
    else {
        std::cout << " [0] Back" << std::endl;
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


MENU_TYPE mainMenu() {
    int answer = optionsMenu("Main Menu", {"Calculate Trip", "Select Map", "Select Preferences", "Select Algorithm"}, true);
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

MENU_TYPE calculateTripMenu(REDUCTION_STEP_ALGORITHM & reductionStepAlgorithm, CCTSP_STEP_ALGORITHM & cctspStepAlgorithm, MAP & map) {
    //TODO

    int answer = optionsMenu("", {}, false);
    return MAIN_MENU;
}

MENU_TYPE algorithmsMenu(REDUCTION_STEP_ALGORITHM & reductionStepAlgorithm, CCTSP_STEP_ALGORITHM & cctspStepAlgorithm) {
    int answer = optionsMenu("Select the Reduction Step Algorithm", {"Dijkstra", "Floyd-Warshall"}, false);
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
    answer = optionsMenu("Select the CCTSP Step Algorithm", {"Branch and Bound", "Nearest Neighbour"}, false);
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
    const std::vector<std::string> maps = {"Porto", "Aveiro"}; //TODO
    int answer = optionsMenu("Select Map", maps, false);
    switch (answer) {
        case 0:
            break;
        case 1:
            map = PORTO;
            break;
        case 2:
            map = AVEIRO;
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

    std::cout << menu::SEPARATOR << std::endl;
    std::cout << '\t' << "Select Preferences" << std::endl;
    std::cout << menu::SEPARATOR << std::endl;

    std::string answerStr;
    int answer;
    preferences.clear();
    for (std::string point : interestPointsCategories) {
        std::cout << " " << point << std::endl;
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
        preferences.push_back(answer);
    }
    preferences.push_back(1.0);
    return MAIN_MENU;
}


void menu::menuLoop() {
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
                menu = calculateTripMenu(reductionStepAlgorithm, cctspStepAlgorithm, map);
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
