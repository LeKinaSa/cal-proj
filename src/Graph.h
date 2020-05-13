#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

template<class T> class Edge;
template<class T> class Graph;

template<class T>
class Vertex {
public:
    const T& getInfo();

    friend class Graph<T>;
private:
    explicit Vertex(T info);

    T info;
};

template<class T>
Vertex<T>::Vertex(T info) {
    this->info = info;
}

template<class T>
const T& Vertex<T>::getInfo() {
    return info;
}

template<class T>
class Graph {
private:
    std::vector<Vertex<T>*> vertexSet;
};

#endif // GRAPH_H
