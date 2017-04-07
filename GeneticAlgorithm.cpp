#include "GeneticAlgorithm.h"


std::default_random_engine GeneticAlgorithm::seed(chrono::high_resolution_clock::now().time_since_epoch().count());


GeneticAlgorithm::GeneticAlgorithm(Function *function, Natural maxIterations, Natural populationSize, Natural newChromosomes, Real mutationRate)
{
	this->f = function;
	this->maxIterations = maxIterations;
	this->populationSize = populationSize;
	this->newChromosomes = newChromosomes;
	this->mutationRate = mutationRate;
	this->costHistory = vector<Real>(maxIterations, maxCost);
}

void GeneticAlgorithm::initPopulation()
{
	std::uniform_int_distribution<int> hms(20, 40);			// Harmony Memory Size 1 to 100
	std::uniform_int_distribution<int> newHarmonies(10, 35);// Number of New Harmonies 1 to 100
	std::uniform_real_distribution<Real> hmcr(0.85, 0.95);	// Harmony Memory Consideration Rate 0.7 to 0.99
	std::uniform_real_distribution<Real> par(0.2, 0.4);		// Pitch Adjustment Rate 0.1 to 0.5
	std::uniform_real_distribution<Real> fw(0.001, 0.01);	// Fret Width(Bandwidth) 0.01 to 0.001
	std::uniform_real_distribution<Real> fwDamp(0.9, 1.0);	// Fret Width Damp Ratio 0.5 to 1

	for (auto i = 0; i < populationSize; i++) {
		Chromosome chromosome;
		chromosome.first.push_back(hms(seed));
		chromosome.first.push_back(newHarmonies(seed));
		chromosome.first.push_back(hmcr(seed));
		chromosome.first.push_back(par(seed));
		chromosome.first.push_back(fw(seed));
		chromosome.first.push_back(fwDamp(seed));
		population.push_back(chromosome);
	}
	fitness(population);
	sortPopulation(population);
	//Chromosome a;
	//Chromosome b;
	//select(population, a, b);
	//Chromosome chromosome;
	//croosover(chromosome, a, b);
}

void GeneticAlgorithm::fitness(Chromosome& chromosome)
{
	Real sum = 0.0;
	#pragma omp parallel for
	for (auto i = 0; i < 10; i++) {
		HarmonySearch* hs;
		hs = new HarmonySearch(f, 50, chromosome.first[0], chromosome.first[1], chromosome.first[2], chromosome.first[3], chromosome.first[4], chromosome.first[5]);
		hs->search();
		Harmony harmony = hs->getBest();
		#pragma omp atomic
		sum += harmony.second;
	}
	chromosome.second = sum / 10.0;
}

void GeneticAlgorithm::croosover(Chromosome &out, Chromosome &p1, Chromosome &p2)
{
	std::uniform_real_distribution<Real> dist(0.0, 1.0);// Proportion
	for (auto i = 0; i < p1.first.size(); i++) {
		auto p = dist(seed);
		out.first.push_back(p*p1.first[i] + (1.0-p)*p2.first[i]);
		//cout << p << " " << out.first[i] << endl;
	}
	out.first[0] = round(out.first[0]);
	out.first[1] = round(out.first[1]);
	//for (auto x : out.first)
	//	cout << x << "\t";
	//cout << endl;
}

void GeneticAlgorithm::fitness(vector<Chromosome>& pop)
{
	for (auto &chromosome : pop) {
		fitness(chromosome);
		cout << chromosome.second << endl;
	}
}

void GeneticAlgorithm::select(vector<Chromosome>& pop, Chromosome &p1, Chromosome &p2)
{
	std::uniform_int_distribution<Natural> dist(0, populationSize-1);	// Index
	auto p = dist(seed);
	auto q = dist(seed);
	auto r = dist(seed);
	while (p==q)
		q = dist(seed);
	p = (p < q) ? p : q;	// Parent 1
	while (p == q)
		q = dist(seed);
	while (r == q || p == r)
		r = dist(seed);
	q = (r < q) ? r : q;	// Parent 1
	p1 = pop[p];
	p2 = pop[q];
	//cout << p << " " << q << endl;
}

void GeneticAlgorithm::mutate(Chromosome &chromosome)
{
	std::uniform_real_distribution<Real> prob(0.0, 1.0);		// Probability
	if (prob(seed) < mutationRate) {
		std::uniform_int_distribution<int> parameter(0, 5);			// Parameter
		std::uniform_int_distribution<int> hms(10, 40);			// Harmony Memory Size 1 to 100
		std::uniform_int_distribution<int> newHarmonies(10, 35);// Number of New Harmonies 1 to 100
		std::uniform_real_distribution<Real> hmcr(0.85, 0.95);	// Harmony Memory Consideration Rate 0.7 to 0.99
		std::uniform_real_distribution<Real> par(0.2, 0.4);		// Pitch Adjustment Rate 0.1 to 0.5
		std::uniform_real_distribution<Real> fw(0.001, 0.01);	// Fret Width(Bandwidth) 0.01 to 0.001
		std::uniform_real_distribution<Real> fwDamp(0.9, 1.0);	// Fret Width Damp Ratio 0.5 to 1
		switch (parameter(seed)) {
		case 0: chromosome.first[0] = hms(seed); break;
		case 1: chromosome.first[1] = newHarmonies(seed); break;
		case 2: chromosome.first[2] = hmcr(seed); break;
		case 3: chromosome.first[3] = par(seed); break;
		case 4: chromosome.first[4] = fw(seed); break;
		case 5: chromosome.first[5] = fwDamp(seed); break;
		}
	}
}

void GeneticAlgorithm::sortPopulation(vector<Chromosome>& pop)
{
	if (f->getMaximize())
		sort(pop.begin(), pop.end(), compareReverse);
	else
		sort(pop.begin(), pop.end(), compare);
}

void GeneticAlgorithm::search()
{
	initPopulation();
	std::fstream fs;
	fs.open("gaBench32.csv", std::fstream::in | std::fstream::out | std::fstream::app);

	for (auto i = 0; i < maxIterations; i++) {
		cout << "Generation: " << i + 1 << endl;
		vector<Chromosome> newPopulation(newChromosomes);
//#pragma omp parallel for
		for (auto &chromosome : newPopulation) {
			Chromosome p1, p2;
			select(population, p1, p2);
			croosover(chromosome, p1, p2);
			mutate(chromosome);
			//fitness(chromosome);
		}
		for (auto &chromosome : newPopulation) 
			population.push_back(chromosome);	// Merge populations
		fitness(population);
		sortPopulation(population);	// Sort population
		population.resize(populationSize);	// Truncate Extra population
		print();
		cout << population[0].second << endl;
		fs << i + 1 << ",";
		for (auto x : population[0].first)
			fs << x << ",";
		fs << population[0].second << endl;
	}
	fs.close();
}

void GeneticAlgorithm::print()
{
	for (auto i = 0; i < populationSize; i++) {
		for (auto j = 0; j < 6; j++)
			cout << population[i].first[j] << "\t";
		cout << population[i].second << endl;
	}
}

GeneticAlgorithm::~GeneticAlgorithm()
{
}
