#include "Macho.hpp"
#include <iostream>

using namespace std;

namespace Example {
	TOPSTATE(Top) {
		struct Box {
			Box(): data(0) {}
			long data;
		};
		
		STATE(Top)
		
		virtual void event1(int i) {}
		virtual void event2(long l) {}

	private:
		void entry();
		void exit();
		void init();
		void init(int);
	};

	SUBSTATE(Super, Top) {
		STATE(Super)

		HISTORY()

	private:
		void entry();
		void exit();
	};

	SUBSTATE(StateA, Super) {
		struct Box {
			Box() : data(0) {}
			int data;
		};

		STATE(StateA)

		void event1(int i);

	private:
		void entry();
		void init(int);
		void exit();
	};

	SUBSTATE(StateB, Super) {
		STATE(StateB)

		void event2(long l);

	private:
		void entry();
		void exit();
	};

	void Top::entry() {cout << "Top::entry" << endl;}
	void Top::exit() {cout << "Top::exit" << endl;}
	void Top::init() {
		setState<StateA>(44);
	}
	void Top::init(int i) {
		box().data = i;
		init();
	}

	void Super::entry() {cout << "Super::entry" << endl;}
	void Super::exit() {cout << "Super::exit" << endl;}

	void StateA::entry() {cout << "StateA::entry" << endl;}
	void StateA::exit() {cout << "StateA:exit" << endl;}
	void StateA::init(int i) {cout << "StateA::init" << i << endl;}
	void StateA::event1(int i) {
		box().data = i;
		cout << "StateA::box().data: " << box().data << endl;
		setState<StateB>();
}

	void StateB::entry() {cout << "StateB::entry" << endl;}
	void StateB::exit() {cout << "StateB::exit" << endl;}
	void StateB::event2(long l) {
		Top::box().data = l;
		cout << "Top::box().data: " << Top::box().data << endl;
		setState<StateA>(42);
	}
}

int main(void)
{
	using namespace Macho;

	Machine<Example::Top> m(State<Example::Top>(11));

	m->event1(42);
	m->event2(43);

	cout << "m.box().data: " << m.box().data << endl;

	return 0;
}