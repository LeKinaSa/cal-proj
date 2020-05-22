#ifndef PARSING_H
#define PARSING_H

#include <string>

#include "Graph.h"
#include "VertexInfo.h"

void parseVertexFile(const std::string& path, Graph<VertexInfo>& graph);
void parseEdgeFile(const std::string& path, Graph<VertexInfo>& graph);
void parseTagsFile(const std::string& path, std::vector<Vertex<VertexInfo>*>& pointsOfInterest,
        std::vector<float>& scores);

#endif // PARSING_H
