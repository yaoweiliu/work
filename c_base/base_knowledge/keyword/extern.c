#include "test.h"

extern int store[MAXNUM];
extern int idx;
int send_event(int code)
{
	store[idx] = code;
	idx++;

	return 0;
}

int get_code(int i)
{
	return store[i];
}

void print(LEVEL level, char *info)
{
	int i;

	if(level >= PRINT_LEVEL) {
		for(i = 0; i < MAXNUM; i++) {
			printf("%s: store is %d\n", __func__, store[i]);
		}
	}
}

