//
// Created by C Martins on 24/05/2020.
//

#include "menu.h"

#include <iostream>

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

float menu::getFloatValue() {
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


MENU_TYPE menu::mainMenu() {
    int answer = optionsMenu("Main Menu", {"Calculate Trip", "Select Map", "Select Preferences", "Select Algorithm"}, menu::EXIT);
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


float menu::getBudget() {
    optionsMenu("Select Budget", {}, menu::NONE);
    float answer = getFloatValue();
    return answer;
}

std::vector<float> menu::calculateScores(const std::vector<POICategory> & pointsOfInterestCategories, const std::vector<float> preferences) {
    std::vector<float> scores;
    for (POICategory category : pointsOfInterestCategories) {
        scores.push_back(preferences[category]);
    }
    return scores;
}


MENU_TYPE menu::mapsMenu(MAP & map) {
    const std::vector<std::string> maps = {"Porto"}; //TODO
    int answer = optionsMenu("Select Map", maps, menu::BACK);
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

MENU_TYPE menu::algorithmsMenu(REDUCTION_STEP_ALGORITHM & reductionStepAlgorithm, CCTSP_STEP_ALGORITHM & cctspStepAlgorithm) {
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

MENU_TYPE menu::preferencesMenu(std::vector<float> & preferences) {
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

