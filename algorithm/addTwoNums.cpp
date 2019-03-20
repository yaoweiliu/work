#include <iostream>
#include <stdio.h>

using namespace std;

typedef struct node{
	int a;
	struct node *next;
	node(int x):a(x),next(NULL) {}
}LIST_NODE;

LIST_NODE *addTwoNums(LIST_NODE *l1, LIST_NODE *l2)
{
	if(!l1 && !l2)
		return NULL;
	else if(!l1)
		return l2;
	else if(!l2)
		return l2;

	int a = l1->a + l2->a;
	LIST_NODE *p = new LIST_NODE(a%10);
	p->next = addTwoNums(l1->next, l2->next);
	printf("A p->next: %p, p->a: %d\n", p->next, p->a);
	if(a >= 10) {
		p->next = addTwoNums(p->next, new LIST_NODE(1));
		printf("B p->next: %p, p->next->a: %d\n", p->next, p->next->a);
	}

	return p;
}

int main(int argc, char const *argv[])
{
	LIST_NODE *l1 = new LIST_NODE(5);
	LIST_NODE *l2 = new LIST_NODE(8);
	LIST_NODE *l3 = NULL;

	cout << l1->a << endl;
	cout << l2->a << endl;

	l3 = addTwoNums(l1, l2);
	cout << "===================================" << endl;

	cout << l3->a << endl;
	if(l3->next)
		cout << l3->next->a << endl;


	delete l1;
	delete l2;
	delete l3;

	l1 = NULL;
	l2 = NULL;
	l3 = NULL;

	return 0;
}




