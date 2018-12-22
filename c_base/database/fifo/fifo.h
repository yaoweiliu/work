#ifndef __FIFO_H__
#define __FIFO_H__

#define container_of(ptr, type, member)	\
		((type *)(((char *)(ptr))-((unsigned long)(&(((type *)0)->member)))))

typedef struct DataNode
{
	int count;
	struct DataNode *next;
}DataNode_T;

int fifo_init(DataNode_T *fifo);
int fifo_enqueue(DataNode_T *fifo, DataNode_T *node);
int fifo_dequeue(DataNode_T *fifo, DataNode_T *node);
void fifo_display(DataNode_T *fifo);

#endif

