#ifndef __MGRBASE_H__
#define __MGRBASE_H__

#include "DaemonService.h"

class MgrBase
{
public:
	MgrBase();
	virtual ~MgrBase();

	virtual int start(void) = 0;
	virtual int stop(void) = 0;
	virtual int suspend(void) = 0;
	virtual int resume(void) = 0;
	
	DaemonService daemon;
};

#endif


