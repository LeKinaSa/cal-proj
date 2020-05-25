
#include "randomGraphs.h"

#include <chrono>
#include <iostream>

void generateRandomGraph(Graph<int>& graph, std::vector<Vertex<int>*>& pointsOfInterest, int numVertices) {
    srand(time(nullptr));

    const double POI_CHANCE = 0.1;

    for (int i = 0; i < numVertices; ++i) {
        graph.addVertex(i);
    }

    for (auto v : graph.getVertexSet()) {
        double r = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
        if (r <= POI_CHANCE) {
            pointsOfInterest.push_back(v);
        }
    }

    for (int i = 0; i < numVertices * 5; ++i) {
        int v1 = rand() % numVertices;
        int v2 = rand() % numVertices;

        if (v1 != v2) {
            bool edgeExists = false;

            Vertex<int>* startPtr = graph.findVertex(v1);
            Vertex<int>* endPtr = graph.findVertex(v2);

            const std::vector<Edge<int>>& adj = startPtr->getAdj();

            for (auto edge : adj) {
                if (edge.getDest() == endPtr) {
                    edgeExists = true;
                    break;
                }
            }

            if (!edgeExists) {
                float weight = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 5.0;
                graph.addEdge(v1, v2, weight);
            }
        }
    }
}


void testReductionStepAlgorithms() {
    std::cout << "numVertices, time Dijkstra (microseconds), time FW (microseconds)" << std::endl;

    const size_t NUM_ITERS = 5;

    for (int n = 10; n <= 1000; n += 10) {
        unsigned long long usDij = 0, usFW = 0;

        for (int i = 0; i < NUM_ITERS; ++i) {
            Graph<int> graph;
            std::vector<Vertex<int>*> pointsOfInterest;

            generateRandomGraph(graph, pointsOfInterest, n);

            int start, finish;

            do {
                start = rand() % n;
                finish = rand() % n;
            } while (start == finish);

            auto t1Dij = std::chrono::high_resolution_clock::now();
            graph.generateAdjacencyMatrixWithDijkstra(pointsOfInterest, graph.findVertex(start), graph.findVertex(finish));
            auto t2Dij = std::chrono::high_resolution_clock::now();

            auto t1FW = std::chrono::high_resolution_clock::now();
            graph.generateAdjacencyMatrixWithFloydWarshall(pointsOfInterest, graph.findVertex(start), graph.findVertex(finish));
            auto t2FW = std::chrono::high_resolution_clock::now();

            long long deltaDij = std::chrono::duration_cast<std::chrono::microseconds>(t2Dij - t1Dij).count();
            long long deltaFW = std::chrono::duration_cast<std::chrono::microseconds>(t2FW - t1FW).count();

            usDij += deltaDij;
            usFW += deltaFW;
        }

        usDij /= NUM_ITERS;
        usFW /= NUM_ITERS;

        std::cout << n << ", " << usDij << ", " << usFW << std::endl;
    }
}