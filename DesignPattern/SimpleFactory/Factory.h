#ifndef __FACTORY_H__
#define __FACTORY_H__

#include "ProductBase.h"

class Factory
{
public:
	Factory(int i);
	~Factory();
	ProductBase *createProduct(int);
	
private:
	//int i;
};


#endif
