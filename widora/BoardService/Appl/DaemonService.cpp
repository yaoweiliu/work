#include "DaemonService.h"
#include <string.h>

DaemonService::DaemonService()
{

}

DaemonService::~DaemonService()
{
	
}

int DaemonService::start(int mode, int interval, timer_notify_cb cb)
{
	switch(mode) {
	case TIMER_ONCE:
		startOnceTimer();
		break;
	case TIMER_LOOP:
		startLoopTimer(interval, cb);
		break;
	default:
		break;
	}
}

int DaemonService::stop(void)
{
	return stopTimer();
}

int DaemonService::suspend(void)
{
	return 0;
}

int DaemonService::resume(void)
{
	return 0;
}

int DaemonService::startLoopTimer(int interval, timer_notify_cb cb)
{
	/* Variable Definition */
	struct timespec spec;
	struct sigevent ent;
	struct itimerspec value;

	/* Init */
	memset(&ent, 0x00, sizeof(struct sigevent));

	/* create a timer */
	ent.sigev_notify = SIGEV_THREAD;
	ent.sigev_notify_function = cb;

	timer_create(CLOCK_REALTIME, &ent, &id);        /* CLOCK_REALTIME */

	/* start a timer */
	clock_gettime(CLOCK_REALTIME, &spec);            /* CLOCK_REALTIME */
	value.it_value.tv_sec = spec.tv_sec + interval;
	value.it_value.tv_nsec = spec.tv_nsec + 0;
	value.it_interval.tv_sec = interval;    /* per second */
	value.it_interval.tv_nsec = 0;
	timer_settime(id, TIMER_ABSTIME, &value, NULL); /* TIMER_ABSTIME */

	return 0;
}

int DaemonService::startOnceTimer(void)
{
	return 0;
}

int DaemonService::stopTimer(void)
{
	timer_delete(id);

	return 0;
}


