#include "MainService.h"
#include <signal.h>

static MainService *mainServ = NULL;

static void bs_handler_sig(int arg)
{
	printf("%s: rev interrupt signal.\n", __func__);
	mainServ->stop();
	OS_sem_post(mainServ->serv_sem);
}

int main(int argc, char const *argv[])
{
	mainServ = new MainService();

	mainServ->threadStart();

	signal(SIGINT, bs_handler_sig);//SIGTERM

	OS_sem_wait(mainServ->serv_sem);

	return 0;
}


