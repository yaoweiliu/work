#ifndef __PRODUCTBASE_H__
#define __PRODUCTBASE_H__

class ProductBase
{
public:
	ProductBase() {};
	virtual ~ProductBase() {};

	virtual void show(void) = 0;
private:

};


#endif
