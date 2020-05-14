#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

template<class T> class Edge;
template<class T> class Graph;

template<class T>
class Vertex {
public:
    const T& getInfo() const;
    double getDist() const;
    Vertex<T>* getPath() const;

    void addEdge(Vertex<T>* dest, double weight);

    friend class Graph<T>;
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

}


#endif // GRAPH_H
