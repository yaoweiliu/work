/**@file 
 * Copyright (C) 2016, Sinovision Tech Ltd.
 * All rights reserved.
 *
 * @brief   This file implements state and evet related functions
 *
 * @author  Yufei Liu
 * @date 2016-8-18
 *
 */

#include "stddef.h"
#include "state.h"


static STATE_TYPE *current_state = NULL;
static int32_t events = 0;


STATE_TYPE * cur_state(void)
{
	return current_state;
}

void set_state(STATE_TYPE *state)
{
	current_state = state;
}

void init_event(void)
{
	events = 0;
}

void set_event(int32_t ev)
{
	events |= ev;
}

void clear_event(int32_t ev)
{
	events &= ~ev;
}

int32_t get_events(void)
{
	return events;
}

void event_process(void)
{
    int32_t event_mask = 0;
    int32_t events;
    const EVENT_TYPE *event_ptr = cur_state()->event_ptr;
    
    /*build event mask first*/
    do{
        event_mask |= event_ptr->event_flag;
    } while ((++event_ptr)->event_flag != EVENT_LIST_END); 
    
    /*wait until event happened*/
    while((event_mask & get_events()) == 0)
        break;
    
    /*find match event and do actions*/
    events = get_events();
    for(event_ptr = cur_state()->event_ptr; event_ptr->event_flag != EVENT_LIST_END; event_ptr++){
        int32_t event_flag = event_ptr->event_flag;
        if( ((event_flag & events) !=0) && (event_ptr->action != NULL) ){
            (*event_ptr->action)(event_ptr->arg);
            clear_event(event_flag);
            set_state(event_ptr->next_state);
        }
        
    }
} 

