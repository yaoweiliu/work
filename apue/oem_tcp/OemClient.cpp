#include "OemClient.h"

OemClient::OemClient()
{
	tableFlags = 0x0;
	sinfo.sockfd = -1;
	FD_ZERO(&sinfo.readfds);
}

OemClient::~OemClient()
{
	if(sinfo->sockfd != -1) {
		close(sinfo->sockfd);
		sinfo->sockfd = -1;
	}
}

int OemClient::oem_tcp_connect(socket_info_t *sinfo)
{
	struct sockaddr_in address;
	int ret = 0;
	int arg = 0;

	address.sin_family = AF_INET;
    address.sin_port = htons(7990);
	inet_pton(AF_INET, BIND_IP, &address.sin_addr);
		
	sinfo->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(sinfo->sockfd >= 0 && "can't create a TCP/IP socket");

    FD_SET(sinfo->sockfd, &sinfo->readfds);
    sinfo->maxFd = sinfo->sockfd;

	// Set to non-blocking mode.
	arg = 1;
	arg = ioctl(sinfo->sockfd, FIONBIO, &arg);
	assert (arg != -1 && "can't set listen socket FIONBIO");

	ret = connect(sinfo->sockfd, &address, sizeof(address));
	if(ret < 0) {
		if (errno == EINPROGRESS || errno == EWOULDBLOCK) {
			// A connection attempt is in progress.
			// wait some time for retry if in blocking mode, current only support non-blocking mode
		} else if (errno == EISCONN || errno == EALREADY) {
			// We are already connected.
		} else if (errno == ECONNREFUSED) {
			// connection refused
			// wait some time for retry if in blocking mode, current only support non-blocking mode
		} else {
		    //connection failed, start timer connect again.
			close(sinfo->sockfd);
			printf("%s: connect failed\n", __func__);
			return ret;
		}
	}
	return ret;
}

static int oemSentEvent2Server(socket_info_t *sinfo, void *event, int lenth)
{
	return write(sinfo->sockfd, event, lenth);
}

static void *oemRecvHeaderEventFromServer(int fd, char *buf, int lenth)
{
	ssize_t rsize;

	buf = (char *)malloc(lenth);
	if(!buf) {
		printf("%s: malloc error\n", __func__);
		return NULL;
	}
	memset(buf, '0', lenth);

	rsize = read(fd, buf, lenth);
	if(rsize < 0)
		return NULL;
	return buf;
}

static void *oemRecvPayloadEventFromServer(int fd, char *buf, int totalLen, int headerLen)
{
	ssize_t rsize;
	
	char *new_buf = (char *)realloc(buf, totalLen);
	if(!new_buf) {
		printf("%s: realloc error\n", __func__);
		return NULL;
	}
	
	rsize = read(fd, new_buf+headerLen, totalLen-headerLen);
	if(rsize < 0)
		return NULL;
	return new_buf;
}

static void *oem_tcp_sent_process(void *arg)
{
	EV_TBL_SS_START_TYPE event;
	socket_info_t *sinfo = (socket_info_t *)arg;

	//start.
	time_t now;
	time(&now);
	event.header.code = EV_TBL_SS_START;
	event.header.length = sizeof(EV_TBL_SS_START_TYPE);
	event.header.sid = IOC;
	event.header.rid = COU;
	event.header.checksum = EV_TBL_SS_START+sizeof(EV_TBL_SS_START_TYPE)+IOC+COU; //code + length + sid + rid
	event.header.sent = now;
	event.header.received = 0;

	oemSentEvent2Server(sinfo, (void *)&event, sizeof(EV_TBL_SS_START_TYPE));
	if(tableFlags & CLIENT_RECV_CRADLE_READY) {
		//sent cradle move cmd, and clear flag.???
	}
}

static void *oem_tcp_recv_process(void *arg)
{
	int ret;
	EVENT_HEADER *header = NULL;
	char *buf = NULL;
	char *newBuf = NULL;
	size_t lenth;
	socket_info_t *sinfo = (socket_info_t *)arg;

	lenth = sizeof(EVENT_HEADER);

	while(1) {
		ret = select(sinfo->maxFd+1, &sinfo->readfds, NULL, NULL, NULL);
		if(ret < 0) {
			if(errno == EBADF || errno == EINTR)
				continue;
		}
		if(FD_ISSET(sinfo->sockfd, &sinfo->readfds)) {
			buf = (char *)oemRecvHeaderEventFromServer(sinfo->sockfd, buf, lenth);
			if(!buf) {
				printf("%s: read header error\n", __func__);
				return NULL;
			}

			header = (EVENT_HEADER *)buf;
			if(header->length > lenth) {
				//read payload.
				newBuf = oemRecvPayloadEventFromServer(sinfo->sockfd, buf, header->length, lenth);
				switch(header->code) {
				case EV_TBL_CRADLE_RESPONSE_STATUS:
					EV_TBL_CRADLE_RESPONSE_STATUS_TYPE *event = (EV_TBL_CRADLE_RESPONSE_STATUS_TYPE *)newBuf;
					printf("EV_TBL_CRADLE_RESPONSE_STATUS: position %d isMoving %d isLatched %d\n", event->horizontalPos, event->isMoving, event->isLatched);
					tableFlags |= CLIENT_RECV_CRADLE_RESP;
					break;
				case EV_TBL_ELEV_RESPONSE_STATUS:
					EV_TBL_ELEV_RESPONSE_STATUS_TYPE *event = (EV_TBL_ELEV_RESPONSE_STATUS_TYPE *)newBuf;
					printf("EV_TBL_ELEV_RESPONSE_STATUS: position %d isMoving %d\n", event->verticalPos, event->isMoving);
					tableFlags |= CLIENT_RECV_ELEV_RESP;
					break;
				case EV_TBL_CRADLE_POSITION_UPDATE:
					EV_TBL_CRADLE_POSITION_UPDATE_TYPE *event = (EV_TBL_CRADLE_POSITION_UPDATE_TYPE *)newBuf;
					printf("EV_TBL_CRADLE_POSITION_UPDATE: position %d speed %d isMoving %d isLatched %d\n", event->currentPositionUm, event->currentSpeedUmPerSec, event->isMoving, event->isLatched);
					break;
				case EV_TBL_ELEV_POSITION_UPDATE:
					EV_TBL_ELEV_POSITION_UPDATE_TYPE *event = (EV_TBL_ELEV_POSITION_UPDATE_TYPE *)newBuf;
					printf("EV_TBL_ELEV_POSITION_UPDATE: position %d isMoving %d\n", event->currentPositionUm, event->isMoving);
					break;
				case EV_TBL_ERROR_REPORT:
					EV_TBL_ERROR_REPORT_TYPE *event = (EV_TBL_ERROR_REPORT_TYPE *)newBuf;
					printf("EV_TBL_ERROR_REPORT: errorMsg %s\n", event->errorMsg);
					break;
				default:
					printf("unsupported event\n");
					break;
				}
			}
			else {
				switch(header->code) {
				case EV_TBL_CRADLE_READY:
					printf("EV_TBL_CRADLE_READY: cradle is ready\n");
					tableFlags |= CLIENT_RECV_CRADLE_READY;
					break;
				case EV_TBL_ELEV_READY:
					printf("EV_TBL_ELEV_READY: elev is ready\n");
					tableFlags |= CLIENT_RECV_ELEV_READY;
					break;
				case EV_TBL_CRADLE_IS_STOPED:
					printf("EV_TBL_CRADLE_IS_STOPED: cradle is stoped\n");
					break;
				case EV_TBL_ELEV_IS_STOPED:
					printf("EV_TBL_ELEV_IS_STOPED: elev is stoped\n");
					break;
				default:
					printf("unsupported event\n");
					break;
				}
			}
		}
	}
}

int OemClient::thread_start(socket_info_t *sinfo)
{
	pthread_t recv_pid, sent_pid;
	int ret;

	signal(SIGPIPE, SIG_IGN);

	ret = pthread_create(&recv_pid, NULL, oem_tcp_recv_process, (void *)sinfo);
	if(ret != 0)
	    printf("can't create thread: %s(errno: %d)\n", strerror(ret),ret);

	ret = pthread_create(&sent_pid, NULL, oem_tcp_sent_process, (void *)sinfo);
	if(ret != 0)
	    printf("can't create thread: %s(errno: %d)\n", strerror(ret),ret);
	
	pthread_join(recv_pid, NULL);
	pthread_join(sent_pid, NULL);

    return 0;
}



