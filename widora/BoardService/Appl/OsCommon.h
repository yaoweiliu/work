#ifndef __OSCOMMON_H__
#define __OSCOMMON_H__

#include <pthread.h>
#include <semaphore.h>

// posix timer; most system supporting posix standard, so no need typedef.
//#define OS_create_timer(clockid, sevp, timerid)		timer_create(clockid, sevp, timerid)
//#define OS_delete_timer(timerid)					timer_delete(timerid)

//thread
typedef pthread_t os_pthread_t; 
#define OS_create_thread(thread, start_routine, arg)		pthread_create(&(thread), NULL, start_routine, arg)
#define OS_thread_join(thread, retval)						pthread_join(&(thread), retval)
#define OS_thread_exit(retval)								pthread_exit(retval)

//semaphore
typedef sem_t os_sem_t;
#define OS_sem_init(sem, pshared, value)			sem_init(&(sem), pshared, value)
#define OS_sem_wait(sem)							sem_wait(&(sem))
#define OS_sem_post(sem)							sem_post(&(sem))

#endif


