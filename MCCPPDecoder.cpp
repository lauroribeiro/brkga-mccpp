/*
 * MCCPPDecoder.cpp
 *
 *  Created on: Mar 16, 2013
 *      Author: Rodrigo
 */

#include "MCCPPDecoder.h"

MCCPPDecoder::MCCPPDecoder(Graph& _instance) : instance(_instance) {
}

MCCPPDecoder::~MCCPPDecoder() {
}

double MCCPPDecoder::decode(const std::vector< double >& chromosome) const {
	// 1) Solve the problem (i.e., create a tour out of this chromosome):
	// Avoids race conditions by making sure we have a single MCCPPSolver for each thread calling
	// ::decode (as long as MCCPPSolver does not make use of 'static' or other gimmicks):
	MCCPPSolver solver(instance, chromosome);

	// 2) Extract the fitness (tour distance):
	const double fitness = solver.getChromaticPartitionCost();

	// 3) Return:
	return fitness;
}

