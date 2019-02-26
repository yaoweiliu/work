#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

static pthread_mutex_t mutex;

static void *start_func(void *arg)
{
	int ret;

	pthread_mutex_lock(&mutex);

	printf("%s: fd %d\n", __func__, *((int *)arg));

	ret = write(*((int *)arg), "thread\n", 7);
	if(ret == -1) {
		printf("%s: write error\n", __func__);
		pthread_mutex_unlock(&mutex);
		pthread_exit(NULL);
		
		return NULL;
	}

	pthread_mutex_unlock(&mutex);

	pthread_exit(NULL);

	return NULL;
}

int main(int argc, char const *argv[])
{
	pthread_t pid[3];
	int ret, i;
	int fd;

	fd = open("./thread.txt", O_RDWR | O_CREAT | O_TRUNC);
	if(fd < 0) {
		perror("open()");
		exit(1);
	}

	pthread_mutex_init(&mutex, NULL);

	for(i = 0; i < 3; i++) {
		ret = pthread_create(&pid[i], NULL, start_func, (void *)(&fd));
		if(ret) {
			perror("pthread_create()");
			exit(1);
		}
	}

	for(i = 0; i < 3; i++)
		pthread_join(pid[i], NULL);

	return 0;
}
