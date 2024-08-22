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
	double decode(const std::vector< double >& chromosome, const std::vector<std::vector<std::pair<int, int> > > minedPatterns = std::vector<std::vector<std::pair<int, int> > >()) const;
	
	double calculatePatternBonus(const std::vector<int> colorAssigned, const std::vector<std::vector<std::pair<int, int> > > minedPatterns) const;

private:
	const Graph& instance;
};

#endif
