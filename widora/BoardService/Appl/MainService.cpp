#include "MainService.h"

MainService::MainService()
{
	OS_sem_init(serv_sem, 0, 0);
}

MainService::~MainService()
{

}

int MainService::init(void)
{
	mgr.push_back(process = new ProcessMgr("/bin/ls"));
	mgr.push_back(network = new NetworkMgr());
	mgr.push_back(dht22 = new Dht22Mgr());
}

int MainService::start(void)
{
	int i;

	for(i = 0; i < mgr.size(); i++) {
		mgr[i]->start();
	}

	return 0;
}

int MainService::stop(void)
{
	int i;

	for(i = 0; i < mgr.size(); i++) {
		mgr[i]->stop();
	}

	return 0;
}

void MainService::timer_notify_function(union sigval val)
{
	dht22->getTemperatureAndHumidity();
}

void MainService::daemonStart(void)
{
	int i;

	//for(i = 0; i < mgr.size(); i++) {
		//mgr[i]->daemon.start(1, 2, timer_notify_function);
	//}
	//dht22->daemon.start(1, 2, timer_notify_function);

	return ;
}

void *thread_entry(void *arg)
{
	((MainService *)arg)->init();
	((MainService *)arg)->start();
	((MainService *)arg)->daemonStart();
}

void MainService::threadStart(void)
{
	pthread_t pid;

	pthread_create(&pid, NULL, thread_entry, this);

	pthread_join(pid, NULL);
}

