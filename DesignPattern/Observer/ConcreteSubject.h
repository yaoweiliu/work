#ifndef __CONCRETESUBJECT_H__
#define __CONCRETESUBJECT_H__

#include <algorithm>
#include "SubjectBase.h"

class ConcreteSubject : public SubjectBase
{
public:
	ConcreteSubject() {}
	~ConcreteSubject() {}

	void Notify(void);
	void Attach(ObserverBase *);
	void Detach(ObserverBase *);
	string GetState(void);
	void SetState(string state);
private:
	string m_state;
	list<ObserverBase *> m_lsit;	
};

#endif
