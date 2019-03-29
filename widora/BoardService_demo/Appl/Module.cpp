#include "Module.h"

Module::Module()
{

}

Module::~Module()
{

}

int Module::insModule(const char *file)
{

}

int Module::rmModule(const char *file)
{

}

int Module::execShell(const char *cmd)
{
	char *buf[20];

	memset(buf, 0, 20);
	sprintf(buf, "%s", "insmod ");
	memcpy(buf+7, );
}


