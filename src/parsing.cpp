
#include "parsing.h"
#include "VertexInfo.h"

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


void parseVertexFile(const std::string& path, Graph<VertexInfo>& graph) {
    std::ifstream ifs(path);

    size_t numVertices;
    ifs >> numVertices;

    std::string line;
    unsigned int id;
    float latitude, longitude;

    for (size_t i = 0; i < numVertices; ++i) {
        getline(ifs, line);
        sscanf(line.c_str(), "(%ud, %f, %f)", &id, &latitude, &longitude);

        graph.addVertex(VertexInfo(id, latitude, longitude));
    }

    ifs.close();
}

void parseEdgeFile(const std::string& path, Graph<VertexInfo>& graph) {
    std::ifstream ifs(path);

    size_t numEdges;
    ifs >> numEdges;

    std::string line;
    unsigned int idSource, idDest;

    for (size_t i = 0; i < numEdges; ++i) {
        getline(ifs, line);
        sscanf(line.c_str(), "(%ud, %ud)", &idSource, &idDest);

        Vertex<VertexInfo>* sourcePtr = graph.findVertex(idSource);
        Vertex<VertexInfo>* destPtr = graph.findVertex(idDest);

        graph.addEdge(idSource, idDest, haversineDistance(sourcePtr->getInfo(), destPtr->getInfo()));
    }

    ifs.close();
}

void parseTagsFile(const std::string& path, std::vector<Vertex<VertexInfo>*>& pointsOfInterest,
        std::vector<float>& scores) {
    std::ifstream ifs(path);

    // TODO: Parse tags file according to user preferences
}
