/*
 * MCCPPSolver.cpp
 *
 *  Created on: Mar 16, 2013
 *      Author: Rodrigo
 */

#include "MCCPPSolver.h"

MCCPPSolver::MCCPPSolver(Graph& instance, const std::vector< double >& chromosome) :
		cost(0), vertexOrder(instance.getVerticesCount()), partition(instance.getVerticesCount()) {
	// Assumes that instance.getNumNodes() == chromosome.size() of course

	// 1) Obtain a permutation out of the chromosome -- order the greedy coloring of the vertices:
	for(unsigned i = 0; i < chromosome.size(); ++i) { vertexOrder[i] = ValueKeyPair(chromosome[i], i); }

	// Here we sort 'rank', which will produce a permutation of [n] stored in ValueKeyPair::second:
	std::sort(vertexOrder.begin(), vertexOrder.end());

	std::vector<Vertex> vertices = instance.getVertices();
	std::vector<double> colorCosts = instance.getColorCosts();
	// 2) Assign the colors to the vertices and compute the cost:
	int color = 0;
	for(unsigned i = 1; i < vertexOrder.size(); ++i) {
		if (instance.isColored(vertexOrder[i].second)) { continue; }

		instance.setColor(vertexOrder[i].second, color);
		cost += colorCosts[color];
		for (unsigned j = i + 1; j < vertexOrder.size(); ++j) {
			if (instance.isColored(vertexOrder[j].second)) { continue; }

			if (!instance.isAdjacent(vertexOrder[i].second, vertexOrder[j].second)) {
				instance.setColor(vertexOrder[j].second, color);
				cost += colorCosts[color];
			}
		}
		++color;
	}

	partition = instance.getGraphColoring();
}

MCCPPSolver::~MCCPPSolver() {
}

double MCCPPSolver::getChromaticPartitionCost() const { return cost; }

std::vector<int> MCCPPSolver::getChromaticPartition() const { return partition; }
