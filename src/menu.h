//
// Created by C Martins on 24/05/2020.
//

#ifndef CAL_PROJ_MENU_H
#define CAL_PROJ_MENU_H

#include "Graph.h"
#include "parsing.h"
#include <string>
#include <vector>

namespace menu {
    const std::string SEPARATOR = "----------------------------------------";
    const std::string INPUT = " -> ";
    template <class T>
    void menuLoop(const std::vector<Graph<T>> & graphs, const std::vector<Vertex<T>*> & pointsOfInterest, const std::vector<POICategory> & pointsOfInterestCategories);
    // const std::vector<Graph<T>> & graphs
}


#endif //CAL_PROJ_MENU_H
