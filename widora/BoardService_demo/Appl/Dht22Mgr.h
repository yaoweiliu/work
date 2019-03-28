#ifndef __DHT22MGR_H__
#define __DHT22MGR_H__

#include "MgrBase.h"

#define DEVNAME	"/dev/dht22"

class Dht22Mgr :public MgrBase
{
public:
	Dht22Mgr();
	~Dht22Mgr();

	int start(void);
	int stop(void);
	int suspend(void);
	int resume(void);

private:
	int fd;

private:
	int insModule(void);
	char *getTemperatureAndHumidity(void);
};

#endif

