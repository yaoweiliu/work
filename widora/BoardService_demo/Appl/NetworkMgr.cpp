#include "NetworkMgr.h"
#include <stdlib.h>

NetworkMgr::NetworkMgr()
{

}

NetworkMgr::~NetworkMgr()
{

}

int NetworkMgr::start(void)
{
#ifdef INSTALLED_NEP_SERVER
	updateNtp("127.0.0.1");
#endif

	return 0;
}

int NetworkMgr::stop(void)
{
	return 0;
}

int NetworkMgr::suspend(void)
{
	return 0;
}

int NetworkMgr::resume(void)
{
	return 0;
}

void NetworkMgr::updateNtp(std::string ip)
{
	std::string cmd = "ntpdate " + ip;
	system((char *)cmd.c_str());
}



