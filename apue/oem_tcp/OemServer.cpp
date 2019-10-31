#include "OemServer.h"
#include "OemErrorMsg.h"

static int client_to_server_table[6][4] = {
	{EV_TBL_SS_START, EV_SS_START, -1, ID_ALL},	
	{EV_TBL_QUERY_STATUS, EV_CRADLE_STATUS_QUERY, EV_ELEV_GET_HEIGHT, ID_ALL},	
	{EV_TBL_CRADLE_START_MV_CMD, EV_CRADLE_MOVE_CMD, -1, ID_CRADLE}, //has payload
	{EV_TBL_ELEV_START_MV_CMD, EV_ELEV_MOVE_CMD, -1, ID_ELEV}, //has payload
	{EV_TBL_CRADLE_STOP_MV_CMD, EV_CRADLE_MOVE_STOP, -1, ID_CRADLE}, //has payload, fromtype
	{EV_TBL_ELEV_STOP_MV_CMD, EV_ELEV_STOP, -1, ID_ELEV},
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

int oemPipeFd[2] = {-1, -1};

OemServer::OemServer()
{
	sinfo.sockfd = -1;
	sinfo.acceptfd = -1;
	oemPipeFd[0] = -1;
	oemPipeFd[1] = -1;
}

OemServer::~OemServer()
{
	oem_close_fd(&sinfo);
}

int OemServer::oem_tcp_listen(socket_info_t *sinfo)
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

void OemServer::oem_close_fd(socket_info_t *sinfo)
{
	if(sinfo->sockfd != -1) {
		close(sinfo->sockfd);
		sinfo->sockfd = -1;
	}

	if(sinfo->acceptfd != -1) {
		close(sinfo->acceptfd);
		sinfo->acceptfd = -1;
	}

	if(oemPipeFd[0] != -1) {
		close(oemPipeFd[0]);
		oemPipeFd[0] = -1;
	}

	if(oemPipeFd[1] != -1) {
		close(oemPipeFd[1]);
		oemPipeFd[1] = -1;
	}
}

int OemServer::oem_tcp_accept(socket_info_t *sinfo)
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

int OemServer::oem_create_pipe(socket_info_t *sinfo)
{
	//return pipe(sinfo->pipefd);
	return pipe(oemPipeFd);
}

static void sentCradleReady(char *data, int fd, time_t now)
{
	EVENT_HEADER_TYPE *sys_event = (EVENT_HEADER_TYPE *)data;
	if(sys_event->sid == SVC_ID_SC_CRADLE_CTRL) {
		EV_TBL_CRADLE_READY_TYPE event;
		event.header.code = EV_TBL_CRADLE_READY;
		event.header.length = sizeof(EV_TBL_CRADLE_READY_TYPE);
		event.header.sid = COU;
		event.header.rid = IOC;
		event.header.checksum = EV_TBL_CRADLE_READY+sizeof(EV_TBL_CRADLE_READY_TYPE)+COU+IOC;
		event.header.sent = now;
		event.header.received = 0;
		write(fd, &event, sizeof(EV_TBL_CRADLE_READY_TYPE));
	}
}

static void sentElevReady(char *data, int fd, time_t now)
{
	EVENT_HEADER_TYPE *sys_event = (EVENT_HEADER_TYPE *)data;
	if(sys_event->sid == SVC_ID_SC_ELEV_CTRL) {
		EV_TBL_ELEV_READY_TYPE event;
		event.header.code = EV_TBL_ELEV_READY;
		event.header.length = sizeof(EV_TBL_ELEV_READY_TYPE);
		event.header.sid = COU;
		event.header.rid = IOC;
		event.header.checksum = EV_TBL_ELEV_READY+sizeof(EV_TBL_ELEV_READY_TYPE)+COU+IOC;
		event.header.sent = now;
		event.header.received = 0;
		write(fd, &event, sizeof(EV_TBL_ELEV_READY_TYPE));
	}
}

static void sentCradleResp(char *data, int fd, time_t now)
{
	EV_CRADLE_STATUS_RESP_TYPE *sys_event = (EV_CRADLE_STATUS_RESP_TYPE *)data;
	if(sys_event->header.sid == SVC_ID_CRADLE) {
		EV_TBL_CRADLE_RESPONSE_STATUS_TYPE event;
		event.header.code = EV_TBL_CRADLE_RESPONSE_STATUS;
		event.header.length = sizeof(EV_TBL_CRADLE_RESPONSE_STATUS_TYPE);
		event.header.sid = COU;
		event.header.rid = IOC;
		event.header.checksum = EV_TBL_CRADLE_RESPONSE_STATUS+sizeof(EV_TBL_CRADLE_RESPONSE_STATUS_TYPE)+COU+IOC;
		event.header.sent = now;
		event.header.received = 0;
		event.horizontalPos = sys_event->curPosUm;
		event.isMoving = sys_event->isMoving;
		event.isLatched = sys_event->isLatched;
		write(fd, &event, sizeof(EV_TBL_CRADLE_RESPONSE_STATUS_TYPE));
	}
}

static void sentElevResp(char *data, int fd, time_t now)
{
	EV_ELEV_RES_HEIGHT_TYPE *sys_event = (EV_ELEV_RES_HEIGHT_TYPE *)data;
	if(sys_event->header.sid == SVC_ID_ELEV_MGR) {
		EV_TBL_ELEV_RESPONSE_STATUS_TYPE event;
		event.header.code = EV_TBL_ELEV_RESPONSE_STATUS;
		event.header.length = sizeof(EV_TBL_ELEV_RESPONSE_STATUS_TYPE);
		event.header.sid = COU;
		event.header.rid = IOC;
		event.header.checksum = EV_TBL_ELEV_RESPONSE_STATUS+sizeof(EV_TBL_ELEV_RESPONSE_STATUS_TYPE)+COU+IOC;
		event.header.sent = now;
		event.header.received = 0;
		event.verticalPos = sys_event->height;
		event.isMoving = sys_event->isMoving;
		write(fd, &event, sizeof(EV_TBL_ELEV_RESPONSE_STATUS_TYPE));
	}
}

static void sentCradlePosUpdate(char *data, int fd, time_t now)
{
	EV_CRADLE_POS_UPDATE_TYPE *sys_event = (EV_CRADLE_POS_UPDATE_TYPE *)data;
	if(sys_event->header.sid == SVC_ID_CRADLE) {
		EV_TBL_CRADLE_POSITION_UPDATE_TYPE event;
		event.header.code = EV_TBL_CRADLE_POSITION_UPDATE;
		event.header.length = sizeof(EV_TBL_CRADLE_POSITION_UPDATE_TYPE);
		event.header.sid = COU;
		event.header.rid = IOC;
		event.header.checksum = EV_TBL_CRADLE_POSITION_UPDATE+sizeof(EV_TBL_CRADLE_POSITION_UPDATE_TYPE)+COU+IOC;
		event.header.sent = now;
		event.header.received = 0;
		event.currentPositionUm = sys_event->curPosUm;
		event.currentSpeedUmPerSec = sys_event->curSpeedUmPerSec;
		event.isMoving = sys_event->isMoving;
		event.isLatched = sys_event->isLatched;
		if(!event.isMoving) {
			EV_TBL_CRADLE_IS_STOPED_TYPE stopedEv;
			stopedEv.header.code = EV_TBL_CRADLE_IS_STOPED;
			stopedEv.header.length = sizeof(EV_TBL_CRADLE_IS_STOPED_TYPE);
			stopedEv.header.sid = COU;
			stopedEv.header.rid = IOC;
			stopedEv.header.checksum = EV_TBL_CRADLE_IS_STOPED+sizeof(EV_TBL_CRADLE_IS_STOPED_TYPE)+COU+IOC;
			stopedEv.header.sent = now;
			stopedEv.header.received = 0;
			write(fd, &stopedEv, sizeof(EV_TBL_CRADLE_IS_STOPED_TYPE));
		}
		else
			write(fd, &event, sizeof(EV_TBL_CRADLE_POSITION_UPDATE_TYPE));
	}
}

static void sentElevPosUpdate(char *data, int fd, time_t now)
{
	EV_ELEV_RES_HEIGHT_TYPE *sys_event = (EV_ELEV_RES_HEIGHT_TYPE *)data;
	if(sys_event->header.sid == SVC_ID_ELEV_MGR) {
		EV_TBL_ELEV_POSITION_UPDATE_TYPE event;
		event.header.code = EV_TBL_ELEV_POSITION_UPDATE;
		event.header.length = sizeof(EV_TBL_ELEV_POSITION_UPDATE_TYPE);
		event.header.sid = COU;
		event.header.rid = IOC;
		event.header.checksum = EV_TBL_ELEV_POSITION_UPDATE+sizeof(EV_TBL_ELEV_POSITION_UPDATE_TYPE)+COU+IOC;
		event.header.sent = now;
		event.header.received = 0;
		event.currentPositionUm = sys_event->height;
		event.isMoving = sys_event->isMoving;
		if(!event.isMoving) {
			EV_TBL_ELEV_IS_STOPED_TYPE stopedEv;
			stopedEv.header.code = EV_TBL_ELEV_IS_STOPED;
			stopedEv.header.length = sizeof(EV_TBL_ELEV_IS_STOPED_TYPE);
			stopedEv.header.sid = COU;
			stopedEv.header.rid = IOC;
			stopedEv.header.checksum = EV_TBL_ELEV_IS_STOPED+sizeof(EV_TBL_ELEV_IS_STOPED_TYPE)+COU+IOC;
			stopedEv.header.sent = now;
			stopedEv.header.received = 0;
			write(fd, &stopedEv, sizeof(EV_TBL_ELEV_IS_STOPED_TYPE));
		}
		else
			write(fd, &event, sizeof(EV_TBL_ELEV_POSITION_UPDATE_TYPE));
	}
}

/*
void sentCradleStoped(char *data, int fd, time_t now)
{
}
*/

static void sentErrorMsg(char *data, int fd, time_t now)
{
	unsigned int i;
	EV_TBL_ERROR_REPORT_TYPE event;
	EV_ERROR_REPORTING_TYPE *sys_event = (EV_ERROR_REPORTING_TYPE *)data;

	event.header.code = EV_TBL_ERROR_REPORT;
	event.header.length = sizeof(EV_TBL_ERROR_REPORT_TYPE);
	event.header.sid = COU;
	event.header.rid = IOC;
	event.header.checksum = EV_TBL_ERROR_REPORT+sizeof(EV_TBL_ERROR_REPORT_TYPE)+COU+IOC;
	event.header.sent = now;
	event.header.received = 0;
	if(sys_event->err.component == ERR_COMP_CRADLE) {
		for(i = 0; i < sizeof(cradleErrorTable)/sizeof(cradleErrorTable[0]); i++) {
			if(sys_event->err.errNum == cradleErrorTable[i].num) {
				event.errorMsg = cradleErrorTable[i].info;
				break;
			}
		}
	}
	else if(sys_event->err.component == ERR_COMP_TILT_ELEV) {
		for(i = 0; i < sizeof(elevErrorTable)/sizeof(elevErrorTable[0]); i++) {
			if(sys_event->err.errNum == elevErrorTable[i].num) {
				event.errorMsg = elevErrorTable[i].info;
				break;
			}
		}
	}
	write(fd, &event, sizeof(EV_TBL_ERROR_REPORT_TYPE));
}

static void oem_msg_to_client(uint32_t code, char *data, int fd)
{
	time_t now;
	time(&now);

	switch(code) {
	case EV_TBL_CRADLE_READY:
		sentCradleReady(data, fd, now);
		break;
	case EV_TBL_ELEV_READY:
		sentElevReady(data, fd, now);
		break;
	case EV_TBL_CRADLE_RESPONSE_STATUS:
		sentCradleResp(data, fd, now);
		break;
	case EV_TBL_ELEV_RESPONSE_STATUS:
		sentElevResp(data, fd, now);
		break;
	case EV_TBL_CRADLE_POSITION_UPDATE:
		sentCradlePosUpdate(data, fd, now);
		break;
	case EV_TBL_ELEV_POSITION_UPDATE:
		sentElevPosUpdate(data, fd, now);
		break;
	case EV_TBL_CRADLE_IS_STOPED:
		//sentCradleStoped(data, fd, now);
		break;
	case EV_TBL_ELEV_IS_STOPED:
		break;
	case EV_TBL_ERROR_REPORT:
		sentErrorMsg(data, fd, now);
		break;
	default:
		printf("unsuport event\n");
		break;
	}
}

//
static void oem_send_event(uint32_t id, uint32_t code, int code_elev, char *data)
{
	EVENT_HEADER_TYPE header;

	switch(id & 0xff) {
	case ID_CRADLE:
		if(code == EV_CRADLE_MOVE_STOP) {
			EV_CRADLE_MOVE_STOP_TYPE cradleStopEv;
			cradleStopEv.fromType = MOTION_PANEL;
			//ECService::sendEvent(SVC_ID_SC_CRADLE_CTRL, EV_CRADLE_MOVE_STOP, &cradleStopEv, sizeof(cradleStopEv));
			ECService::sendEventFrom(SVC_ID_SC_CRADLE_CTRL, SVC_ID_SC_CRADLE_CTRL, EV_CRADLE_MOVE_STOP, (void*)(&cradleStopEv), sizeof(EV_CRADLE_MOVE_STOP_TYPE));
		}
		else if(code == EV_CRADLE_MOVE_CMD) {
			EV_TBL_CRADLE_START_MV_CMD_TYPE *event = (EV_TBL_CRADLE_START_MV_CMD_TYPE *)data;
			EV_CRADLE_MOVE_CMD_TYPE moveCmdEv;
			moveCmdEv.fromType = MOTION_PANEL;
			moveCmdEv.targetSpeedUmPerSec = event->targetSpeedUmPerSec;
			moveCmdEv.targetPositionUm = event->targetPositionUm;
			moveCmdEv.xrayStartPosUm = 0;
			moveCmdEv.scanRealStartPosUm = 0;
			moveCmdEv.scanStartPosUm = 0;
			moveCmdEv.syncWithAxial = FW_FALSE;
			moveCmdEv.isScanMode = FW_FALSE;
			moveCmdEv.preScanDelayMs = 0;
			moveCmdEv.gpSide = PANEL_DEFAULT;
			moveCmdEv.btnSource = GP_FRONT;
			//moveCmdEv.dir = elevStatus.direction;
			//ECService::sendEvent(SVC_ID_SC_CRADLE_CTRL, EV_CRADLE_MOVE_CMD, &moveCmdEv, sizeof(EV_CRADLE_MOVE_CMD_TYPE));
			ECService::sendEventFrom(SVC_ID_SC_CRADLE_CTRL, SVC_ID_SC_CRADLE_CTRL, EV_CRADLE_MOVE_CMD, (void*)(&moveCmdEv), sizeof(EV_CRADLE_MOVE_CMD_TYPE));
		}
		else {
			//ECService::sendEvent(SVC_ID_CRADLE, code);
			ECService::sendEventFrom(SVC_ID_SC_CRADLE_CTRL, SVC_ID_CRADLE, code, (void*)(&header), sizeof(EVENT_HEADER_TYPE));
		}
		break;
    case ID_ELEV:
		if(code == EV_ELEV_MOVE_CMD) {
			EV_TBL_ELEV_START_MV_CMD_TYPE *event_elev = (EV_TBL_ELEV_START_MV_CMD_TYPE *)data;
			EV_ELEV_MOVE_CMD_TYPE moveCmdEv;
			moveCmdEv.height = event_elev->targetPositionUm;
			moveCmdEv.motionType = ELEV_MV_TYPE_NORMAL;
			moveCmdEv.fromType = ELEV_MOTION_PANEL;
			moveCmdEv.btnSource = GP_FRONT;
			//moveCmdEv.dir = elevStatus.direction;
			moveCmdEv.gpSide = PANEL_DEFAULT;
			//ECService::sendEvent(SVC_ID_SC_ELEV_CTRL, EV_ELEV_MOVE_CMD, &moveCmdEv, sizeof(EV_ELEV_MOVE_CMD_TYPE));
			ECService::sendEventFrom(SVC_ID_SC_ELEV_CTRL, SVC_ID_SC_ELEV_CTRL, EV_ELEV_MOVE_CMD, (void*)(&moveCmdEv), sizeof(EV_ELEV_MOVE_CMD_TYPE));
		}
		else {
			//ECService::sendEvent(SVC_ID_SC_ELEV_CTRL, code);
			ECService::sendEventFrom(SVC_ID_SC_ELEV_CTRL, SVC_ID_SC_ELEV_CTRL, code, (void*)(&header), sizeof(EVENT_HEADER_TYPE));
		}
		break;
	case ID_ALL:
		if(code_elev != -1) {
			//ECService::sendEvent(SVC_ID_SC_ELEV_CTRL, code_elev);
			ECService::sendEventFrom(SVC_ID_SC_ELEV_CTRL, SVC_ID_SC_ELEV_CTRL, code_elev, (void*)(&header), sizeof(EVENT_HEADER_TYPE));
			ECService::sendEventFrom(SVC_ID_SC_CRADLE_CTRL, SVC_ID_CRADLE, code, (void*)(&header), sizeof(EVENT_HEADER_TYPE));
		}
		else {
			//ECService::sendEvent(SVC_ID_SC_ELEV_CTRL, code);
			ECService::sendEventFrom(SVC_ID_SC_ELEV_CTRL, SVC_ID_ELEV_MGR, code, (void*)(&header), sizeof(EVENT_HEADER_TYPE));
			ECService::sendEventFrom(SVC_ID_SC_CRADLE_CTRL, SVC_ID_CRADLE, code, (void*)(&header), sizeof(EVENT_HEADER_TYPE));
		}
		//ECService::sendEvent(SVC_ID_SC_CRADLE_CTRL, code);
		break;
	default:
		printf("unsuport id\n");
		break;
	}

	return ;
}

static void *oem_read_header(int fd, size_t lenth, ssize_t *size)
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

static void *oem_read_payload(int fd, char *buf, size_t lenth, size_t header_len)
{
	//char *data = NULL;
	ssize_t rsize;
	
	char *new_buf = (char *)realloc(buf, lenth);
	
	rsize = read(fd, new_buf+header_len, lenth-header_len);
	if(rsize < 0)
		return NULL;
	return new_buf;
}

//lenth???
//void *oem_read_pipe(int fd, )

static void *oem_tcp_process(void *arg)
{
	int ret;
    unsigned int i;
	fd_set readfds;
	socket_info_t *sinfo = (socket_info_t *)arg;
	char *buf = NULL;
	char *new_buf = NULL;
	size_t lenth;
	ssize_t rsize;
	EVENT_HEADER *header;
	EVENT_HEADER_TYPE *sys_header;
	size_t sys_lenth;
	
	lenth = sizeof(EVENT_HEADER);
	sys_lenth = sizeof(EVENT_HEADER_TYPE);
	//buf = malloc(lenth);
	
	FD_ZERO(&readfds);
	FD_SET(sinfo->acceptfd, &readfds);
	FD_SET(oemPipeFd[0], &readfds);
	
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
				if(header->sid == COU) {
					//free(buf);
					//recalloc
					if(header->length > (int)lenth) {
						//read payload.
						new_buf = (char *)oem_read_payload(sinfo->acceptfd, buf, header->length, lenth);
						for(i = 0; i < sizeof(client_to_server_table)/sizeof(client_to_server_table[0]); i++) {
							if(header->code == client_to_server_table[i][0]) {
								//sent code to ctl but how? TODO
								oem_send_event(client_to_server_table[i][3], client_to_server_table[i][1], client_to_server_table[i][2], new_buf);
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
								oem_send_event(client_to_server_table[i][3], client_to_server_table[i][1], client_to_server_table[i][2], buf);
								printf("%s: %d event code is %d\n", __func__, __LINE__, header->code);
								break;
							}
						}
					}
				}
				
				free(buf);
				buf = new_buf = NULL;
			}
			//else
				//return NULL;
		}
		else if(FD_ISSET(oemPipeFd[0], &readfds))
		{
			//read pipe[0], then write sockfd.
			//1. read header; 2. read payload.
			rsize = 0;
			buf = (char *)oem_read_header(oemPipeFd[0], sys_lenth, &rsize);
			printf("%s: %d buf is %p, rsize is %d\n", __func__, __LINE__, buf, rsize);
			if(buf && rsize) {
				sys_header = (EVENT_HEADER_TYPE *)buf;
				if(sys_header->length > (int)sys_lenth) {
					//read payload.
					printf("%s: %d event code is %d\n", __func__, __LINE__, sys_header->code);
					new_buf = (char *)oem_read_payload(oemPipeFd[0], buf, sys_header->length, sys_lenth);
					for(i = 0; i < sizeof(server_to_client_table)/sizeof(server_to_client_table[0]); i++) {
						if((int)sys_header->code == server_to_client_table[i][0]) {
							oem_msg_to_client(server_to_client_table[i][1], new_buf, sinfo->acceptfd);
							break;
						}
					}
				}
				else
				{
					//write sockfd.
					for(i = 0; i < sizeof(server_to_client_table)/sizeof(server_to_client_table[0]); i++) {
						if((int)sys_header->code == server_to_client_table[i][0]) {
							oem_msg_to_client(server_to_client_table[i][1], buf, sinfo->acceptfd);
							break;
						}
					}
					printf("%s: %d event code is %d\n", __func__, __LINE__, sys_header->code);
				}

				free(buf);
				buf = new_buf = NULL;
			}
		}
		
		sleep(2);
	}
    return NULL;
}

int OemServer::thread_start(socket_info_t *sinfo)
{
	pthread_t pid;
	int ret;
	signal(SIGPIPE, SIG_IGN);
	ret = pthread_create(&pid, NULL, oem_tcp_process, (void *)sinfo);
	if(ret != 0)
	    printf("can't create thread: %s(errno: %d)\n", strerror(ret),ret);
	
	pthread_join(pid, NULL);

    return 0;
}

void OemServer::oem_service_start(OemServer *oem)
{
	oem_tcp_listen(&oem->sinfo);
	oem_tcp_accept(&oem->sinfo);
	oem_create_pipe(&oem->sinfo);
	thread_start(&oem->sinfo);
}




