#include "event.h"

class OemClient
{
public:
	OemClient();
	~OemClient();

private:
	int oem_tcp_connect(socket_info_t *sinfo);
	
public:
	int oemClientStart(OemClient *oem);
};