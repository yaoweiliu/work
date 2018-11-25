#ifndef __CONCRETEOBSERVER_H__
#define __CONCRETEOBSERVER_H__

#include <stdio.h>
#include <string>
#include "ObserverBase.h"

using namespace std;

class ConcreteObserver : public ObserverBase
{
public:
	ConcreteObserver() {}
	~ConcreteObserver() {}

	void Update(SubjectBase *sub);
	
private:
	string m_state;
};

#endif
