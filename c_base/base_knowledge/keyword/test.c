#include "test.h"

void add_code(int code)
{
	send_event(code);
}

void process_event(int code)
{
	int i;

	for(i = 0; i < MAXNUM; i++) {
		if(get_code(i) == code) {
			printf("%s: find the code, code is %d, idx is %d.\n", __func__, get_code(i), i);
			break;
		}
	}
}

int main(int argc, char const *argv[])
{
	int i;

	for(i = 0; i < MAXNUM; i++) {
		add_code(i);
	}

	print(LEVEL_INFO, "print store code");
	
	process_event(107);

	return 0;
}

