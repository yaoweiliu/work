#include <stdio.h>
#include <glob.h>

int main(void)
{
	int i = 0;
	glob_t buf;

	glob("/home/lyw/*", GLOB_TILDE, NULL, &buf);

	glob("/home/lyw/Appl/*", GLOB_TILDE | GLOB_APPEND, NULL, &buf);

	for(i = 0; i < buf.gl_pathc; i++) {
		printf("the gl_pathv[%2d] is %s\n", i, buf.gl_pathv[i]);
	}

	globfree(&buf);
	
	return 0;
}
