#include "ConcreteObserver.h"
#include "SubjectBase.h"

void ConcreteObserver::Update(SubjectBase *sub)
{
	this->m_state = sub->GetState();
	printf("%s: observer'state is %s\n", __func__, this->m_state.c_str());
}