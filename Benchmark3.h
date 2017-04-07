#pragma once
#include "Function.h"
class Benchmark3 :
	public Function
{
public:
	Benchmark3(Natural);
	Real CostFunction(vector<Real> x);
	~Benchmark3();
};

