#include "ring_buff.h"
#include <stdio.h>

void ring_buff_init(void)
{
	ring_buff.head = 0;
	ring_buff.tail = 0;
	ring_buff.lenth = 0;
}

int ring_buff_push(int value)
{
	if(ring_buff_is_full())
		return -1;

	ring_buff.data[ring_buff.tail] = value;
	ring_buff.lenth += 1;
	ring_buff.tail = (ring_buff.tail + 1) % MAX_LEN;

	return 0;
}

int ring_buff_pop(int *value)
{
	if(ring_buff_is_empty())
		return -1;

	*value = ring_buff.data[ring_buff.head];
	ring_buff.lenth -= 1;
	ring_buff.head = (ring_buff.head + 1) % MAX_LEN;

	return 0;
}

int ring_buff_is_full(void)
{
	return ring_buff.lenth >= MAX_LEN ? 1 : 0;
}

int ring_buff_is_empty(void)
{
	return ring_buff.lenth == 0 ? 1 : 0;
}


int main(int argc, char const *argv[])
{
	ring_buff_init();

	for(int i = 0; i < MAX_LEN; i++)
		ring_buff_push(i);

	for(int i = 0; i < MAX_LEN; i++) {
		printf("%d ", ring_buff.data[i]);
		if((i+1)%10 == 0)
			printf("\n");
	}
	printf("\nlenth: %d\n", ring_buff.lenth);

	for(int i = 0; i < MAX_LEN; i++)
		ring_buff_pop(&i);

	for(int i = 0; i < ring_buff.lenth; i++)
		printf("%d ", ring_buff.data[i]);
	printf("\nlenth: %d\n", ring_buff.lenth);

	return 0;
}

