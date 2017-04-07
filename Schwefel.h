#pragma once
#include "Function.h"
class Schwefel :
	public Function
{
public:
	Schwefel(Natural);
	Real CostFunction(vector<Real> x);
	~Schwefel();
};

