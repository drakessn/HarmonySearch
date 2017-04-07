#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>
#include <omp.h>
//#include <math.h>

using namespace std;

typedef unsigned int Natural;
typedef int Integer;
typedef double Real;
typedef pair<vector<Real>, Real> Harmony;
typedef pair<vector<Real>, Real> Chromosome;

#define maxCost 1000000.0
#define PI 3.141592653589793
#define testSize 100
#define debug false

enum functionType{Ackley2, Ackley10, Schwefel2, Schwefel10, Bench2, Bench10, Regular};