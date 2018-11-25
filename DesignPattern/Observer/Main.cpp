#include "ConcreteSubject.h"
#include "ConcreteObserver.h"
#include "SubjectBase.h"
#include "ObserverBase.h"

/* 基类中声明的纯虚函数在派生类中也要再次声明，但是不用声明为虚函数 */

int main(int argc, char const *argv[])
{
	ObserverBase *observer1 = new ConcreteObserver();
	ObserverBase *observer2 = new ConcreteObserver();

	SubjectBase *subject = new ConcreteSubject();

	subject->Attach(observer1);
	subject->Attach(observer2);

	subject->SetState("old");
	subject->Notify();

	subject->SetState("new");
	subject->Detach(observer2);
	subject->Notify();

	return 0;
}
