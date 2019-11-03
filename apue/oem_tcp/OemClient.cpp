#include "OemClient.h"

OemClient::OemClient()
{

}

OemClient::~OemClient()
{

}

int OemClient::oem_tcp_connect(socket_info_t *sinfo)
{
	struct sockaddr address;
	int fd = 0, ret = 0;
	int arg = 0;
	
	address = *(struct sockaddr *)&(svc->addr);
	
	fd = socket(AF_INET, SOCK_STREAM, 0);
    assert(fd >= 0 && "can't create a TCP/IP socket");

	// Set to non-blocking mode.
	arg = 1;
	arg = ioctl(fd, FIONBIO, &arg);
	assert (arg != -1 && "can't set listen socket FIONBIO");

	ret = connect(fd, &address, sizeof(address));
}



