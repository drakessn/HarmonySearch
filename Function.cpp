#include "Function.h"

void Function::print()
{
	cout << "(" << dimension << ", " << down << ", " << top << ")" << endl;
}

ostream & Function::operator<<(ostream & o)
{
	o << "(" << dimension << ", " << top << ", " << down << ")";
	return o;
}

Natural Function::getDimension()
{
	return dimension;
}

Real Function::getTop()
{
	return top;
}

Real Function::getDown()
{
	return down;
}

bool Function::getMaximize()
{
	return maximize;
}
