#pragma once
#include "Function.h"
class Ackley :
	public Function
{
public:
	Ackley(Natural, Real = 20, Real = 0.2, Real = 2*PI);
	Real CostFunction(vector<Real> x);
	~Ackley();
private:
	Real a;
	Real b;
	Real c;
};

