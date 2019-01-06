#include <iostream>
#include <stdio.h>

namespace Data {
	int a = 10;
	short b = 5;
	char c = 'c';
};

int main(int argc, char const *argv[])
{
	using namespace Data;

	Data data = {
		.a = 10,
		.b = 12,
		.c = 'a',
	};

	printf("%d %d %c\n", data.a, data.b, data.c);

	return 0;
}