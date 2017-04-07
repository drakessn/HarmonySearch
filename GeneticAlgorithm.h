#pragma once
#include "Common.h"
#include "Function.h"
#include "HarmonySearch.h"

class GeneticAlgorithm
{
public:
	GeneticAlgorithm(Function*, Natural = 50, Natural = 30, Natural = 25, Real = 0.1);
	void initPopulation();
	void fitness(Chromosome &);
	void croosover(Chromosome&, Chromosome&, Chromosome&);
	void fitness(vector<Chromosome>&);
	void select(vector<Chromosome>&, Chromosome&, Chromosome&);
	void mutate(Chromosome&);
	void sortPopulation(vector<Chromosome>&);
	void search();
	void print();

	~GeneticAlgorithm();
private:
	Function* f;				// Cost Function
	vector<Chromosome> population;		
	Natural maxIterations;		// Maximum Number of Iterations
	Natural populationSize;		// Population Size
	Natural newChromosomes;		// Number of New Crhomosomes
	Real mutationRate;			// mutation Rate
	Harmony bestChromosome;		// Best Solution
	vector<Real> costHistory;	// History of Costs

	static std::default_random_engine seed;
};

