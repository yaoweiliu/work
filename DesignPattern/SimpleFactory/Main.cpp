#include "Factory.h"
#include "ProductBase.h"
#include <iostream>

int main(int argc, char const *argv[])
{
	ProductBase *base = NULL;

	Factory *factory = new Factory(2);

	base = factory->createProduct(1);
	base->show();
	delete base;
	base = NULL;

	base = factory->createProduct(2);
	base->show();
	delete base;
	base = NULL;

	factory->createProduct(3);

	delete factory;

	return 0;
}