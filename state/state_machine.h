/**@file 
 * Copyright (C) 2016, Sinovision Tech Ltd.
 * All rights reserved.
 *
 * @brief   This file defines state struct
 *
 * @author  Yufei Liu
 * @date 2016-8-18
 *
 */

#ifndef STATE_MACHINE_INCLUDE
#define STATE_MACHINE_INCLUDE

#include "stdint.h"


#define EVENT_LIST_END  0


#define DEFINE_STATE(NAME, ID) \
	extern const EVENT_TYPE NAME ## _events[]; \
	STATE_TYPE NAME = {ID,NAME ## _events}; \
	const EVENT_TYPE NAME ## _events[]
	
 
typedef struct eventStruct
{
	int32_t event_flag;							/*identification if the event*/
	void (*action)(int32_t); 					/*action function pointer*/
	int32_t arg;								/*argument of the action pointer*/
	struct stateStruct *next_state;		/*pointer to the next state*/
}EVENT_TYPE;
 
typedef struct stateStruct
{
	int32_t state_id;
	const struct eventStruct *event_ptr; 
}STATE_TYPE;


STATE_TYPE *cur_state(void);
void set_state(STATE_TYPE *state);

void init_event(void);
void set_event(int32_t ev);
void clear_event(int32_t ev);
int32_t get_events(void);
void event_process(void);
 
#endif

