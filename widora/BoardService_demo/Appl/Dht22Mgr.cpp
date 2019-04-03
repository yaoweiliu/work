#include "Dht22Mgr.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

/* in a timer(daemon service) to call  getTemperatureAndHumidity() */

Dht22Mgr::Dht22Mgr()
{
	fd = -1;
	mod = new Module();
}

Dht22Mgr::~Dht22Mgr()
{
	delete mod;
}

int Dht22Mgr::start(void)
{
	//open /dev/dht22
	int ret;

	ret = mod->insModule("dht22.ko");//libxml2.
	if(ret) {
		printf("%s: insModule error.\n", __func__);
		return -1;
	}

	fd = open(DEVNAME, O_RDONLY);
	if(fd == -1) {
		perror("open()");
		return -1;
	}

	return 0;
}

int Dht22Mgr::stop(void)
{
	//close /dev/dht22
	close(fd);
	mod->rmModule("dht22.ko");

	return 0;
}

int Dht22Mgr::suspend(void)
{
	return 0;
}

int Dht22Mgr::resume(void)
{
	return 0;
}

char *Dht22Mgr::getTemperatureAndHumidity(void)
{
	char buf[5] = {0};
	int ret;

	ret = read(fd, buf, 5);
	if(ret == -1) {
		perror("read()");
		return NULL;
	}

	if(buf[0] + buf[1] + buf[2] + buf[3] == buf[4]) {//checksum
		printf("temperature: %d\n", buf[3] | (buf[2] << 8));
		printf("humidity: %d\n", buf[1] | (buf[0] << 8));
	}

	return NULL;
}




