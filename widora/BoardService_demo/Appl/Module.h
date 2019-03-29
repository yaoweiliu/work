#ifndef __MODULE_H__
#define __MODULE_H__

/* install/remove module */

class Module
{
public:
	Module();
	~Module();

	int insModule(const char *file);
	int rmModule(const char *file);

private:
	int execShell(const char *cmd);
};

#endif

