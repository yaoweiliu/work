#include "DaemonService.h"
#include <signal.h>
#include <time.h>

DaemonService::DaemonService()
{

}

DaemonService::~DaemonService()
{
	
}

int DaemonService::start(int mode)
{

}

int DaemonService::stop(void)
{

}

int DaemonService::suspend(void)
{

}

int DaemonService::resume(void)
{

}

int DaemonService::startLoopTimer(void)
{
	//timer_create(clockid_t clockid, struct sigevent *restrict evp, timer_t *restrict timerid)
	timer_create(CLOCK_REALTIME, );
}

int DaemonService::startOnceTimer(void)
{

}

int DaemonService::stopTimer(void)
{

}


