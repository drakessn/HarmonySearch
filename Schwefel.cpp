#include "Schwefel.h"



Schwefel::Schwefel(Natural dimension)
{
	this->dimension = dimension;
	this->top = 500;
	this->down = -500;
	this->maximize = false;
}

Real Schwefel::CostFunction(vector<Real> x)
{
	Real sum1 = 0;
	for (auto xi : x)
		sum1 += xi*sin(sqrt(abs(xi)));

	return 418.9829*x.size() - sum1;
}


Schwefel::~Schwefel()
{
}
