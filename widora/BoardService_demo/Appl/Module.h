#ifndef __MODULE_H__
#define __MODULE_H__

/* install/remove module */
#include <stdio.h>
#include <stdlib.h>

class Module
{
public:
	Module();
	~Module();

	int insModule(const char *file);
	int rmModule(const char *file);

private:
	int execShell(const char *cmd, const char *file);
};

#endif

