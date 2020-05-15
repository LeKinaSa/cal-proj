#include "Graph.h"

#include <vector>

void initReportGraph(Graph<char>& graph, std::vector<Vertex<char>*>& pointsOfInterest, std::vector<double>& scores) {
    for (char c = 'a'; c <= 'k'; ++c) {
        graph.addVertex(c);
    }

    graph.addVertex('s');

    graph.addEdge('a', 's', 4.0);
    graph.addEdge('a', 'i', 5.0);
    graph.addEdge('b', 'c', 4.0);
    graph.addEdge('b', 's', 3.0);
    graph.addEdge('b', 'g', 7.0);
    graph.addEdge('c', 'b', 4.0);
    graph.addEdge('c', 'e', 4.0);
    graph.addEdge('d', 's', 2.0);
    graph.addEdge('d', 'e', 3.0);
    graph.addEdge('e', 'd', 3.0);
    graph.addEdge('e', 'i', 3.0);
    graph.addEdge('e', 'k', 2.0);
    graph.addEdge('e', 'j', 2.0);
    graph.addEdge('f', 'h', 1.0);
    graph.addEdge('f', 'j', 1.0);
    graph.addEdge('f', 'k', 2.0);
    graph.addEdge('g', 'c', 2.0);
    graph.addEdge('g', 'h', 3.0);
    graph.addEdge('h', 'f', 2.0);
    graph.addEdge('h', 'g', 2.0);
    graph.addEdge('i', 'e', 2.0);
    graph.addEdge('j', 'e', 2.0);
    graph.addEdge('j', 'f', 2.0);
    graph.addEdge('k', 'f', 3.0);
    graph.addEdge('s', 'a', 1.0);
    graph.addEdge('s', 'b', 3.0);
    graph.addEdge('s', 'c', 5.0);
    graph.addEdge('s', 'd', 3.0);

    pointsOfInterest.push_back(graph.findVertex('b'));
    pointsOfInterest.push_back(graph.findVertex('h'));
    pointsOfInterest.push_back(graph.findVertex('i'));
    pointsOfInterest.push_back(graph.findVertex('j'));

    scores.push_back(1.0);
    scores.push_back(2.0);
    scores.push_back(3.0);
    scores.push_back(4.0);
}


template<class T>
std::vector<Vertex<T>*> mmpMethod(
        Graph<T>& graph,
        const std::vector<Vertex<T>*>& pointsOfInterest,
        const std::vector<double>& scores,
        T start,
        T finish,
        double budget
        ) {

    return std::vector<Vertex<T>*>();
}

int main() {
    Graph<char> graph;
    std::vector<Vertex<char>*> pointsOfInterest;
    std::vector<double> scores;

    initReportGraph(graph, pointsOfInterest, scores);

    return 0;
}
