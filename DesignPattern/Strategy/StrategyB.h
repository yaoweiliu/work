#ifndef _STRATEGYB_H__
#define _STRATEGYB_H__

#include <iostream>
#include "StrategyBase.h"

class StrategyB : public StrategyBase
{
public:
	StrategyB() {};
	~StrategyB() {};
	
	void algorithm(void);
};


#endif

