#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	int number;
	struct node *next;
}person;

person *init_link(int n)
{
	person *head = (person *)malloc(sizeof(person));
	//if error
	head->number = 1;
	head->next = NULL;

	person *cyclic = head;

	for(int i = 2; i <= n; i++) {
		person *body = (person *)malloc(sizeof(person));
		body->number = i;
		body->next = NULL;
		cyclic->next = body;
		cyclic = body; //把head后移到下个节点
	}

	cyclic->next = head; //组成循环链表

	return head;
}

void find_and_kill_k(person *head, int k, int m)
{
	person *tail = head;

	//找到head的前驱节点
	while(tail->next != head) {
		tail = tail->next;
	}

	person *p = head;

	while(p->number != k) {
		tail = p;
		p = p->next;
	}

	while(p->next != p) { //p->next == p说明只有p了
		for(int i = 1; i < m; i++) {
			tail = p;
			p = p->next;
		}
		tail->next = p->next;
		printf("出列的人号码为: %d\n", p->number);
		free(p);
		p = tail->next;
	}

	printf("出列的人号码为: %d\n", p->number);
	free(p);
}

int main(int argc, char const *argv[])
{
	printf("输入人数: \n");
	int n;
	scanf("%d", &n);

	person *head = init_link(n);
	printf("从第k人开始报数(k>1 && k<%d)\n", n);
	int k;
	scanf("%d", &k);
	printf("数到m的人出列: \n");
	int m;
	scanf("%d", &m);
	find_and_kill_k(head, k, m);

	return 0;
}

