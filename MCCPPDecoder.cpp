/*
 * MCCPPDecoder.cpp
 *
 *  Created on: Mar 16, 2013
 *      Author: Rodrigo
 */

#include "MCCPPDecoder.h"

MCCPPDecoder::MCCPPDecoder(const Graph& _instance) : instance(_instance) {
}

MCCPPDecoder::~MCCPPDecoder() {
}

double MCCPPDecoder::decode(const std::vector< double >& chromosome, const std::vector<std::vector<std::pair<int, int> > > minedPatterns) const {
	// 1) Solve the problem (i.e., create a tour out of this chromosome):
	// Avoids race conditions by making sure we have a single MCCPPSolver for each thread calling
	// ::decode (as long as MCCPPSolver does not make use of 'static' or other gimmicks):
	MCCPPSolver solver(instance, chromosome);

	// 2) Extract the fitness (tour distance):
	double fitness = solver.getChromaticPartitionCost();

	if (minedPatterns.size() > 0) {
		// 3) Calculate the bonus for respecting the patterns
		const std::vector<int> colorAssigned = solver.getChromaticPartition();
		const double patternBonus = calculatePatternBonus(colorAssigned, minedPatterns);

		// cout << "Fitness: " << fitness << " Pattern Bonus: " << patternBonus << endl;
		fitness = fitness - patternBonus;
	}

	return fitness;
}

double MCCPPDecoder::calculatePatternBonus(std::vector<int> colorAssigned, const std::vector<std::vector<std::pair<int, int>>> minedPatterns) const {
	int patternMatchCount = 0;

	// Verifique quantos padrões o indivíduo respeita
	for (const auto& pattern : minedPatterns) {
			bool respectsPattern = true;
			for (const auto& vc : pattern) {
					int vertex = vc.first;
					int color = vc.second;

					// Se o vértice não foi colorido com a cor esperada, o padrão não é respeitado
					if (colorAssigned[vertex] != color) {
							respectsPattern = false;
							break;
					}
			}
			if (respectsPattern) {
					patternMatchCount++;
			}
	}

	// Calcule o bônus com base nos padrões respeitados
	const double patternWeight = 5; // Peso do bônus
	double patternBonus = patternWeight * patternMatchCount;

	return patternBonus;
}
