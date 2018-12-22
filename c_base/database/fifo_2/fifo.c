/*
 * fifo.c
 * copy from https://github.com/BiscuitOS/HardStack/blob/master/bus/fifo/algo/fifo.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct node
{
	int data;
	unsigned long base;
	unsigned long size;
	struct node *next;
};

struct head
{
	struct node *front;
	struct node *rear;
};

static int init_link_queue(struct head *LQ)
{
	struct node *p;

	p = (struct node *)malloc(sizeof(struct node));
	if(p == NULL) {
		printf("%s: no memory.\n", __func__);
		return -1;
	}
	else {
		p->next = NULL;
		LQ->front = p;
		LQ->rear = p;
	}

	return 0;
}

static int is_queue_empty(struct head *LQ)
{
	if(LQ->front == LQ->rear)//empty
		return 1;
	return 0;
}

static int push_element(struct head *LQ, int data)
{
	struct node *p = NULL;

	p = (struct node *)malloc(sizeof(struct node));
	if(p == NULL) {
		printf("%s: no memory.\n", __func__);
		return -1;
	}

	p->data = data;
	p->next = NULL;
	LQ->rear->next = p;
	LQ->rear = p;

	return 0;
}

static int pop_element(struct head *LQ, int *data)
{
	struct node *p = NULL;

	if(is_queue_empty(LQ)) {
		printf("%s: fifo is empty.\n", __func__);
		return -1;
	}

	p = LQ->front->next;
	*data = p->data;
	LQ->front->next = p->next;
	if(LQ->front->next == NULL)
		LQ->rear = LQ->front;
	free(p);

	return 0;
}

static int get_head_element(struct head *LQ, int *data)
{
	if(is_queue_empty(LQ)) {
		printf("%s: fifo is empty.\n", __func__);
		return -1;
	}

	*data = LQ->front->next->data;

	return 0;
}

int main(int argc, char const *argv[])
{
	struct head fifo;
	int i;

	if(init_link_queue(&fifo) < 0) {
		printf("%s: init fifo error.\n", __func__);
		return -1;
	}

	for(i = 0; i < 20; i++) {
		if(push_element(&fifo, i) < 0) {
			printf("%s: push data to fifo error.\n", __func__);
			return -1;
		}
	}

	while(!is_queue_empty(&fifo)) {
		int value;

		pop_element(&fifo, &value);
		sleep(1);
		printf("pop data: %2d\n", value);
	}

	return 0;
}


