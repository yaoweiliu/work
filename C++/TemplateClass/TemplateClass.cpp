#include <iostream>
#include <cstring>
#include <string>

using namespace std;

template <class T>
class TempClass
{
public:
	TempClass(T a):i(a) {}
	~TempClass() {}

	void print(void);

private:
	T i;
};

template <class T>
void TempClass<T>::print(void)
{
	cout << "i = " << i << endl;
}

int main(int argc, char const *argv[])
{
	/*
	TempClass tc = TempClass(12);
	tc.print();
	*/
	TempClass <int> temp(12);//类模板的实例化
	temp.print();

	return 0;
}