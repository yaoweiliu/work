#ifndef __DAEMONSERVICE_H__
#define __DAEMONSERVICE_H__

class DaemonService
{
public:
	DaemonService();
	~DaemonService();

	int start(void);
	int stop(void);
	int suspend(void);
	int resume(void);

protected:
	int startLoopTimer();
	int startOnceTimer();
	int stopTimer();
};



#endif

