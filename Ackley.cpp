#include "Ackley.h"

Ackley::Ackley(Natural dimension, Real a, Real b, Real c)
{
	this->dimension = dimension;
	this->top = 32.768;
	this->down = -32.768;
	this->maximize = false;
	this->a = a;
	this->b = b;
	this->c = c;
}

Real Ackley::CostFunction(vector<Real> x)
{
	Real sum1 = 0;
	for (auto xi : x)
		sum1 += xi*xi;
	Real sum2 = 0;
	for (auto xi : x)
		sum2 += cos(c*xi);

	return -a*exp(-b*sqrt(sum1 / (Real)x.size())) - exp(sum2 / (Real)x.size()) + a + exp(1);
}


Ackley::~Ackley()
{
}
