/*
 * Graph.h
 *
 * Reads an graph's adjacency matrix.
 *
 * Here's the format:
 *
 * vertex count (3)
 * edge count (4)
 * list of color costs (20.00 14.00 30.00)
 * 0 1 0 
 * 1 0 1 
 * 0 1 0
 *
 *  Created on: May 11, 2024
 *      Author: Lauro
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

struct Edge {
    int dest;
};

class Graph {
private:
    int V;
    vector<vector <Edge> > adjList;
    vector<double> colorCosts;

public:
    Graph(int vertices);

    int getVerticesCount() const;

    std::vector<double> getColorCosts() const;

    void addEdge(int src, int dest);

    bool isAdjacent(int vertexIndex1, int vertexIndex2) const;

    void sortColorCosts();

    bool validColoring(vector<int> colorAssigned) const;

    void loadFromFile(const string& filename);
};

#endif // GRAPH_H
