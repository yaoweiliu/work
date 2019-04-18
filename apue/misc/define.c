#include <stdio.h>
#include <stdlib.h>

#define NAMEENTRY(name)	{name, #name}  //key-value, dict

struct enumName
{
	int value;
	const char *name;
};

static struct enumName ssname[] = 
{
	NAMEENTRY(0x1),
	NAMEENTRY(0x2),
	NAMEENTRY(0x3),
	NAMEENTRY(0x4),
	{-1, NULL},
};

int main(int argc, char const *argv[])
{
	int i = 0;

	while(ssname[i].value != -1) {
		printf("%s\n", ssname[i].name);
		i++;
	}
	
	return 0;
}
