#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <glob.h>

#define	DELIM	" \t\n"

typedef struct gl_st
{
	glob_t gl;
}glob_st;

void print(void)
{
	printf("[Lyw-sh]$ ");
}

void parse_cmd(char *buffer, glob_st *gl)
{
	char *buf;
	int flag = 0;

	while(1) {
		buf = strsep(&buffer, DELIM);
		if(buf == NULL)
			break;

		if(buf[0] == '\0')
			continue;

		if(glob(buf, GLOB_NOCHECK | GLOB_APPEND * flag, NULL, &gl->gl)) {
			perror("glob()");
			exit(1);
		}

		flag = 1;
	}
}

int main(int argc, char const *argv[])
{
	pid_t pid;
	char *cmd_buffer = NULL;
	size_t n = 0;
	int ret;
	glob_st g_type;

	while(1) {
		print();

		ret = getline(&cmd_buffer, &n, stdin);
		if(ret == -1) {
			perror("getline()");
			break;
		}

		parse_cmd(cmd_buffer, &g_type);

		pid = fork();
		if(pid < 0) {
			perror("fork()");
			exit(1);
		}
		else if(pid == 0) {
			execvp(g_type.gl.gl_pathv[0], g_type.gl.gl_pathv);
			perror("execvp()");
			exit(1);
		}

		wait(NULL);
	}

	return 0;
}