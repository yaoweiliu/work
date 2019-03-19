#include <iostream>

using namespace std;

typedef struct node{
	int a;
	int b;
	struct node *next;
	node(int x, int y):a(x),b(y),next(NULL) {}
}LIST_NODE;

LIST_NODE *addTwoNums(LIST_NODE *l1, LIST_NODE *l2)
{
	if(!l1 && !l2)
		return NULL;
	else if(!l1)
		return l2;
	else if(!l2)
		return l2;

	int a = l1->b + l2->b;
	LIST_NODE *p = new LIST_NODE(a, a%10);
	p->next = addTwoNums(l1->next, l2->next);
	if(a >= 10)
		p->next = new LIST_NODE(p->next, new LIST_NODE(0, 1));

	return p;
}

int main(int argc, char const *argv[])
{
	LIST_NODE *l1 = new LIST_NODE(5, 7);
	LIST_NODE *l2 = new LIST_NODE(3, 9);
	LIST_NODE *l3 = NULL;

	cout << l1->a << endl << l1->b << endl;
	cout << l2->a << endl << l2->b << endl;

	l3 = addTwoNums(l1, l2);

	cout << l3->a << endl << l3->b << endl;

	delete l1;
	delete l2;

	return 0;
}




