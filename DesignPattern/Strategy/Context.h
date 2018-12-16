#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "StrategyBase.h"
#include <iostream>

class Context
{
public:
	Context(StrategyBase *base);
	~Context();

	void doAction(void);

private:
	StrategyBase *strategy;	
};

#endif
