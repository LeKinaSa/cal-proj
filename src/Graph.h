#ifndef GRAPH_H
#define GRAPH_H

#include "MutablePriorityQueue.h"

#include <iostream>
#include <vector>
#include <string>
#include <limits>

template<class T> class Edge;
template<class T> class Graph;

constexpr float MAX_FLOAT = std::numeric_limits<float>::max();

template<class T>
class Vertex {
public:
    const T& getInfo() const;
    float getDist() const;
    Vertex<T>* getPath() const;

    void addEdge(Vertex<T>* dest, float weight);

    bool isPOI(const std::vector<Vertex<T>*>& pointsOfInterest);

    // Required by MutablePriorityQueue
    bool operator<(const Vertex<T>& vertex) const;

    friend class Graph<T>;

    // Required by MutablePriorityQueue
    int queueIndex = 0;
private:
    explicit Vertex(T info);

    T info;
    std::vector<Edge<T>> adj;

    // Fields used in Dijkstra's Shortest Path
    float dist = 0;
    Vertex<T>* path = nullptr;
};

template<class T>
Vertex<T>::Vertex(T info) : info(info) {}

template<class T>
const T& Vertex<T>::getInfo() const {
    return info;
}

template<class T>
float Vertex<T>::getDist() const {
    return dist;
}

template<class T>
Vertex<T>* Vertex<T>::getPath() const {
    return path;
}

template<class T>
void Vertex<T>::addEdge(Vertex<T>* dest, float weight) {
    adj.push_back(Edge<T>(dest, weight));
}

template <class T>
bool Vertex<T>::isPOI(const std::vector<Vertex<T>*>& pointsOfInterest) {
    for (Vertex<T> * u : pointsOfInterest) {
        if (this->getInfo() == u->getInfo()) {
            return true;
        }
    }
    return false;
}

template<class T>
bool Vertex<T>::operator<(const Vertex<T>& vertex) const {
    return dist < vertex.dist;
}



template<class T>
class Edge {
public:
    Edge(Vertex<T>* dest, float weight);
    friend class Vertex<T>;
    friend class Graph<T>;
private:
    Vertex<T>* dest;
    float weight;
};

template<class T>
Edge<T>::Edge(Vertex<T>* dest, float weight) : dest(dest), weight(weight) {}


/**
 * Class for representing a Graph with weighted directed edges.
 */
template<class T>
class Graph {
public:
    ~Graph();

    std::vector<Vertex<T>*> getVertexSet() const;
    std::vector<std::string> getStringList() const;

    Vertex<T>* findVertex(const T& info);
    bool addVertex(const T& info);
    bool addEdge(const T& source, const T& dest, float weight);

    void dijkstraShortestPath(const T& source);

    std::vector<std::vector<float>> generateAdjacencyMatrixWithDijkstra(
            const std::vector<Vertex<T>*>& pointsOfInterest, Vertex<T> * start, Vertex<T> * finish);
private:
    std::vector<Vertex<T>*> vertexSet;
};

template<class T>
Graph<T>::~Graph() {
    for (Vertex<T>* v : vertexSet) {
        delete v;
    }
}

template<class T>
std::vector<Vertex<T>*> Graph<T>::getVertexSet() const {
    return vertexSet;
}

template <class T>
std::vector<std::string> Graph<T>::getStringList() const {
    std::vector<std::string> stringList;
    std::string answer;

    for (Vertex<T>* vertex : vertexSet) {
        std::cout << vertex->getInfo() << std::endl;
        getline(std::cin, answer);
        stringList.push_back(answer);
    }

    return stringList;
}

template<class T>
Vertex<T>* Graph<T>::findVertex(const T& info) {
    for (auto v : vertexSet) {
        if (v->info == info) {
            return v;
        }
    }
    return nullptr;
}

template<class T>
bool Graph<T>::addVertex(const T& info) {
    if (findVertex(info) != nullptr)
        return false;

    vertexSet.push_back(new Vertex<T>(info));
    return true;
}

template<class T>
bool Graph<T>::addEdge(const T& source, const T& dest, float weight) {
    Vertex<T>* sourcePtr = findVertex(source);
    Vertex<T>* destPtr = findVertex(dest);

    if (sourcePtr == nullptr || destPtr == nullptr)
        return false;

    sourcePtr->addEdge(destPtr, weight);
    return true;
}

template<class T>
void Graph<T>::dijkstraShortestPath(const T& source) {
    MutablePriorityQueue<Vertex<T>> queue;

    for (Vertex<T>* vertex : vertexSet) {
        vertex->path = NULL;

        if (vertex->info == source) {
            vertex->dist = 0;
            vertex->queueIndex = 0;
            queue.insert(vertex);
        }
        else {
            vertex->dist = MAX_FLOAT;
        }
    }

    while (!queue.empty()) {
        Vertex<T>* vertex = queue.extractMin();

        for (const Edge<T>& edge : vertex->adj) {
            bool notInQueue = edge.dest->dist == MAX_FLOAT;

            if (edge.dest->dist > vertex->dist + edge.weight) {
                edge.dest->dist = vertex->dist + edge.weight;
                edge.dest->path = vertex;

                if (notInQueue) {
                    edge.dest->queueIndex = edge.dest->dist;
                    queue.insert(edge.dest);
                }
                else {
                    queue.decreaseKey(edge.dest);
                }
            }
        }
    }
}

template <class T>
std::vector<std::vector<float>> Graph<T>::generateAdjacencyMatrixWithDijkstra(
        const std::vector<Vertex<T>*>& pointsOfInterest, Vertex<T> * start, Vertex<T> * finish) {
    std::vector<std::vector<float>> adjacencyMatrix;
    adjacencyMatrix.resize(pointsOfInterest.size() + 1);

    dijkstraShortestPath(start->getInfo());
    adjacencyMatrix[0].push_back(start->getDist());
    for (Vertex<T>* POI : pointsOfInterest) {
        adjacencyMatrix[0].push_back(POI->getDist());
    }

    for (int i = 0; i < pointsOfInterest.size(); ++i) {
        dijkstraShortestPath(pointsOfInterest[i]->getInfo());
        adjacencyMatrix[i + 1].push_back(finish->getDist());
        for (int j = 0; j < pointsOfInterest.size(); ++j) {
            adjacencyMatrix[i + 1].push_back(pointsOfInterest[j]->getDist());
        }
    }

    return adjacencyMatrix;
}

#endif // GRAPH_H
