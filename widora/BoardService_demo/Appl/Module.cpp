#include "Module.h"
#include <string.h>

Module::Module()
{

}

Module::~Module()
{

}

int Module::insModule(const char *file)
{
	return execShell("insmod ", file);
}

int Module::rmModule(const char *file)
{
	return execShell("rmmod ", file);
}

int Module::execShell(const char *cmd, const char *file)
{
	char *buf[20];

	memset(buf, 0, 20);
	sprintf(buf, "%s", cmd);
	memcpy(buf+strlen(cmd), file, strlen(file));

	return system(buf);
}


