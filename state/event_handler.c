/**@file 
 * Copyright (C) 2016, Sinovision Tech Ltd.
 * All rights reserved.
 *
 * @brief   This file implements event handlers
 *
 * @author  Yufei Liu
 * @date 2016-8-18
 *
 */

#include "event_handler.h"
#include "stdio.h"


extern unsigned short fram_waddress;
extern unsigned short fram_wdata;
extern SCU_COMM scu_message;
extern SERVO_COMM servo_message_send;
extern SERVO_COMM servo_message_recieve;
extern SERVO_MSG_PROC servo_message_recieve_info;


void init_done_handler(int32_t arg)
{
    //printf("init done handler \n");
}

void ev_1_handler(int32_t arg)
{
    Save_Setting(fram_waddress, fram_wdata);
}

void ev_2_handler(int32_t arg)
{
    scu_message.head = SEND_FRAME_HEAD;
    Send_To_Scu(scu_message);
}

void ev_3_handler(int32_t arg)
{
    static unsigned short cnt_servo_recieve_timeout = 0;
    
    if(servo_message_recieve_info.recieve_message_valid == 0x00) {
        if(servo_message_recieve_info.servo_send_flag == 0 && \
                servo_message_recieve_info.servo_recieve_flag == 0) {
            Send_To_Servo(servo_message_send);
            servo_message_recieve_info.servo_send_flag = 1;            
        }  
		if(servo_message_recieve_info.servo_send_flag == 1 && \
                 servo_message_recieve_info.servo_recieve_flag == 0) {
			Delay_us(10);
			cnt_servo_recieve_timeout ++;
			if(cnt_servo_recieve_timeout == 2000){
                cnt_servo_recieve_timeout = 0;
                servo_message_recieve_info.servo_send_flag = 0;
			}
		}
        if(servo_message_recieve_info.servo_send_flag == 0 && \
                servo_message_recieve_info.servo_recieve_flag == 1) {
            servo_message_recieve_info.servo_recieve_flag = 0x00;
            if(Servo_CRC_Check(servo_message_recieve) == 1) {
                servo_message_recieve_info.recieve_message_valid =  0x01;
            } else {
                servo_message_recieve_info.recieve_message_valid =  0x00;
            }
        }
    }
}

