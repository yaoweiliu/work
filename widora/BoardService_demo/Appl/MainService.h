#ifndef __MAINSERVICE_H__
#define __MAINSERVICE_H__

#include <vector>
#include "MgrBase.h"
#include "ProcessMgr.h"
#include "NetworkMgr.h"
#include "Dht22Mgr.h"
#include "OsCommon.h"

using namespace std;

class MainService
{
public:
	MainService();
	~MainService();

	int start(void);
	int stop(void);
	int init(void);
	void threadStart(void);
	void daemonStart(void);

	os_sem_t serv_sem;

private:
	std::vector<MgrBase *> mgr;
	ProcessMgr *process;
	NetworkMgr *network;
	Dht22Mgr *dht22;

private:
	//void daemonStart(void);
	//void threadStart(void);
	void timer_notify_function(union sigval val);
	//static void *thread_entry(void *arg);
};

static void *thread_entry(void *arg);

#endif

