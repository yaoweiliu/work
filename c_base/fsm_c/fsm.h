#ifndef __FSM_H__
#define __FSM_H__

enum
{
	Monday,
	Tuesday,
	Wednesday,
	Thursday,
	Friday,
	Saturday,
	Sunday,
};

enum
{
	EVENT1 = 1,
	EVENT2,
	EVENT3,
	EVENT4,
	EVENT5,
	EVENT6,
	EVENT7,
};

typedef struct FsmTable_s
{
	int event;
	int curState;
	void (*eventActFun)(void);
	int nextState;
}FsmTable_t;

typedef struct Fsm_s
{
	FsmTable_t *FsmTable;
	int curState;
}Fsm_t;

void FSM_register(Fsm_t *pFsm, FsmTable_t *pTable);

void FSM_StateTransfer(Fsm_t *pFsm, int state);

void FSM_EventHandle(Fsm_t *pFsm, int event);

void getMonday(void);

void goTuesday(void);

void goWednesday(void);

void goTursday(void);

void goFriday(void);

void goSaturday(void);

void goSunday(void);


#endif

