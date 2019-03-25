#ifndef __OSCOMMON_H__
#define __OSCOMMON_H__

#include <signal.h>
#include <time.h>
#include <pthread.h>

//timer
#define OS_create_timer(clockid, sevp, timerid)		timer_create(clockid, sevp, timerid)
#define OS_delete_timer(timerid)					timer_delete(timerid)

//thread
#define OS_create_thread(thread, attr, start_routine, arg)	pthread_create(thread, attr, start_routine, arg)
#define OS_thread_join(thread, retval)						pthread_join(thread, retval)
#define OS_thread_exit(retval)								pthread_exit(retval)

#endif


