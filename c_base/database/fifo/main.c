#include <stdio.h>
#include <stdlib.h>
#include "fifo.h"

int main(int argc, char const *argv[])
{
	DataNode_T *fifo = NULL;
	int i;

	fifo = malloc(sizeof(DataNode_T));

	fifo_init(fifo);

	for(i = 0; i < 2; i++) {
		DataNode_T *node = malloc(sizeof(DataNode_T));
		fifo_enqueue(fifo, node);
	}

	DataNode_T *tmp = NULL;
	i = fifo_dequeue(fifo, tmp);
	printf("i = %d\n", i);

	//printf("count = %d\n", fifo->count);

	return 0;
}