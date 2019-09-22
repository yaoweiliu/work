#include <stdio.h>
#include <stdarg.h>

static void char_to_ascii(unsigned int arg)
{
	unsigned int tmp = arg;

	tmp = (arg / 10);
	if(tmp != 0)
		char_to_ascii(tmp);//此处不会在继续输出字符，待递归结束再依次输出前一次递归栈上的值

	putchar(arg % 10 +'0');
}

int main(int argc, char const *argv[])
{
	unsigned int arg = 4567;

	char_to_ascii(arg);
	printf("\n");

	return 0;
}


