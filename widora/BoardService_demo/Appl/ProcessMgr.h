#ifndef __PROCESSMGR_H__
#define __PROCESSMGR_H__

#include "MgrBase.h"
#include "Process.h"

class ProcessMgr :public MgrBase
{
public:
	ProcessMgr(char *path);
	~ProcessMgr();
	
	int start(void);
	int stop(void);
	int suspend(void);
	int resume(void);
private:
	Process *proc;
	char *filePath;

private:
	int startDaemon(void);
};

#endif

