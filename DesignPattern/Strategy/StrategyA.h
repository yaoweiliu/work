#ifndef __STRATEGYA_H__
#define __STRATEGYA_H__

#include <iostream>
#include "StrategyBase.h"

class StrategyA : public StrategyBase
{
public:
	StrategyA() {};
	~StrategyA() {};

	void algorithm(void);
	
private:

};

#endif
