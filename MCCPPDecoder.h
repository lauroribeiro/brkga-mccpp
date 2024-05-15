/*
 * MCCPPDecoder.h
 *
 *  Created on: Mar 16, 2013
 *      Author: Rodrigo
 */

#ifndef MCCPPDECODER_H
#define MCCPPDECODER_H

#include "MCCPPSolver.h"
#include "Graph.h"

class MCCPPDecoder {
public:
	MCCPPDecoder(const Graph& instance);
	virtual ~MCCPPDecoder();

	// Decodes a chromosome into a solution to the MCCPP:
	double decode(const std::vector< double >& chromosome) const;

private:
	const Graph& instance;
};

#endif
