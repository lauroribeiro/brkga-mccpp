/*
 * MCCPPSolver.h
 *
 *  Created on: Mar 16, 2013
 *      Author: Rodrigo
 */

#ifndef MCCPPSOLVER_H
#define MCCPPSOLVER_H

#include <list>
#include <limits>
#include <vector>
#include <algorithm>
#include "Graph.h"

class MCCPPSolver {
public:
	// The constructor 'solves' the problem in O(n log n) by transforming the chromosome into
	// a tour (by getting a permutation out of the chromosome):
	MCCPPSolver(Graph& instance, const std::vector< double >& chromosome);
	virtual ~MCCPPSolver();

	double getChromaticPartitionCost() const;		        // Returns the chromatic partition cost
	std::vector<int> getChromaticPartition() const;	// Returns the chromatic partition

private:
	typedef std::pair< double, unsigned > ValueKeyPair;

	double cost;
	std::vector<ValueKeyPair> vertexOrder;
	std::vector<int> partition;
};

#endif
