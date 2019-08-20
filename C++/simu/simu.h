#ifndef __SIMU_H__
#define __SIMU_H__

#include <iostream>

using namespace std;

#ifdef __cplusplus
extern "C" { 
#endif

namespace SIMUSTATEMACHINE {
	//state transtation: state instance and destory
class BaseState;

class ObjHoldState
{
public:
	explicit ObjHoldState(BaseState *bs);
	~ObjHoldState();

	void setState(int state);
	void getState()
	{
		cout << "get null temp" << endl;
	}

public:
	BaseState *getStatePoint()
	{
		return mState;
	}

	BaseState *getCurrStatePoint()
	{
		return curState;
	}
private:
	BaseState *mState;
	BaseState *curState;
};

class SubState1;
class SubState2;

class BaseState
{
public:
	explicit BaseState(int type)
	{
		cout << "Base State Entry2!" << endl;
	}

	BaseState() {}
	~BaseState();

	void start();

	virtual void event1()
	{
		cout << "dispatch event1." << endl;
	}
	virtual void event2()
	{
		cout << "dispatch event2." << endl;
	}
	virtual void event3()
	{
		cout << "dispatch event3." << endl;
	}

public:
	BaseState *setState(int s);

private:
	SubState1 *mSub1;
	SubState2 *mSub2;
};

class SubState1 :public BaseState
{
public:
	explicit SubState1()
	{
		cout << "Sub State1 Entry!" << endl;
	}
	~SubState1()
	{
		cout << "Sub State1 exit!" << endl;
	}

	void init()
	{
		cout << "Sub State1 init." << endl;
	}
};

class SubState2 :public BaseState
{
public:
	explicit SubState2()
	{
		cout << "Sub State2 Entry!" << endl;
	}
	~SubState2()
	{
		cout << "Sub State2 exit!" << endl;
	}

	void init()
	{
		cout << "Sub State2 Init." << endl;
	}
};

ObjHoldState::ObjHoldState(BaseState *bs):mState(bs),curState(bs)
{
	mState->start();
}

ObjHoldState::~ObjHoldState()
{
	if(mState) {
		delete mState;
		mState = NULL;
	}
	if(curState)
	{
		delete curState;
		curState = NULL;
	}
}

void ObjHoldState::setState(int state)
{
	curState = mState->setState(state);
}

BaseState::~BaseState()
{
	if(mSub1) {
		delete mSub1;
		mSub1 = NULL;
	}
	if(mSub2) {
		delete mSub2;
		mSub2 = NULL;
	}
}

void BaseState::start()
{
	mSub1 = new SubState1();
	mSub2 = new SubState2();
	cout << "start state transtation." << endl;
}

BaseState *BaseState::setState(int s)
{
	switch (s) {
	case 0:
		//jump to self.
		cout << "this is self." << endl;
		return this;
	case 1:
		//jump sub1
		cout << "this is subState1" << endl;
		return (SubState1 *)mSub1;
	case 2:
		//jump sub2
		cout << "this is subState2" << endl;
		return (SubState2 *)mSub2;
	default:
		return NULL;
	}
}

}


#ifdef __cplusplus
}
#endif

#endif

