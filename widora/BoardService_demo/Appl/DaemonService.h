#ifndef __DAEMONSERVICE_H__
#define __DAEMONSERVICE_H__

class DaemonService
{
public:
	DaemonService();
	~DaemonService();

	int start(int mode);
	int stop(void);
	int suspend(void);
	int resume(void);

protected:
	int startLoopTimer(void);
	int startOnceTimer(void);
	int stopTimer(void);
};



#endif

