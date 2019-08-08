#ifndef __DAEMONSERVICE_H__
#define __DAEMONSERVICE_H__

#include <signal.h>
#include <time.h>

typedef void (*timer_notify_cb)(union sigval val);

class DaemonService
{
public:
	DaemonService();
	~DaemonService();

	int start(int mode, int interval, timer_notify_cb cb);
	int stop(void);
	int suspend(void);
	int resume(void);

	#define TIMER_ONCE	0
	#define TIMER_LOOP	1

protected:
	int startLoopTimer(int interval, timer_notify_cb cb);
	int startOnceTimer(void);
	int stopTimer(void);
	//void timer_notify_function(union sigval val);

private:
	timer_t id;
};



#endif

