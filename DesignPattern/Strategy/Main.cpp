#include <iostream>
#include "Context.h"
#include "StrategyBase.h"
#include "StrategyA.h"
#include "StrategyB.h"

int main(int argc, char const *argv[])
{
	StrategyBase *a = new StrategyA();
	StrategyBase *b = new StrategyB();
	Context *con = new Context(b);

	//con->strategy->algorithm();
	con->doAction();

	delete con;

	return 0;
}