#ifndef __NETWORKMGR_H__
#define __NETWORKMGR_H__

#include <string>

class NetworkMgr
{
public:
	NetworkMgr();
	~NetworkMgr();
	
	int start(void);
	int stop(void);
	int suspend(void);
	int resume(void);

private:
	void updateNtp(string ip);
};

#endif

