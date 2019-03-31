#include "MainService.h"
#include <signal.h>

static MainService *mainServ = NULL;

static void bs_handler_sig(int arg)
{
	OS_sem_post(mainServ->serv_sem);
}

int main(int argc, char const *argv[])
{
	mainServ = new MainService();

	mainServ->threadStart();

	signal(SIGTERM, bs_handler_sig);

	OS_sem_wait(mainServ->serv_sem);

	return 0;
}


