#include "Context.h"

using namespace std;

Context::Context(StrategyBase *base)
{
	this->strategy = base;
}

Context::~Context()
{
	cout << "~Context()" << endl;
	delete this->strategy;
}

void Context::doAction(void)
{
	this->strategy->algorithm();
}