#ifndef PARSING_H
#define PARSING_H

#include <string>
#include <map>

#include "Graph.h"
#include "VertexInfo.h"

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

void parseVertexFile(const std::string& path, Graph<VertexInfo>& graph);
void parseEdgeFile(const std::string& path, Graph<VertexInfo>& graph, bool haversine = true);
void parseTagsFile(const std::string& path, Graph<VertexInfo>& graph,
        std::vector<Vertex<VertexInfo>*>& pointsOfInterest, std::vector<POICategory>& categories);

#endif // PARSING_H
