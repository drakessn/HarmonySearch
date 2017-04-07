#pragma once
#include "Common.h"
#include "Function.h"


bool compare(Harmony i, Harmony j);
bool compareReverse(Harmony i, Harmony j);

class HarmonySearch
{
public:
	HarmonySearch(Function*, Natural = 50, Natural = 30, Natural = 25, Real = 0.9, Real = 0.3, Real = 0.01, Real = 0.995);
	void randomHarmonies(vector<Harmony>&, Natural);
	void sortMemory();
	void createHarmony(Harmony&);
	void printMemory();
	Real search();
	void cloneHarmony(Harmony&, Harmony&);
	Harmony getBest();
	vector<Real> getCostHistory();
	~HarmonySearch();
private:
	Function* f;				// Cost Function		
	Natural maxIterations;		// Maximum Number of Iterations
	Natural hms;				// Harmony Memory Size
	Natural newHarmonies;		// Number of New Harmonies
	Real hmcr;					// Harmony Memory Consideration Rate
	Real par;					// Pitch Adjustment Rate
	Real fw;					// Fret Width(Bandwidth)
	Real fwDamp;				// Fret Width Damp Ratio
	vector<Harmony> harmemory;	// Harmony Memory
	Harmony bestHarmony;		// Best Solution
	vector<Real> costHistory;	// History of Costs

	static std::default_random_engine seed;
};

