/**
 * brkga-tsp.cpp
 *
 * Driver class with a simple example of how to instantiate and use the BRKGA API to find solutions
 * to the symmetric traveling salesman problem (TSP) on TSPLIB instances.
 *
 * See TSPDecoder.h for details on the decoder's implementation.
 *
 * Created on : Nov 17, 2011 by rtoso
 * Authors    : Rodrigo Franco Toso <rtoso@cs.rutgers.edu>
 *              Mauricio G.C. Resende <mgcr@research.att.com>
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2018
 * Rodrigo Franco Toso (rfrancotoso@gmail.com) and
 * Mauricio G.C. Resende
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <iostream>
#include <algorithm>
#include "brkgaAPI/BRKGA.h"
#include "brkgaAPI/MTRand.h"

#include "Graph.h"
#include "MCCPPSolver.h"
#include "MCCPPDecoder.h"

int main(int argc, char* argv[]) {
	if(argc < 2) { std::cerr << "usage: <TSPLIB-file>" << std::endl; return -1; }

	std::cout << "Welcome to the BRKGA API sample driver.\nFinding a (heuristic) minimizer for "
			<< " the TSP." << std::endl;

	const clock_t begin = clock();
	clock_t now = clock();

	const std::string instanceFile = std::string(argv[1]);
	std::cout << "Instance file: " << instanceFile << std::endl;

   	Graph instance(1); // Inicialização temporária

	// Read the instance:
    instance.loadFromFile(instanceFile);

	// bool adj = instance.isAdjacent(1, 10);
	// std::cout << "Is adjacent 1, 10: " << adj << std::endl;
	// adj = instance.isAdjacent(3, 10);
	// std::cout << "Is adjacent 3, 10: " << adj << std::endl;
	// return 0;


	// double arr[] = {0.1, 0.4, 0.2, 0.3, 0.5, 0.6, 0.7, 0.8, 0.9, 0.14, 0.04};
	// std::vector<double> chromossome(arr, arr + sizeof(arr) / sizeof(arr[0]));

	// MCCPPSolver solution(instance, chromossome);


	// std::cout << "Solution cost: " << solution.getChromaticPartitionCost() << std::endl;
	// std::vector<int> colorAssigned = solution.getChromaticPartition();
	// for (unsigned i = 0; i < colorAssigned.size(); ++i) {
	// 	std::cout << " [" << i << ", " << colorAssigned[i] << "]";
	// }
	
	// return 0;

	const unsigned n = instance.getVerticesCount();		// size of chromosomes
	const unsigned p = 256;		// size of population
	const double pe = 0.10;		// fraction of population to be the elite-set
	const double pm = 0.10;		// fraction of population to be replaced by mutants
	const double rhoe = 0.70;	// probability that offspring inherit an allele from elite parent
	const unsigned K = 1;		// number of independent populations
	const unsigned MAXT = 1;	// number of threads for parallel decoding

	MCCPPDecoder decoder(instance);
	const long unsigned rngSeed = time(0);	// seed to the random number generator
	MTRand rng(rngSeed);					// initialize the random number generator

	// initialize the BRKGA-based heuristic
	BRKGA< MCCPPDecoder, MTRand > algorithm(n, p, pe, pm, rhoe, decoder, rng, K, MAXT);

	// for (int i = 0; i < instance.getVerticesCount(); i++) {
	// 	instance.setColor(i, i);
	// }
	// std::vector<double> colorCosts = instance.getColorCosts();
	// std::vector<int> coloring = instance.getGraphColoring();
	// std::vector<Vertex> vertices = instance.getVertices();
	// std::string colored = instance.isColored(2) ? "true" : "false";
	// std::cout << "\niscolored 2 " << colored;
	// for (int i = 0; i < instance.getVerticesCount(); i++) {
	// 	std::cout << "\n Vertex[" << vertices[i].index << 
	// 	 			 "] color: " << vertices[i].color << 
	// 				 " color cost: " << colorCosts[vertices[i].color];
	// }
	
	
	// // BRKGA inner loop (evolution) configuration: Exchange top individuals
	// const unsigned X_INTVL = 100;	// exchange best individuals at every 100 generations
	// const unsigned X_NUMBER = 2;	// exchange top 2 best
	const unsigned MAX_GENS = 1000;	// run for 1000 gens

	// // BRKGA evolution configuration: restart strategy
	unsigned relevantGeneration = 1;	// last relevant generation: best updated or reset called
	const unsigned RESET_AFTER = 200;
	std::vector< double > bestChromosome;
	double bestFitness = std::numeric_limits< double >::max();
	std::cout << "Best fitness: " << bestFitness << std::endl;

	// Print info about multi-threading:
	#ifdef _OPENMP
		std::cout << "Running for " << MAX_GENS << " generations using " << MAXT
				<< " out of " << omp_get_max_threads()
				<< " available thread units..." << std::endl;
	#endif
	#ifndef _OPENMP
		std::cout << "Running for " << MAX_GENS
				<< " generations without multi-threading..." << std::endl;
	#endif

	double games120 = 4351;
	
	// Run the evolution loop:
	unsigned generation = 1;		// current generation
	do {
		algorithm.evolve();	// evolve the population for one generation

		// Bookeeping: has the best solution thus far improved?
		if(algorithm.getBestFitness() < bestFitness) {
			// Save the best solution to be used after the evolution chain:
			// relevantGeneration = generation;
			bestFitness = algorithm.getBestFitness();
			bestChromosome = algorithm.getBestChromosome();
			
			std::cout << "\t" << generation
					<< ") Improved best solution thus far: "
					<< bestFitness << std::endl;
		}

		//  Evolution strategy: restart
		if(generation - relevantGeneration > RESET_AFTER) {
			algorithm.reset();	// restart the algorithm with random keys
			relevantGeneration = generation;
			
			std::cout << "\t" << generation << ") Reset at generation "
					<< generation << std::endl;
		}

		// Evolution strategy: exchange top individuals among the populations
		// if(generation % X_INTVL == 0 && relevantGeneration != generation) {
		// 	algorithm.exchangeElite(X_NUMBER);
			
		// 	std::cout << "\t" << generation
		// 			<< ") Exchanged top individuals." << std::endl;
		// }

		// Next generation?
		now = clock();
		++generation;
	} while (games120 > bestFitness ); //

	// print the fitness of the top 10 individuals of each population:
	std::cout << "Fitness of the top 10 individuals of each population:" << std::endl;
	const unsigned bound = std::min(p, unsigned(10));	// makes sure we have 10 individuals
	for(unsigned i = 0; i < K; ++i) {
		std::cout << "Population #" << i << ":" << std::endl;
		for(unsigned j = 0; j < bound; ++j) {
			std::cout << "\t" << j << ") "
					<< algorithm.getPopulation(i).getFitness(j) << std::endl;
		}
	}

	// rebuild the best solution:
	MCCPPSolver bestSolution(instance, bestChromosome);

	std::cout << "Best solution found has objective value = "
	 		<< bestSolution.getChromaticPartitionCost() << std::endl;

	const std::vector<int> bestChromaticPartition = bestSolution.getChromaticPartition();
	std::cout << "Best chromatic partition: [vextex, color]" << std::endl;
	for (unsigned i = 0; i < bestChromaticPartition.size(); ++i) {
		std::cout << " [" << i << ", " << bestChromaticPartition[i] << "]";
	}
	std::cout << std::endl;

	const clock_t end = clock();
	std::cout << "BRKGA run finished in " << (end - begin) / double(CLOCKS_PER_SEC) << " s." << std::endl;

	return 0;
}
