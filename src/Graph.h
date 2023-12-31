#ifndef GRAPH_H
#define GRAPH_H

#include "MutablePriorityQueue.h"

#include <iostream>
#include <sstream>
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
    const std::vector<Edge<T>>& getAdj() const;

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
const vector<Edge<T>> &Vertex<T>::getAdj() const {
    return adj;
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

    const Vertex<T>* getDest() const;

    friend class Vertex<T>;
    friend class Graph<T>;
private:
    Vertex<T>* dest;
    float weight;
};

template<class T>
Edge<T>::Edge(Vertex<T>* dest, float weight) : dest(dest), weight(weight) {}

template<class T>
const Vertex<T>* Edge<T>::getDest() const { return dest; }


/**
 * Class for representing a Graph with weighted directed edges.
 */
template<class T>
class Graph {
public:
    ~Graph();

    std::vector<Vertex<T>*> getVertexSet() const;

    Vertex<T>* findVertex(const T& info);
    bool addVertex(const T& info);
    bool addEdge(const T& source, const T& dest, float weight);

    void dijkstraShortestPath(const T& source);
    void floydWarshallShortestPath(std::vector<std::vector<float>> & weight, std::vector<std::vector<int>> & path);
    std::vector<std::vector<float>> initializeFloydWarshallWeightVector();
    std::vector<std::vector< int  >> initializeFloydWarshallPathVector();

    std::vector<std::vector<float>> generateAdjacencyMatrixWithFloydWarshall(const std::vector<Vertex<T>*>& pointsOfInterest, Vertex<T> * start, Vertex<T> * finish);

    std::vector<std::vector<float>> generateAdjacencyMatrixWithDijkstra(
            const std::vector<Vertex<T>*>& pointsOfInterest, Vertex<T> * start, Vertex<T> * finish);
private:
    std::vector<Vertex<T>*> vertexSet;
    int findVertexIdx(const T& in) const;
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

template<class T>
Vertex<T>* Graph<T>::findVertex(const T& info) {
    for (auto v : vertexSet) {
        if (v->info == info) {
            return v;
        }
    }
    return nullptr;
}

/**
 * @brief Finds the index of the vertex with a given content.
 */
template <class T>
int Graph<T>::findVertexIdx(const T &in) const {
    for (unsigned i = 0; i < vertexSet.size(); i++)
        if (vertexSet[i]->info == in)
            return i;
    return -1;
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

/**
 * @brief Calculates the shortest path from a given vertex to all others using
 * Dijkstra's algorithm.
 * @param source    source vertex information
 */
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

/**
 * @brief Calculates the shortest paths for every pair of vertices, creating adjacency matrices.
 * @param weight    adjacency matrix for the cost of the paths
 * @param path      adjacency matrix for the next vertex in the paths
 */
template<class T>
void Graph<T>::floydWarshallShortestPath(std::vector<std::vector<float>> & weight, std::vector<std::vector<int>> & path) {
    unsigned n = vertexSet.size();

    for (unsigned i = 0; i < n; i++) {
        for (Edge<T> e : vertexSet[i]->adj) {
            int j = findVertexIdx(e.dest->info);
            weight[i][j] = e.weight;
            path  [i][j] = i;
        }
    }

    for (unsigned k = 0; k < n; k++) {
        for (unsigned i = 0; i < n; i++) {
            for (unsigned j = 0; j < n; j++) {
                if (weight[i][k] == MAX_FLOAT || weight[k][j] == MAX_FLOAT)
                    continue; // avoid overflow
                int val = weight[i][k] + weight[k][j];
                if (val < weight[i][j]) {
                    weight[i][j] = val;
                    path  [i][j] = path[k][j];
                }
            }
        }
    }
}

template<class T>
std::vector<std::vector<float>> Graph<T>::initializeFloydWarshallWeightVector() {
    std::vector<std::vector<float>> weight;
    std::vector<float> aux;
    for (int i = 0; i < vertexSet.size(); ++ i) {
        for (int j = 0; j < vertexSet.size(); ++j) {
            if (i == j) {
                aux.push_back(0);
            } else {
                aux.push_back(MAX_FLOAT);
            }
        }
        weight.push_back(aux);
        aux.clear();
    }
    return weight;
}

template<class T>
std::vector<std::vector<int>> Graph<T>::initializeFloydWarshallPathVector() {
    std::vector<std::vector<int>> path;
    std::vector<int> aux;
    for (int i = 0; i < vertexSet.size(); ++ i) {
        for (int j = 0; j < vertexSet.size(); ++j) {
            if (i == j) {
                aux.push_back(i);
            }
            else {
                aux.push_back(-1);
            }
        }
        path.push_back(aux);
        aux.clear();
    }
    return path;
}

/**
 * @brief Generates the adjacency matrix required for the CCTSP problem with repeated applications of Dijkstra's
 * shortest path, choosing the start vertex and then each point of interest as the source
 * @param pointsOfInterest      list of pointers to vertices which are points of interest
 * @param start                 pointer to start vertex
 * @param finish                pointer to finish vertex
 * @return                      adjacency matrix with shortest paths between all points of interest
 */
template <class T>
std::vector<std::vector<float>> Graph<T>::generateAdjacencyMatrixWithDijkstra(
        const std::vector<Vertex<T>*>& pointsOfInterest, Vertex<T> * start, Vertex<T> * finish) {
    std::vector<std::vector<float>> adjacencyMatrix;
    adjacencyMatrix.resize(pointsOfInterest.size() + 1);

    // We construct the adjacency matrix line to line
    // Starting with the start vertex
    dijkstraShortestPath(start->getInfo());
    adjacencyMatrix[0].push_back(start->getDist());
    for (Vertex<T>* POI : pointsOfInterest) {
        adjacencyMatrix[0].push_back(POI->getDist());
    }

    for (int i = 0; i < pointsOfInterest.size(); ++i) {
        // And then all the other POIs
        // The first value corresponds to the distance from the POI to the finish vertex
        //     Which will be the distance to the start vertex when we look at it in the loop format
        dijkstraShortestPath(pointsOfInterest[i]->getInfo());
        adjacencyMatrix[i + 1].push_back(finish->getDist());
        for (int j = 0; j < pointsOfInterest.size(); ++j) {
            adjacencyMatrix[i + 1].push_back(pointsOfInterest[j]->getDist());
        }
    }

    return adjacencyMatrix;
}

/**
 * @brief Generates the adjacency matrix required for the CCTSP problem by filtering the adjacency matrix generated by
 * the Floyd-Warshall algorithm, selecting only vertices which are points of interest
 * @param pointsOfInterest      list of pointers to vertices which are points of interest
 * @param start                 pointer to start vertex
 * @param finish                pointer to finish vertex
 * @return                      adjacency matrix with shortest paths between all points of interest
 */
template <class T>
std::vector<std::vector<float>> Graph<T>::generateAdjacencyMatrixWithFloydWarshall(const std::vector<Vertex<T>*>& pointsOfInterest, Vertex<T> * start, Vertex<T> * finish) {
    std::vector<std::vector<float>> adjacencyMatrix;
    std::vector<float> aux;

    std::vector<std::vector<float>> weight = initializeFloydWarshallWeightVector();
    std::vector<std::vector< int  >>  path  = initializeFloydWarshallPathVector();
    floydWarshallShortestPath(weight, path);

    int startIndex  = findVertexIdx(start->getInfo());
    int finishIndex = findVertexIdx(finish->getInfo());

    // We construct the adjacency matrix line to line
    // Starting with the start vertex
    aux.push_back(0);
    for (int j = 0; j < vertexSet.size(); ++ j) {
        if (vertexSet[j]->isPOI(pointsOfInterest)) {
            aux.push_back(weight[startIndex][j]);
        }
    }
    adjacencyMatrix.push_back(aux);
    aux.clear();

    for (int i = 0; i < vertexSet.size(); ++ i) {
        // And then all the other POIs
        // The first value corresponds to the distance from the POI to the finish vertex
        //     Which will be the distance to the start vertex when we look at it in the loop format
        if (vertexSet[i]->isPOI(pointsOfInterest)) {
            aux.push_back(weight[i][finishIndex]);
            for (int j = 0; j < vertexSet.size(); ++ j) {
                if (vertexSet[j]->isPOI(pointsOfInterest)) {
                    aux.push_back(weight[i][j]);
                }
            }
            adjacencyMatrix.push_back(aux);
            aux.clear();
        }
    }
    return adjacencyMatrix;
}


#endif // GRAPH_H
