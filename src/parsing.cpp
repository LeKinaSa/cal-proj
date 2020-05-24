
#include "parsing.h"
#include "VertexInfo.h"

#include <iostream>
#include <fstream>
#include <cmath>

const float EARTH_RADIUS_M = 6371000;

float degreesToRadians(float degrees) {
    return M_PI * degrees / 180.0;
}

float haversineDistance(const VertexInfo& from, const VertexInfo& to) {
    float fromLatRad = degreesToRadians(from.getLatitude());
    float fromLongRad = degreesToRadians(from.getLongitude());
    float toLatRad = degreesToRadians(to.getLatitude());
    float toLongRad = degreesToRadians(to.getLongitude());

    float latitudeDiff = toLatRad - fromLatRad;
    float longitudeDiff = toLongRad - fromLongRad;

    return 2 * EARTH_RADIUS_M * asin(sqrt(pow(sin(latitudeDiff / 2), 2) +
            cos(fromLatRad) * cos(toLatRad) * pow(sin(longitudeDiff / 2), 2)));
}

// Interpret lat and long as x and y values (use euclidean distance formula)
float euclideanDistance(const VertexInfo& from, const VertexInfo& to) {
    return sqrt(pow(from.getLatitude() - to.getLatitude(), 2) + pow(from.getLongitude() - to.getLongitude(), 2));
}


void parseVertexFile(const std::string& path, Graph<VertexInfo>& graph) {
    std::ifstream ifs;
    ifs.open(path);

    size_t numVertices;
    ifs >> numVertices;

    std::string line;
    unsigned int id;
    float latitude, longitude;

    char c;

    for (size_t i = 0; i < numVertices; ++i) {
        ifs >> c >> id >> c >> latitude >> c >> longitude >> c;

        graph.addVertex(VertexInfo(id, latitude, longitude));
    }

    ifs.close();
}

void parseEdgeFile(const std::string& path, Graph<VertexInfo>& graph, bool haversine) {
    std::ifstream ifs;

    ifs.open(path);

    size_t numEdges;
    ifs >> numEdges;

    std::string line;
    unsigned int idSource, idDest;
    char c;

    for (size_t i = 0; i < numEdges; ++i) {
        ifs >> c >> idSource >> c >> idDest >> c;

        Vertex<VertexInfo>* sourcePtr = graph.findVertex(idSource);
        Vertex<VertexInfo>* destPtr = graph.findVertex(idDest);

        float dist = haversine ? haversineDistance(sourcePtr->getInfo(), destPtr->getInfo()) :
                euclideanDistance(sourcePtr->getInfo(), destPtr->getInfo());

        graph.addEdge(idSource, idDest, dist);
    }

    ifs.close();
}

void parseTagsFile(const std::string& path, Graph<VertexInfo>& graph,
        std::vector<Vertex<VertexInfo>*>& pointsOfInterest, std::vector<POICategory>& categories) {

    std::ifstream ifs(path);

    size_t numTags;
    ifs >> numTags;

    size_t numEntries;
    std::string line;
    POICategory category;
    unsigned int id;

    for (size_t i = 0; i < numTags; ++i) {
        ifs >> line;
        category = categoryStringMap.at(line);

        ifs >> numEntries;

        for (size_t j = 0; j < numEntries; ++j) {
            ifs >> id;
            pointsOfInterest.push_back(graph.findVertex(id));
            categories.push_back(category);
        }
    }
}
