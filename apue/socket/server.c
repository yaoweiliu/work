#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h> 

#include "common.h"

int create_socket(struct socket_file *file)
{
	if((file->sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("sockt()");
		return -1;
	}
	printf("%s: fd = %d\n", __func__, file->sockfd);
	return 0;
}

int bind_socketfd(struct socket_file *file)
{
	struct sockaddr_in servaddr;
	socklen_t count;

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(DEFAULT_PORT);

	count = 1;
    setsockopt(file->sockfd, SOL_SOCKET, SO_REUSEADDR, &count, sizeof(count));
	printf("%s: fd = %d\n", __func__, file->sockfd);
	if(bind(file->sockfd, &servaddr, sizeof(servaddr)) == -1) {
		perror("bind()");
		return -1;
	}
	return 0;
}

int listen_socketfd(struct socket_file *file)
{
	if(listen(file->sockfd, 10) == -1) {
		perror("listen()");
		return -1;
	}
	return 0;
}

int accept_scoketfd(struct socket_file *file)
{
	while(1) {
		if((file->new_fd = accept(file->sockfd, (struct sockaddr*)NULL, NULL)) == -1){  
        	printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
        	continue;  
    	}

    	read(file->new_fd, file->buf, BUFLEN);
    	printf("data: %s", file->buf);
    	write(file->new_fd, file->buf, BUFLEN);
    	//file->buf = {0};
    	memset(file->buf, '\0', strlen(file->buf));

    	close(file->new_fd);
	}
}

int main(int argc, char const *argv[])
{
	struct socket_file *file = NULL;

	file = malloc(sizeof(struct socket_file));
	memset(file->buf, '\0', strlen(file->buf));

	if(create_socket(file)) {
		fprintf(stderr, "%s\n", "create socket error.");
		return -1;
	}

	if(bind_socketfd(file)) {
		fprintf(stderr, "%s\n", "bind socket error.");
		return -1;
	}

	listen_socketfd(file);

	accept_scoketfd(file);

	close(file->sockfd);
	free(file);

	return 0;
}


