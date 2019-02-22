#include <iostream>
#include <cstring>

using namespace std;

class Base
{
public:
	Base() {a = new int; *a = 10;}
	virtual ~Base() {delete a; a = NULL;}
	int *a;	
};

class Base2 :public Base
{
public:
	Base2() {};
	~Base2() {};
};

int main(int argc, char const *argv[])
{
	Base2 *b = new Base2();

	cout << b->a << endl << *(b->a) << endl;

	delete b;

	b = NULL;

	return 0;
}