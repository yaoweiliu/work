#include "Process.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

extern char **environ;

Process::Process()
{

}

Process::~Process()
{

}

/*
 * param: path is file's path
 * 
 * return 0 is success, -1 is failed.
 */
int Process::run(const char *path)
{
	return execBin(path);
}

/*
 * param: path is file's path
 * 
 * return 0 is success, -1 is failed.
 */
int Process::kill(const char *path)
{
	return systemCmd(path);
}

/*
 * param: path is file's path
 * 
 * return 0 is success, -1 is failed.
 */
int Process::execBin(const char *path)
{
	pid_t pid;

	pid = fork();
	if(pid < 0) {
		perror("fork()");
		return -1;
	}
	else if(pid == 0) {
		execle(path, path, NULL, environ);
		exit(0);
	}

	return 0;
}

/*
 * param: cmd is 
 * 
 * return 0 is success, -1 is failed.
 */
int Process::systemCmd(const char *cmd)
{
	char shellCmd[SYS_CMD_LEN];

	memset(shellCmd, 0, SYS_CMD_LEN);

	sprintf(shellCmd, "ps | grep %s | grep -v 'grep' | awk '{print $1}' | xargs kill -s 9", cmd);

	return system(shellCmd);
}

