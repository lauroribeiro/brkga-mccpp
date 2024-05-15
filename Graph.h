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

struct Vertex {
    int index;
    int color;

    // Vertex(int idx) : index(idx), color(-1) {} // -1 indica que a cor ainda não foi atribuída
};

class Graph {
private:
    int V;
    vector<vector <Edge> > adjList;
    vector<double> colorCosts;
    vector<Vertex> vertices;

public:
    Graph(int vertices);

    std::vector<Vertex> getVertices() const;

    int getVerticesCount() const;

    std::vector<double> getColorCosts() const;

    void addEdge(int src, int dest);

    void setColor(int vertexIndex, int color);

    int getColor(int vertexIndex) const;

    bool isColored(int vertexIndex) const;

    bool isAdjacent(int vertexIndex1, int vertexIndex2) const;

    vector<int> getGraphColoring() const;

    void sortColorCosts();

    void loadFromFile(const string& filename);
};

#endif // GRAPH_H


// class Graph {
// public:
// 	typedef std::runtime_error Error;

// 	Graph(const std::string& instanceFile) throw(Error);
// 	virtual ~Graph();

// 	// Getters:
// 	unsigned getNumNodes() const;

// 	// Returns the distance from node i to node j:
// 	unsigned getDistance(unsigned i, unsigned j) const;

// private:
// 	std::string name;
// 	std::string comment;
// 	std::string problemType;
// 	std::string edgeWeightType;

// 	unsigned nNodes;

// 	class Coord2D {
// 	public:
// 		Coord2D() : x(0.0), y(0.0) { }
// 		Coord2D(double _x, double _y) : x(_x), y(_y) {}

// 		double getX() const { return x; }
// 		double getY() const { return y; }

// 	private:
// 		double x;
// 		double y;
// 	};

// 	std::vector< Coord2D > nodeCoords;

// 	void readVertexCount(const std::string& line) throw(Error);
// 	void readComment(const std::string& line) throw(Error);
// 	void readProblemType(const std::string& line) throw(Error);
// 	void readDimension(const std::string& line) throw(Error);
// 	void readEdgeWeightType(const std::string& line) throw(Error);

// 	void readNodeCoords(const std::string& line) throw(Error);
// 	bool isEOF(const std::string& line) const;
// 	void trim(std::string& str) const;
// };

