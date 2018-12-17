/**@file 
 * Copyright (C) 2016, Sinovision Tech Ltd.
 * All rights reserved.
 *
 * @brief   This file defines events
 *
 * @author  Yufei Liu
 * @date 2016-8-18
 *
 */

#ifndef EVENT_DEF_INCLUDE
#define EVENT_DEF_INCLUDE

#include "stdint.h"
#include "fm25cl64b.h"
#include "scu_comm.h"
#include "servo_comm.h"


void init_done_handler(int32_t arg);
void ev_1_handler(int32_t arg);
void ev_2_handler(int32_t arg);
void ev_3_handler(int32_t arg);

#endif


