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

void print_list(void)
{
	list<int>::iterator it;

	for(it = lt.begin(); it != lt.end(); it++)
		printf("%s: %d\n", __func__, *it);
}

int main(int argc, char const *argv[])
{
	int i;

	for(i = 0; i < 12; i++) {
		addToListBack(i);
		if(lt.size() >= 2) {
			delFromListFront();
			print_list();
		}
	}

	printf("%s: size(%lu)\n", __func__, lt.size());

	while(!lt.empty())
		printf("%s: pop(%d) size(%lu)\n", __func__, delFromListFront(), lt.size());

	return 0;
}