#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <string>
#include <string.h>

#define SYS_CMD_LEN	128

class Process
{
public:
	Process();
	~Process();

	int run(const char *path);
	int kill(const char *path);

private:
	int execBin(const char *path);
	int systemCmd(const char *cmd);
};

#endif

