#pragma once
#include "Common.h"
class Function
{
public:
	virtual ~Function() {};
	virtual Real CostFunction(vector<Real>) = 0;
	void print();
	ostream& operator << (ostream &o);
	Natural getDimension();
	Real getTop();
	Real getDown();
	bool getMaximize();
protected:
	Natural dimension;	// dimension
	Real	top;		// upper bound
	Real	down;		// lower bound
	bool	maximize;	// maximize flag
};

