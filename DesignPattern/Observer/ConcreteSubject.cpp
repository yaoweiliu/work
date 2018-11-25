#include "ConcreteSubject.h"

void ConcreteSubject::Notify(void)
{
	list<ObserverBase *>::iterator it;

	for(it = m_lsit.begin(); it != m_lsit.end(); it++) {
		(*it)->Update(this);
	}
}

void ConcreteSubject::Attach(ObserverBase *observer)
{
	this->m_lsit.push_back(observer);
	printf("%s: attach a observer\n", __func__);
}

void ConcreteSubject::Detach(ObserverBase *observer)
{
	list<ObserverBase *>::iterator it;

	it = find(m_lsit.begin(), m_lsit.end(), observer);
	if(it != m_lsit.end())
		m_lsit.erase(it);

	printf("%s: detach a observer\n", __func__);
}

string ConcreteSubject::GetState(void)
{
	return this->m_state;
}

void ConcreteSubject::SetState(string state)
{
	this->m_state = state;
}

