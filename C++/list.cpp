#include <iostream>
#include <string>
#include <list>
#include <stdio.h>

using namespace std;

list<int> lt;

void addToListBack(int data)
{
	lt.push_back(data);
}

int delFromListFront(void)
{
	int f = lt.front();

	lt.pop_front();

	return f;
}

int main(int argc, char const *argv[])
{
	int i;
	list<int>::iterator it;

	for(i = 0; i < 5; i++)
		addToListBack(i);

	printf("%s: size(%lu)\n", __func__, lt.size());

	for(it = lt.begin(); it != lt.end(); it++)
		printf("%s: %d\n", __func__, *it);

	while(!lt.empty())
		printf("%s: pop(%d) size(%lu)\n", __func__, delFromListFront(), lt.size());

	return 0;
}