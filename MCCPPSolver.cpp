/*
 * MCCPPSolver.cpp
 *
 *  Created on: Mar 16, 2013
 *      Author: Rodrigo
 */

#include "MCCPPSolver.h"

MCCPPSolver::MCCPPSolver(const Graph& instance, const std::vector< double >& chromosome) :
		cost(0.0), vertexOrder(instance.getVerticesCount()), colorAssigned(instance.getVerticesCount(), -1) {
	// Assumes that instance.getVerticesCount() == chromosome.size()


	// 1) Obtain a permutation out of the chromosome -- order the greedy coloring of the vertices:
	for(unsigned i = 0; i < chromosome.size(); ++i) { vertexOrder[i] = ValueKeyPair(chromosome[i], i); }

	// Here we sort 'rank', which will produce a permutation of [n] stored in ValueKeyPair::second:
	std::sort(vertexOrder.begin(), vertexOrder.end());

	std::vector<double> colorCosts = instance.getColorCosts();

	// 2) Assign the colors to the vertices and compute the cost:
    std::vector<bool> available(vertexOrder.size(), true);

    int color = 0;
    colorAssigned[vertexOrder[0].second] = color;
    cost += colorCosts[color];

    for(unsigned i = 1; i < vertexOrder.size(); ++i) {
        // Mark the colors assigned to adjacent vertices as unavailable
        for (unsigned j = 0; j < i; ++j) {
            if (instance.isAdjacent(vertexOrder[i].second, vertexOrder[j].second) && colorAssigned[vertexOrder[j].second] != -1) {
                available[colorAssigned[vertexOrder[j].second]] = false;
            }
        }

        // Find the first available color
        color = 0;
        while (!available[color]) {
            color++;
        }

        colorAssigned[vertexOrder[i].second] = color;
        cost += colorCosts[color];

        // Reset the available colors for the next iteration
        std::fill(available.begin(), available.end(), true);
    }
}


MCCPPSolver::~MCCPPSolver() {
}

double MCCPPSolver::getChromaticPartitionCost() { return cost; }

std::vector<int>& MCCPPSolver::getChromaticPartition() { return colorAssigned; }
