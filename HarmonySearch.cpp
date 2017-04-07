#include "HarmonySearch.h"

std::default_random_engine HarmonySearch::seed(chrono::high_resolution_clock::now().time_since_epoch().count());

HarmonySearch::HarmonySearch(Function* f, Natural maxIterations, Natural hms, Natural newHarmonies, Real hmcr, Real par, Real fw, Real fwDamp)
{
	this->f = f;
	this->maxIterations = maxIterations;
	this->hms = hms;
	this->newHarmonies = newHarmonies;
	this->hmcr = hmcr;
	this->par = par;
	this->fw = fw*(f->getTop() - f->getDown());
	this->fwDamp = fwDamp;
	this->costHistory = vector<Real>(maxIterations, maxCost);

	randomHarmonies(harmemory, hms);
	for (auto &harmony : harmemory)
		harmony.second = f->CostFunction(harmony.first);

	sortMemory();
	cloneHarmony(harmemory[0], bestHarmony);

	if (debug) 
		printMemory();
}

void HarmonySearch::randomHarmonies(vector<Harmony>& hm, Natural n)
{
	std::uniform_real_distribution<Real> distribution(f->getDown(), f->getTop());

	for (auto i = 0; i < n; i++) {
		Harmony harmony;
		for (auto j = 0; j < f->getDimension(); j++)
			harmony.first.push_back(distribution(seed));
		hm.push_back(harmony);
	}
}

void HarmonySearch::sortMemory()
{
	if (f->getMaximize())
		sort(harmemory.begin(), harmemory.end(), compareReverse);
	else
		sort(harmemory.begin(), harmemory.end(), compare);
}

void HarmonySearch::createHarmony(Harmony &harmony)
{
	std::uniform_real_distribution<Real> uniRndProbability(0.0, 1.0);
	std::uniform_int_distribution<Natural> uniRndIndex(0, hms-1);
	std::normal_distribution<double> normRnd;
	for (auto i = 0; i < f->getDimension(); i++) {
		// Use Harmony Memory
		if (uniRndProbability(seed) <= hmcr)
			harmony.first[i] = harmemory[uniRndIndex(seed)].first[i];
		// Pitch Adjustment
		if (uniRndProbability(seed) <= par)
			harmony.first[i] += fw*normRnd(seed);	// Gaussian (Normal)
		// Apply Variable Limits
		if (harmony.first[i] < f->getDown())
			harmony.first[i] = f->getDown();
		if (harmony.first[i] > f->getTop())
			harmony.first[i] = f->getTop();
	}
}

void HarmonySearch::printMemory()
{
	cout << "============Harmony Memory=============" << endl;
	for (auto i = 0; i < harmemory.size(); i++) {
		for (auto j = 0; j < f->getDimension(); j++)
			cout << harmemory[i].first[j] << "\t";
		cout << harmemory[i].second << endl;
	}
	cout << "============Bestharmony=================" << endl;
	cout << bestHarmony.first[0] << "\t" << bestHarmony.first[1] << "\t" << bestHarmony.second << endl;
}

bool compare(Harmony i, Harmony j)
{
	return (i.second<j.second);
}

bool compareReverse(Harmony i, Harmony j)
{
	return (i.second > j.second);
}

Real HarmonySearch::search()
{
	for (auto i = 0; i < maxIterations; i++) {
		vector<Harmony> harmonies;
		randomHarmonies(harmonies, newHarmonies);

		for (auto &harmony : harmonies) {
			createHarmony(harmony);	// Create New Harmony
			harmony.second = f->CostFunction(harmony.first);	// Evaluation
			harmemory.push_back(harmony);	// Merge Harmony Memory and New Harmonies
		}
		sortMemory();	// Sort Harmony Memory
		harmemory.resize(hms);	// Truncate Extra Harmonies
		cloneHarmony(harmemory[0], bestHarmony);	//Update Best Solution Ever Found
		costHistory[i] = harmemory[0].second;	// Store Best Cost Ever Found
		fw *= fwDamp;	// Damp Fret Width

		if (debug)
			printMemory();
	}
	return 1;
}

void HarmonySearch::cloneHarmony(Harmony &h1, Harmony &h2)
{
	h2.second = h1.second;
	h2.first.clear();
	for (auto x : h1.first)
		h2.first.push_back(x);
}

Harmony HarmonySearch::getBest()
{
	return bestHarmony;
}

vector<Real> HarmonySearch::getCostHistory()
{
	return costHistory;
}

HarmonySearch::~HarmonySearch()
{
}
