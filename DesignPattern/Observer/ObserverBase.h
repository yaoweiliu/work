#ifndef __OBSERVERBASE_H__
#define __OBSERVERBASE_H__

class SubjectBase;
//#include "SubjectBase.h"

class ObserverBase
{
public:
	ObserverBase() {}
	~ObserverBase() {}

	virtual void Update(SubjectBase *sub) = 0;
private:

};

#endif
