#include "ProcessMgr.h"

ProcessMgr::ProcessMgr(path):filePath(path)
{
	this->proc = new Process();
}

ProcessMgr::~ProcessMgr()
{
	delete this->proc;
}

int ProcessMgr::start(void)
{
	return this->proc->run(filePath);
}

int ProcessMgr::stop(void)
{
	return this->proc->kill(filePath);
}

int ProcessMgr::suspend(void)
{
	return 0;
}

int ProcessMgr::resume(void)
{
	return 0;
}

int ProcessMgr::startDaemon(void)
{
	//TODO.
}

