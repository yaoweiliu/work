#include <stdlib.h>
#include <stdio.h>
#include "fifo.h"

int fifo_init(DataNode_T *fifo)
{
	fifo->count = 1;
	fifo->next = fifo;

	return 0;
}

int fifo_enqueue(DataNode_T *fifo, DataNode_T *node)
{
	fifo->next = node;
	node->next = fifo;

	fifo->count += 1;

	return 0;
}

int fifo_dequeue(DataNode_T *fifo, DataNode_T *node)
{
	DataNode_T *tmp;
	int lenth = 1;

	for(tmp = fifo; tmp->next != fifo; tmp++) {
		lenth += 1;
		printf("%d\n", lenth);
	}
	//node = tmp->next;
	tmp->next = tmp->next->next;
	free(tmp->next);

	return lenth-1;
}

void fifo_display(DataNode_T *fifo);