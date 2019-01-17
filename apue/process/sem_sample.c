#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>

static sem_t sem;

static void *start_func(void *arg)
{
	sem_post(&sem);
	printf("%s: post sem.\n", __func__);

	pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
	int ret;
	pthread_t pid;

	sem_init(&sem, 0, 0);

	ret = pthread_create(&pid, NULL, start_func, NULL);
	if(ret != 0) {
		perror("pthread_create()");
		sem_destroy(&sem);

		return -1;
	}

	do
	{
		ret = sem_wait(&sem);//if sem no increments[sem_post()], this call will blocked.
		printf("%s: wait sem.\n", __func__);
	} while(ret < 0);

	pthread_join(pid, NULL);

	return 0;
}