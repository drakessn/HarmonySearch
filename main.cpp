#include <stdio.h>

#include "HarmonySearch.h"
#include "Ackley.h"
#include "Schwefel.h"
#include "Benchmark3.h"
#include "GeneticAlgorithm.h"

void searchBestParameters(Function*);
void testBestParameters(Function*, functionType, string = "Ackley.csv");

int main()
{
	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();

	Function* f = new Schwefel(2);
	//searchBestParameters(f);
	testBestParameters(f, Schwefel2, "schwefel2d.csv");

	end = std::chrono::system_clock::now();
	int elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>
		(end - start).count();

	std::cout << "tiempo transcurrido: " << elapsed_seconds << "s\n";
	system("pause");
    return 0;
}

void searchBestParameters(Function *f)
{
	GeneticAlgorithm *ga;
	ga = new GeneticAlgorithm(f, 50, 20, 10, 0.25);
	ga->search();
	ga->print();

}

void testBestParameters(Function *f, functionType op, string filename)
{
	vector<vector<Real>> costs;
	Real sum = 0.0;
	cout << "Processing..." << endl;
	#pragma omp parallel for
	for (auto i = 0; i < testSize; i++) {
		HarmonySearch* hs;
		switch (op)
		{
		case Ackley2: hs = new HarmonySearch(f, 50, 22, 33, 0.89666, 0.334599, 0.00889924, 0.961022);		// Ackley 2D
			break;
		case Ackley10: hs = new HarmonySearch(f, 50, 25, 42, 0.941428, 0.354985, 0.0089194, 0.983156);		// Ackley 10D																						
			break;
		case Schwefel2: hs = new HarmonySearch(f, 50, 24, 32, 0.901981, 0.330909, 0.0079172, 0.934118);		// Schwefel 2D
			break;
		case Schwefel10: hs = new HarmonySearch(f, 50, 30, 46, 0.940452, 0.421773, 0.00859703, 0.962777);	// Schwefel 10D
			break;
		case Bench2: hs = new HarmonySearch(f, 50, 58, 44, 0.803035, 0.379934, 0.00713633, 0.90182);		// Benchmark3 2D
			break;
		case Bench10: hs = new HarmonySearch(f, 50, 34, 46, 0.931578, 0.404419, 0.0074238, 0.946819);		// Benchmark3 10D
			break;
		default: hs = new HarmonySearch(f);
			break;
		}
		hs->search();
		Harmony harmony = hs->getBest();
		//cout << harmony.second << endl;
		#pragma omp atomic
		sum += harmony.second;
		#pragma omp critical
		costs.push_back(hs->getCostHistory());
		//hs->printMemory();
	}
	std::fstream fs;
	fs.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);

	for (auto v : costs) {
		for (auto x : v)
			fs << x << ",";
		fs << endl;
	}
	cout << "Harmony avg: " << sum /(Real)testSize << endl;
	fs.close();
}
