#include "event.h"

class OemClient
{
public:
	OemClient();
	~OemClient();

private:
	socket_info_t sinfo;
	uint32_t tableFlags;
	int oem_tcp_connect(socket_info_t *sinfo);
	int thread_start(socket_info_t *sinfo);

	#define CLIENT_RECV_CRADLE_READY	(0x1 << 0)
	#define CLIENT_RECV_ELEV_READY		(0x1 << 1)
	#define CLIENT_RECV_CRADLE_RESP		(0x1 << 2)
	#define CLIENT_RECV_ELEV_RESP		(0x1 << 3)
public:
	int oemClientStart(OemClient *oem);
};


