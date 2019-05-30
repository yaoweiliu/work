#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fsm.h"

static FsmTable_t Table[] = 
{
	{EVENT1, Monday, getMonday, Tuesday},
	{EVENT2, Tuesday, goTuesday, Wednesday},
	{EVENT3, Wednesday, goWednesday, Thursday},
	{EVENT4, Thursday, goTursday, Friday},
	{EVENT5, Friday, goFriday, Saturday},
	{EVENT6, Saturday, goSaturday, Sunday},
	{EVENT7, Sunday, goSunday, Monday},
};

void FSM_register(Fsm_t *pFsm, FsmTable_t *pTable)
{
	pFsm->FsmTable = pTable;
}

void FSM_StateTransfer(Fsm_t *pFsm, int state)
{
	pFsm->curState = state;
}

void FSM_EventHandle(Fsm_t *pFsm, int event)
{
	FsmTable_t *pActTable = pFsm->FsmTable;
	void (*eventActFun)() = NULL;
	int nextState;
	int curState = pFsm->curState;
	int flag = 0;
	int i = 0;

	for(i = 0; i < sizeof(Table)/sizeof(Table[0]); i++) {
		if(event == pActTable[i].event && curState == pActTable[i].curState) {
			flag = 1;
			eventActFun = pActTable[i].eventActFun;
			nextState = pActTable[i].nextState;
			break;
		}
	}

	if(flag) {
		if(eventActFun)
			eventActFun();
		FSM_StateTransfer(pFsm, nextState);
		//printf("%s\n", __func__);
	}
	else {
		//do something.
	}
}

void getMonday(void)
{
	printf("%s: 1\n", __func__);
}

void goTuesday(void)
{
	printf("%s: 2\n", __func__);
}

void goWednesday(void)
{
	printf("%s: 3\n", __func__);
}

void goTursday(void)
{
	printf("%s: 4\n", __func__);
}

void goFriday(void)
{
	printf("%s: 5\n", __func__);
}

void goSaturday(void)
{
	printf("%s: 6\n", __func__);
}

void goSunday(void)
{
	printf("%s: 7\n", __func__);
}

void initFsm(Fsm_t *pFsm)
{
	pFsm->curState = Monday;
	FSM_register(pFsm, Table);
}

void pollingDay(int *event)
{
	//printf("%s: event is %d\n", __func__, *event);
	if(*event == 7)
		*event = 1;
	else {
		(*event)++;
		//printf("%s: event is %d\n", __func__, *event);
	}
}

int main(int argc, char const *argv[])
{
	Fsm_t fsm;
	int event = EVENT1;

	initFsm(&fsm);

	while(1) {
		printf("event %d is coming...\n", event);
		FSM_EventHandle(&fsm, event);
		printf("fsm current state %d\n", fsm.curState);
		pollingDay(&event);
		//printf("%s: event is %d\n", __func__, event);
		sleep(2);
	}

	return 0;
}

