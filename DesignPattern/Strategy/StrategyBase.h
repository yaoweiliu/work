#ifndef _STRATEGYBASE_H__
#define _STRATEGYBASE_H__

class StrategyBase
{
public:
	StrategyBase() {};
	~StrategyBase() {};
	
	virtual void algorithm(void) = 0;
private:
	
};

#endif

