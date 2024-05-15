
#include <cmath>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include "Graph.h"

Graph::Graph(int vertices) : V(vertices), adjList(vertices), colorCosts(vertices), vertices(vertices) {}

std::vector<Vertex> Graph::getVertices() const {
    return vertices;
}

int Graph::getVerticesCount() const {
    return V;
}

std::vector<double> Graph::getColorCosts() const {
    return colorCosts;
}

void Graph::addEdge(int src, int dest) {
    Edge edge;
    edge.dest = dest;
    adjList[src].push_back(edge);
}

void Graph::setColor(int vertexIndex, int color) {
    vertices[vertexIndex].color = color;
}

int Graph::getColor(int vertexIndex) const {
    return vertices[vertexIndex].color;
}

bool Graph::isColored(int vertexIndex) const {
    return vertices[vertexIndex].color != -1;
}

bool Graph::isAdjacent(int vertexIndex1, int vertexIndex2) const {
    for (size_t i = 0; i < adjList[vertexIndex1].size(); ++i) {
        if (adjList[vertexIndex1][i].dest == vertexIndex2) {
            return true;
        }
    }
    return false;
}

std::vector<int> Graph::getGraphColoring() const {
    std::vector<int> coloring(V);
    for (int i = 0; i < V; ++i) {
        coloring[i] = vertices[i].color;
    }
    return coloring;
}

void Graph::sortColorCosts() {
    std::sort(colorCosts.begin(), colorCosts.end());
}

void Graph::loadFromFile(const std::string& filename) {
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo " << filename << std::endl;
        exit(1);
    }

    int E;
    file >> V >> E;
    adjList.resize(V);
    colorCosts.resize(V);
    vertices.resize(V);

    // Ler custos de cores
    for (int i = 0; i < V; i++) {
        file >> colorCosts[i];
    }

    // Ordenar custos de cores de forma crescente
    sortColorCosts();

    // Ler matriz de adjacência e adicionar arestas ao grafo
    // std::cout << "Adjacency matrix:\n";
    for (int i = 0; i < V; i++) {
        vertices[i].index = i;
        vertices[i].color = -1;
        for (int j = 0; j < V; j++) {
            int isConnected;
            file >> isConnected;
            // std::cout << isConnected << " ";
            if (isConnected)
                addEdge(i, j);
        }
        // std::cout << std::endl;
    }

    file.close();
}
