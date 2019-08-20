#include "simu.h"

using namespace SIMUSTATEMACHINE;

int main(int argc, char const *argv[])
{
	BaseState *bs = new BaseState();
	ObjHoldState obj(bs);

	obj.setState(1);
	SubState1 *tmp = (SubState1 *)(obj.getCurrStatePoint());
	tmp->init();
	tmp->event3();

	obj.setState(2);
	SubState2 *tmp1 = (SubState2 *)(obj.getCurrStatePoint());
	tmp1->init();
	tmp1->event2();

	return 0;
}

