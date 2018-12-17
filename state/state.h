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

#ifndef STATE_INCLUDE
#define STATE_INCLUDE

#include "state_machine.h"
#include "event_handler.h"

extern STATE_TYPE state_init,state_idle;

enum
{
	STATE_INIT = 1,
	STATE_IDLE,
	STATE_WORK1
};

enum
{
	EV_INIT_DONE = 1<<0,
	EV_1 = 1<<1,
	EV_2 = 1<<2,
	EV_3 = 1<<3,
};
 
#endif

