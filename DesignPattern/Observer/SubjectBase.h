#ifndef __SUBJECTBASE_H__
#define __SUBJECTBASE_H__

#include <string>
#include <list>
#include <stdio.h>
#include "ObserverBase.h"
using namespace std;

//class ObserverBase;

class SubjectBase
{
public:
	SubjectBase() {}
	~SubjectBase() {}

	virtual void Notify(void) = 0;
	virtual void Attach(ObserverBase *) = 0;
	virtual void Detach(ObserverBase *) = 0;
	virtual string GetState(void) = 0;
	virtual void SetState(string state) = 0;
private:
	
};

#endif
