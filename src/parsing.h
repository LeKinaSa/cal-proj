#ifndef PARSING_H
#define PARSING_H

#include <string>
#include <map>

#include "Graph.h"
#include "PosInfo.h"

enum POICategory {
    INFORMATION,
    HOTEL,
    ATTRACTION,
    VIEWPOINT,
    GUEST_HOUSE,
    PICNIC_SITE,
    ARTWORK,
    CAMP_SITE,
    MUSEUM,
    UNSPECIFIED
};

const std::map<std::string, POICategory> categoryStringMap = {
        {"tourism=information", INFORMATION},
        {"tourism=hotel", HOTEL},
        {"tourism=attraction", ATTRACTION},
        {"tourism=viewpoint", VIEWPOINT},
        {"tourism=guest_house", GUEST_HOUSE},
        {"tourism=picnic_site", PICNIC_SITE},
        {"tourism=artwork", ARTWORK},
        {"tourism=camp_site", CAMP_SITE},
        {"tourism=museum", MUSEUM},
        {"tourism=*", UNSPECIFIED}
};

/**
 * @brief Parses a text file containing vertex information
 * @param path      path to the file
 * @param graph     reference to the graph where the vertices will be inserted
 */
void parseVertexFile(const std::string& path, Graph<PosInfo>& graph);

/**
 * @brief Parses a text file containing edge information
 * @param path          path to the file
 * @param graph         reference to a graph where the edges will be inserted
 * @param haversine     if true, uses the haversine distance formula to calculate edge weight (set this to true if the
 * x and y values in the vertex info are meant to be latitudes and longitudes), otherwise the euclidean distance is used
 */
void parseEdgeFile(const std::string& path, Graph<PosInfo>& graph, bool haversine = true);

/**
 * Parses a tags file, filling a POI vector and its respective categories
 * @param path              path to the file
 * @param graph             reference to the graph with the points of interest
 * @param pointsOfInterest  reference to the POI vector
 * @param categories        reference to the categories vector (each POI is assigned to a category)
 */
void parseTagsFile(const std::string& path, Graph<PosInfo>& graph,
                   std::vector<Vertex<PosInfo>*>& pointsOfInterest, std::vector<POICategory>& categories);

#endif // PARSING_H
