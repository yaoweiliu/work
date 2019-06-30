#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	char c;

	fprintf(stdout, "%s\n", "Please entry a char: ");
	while(1) 
	{
		if(c = getchar() == 'c')
			break;
	}

	printf("%s %s %s %s\n", __DATE__, __TIME__, __func__, __FILE__);

	return 0;
}

