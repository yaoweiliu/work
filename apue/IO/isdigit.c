#include <stdio.h>
#include <ctype.h>

int main(int argc, char const *argv[])
{
	char ch;

	printf("please enter a char or digit!\n");

	while(1) {
		scanf("%c", &ch);
		if(isdigit(ch))
			printf("It is digit!\n");
		else if(isalpha(ch))
			printf("It is alpha!\n");
		/*
		else if(ch == '\n') {
			printf("It is enter\n");
			//break;
		}
		*/
	}

	return 0;
}