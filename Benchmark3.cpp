#include "Benchmark3.h"



Benchmark3::Benchmark3(Natural dimension)
{
	this->dimension = dimension;
	this->top = 100;
	this->down = -100;
	this->maximize = true;
}

Real Benchmark3::CostFunction(vector<Real> x)
{
	Real sum1 = 0;
	for (auto xi : x)
		sum1 += xi*xi;
	return 0.5 - (sin(sqrt(sum1))*sin(sqrt(sum1)) - 0.5) / ((1 + 0.001*sum1)* (1 + 0.001*sum1));
}


Benchmark3::~Benchmark3()
{
}
