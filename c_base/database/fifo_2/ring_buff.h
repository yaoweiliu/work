#ifndef __RING_BUFF_H__
#define __RING_BUFF_H__

#define	MAX_LEN	1024

typedef struct ring_buff_date
{
	int head;
	int tail;
	int lenth;
	int data[MAX_LEN];
}ring_buff_data_t;

static ring_buff_data_t ring_buff;

void ring_buff_init(void);

int ring_buff_push(int value);

int ring_buff_pop(int *value);

int ring_buff_is_full(void);

int ring_buff_is_empty(void);

#endif
