#ifndef __OEMSERVER_H__
#define __OEMSERVER_H__

#include "event.h"
#include "ecl_events.h"

class OemServer
{
public:
	OemServer();
	~OemServer();

private:
	socket_info_t sinfo;
	//int client_to_server_table[6][3];
	//int server_to_client_table[9][3];

	int oem_create_pipe(socket_info_t *sinfo);

	int oem_tcp_listen(socket_info_t *sinfo);
	void oem_close_fd(socket_info_t *sinfo);
	int oem_tcp_accept(socket_info_t *sinfo);
	//void oem_send_event(uint32_t id, uint32_t code, int code_elev, char *data);
	//void *oem_read_header(int fd, size_t lenth, ssize_t *size);
	//void *oem_read_payload(int fd, char *buf, size_t lenth, size_t header_len);
    //void *oem_tcp_process(void *arg);
	int thread_start(socket_info_t *sinfo);
public:
	void oem_service_start(OemServer *oem);
};











#endif


