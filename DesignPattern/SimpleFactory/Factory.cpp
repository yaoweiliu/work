#include "Factory.h"
#include "ProductBase.h"
#include "ProductA.h"
#include "ProductB.h"
#include <iostream>

using namespace std;

Factory::Factory(int i)
{
	//this->i = i;
}

Factory::~Factory()
{

}

ProductBase *Factory::createProduct(int i)
{
	ProductBase *product = NULL;

	switch(i) {
	case 1:
		product = new ProductA();
		break;
	case 2:
		product = new ProductB();
		break;
	default:
		cout << "not suport." << endl;
		break;
	}

	return product;
}
