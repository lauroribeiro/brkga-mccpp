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

	// std::cout << "Welcome to the BRKGA API sample driver.\nFinding a (heuristic) minimizer for "
			// << " the TSP." << std::endl;

	const clock_t begin = clock();
	clock_t now = clock();

	const std::string instanceFile = std::string(argv[1]);
	// std::cout << "Instance file: " << instanceFile << std::endl;

   	Graph instance(1); // Inicialização temporária

	// Read the instance:
    instance.loadFromFile(instanceFile);


	const unsigned n = instance.getVerticesCount();		// size of chromosomes
	const unsigned p = atoi(argv[2]);		// size of population
	// const unsigned p = 200;		// size of population
	const double pe = atof(argv[3]);		// fraction of population to be the elite-set
	// const double pe = 0.25;		// fraction of population to be the elite-set
	const double pm = atof(argv[4]);		// fraction of population to be replaced by mutants
	// const double pm = 0.10;		// fraction of population to be replaced by mutants
	const double rhoe = atof(argv[5]);	// probability that offspring inherit an allele from elite parent
	// const double rhoe = 0.70;	// probability that offspring inherit an allele from elite parent
	const unsigned K = 1;		// number of independent populations
	const unsigned MAXT = 1;	// number of threads for parallel decoding
	
	MCCPPDecoder decoder(instance);

	const int seed = atoi(argv[8]);	// seed for random number generator
	const double stopTime = atof(argv[9]);	// stop time in seconds
	// const double objectiveValue = atof(argv[10]);	// objective value to stop the algorithm
	const time_t rngSeed = seed;	// seed to the random number generator
	MTRand rng(rngSeed);					// initialize the random number generator

	// initialize the BRKGA-based heuristic
	BRKGA< MCCPPDecoder, MTRand > algorithm(n, p, pe, pm, rhoe, decoder, rng, K, MAXT);
	
	// // BRKGA inner loop (evolution) configuration: Exchange top individuals
	// const unsigned X_INTVL = 100;	// exchange best individuals at every 100 generations
	// const unsigned X_NUMBER = 2;	// exchange top 2 best
	const unsigned MAX_GENS = 200;	// run for 1000 gens

	// BRKGA evolution configuration: restart strategy
	const int restartStrategy = atoi(argv[7]);	// 0: no restart, 1: restart with consequent generations, 2: restart without improvements
	unsigned relevantGeneration = 1;	// last relevant generation: best updated or reset called
	unsigned generationCounter = 1;	// generation counter for the restart strategy
	const unsigned RESET_AFTER = atoi(argv[6]);
	std::vector< double > bestChromosome;
	double bestFitness = std::numeric_limits< double >::max();
	// std::cout << "Best fitness: " << bestFitness << std::endl;

	// Print info about multi-threading:
	#ifdef _OPENMP
		std::cout << "Running for " << MAX_GENS << " generations using " << MAXT
				<< " out of " << omp_get_max_threads()
				<< " available thread units..." << std::endl;
	#endif
	#ifndef _OPENMP
		// std::cout << "Running for " << MAX_GENS
		// 		<< " generations without multi-threading..." << std::endl;
	#endif

	// double objectiveValue = 4300;
	
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
			
			// std::cout << "\t" << generation
			// 		<< ") Improved best solution thus far: "
			// 		<< bestFitness << std::endl;
		}

		//  Evolution strategy: restart
		switch (restartStrategy) {
			case 1:	// restart with consequent generations
				if(generation - relevantGeneration > RESET_AFTER) {
					algorithm.reset();	// restart the algorithm with random keys
					relevantGeneration = generation;
					
					// std::cout << "\t" << generation << ") Reset at generation "
					// 		<< generation << std::endl;
				}
				break;
			case 2:	// restart without improvements
				if(generationCounter > RESET_AFTER) {
					generationCounter = 1;
					algorithm.reset();	// restart the algorithm with random keys
					relevantGeneration = generation;
					
					// // std::cout << "\t" << generation << ") Reset at generation "
					// 		<< generation << std::endl;
				}
				generationCounter++;
				break;
			default: break;
		}

		// Evolution strategy: exchange top individuals among the populations
		// if(generation % X_INTVL == 0 && relevantGeneration != generation) {
		// 	algorithm.exchangeElite(X_NUMBER);
			
		// 	std::cout << "\t" << generation
		// 			<< ") Exchanged top individuals." << std::endl;
		// }

		// // 1. Crie uma estrutura de dados para armazenar as transações
		// std::vector<std::vector<std::pair<int, int>>> transactions;

		// // 2. Preencha as transações com as melhores soluções da população
		// for (unsigned i = 0; i < bestChromaticPartition.size(); ++i) {
		// 		std::vector<std::pair<int, int>> transaction;
		// 		for (unsigned j = 0; j < bound; ++j) {
		// 				transaction.push_back(std::make_pair(j, bestChromaticPartition[j]));
		// 		}
		// 		transactions.push_back(transaction);
		// }

		// Next generation?
		now = clock();
		++generation;
	// } while (generation < MAX_GENS);
	} while ((now - begin) / double(CLOCKS_PER_SEC) < stopTime);
	// } while ((bestFitness > objectiveValue) && ((now - begin) / double(CLOCKS_PER_SEC) < stopTime));
	// } while (bestFitness > objectiveValue);

	// print the fitness of the top 10 individuals of each population:
	// std::cout << "Fitness of the top 10 individuals of each population:" << std::endl;
	// const unsigned bound = std::min(p, unsigned(10));	// makes sure we have 10 individuals
	// for(unsigned i = 0; i < K; ++i) {
	// 	std::cout << "Population #" << i << ":" << std::endl;
	// 	for(unsigned j = 0; j < bound; ++j) {
	// 		std::cout << "\t" << j << ") "
	// 				<< algorithm.getPopulation(i).getFitness(j) << std::endl;
	// 	}
	// }

	// rebuild the best solution:
	MCCPPSolver bestSolution(instance, bestChromosome);

	// std::cout << "Best solution found has objective value = "
	//  		<< bestSolution.getChromaticPartitionCost() << std::endl;
	
	// std::cout << "Valid coloring: " << instance.validColoring(bestSolution.getChromaticPartition()) << std::endl;

	// const std::vector<int> bestChromaticPartition = bestSolution.getChromaticPartition();
	// std::cout << "Best chromatic partition: [vextex, color]" << std::endl;
	// for (unsigned i = 0; i < bestChromaticPartition.size(); ++i) {
	// 	std::cout << " [" << i << ", " << bestChromaticPartition[i] << "]";
	// }
	// std::cout << std::endl;

	const clock_t end = clock();
	// std::cout << bestFitness << " - " << (end - begin) / double(CLOCKS_PER_SEC) << std::endl;
	std::cout << bestFitness << "," << (end - begin) / double(CLOCKS_PER_SEC);
	// std::cout << "BRKGA run finished in " << (end - begin) / double(CLOCKS_PER_SEC) << " s." << std::endl;

	return 0;
}
