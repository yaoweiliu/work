#include "event.h"
#include "ecl_events.h"

/*
static socket_info_t sinfo = {
	.sockfd = -1,
	.listenfd = -1,
	.acceptfd = -1,
	.pipefd = {-1, -1},
	.func = NULL,
};
*/

static int client_to_server_table[6][3] = {
	{EV_TBL_SS_START, EV_SS_START, -1},	
	{EV_TBL_QUERY_STATUS, EV_CRADLE_STATUS_QUERY, EV_ELEV_GET_HEIGHT},	
	{EV_TBL_CRADLE_START_MV_CMD, EV_CRADLE_MOVE_CMD, -1},
	{EV_TBL_ELEV_START_MV_CMD, EV_ELEV_MOVE_CMD, -1},
	{EV_TBL_CRADLE_STOP_MV_CMD, EV_CRADLE_MOVE_STOP, -1},
	{EV_TBL_ELEV_STOP_MV_CMD, EV_ELEV_STOP_CMD, -1},
};

static int server_to_client_table[9][3] = {
	{EV_SS_READY, EV_TBL_CRADLE_READY, -1},
	{EV_SS_READY, EV_TBL_ELEV_READY, -1},
	{EV_CRADLE_STATUS_RESP, EV_TBL_CRADLE_RESPONSE_STATUS, -1},
	{EV_ELEV_RES_HEIGHT, EV_TBL_ELEV_RESPONSE_STATUS, -1},
	{EV_CRADLE_POS_UPDATE, EV_TBL_CRADLE_POSITION_UPDATE, -1},
	{EV_ELEV_RES_HEIGHT, EV_TBL_ELEV_POSITION_UPDATE, -1},
	{EV_CRADLE_STOPED, EV_TBL_CRADLE_IS_STOPED, -1},
	{EV_ELEV_STOPED, EV_TBL_ELEV_IS_STOPED, -1},
	{EV_ERROR_REPORTING, EV_TBL_ERROR_REPORT, -1},
};

static int oem_tcp_listen(socket_info_t *sinfo)
{
	struct sockaddr_in address;
    int result;
    socklen_t count;

    sinfo->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(sinfo->sockfd >= 0 && "create tcp socket error");

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    //address.sin_addr.s_addr = inet_addr(BIND_IP); //client
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	//address.sin_port = htons(7990);
	address.sin_port = 7990;

    count = 1;
    setsockopt(sinfo->sockfd, SOL_SOCKET, SO_REUSEADDR, &count, sizeof(count));

	result = bind(sinfo->sockfd, (struct sockaddr *) &address, sizeof(address));
	assert(result >= 0 && "bind tcp socket error\n");
	
	if(result >= 0) {
		int arg;
		
		arg = 1;
		//set the socket non blocking. ???
		//arg = ioctl(sinfo->sockfd, FIONBIO, &arg);
		//assert (arg != -1 && "can't set listen socket FIONBIO");
		
		//Listen for connections.
		listen(sinfo->sockfd, MAXCONN);
		
		//sinfo->listenfd = sinfo->sockfd;
	} else {
		printf("bind failure on TCP\n");
	}
	
	//printf("%s:socket fd is %d\n", __func__, sinfo->sockfd);

    return 0;
}

static int destroy_socket(socket_info_t *sinfo)
{
	if(sinfo->sockfd != -1) {
		close(sinfo->sockfd);
		sinfo->sockfd = -1;
	}
}

static int oem_tcp_accept(socket_info_t *sinfo)
{
    struct sockaddr_in address;
    socklen_t addrlen;
    int arg = 0;
    int flag = 0;
	
	//printf("%s:socket fd is %d\n", __func__, sinfo->sockfd);
    addrlen = sizeof(address);
    sinfo->acceptfd = accept(sinfo->sockfd, (struct sockaddr *)&address, &addrlen);
	printf("%s:socket acceptfd is %d, error msg is %s, errno is %d\n", __func__, sinfo->acceptfd, strerror(errno), errno);
    assert(sinfo->acceptfd >= 0 && "Accept of connection failed");
    
    arg = 1;
    //set the socket to non block mode
    arg = ioctl(sinfo->acceptfd, FIONBIO, &arg);
    assert (arg != -1 && "can't set connecting socket FIONBIO");
    //Turn off the Nagle algorithm.
    flag = 1;
    setsockopt(sinfo->acceptfd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int));
    //Set keepalive on socket
    flag = 1;
    setsockopt(sinfo->acceptfd, SOL_SOCKET, SO_KEEPALIVE, &flag, sizeof(int));
	
    return 0;
}

void *oem_read_header(int fd, size_t lenth, ssize_t *size)
{
	char *buf = NULL;
	ssize_t rsize;
	//size_t lenth;
	
	//lenth = sizeof(EVENT_HEADER);
	buf = (char *)malloc(lenth);
	
	rsize = read(fd, buf, lenth);
	*size = rsize;
	if(rsize < 0)
		return NULL;
	return buf;
}

void *oem_read_payload(int fd, char *buf, size_t lenth, size_t header_len)
{
	char *data = NULL;
	ssize_t rsize;
	
	char *new_buf = (char *)realloc(buf, lenth);
	
	rsize = read(fd, new_buf+header_len, lenth-header_len);
	if(rsize < 0)
		return NULL;
	return new_buf;
}

void *oem_tcp_process(void *arg)
{
	int ret, i;
	fd_set readfds;
	socket_info_t *sinfo = (socket_info_t *)arg;
	char *buf = NULL;
	char *new_buf = NULL;
	size_t lenth;
	ssize_t rsize;
	EVENT_HEADER *header;
	
	lenth = sizeof(EVENT_HEADER);
	//buf = malloc(lenth);
	
	FD_ZERO(&readfds);
	FD_SET(sinfo->acceptfd, &readfds);
	
	while(1) {
		ret = select(sinfo->acceptfd+1, &readfds, NULL, NULL, NULL);
		if(ret < 0) {
			if(errno == EBADF || errno == EINTR)
				continue;
		}
		if(FD_ISSET(sinfo->acceptfd, &readfds)) {
			/*
			rsize = read(sinfo->acceptfd, buf, lenth);
			if(rsize < 0)
				return NULL;
			
			header = (EVENT_HEADER *)buf;
			if(header->length > lenth) //recalloc
			*/
			rsize = 0;
			buf = (char *)oem_read_header(sinfo->acceptfd, lenth, &rsize);
			printf("%s: %d buf is %p, rsize is %d\n", __func__, __LINE__, buf, rsize);
			if(buf && rsize) {
				header = (EVENT_HEADER *)buf;
				//free(buf);
				//recalloc
				if(header->length > lenth) {
					//read payload.
					new_buf = (char *)oem_read_payload(sinfo->acceptfd, buf, header->length, lenth);
					for(i = 0; i < sizeof(client_to_server_table)/sizeof(client_to_server_table[0]); i++) {
						if(header->code == client_to_server_table[i][0]) {
							//sent code to ctl but how? TODO
							printf("%s: %d event code is %d\n", __func__, __LINE__, header->code);
							break;
						}
					}
				}
				else {
					//this event has no payload.
					for(i = 0; i < sizeof(client_to_server_table)/sizeof(client_to_server_table[0]); i++) {
						if(header->code == client_to_server_table[i][0]) {
							//sent code to ctl but how? TODO
							printf("%s: %d event code is %d\n", __func__, __LINE__, header->code);
							break;
						}
					}
				}
				
				free(buf);
				buf = new_buf = NULL;
			}
			//else
				//return NULL;
		}
		
		sleep(2);
	}
}

static int thread_start(socket_info_t *sinfo)
{
	pthread_t pid;
	int ret;
	signal(SIGPIPE, SIG_IGN);
	ret = pthread_create(&pid, NULL, oem_tcp_process, (void *)sinfo);
	if(ret != 0)
	    printf("can't create thread: %s(errno: %d)\n", strerror(ret),ret);
	
	pthread_join(pid, NULL);
}

int main(int argc, char *argv[])
{
	socket_info_t *sinfo = (socket_info_t *)malloc(sizeof(socket_info_t));
	assert(sinfo != NULL && "malloc error");
	
	oem_tcp_listen(sinfo);
	oem_tcp_accept(sinfo);
	thread_start(sinfo);
	
	free(sinfo);
	
    return 0;
}



