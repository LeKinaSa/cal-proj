#ifndef GRAPH_H
#define GRAPH_H

#include "MutablePriorityQueue.h"

#include <vector>
#include <limits>

template<class T> class Edge;
template<class T> class Graph;

constexpr double MAX_DOUBLE = std::numeric_limits<double>::max();

template<class T>
class Vertex {
public:
    const T& getInfo() const;
    double getDist() const;
    Vertex<T>* getPath() const;

    void addEdge(Vertex<T>* dest, double weight);

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
    double dist = 0;
    Vertex<T>* path = nullptr;
};

template<class T>
Vertex<T>::Vertex(T info) : info(info) {}

template<class T>
const T& Vertex<T>::getInfo() const {
    return info;
}

template<class T>
double Vertex<T>::getDist() const {
    return dist;
}

template<class T>
Vertex<T>* Vertex<T>::getPath() const {
    return path;
}

template<class T>
void Vertex<T>::addEdge(Vertex<T>* dest, double weight) {
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
    Edge(Vertex<T>* dest, double weight);
    friend class Vertex<T>;
    friend class Graph<T>;
private:
    Vertex<T>* dest;
    double weight;
};

template<class T>
Edge<T>::Edge(Vertex<T>* dest, double weight) : dest(dest), weight(weight) {}


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
    bool addEdge(const T& source, const T& dest, double weight);

    void dijkstraShortestPath(const T& source);
    void floydWarshallShortestPath(std::vector<std::vector<double>> & weight, std::vector<std::vector<int>> & path);
    std::vector<std::vector<double>> initializeFloydWarshallWeightVector();
    std::vector<std::vector< int  >> initializeFloydWarshallPathVector();

    std::vector<std::vector<double>> generateAdjacencyMatrixWithFloydWarshall(const std::vector<Vertex<T>*>& pointsOfInterest, Vertex<T> * start, Vertex<T> * finish);

    std::vector<std::vector<double>> generateAdjacencyMatrixWithDijkstra(
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

/*
 * Finds the index of the vertex with a given content.
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
bool Graph<T>::addEdge(const T& source, const T& dest, double weight) {
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
            vertex->dist = MAX_DOUBLE;
        }
    }

    while (!queue.empty()) {
        Vertex<T>* vertex = queue.extractMin();

        for (const Edge<T>& edge : vertex->adj) {
            bool notInQueue = edge.dest->dist == MAX_DOUBLE;

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

template<class T>
void Graph<T>::floydWarshallShortestPath(std::vector<std::vector<double>> & weight, std::vector<std::vector<int>> & path) {
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
                if (weight[i][k] == MAX_DOUBLE || weight[k][j] == MAX_DOUBLE)
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
std::vector<std::vector<double>> Graph<T>::initializeFloydWarshallWeightVector() {
    std::vector<std::vector<double>> weight;
    std::vector<double> aux;
    for (int i = 0; i < vertexSet.size(); ++ i) {
        for (int j = 0; j < vertexSet.size(); ++j) {
            if (i == j) {
                aux.push_back(0);
            } else {
                aux.push_back(MAX_DOUBLE);
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


template <class T>
std::vector<std::vector<double>> Graph<T>::generateAdjacencyMatrixWithDijkstra(
        const std::vector<Vertex<T>*>& pointsOfInterest, Vertex<T> * start, Vertex<T> * finish) {
    std::vector<std::vector<double>> adjacencyMatrix;
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

template <class T>
std::vector<std::vector<double>> Graph<T>::generateAdjacencyMatrixWithFloydWarshall(const std::vector<Vertex<T>*>& pointsOfInterest, Vertex<T> * start, Vertex<T> * finish) {
    std::vector<std::vector<double>> adjacencyMatrix;
    std::vector<double> aux;

    std::vector<std::vector<double>> weight = initializeFloydWarshallWeightVector();
    std::vector<std::vector< int  >>  path  = initializeFloydWarshallPathVector();
    floydWarshallShortestPath(weight, path);

    int startIndex  = findVertexIdx(start->getInfo());
    int finishIndex = findVertexIdx(finish->getInfo());

    aux.push_back(0);
    for (int j = 0; j < vertexSet.size(); ++ j) {
        if (vertexSet[j]->isPOI(pointsOfInterest)) {
            aux.push_back(weight[startIndex][j]);
        }
    }
    adjacencyMatrix.push_back(aux);
    aux.clear();

    for (int i = 0; i < vertexSet.size(); ++ i) {
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
