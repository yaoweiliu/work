#ifndef __NETWORKMGR_H__
#define __NETWORKMGR_H__

#include <string>
#include "MgrBase.h"

class NetworkMgr :public MgrBase
{
public:
	NetworkMgr();
	~NetworkMgr();
	
	int start(void);
	int stop(void);
	int suspend(void);
	int resume(void);

private:
	void updateNtp(std::string ip);
};

#endif

