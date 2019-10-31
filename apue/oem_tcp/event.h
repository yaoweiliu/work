#ifndef __EVENT_H__
#define __EVENT_H__

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <assert.h>
#include <signal.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <netinet/tcp.h>

#include <netinet/in.h>  
#include <netdb.h>
#include <pthread.h>

#include <string>
#include <iostream>

/* According to earlier standards */
#include <sys/time.h>

#include "ECService.h"
#include "CmErrorStruct.h"
#include "ev_base.h"
#include "EvCradle.h"
#include "EvTiltElev.h"

#ifdef __cplusplus
extern "C" {
#endif

//#define BIND_IP		"172.20.20.1"
#define BIND_IP		"127.0.0.1"
#define MAXCONN		5

#define ID_CRADLE    (0x1 << 0)
#define ID_ELEV      (0x1 << 1)
#define ID_ALL       (0x1 << 2)

//#define DEBUG

typedef int (*socket_handler_t)(void);

typedef struct socket_info {
	int sockfd;
	int listenfd;
	int acceptfd;
	int pipefd[2];
	socket_handler_t func;
}socket_info_t;

/*
static int thread_start(socket_info_t *sinfo);
static int create_socket(socket_info_t *sinfo);
static int destroy_socket(socket_info_t *sinfo);
static int bind_socket(socket_info_t *sinfo);
static int tcp_listen(socket_info_t *sinfo);
static void *tcp_process(void *sinfo);
*/

typedef enum {
	EV_TBL_SS_START, //used when to setup table subsystem; the client send to the server
	EV_TBL_CRADLE_READY, //used when to horizontal motion subsystem is ready; the server send to the client
	EV_TBL_ELEV_READY, //used when to vertical motion subsystem is ready; the server send to the client
	EV_TBL_QUERY_STATUS, //used when to query table current status; the client send to the server
	EV_TBL_CRADLE_RESPONSE_STATUS, //used when to horizontal motion subsystem response; the server send to the client
	EV_TBL_ELEV_RESPONSE_STATUS, //used when to vertical motion subsystem response; the server send to the client
	EV_TBL_CRADLE_START_MV_CMD, //used when to set horizontal motion subsystem move param; the client send to the server
	EV_TBL_ELEV_START_MV_CMD, //used when to set vertical motion subsystem move param; the client send to the server
	EV_TBL_CRADLE_STOP_MV_CMD, //used when to set horizontal motion subsystem stop; the client send to the server
	EV_TBL_ELEV_STOP_MV_CMD, //used when to set vertical motion subsystem stop; the client send to the server
	EV_TBL_CRADLE_POSITION_UPDATE, //used when to horizontal motion subsystem report current position; the server send to the client
	EV_TBL_ELEV_POSITION_UPDATE, //used when to vertical motion subsystem report current position; the server send to the client
	EV_TBL_CRADLE_IS_STOPED, //used when to horizontal motion subsystem stoped; the server send to the client
	EV_TBL_ELEV_IS_STOPED, //used when to vertical motion subsystem stoped; the server send to the client
	EV_TBL_ERROR_REPORT, //used when to table report error; the server send to the client
}EVENT_TYPE;

typedef enum {
	IOC, //input output control, identify the identity of the client
	COU, //couch, identify the identity of the server
}ROLE_ID;

#ifdef DEBUG
enum
{
    EV_CRADLE_BASE = 0,
    EV_CRADLE_UNLATCH, // type: EV_CRADLE_UNLATCH_TYPE
    EV_CRADLE_LATCH,
    EV_CRADLE_SPEEDUP,
    EV_CRADLE_SPEEDDOWN,
    EV_CRADLE_TARGET_SPEED,
    EV_CRADLE_STOPED,
    EV_CRADLE_ESTOP,
    EV_CRADLE_PI_SPEED,

    //from other modules ,for example SC ,button.
    EV_CRADLE_MOVE_CMD,
    EV_CRADLE_MOVE_STOP,
    EV_CRADLE_STATUS_QUERY,
    EV_CRADLE_STATUS_RESP,
    EV_CRADLE_POS_UPDATE,
    EV_CRADLE_MOTION_END,
    EV_CRADLE_MOTION_RESULT,

    //to the driver.
    EV_CRADLE_DEV_UPDATE_MOVE_PARA,
    EV_CRADLE_DEV_MOVE_START,
    EV_CRADLE_DEV_MOVE_UPDATE,
    EV_CRADLE_DEV_MOVE_STOP,
    EV_CRADLE_DEV_MOVE_ESTOP,
    EV_CRADLE_DEV_MOVE_SETUP,
    EV_CRADLE_DEV_TIMER,

    EV_CRADLE_DEV_UPDATE_SIMULATOR,

    //from the driver
    EV_CRADLE_DEV_RET_TIMER,
    EV_CRADLE_DEV_STATE_CHANGE,
    EV_CRADLE_DEV_LATCH_CHANGE,

    EV_CRADLE_MOVING_TIMER,
    EV_CRADLE_TARGET_SPEED_TIMER,
    EV_CRADLE_STOP_TIMER,
    EV_CRADLE_DEV_RET_PLUSE_TIME,
    EV_CRADLE_DEV_PI_DEBUG,

    EV_CRADLE_DEV_UPDATE_PI_PARA,
    EV_CRADLE_DEV_POT_ERR,
    EV_CRADLE_PULSE_DRI_INT_SETUP,
    EV_CRADLE_PULSE_DRI_INT_HAPPEN,
    EV_CRADLE_SET_LIMIT_BY_ROOM,

    EV_CRADLE_DEV_UPDATE_FPGA_KEY,
    EV_CRADLE_INT_FPGA_KEY_STOP,
    EV_CRADLE_DEV_RST,
    EV_CRADLE_DEV_STATE_MACHINE_RST_FROM_ESTOP,
    EV_CRADLE_TIMER_UPDATE,

    EV_CRADLE_DEV_SETUP_POS_DIFF_IDLE,
    EV_CRADLE_INT_POS_DIFF_IDLE,
    EV_CRADLE_CAL_OUT_POS,
    EV_CRADLE_CAL_IN_POS,
    EV_CRADLE_CAL_DONE,
    EV_CRADLE_CAL_STATE,

    EV_CRADLE_POS_DIFF_IDLE_TIMER,

    //just for cradle test -- magnescale
    EV_CRADLE_MAGN_STATUS_QERY,
    EV_CRADLE_MAGN_STATUS_RESP,
    EV_CRADLE_MAGN_POS_UPDATE,
    EV_CRADLE_MAGN_INT,//about 40ms once
    EV_CRADLE_DEV_SET_MAGN_INT,

    EV_CRADLE_POS_WHEN_ESTOP,

    //add by wjw for cradle16
    EV_CRADLE_CAL_UNLATCH_ENTRY,
    EV_CRADLE_CAL_UNLATCH_EXIT,
    EV_CRADLE_CAL_WAIT_START,
    EV_CRADLE_CAL_START_PRESSED,
    EV_CRADLE_CAL_START_CMD,
    EV_CRADLE_CAL_STARTED,
    EV_CRADLE_CAL_TIMER,
    EV_CRADLE_CAL_STOP_CMD,

    EV_CRADLE_SWITCH_CHANGED,

    //driver error
    EV_CRADLE_DEV_DRI_ERR,
    EV_CRADLE_SERVO_OR_STEP,

    //table mcu operations
    EV_TABLE_UPGRADE_MCU_VERSION,
    EV_TABLE_VERIFY_MCU_VERSION,
    EV_TABLE_VERIFY_MCU_VERSION_RESP,
    EV_TABLE_UPGRADE_MCU_VERSION_RESP,
    //to sc
    EV_TABLE_MCU_VERSION,
};

enum
{
    EV_TILT_ELEV_BASE = 0x10000,

    //for tilt 
    EV_TILT_MOVE,
    EV_TILT_MOVE_START,
    EV_TILT_GET_ANGLE,
    EV_TILT_RES_ANGLE,
    EV_TILT_REACH,
    EV_TILT_STOP,
    EV_TILT_STOPED,
    EV_TILT_GET_SPEED,
    EV_TILT_RES_SPEED,
    EV_TILT_SET_SPEED,
    EV_TILT_SET_SPEED_OK,
    EV_TILT_INTERRUPT,
    EV_TILT_ERROR,

    //for elevation
    EV_ELEV_MOVE,
    EV_ELEV_MOVE_START,
    EV_ELEV_GET_HEIGHT,
    EV_ELEV_RES_HEIGHT,
    EV_ELEV_REACH,
    EV_ELEV_STOP,
    EV_ELEV_STOPED,
    EV_ELEV_GET_SPEED,
    EV_ELEV_RES_SPEED,
    EV_ELEV_SET_SPEED,
    EV_ELEV_SET_SPEED_OK,
    EV_ELEV_ERROR,
    EV_ELEV_REPORT_POS,
    EV_ELEV_REPORT_TIME,
    EV_ELEV_OVER_CURR,
    EV_ELEV_MGR_TIMER,
    EV_ELEV_MOVE_TIMER,
    EV_ELEV_STOP_TIMER,

    //test event
    EV_TILT_ELEV_CONNECT,
    EV_TILT_ELEV_DISCONNECT,
    EV_TILT_GET_STATUS,
    EV_TILT_GET_ERROR,
    EV_ELEV_GET_STATUS,
    EV_ELEV_GET_ERROR,

    //for tiltElev control
    EV_TILT_ELEV_PROBE_TIMER,
    EV_TILT_MOVE_STATUS_CHECK,
    EV_ELEV_MOVE_STATUS_CHECK,

    //event code for driver
    EV_TILT_ELEV_CTRL_REPLAY,        
    EV_TILT_ELEV_CTRL_CMD,
    EV_TILT_REPORT_ANGLE,
    EV_TILT_SLEEP_TIMER,
    EV_ELEV_TILT_ESTOP,
    EV_ELEV_TILT_POWER_ON,
    EV_ELEV_MOVE_CMD,
    EV_ELEV_STOP_CMD,
    EV_EVLEV_JOGMOV_TIMER,
    EV_ELEV_MOVE_RESTART,
    EV_ELEV_MOVE_RESTART_TIMER,
    EV_TILT_MOVE_TIMER,
    EV_TILT_STOP_TIMER,

    EV_TILT_UPDATE_FPGA_KEY,
    EV_TILT_INT_FPGA_KEY_STOP,
    EV_ELEV_UPDATE_FPGA_KEY,
    EV_ELEV_INT_FPGA_KEY_STOP,

    EV_TILT_POS_DIFF_IDLE,
    EV_TLIT_INT_POS_DIFF,
    EV_ELEV_POS_DIFF_IDLE,
    EV_ELEV_INT_POS_DIFF,

    EV_ELEV_CAL_LOW_POS,
    EV_ELEV_CAL_UP_POS,
    EV_ELEV_CAL_DONE,

    EV_TILT_CAL_REAL_DEGREE,
    EV_TILT_CAL_DONE,
    EV_ELEV_TEST_MOVE,
    EV_ELEV_CAL_STATE,
    EV_TILT_CAL_STATE,

    EV_TILT_POS_DIFF_IDLE_TIMER,
    EV_ElEV_POS_DIFF_IDLE_TIMER,
    EV_KEY_INT_FOR_TABLE_TEST,
    EV_ELEV_CRADLE_INIT_DONE,

    EV_TABLE_FACTORY_MODE_UPDATE,
    EV_CRADLE_FACTORY_OUT_CHECK_START,
    EV_CRADLE_FACTORY_OUT_CHECK_STOP,
    EV_ELEV_FACTORY_OUT_MOVE_TIMER,
    EV_TILT_COMPARE_INT,
    EV_TILT_TIMER_INT,
    EV_TILT_RESET_TIMER,
    EV_ELEV_RESET_TIMER,
    EV_ELEV_INT_FPGA_STATE_CHG,
    EV_ELEV_SWITCH_CHG,

    /*add code for record elev infomation 2017.10.12*/
    EV_ELEV_GET_INFO,
    EV_ELEV_OVER_CURRENT_TIMER,
    EV_FPGA_STATE_CHANGE_TIMER,
};
#endif

typedef struct {
	EVENT_TYPE code; // event code
	int length; // total size of header and payload
	ROLE_ID sid; // sender id
	ROLE_ID rid; // receiver id
	long long checksum; // code + length + sid + rid 
	long long sent; // sent time, in nanoseconds since Jan. 1, 1970.
	long long received; // received time, in nanoseconds since Jan. 1, 1970.
}EVENT_HEADER;

/* This message is used to start the subsystem and is sent by the client to the server, and no payload. */
typedef struct
{
	EVENT_HEADER header;
}EV_TBL_SS_START_TYPE;

/* This message is used to reflect the ready state of the couch, sent by the server to the client, and no payload. */
typedef struct
{
	EVENT_HEADER header;
}EV_TBL_CRADLE_READY_TYPE;

/* This message is used to reflect the ready state of the couch, sent by the server to the client, and no payload. */
typedef struct
{
	EVENT_HEADER header;
}EV_TBL_ELEV_READY_TYPE;

/* This message is queried by the client to the server for the current state, and no payload, client sends to server */
typedef struct
{
	EVENT_HEADER header;
}EV_TBL_QUERY_STATUS_TYPE;

/* This message is the information of the couch response query, server sends to client */
typedef struct
{
	EVENT_HEADER header;
	int horizontalPos; // Current horizontal position, the unit is um
	bool isMoving; // true is moving, false is stoped
	bool isLatched; // couch latch state; true is latch, false is unlatch
}EV_TBL_CRADLE_RESPONSE_STATUS_TYPE;

/* This message is the information of the couch response query, server sends to client */
typedef struct
{
	EVENT_HEADER header;
	int verticalPos; // current vertical position, the unit is um
	bool isMoving; // true is moving, false is stoped
}EV_TBL_ELEV_RESPONSE_STATUS_TYPE;

/* Horizontal motion command, client sends to server */
typedef struct
{
	EVENT_HEADER header;
	int targetPositionUm; // target position, the unit is um
	int targetSpeedUmPerSec; // target speed, the unit is um/s
}EV_TBL_CRADLE_START_MV_CMD_TYPE;

/* Vertical motion command, client sends to server */
typedef struct
{
	EVENT_HEADER header;
	int targetPositionUm; // target position, the unit is um
}EV_TBL_ELEV_START_MV_CMD_TYPE;

/* Horizontal stop command, and no payload; client sends to server */
typedef struct
{
	EVENT_HEADER header;
}EV_TBL_CRADLE_STOP_MV_CMD_TYPE;

/* Vertical stop command, and no payload; client sends to server */
typedef struct
{
	EVENT_HEADER header;
}EV_TBL_ELEV_STOP_MV_CMD_TYPE;

/* Horizontal position update command, server sends to client */
typedef struct
{
	EVENT_HEADER header;
	int currentPositionUm; // Current horizontal position, the unit is um
	int currentSpeedUmPerSec; // current speed, the unit is um/s
	bool isMoving; // true is moving, false is stoped
	bool isLatched; // couch latch state; true is latch, false is unlatch
}EV_TBL_CRADLE_POSITION_UPDATE_TYPE;

/* Vertical position update command, server sends to client */
typedef struct
{
	EVENT_HEADER header;
	int currentPositionUm; // current vertical position, the unit is um
	bool isMoving; // true is moving, false is stoped
}EV_TBL_ELEV_POSITION_UPDATE_TYPE;

/* This message indicates that the horizontal movement has stopped, and no payload, server sends to client */
typedef struct
{
	EVENT_HEADER header;
}EV_TBL_CRADLE_IS_STOPED_TYPE;

/* This message indicates that the vertical movement has stopped, and no payload, server sends to client */
typedef struct
{
	EVENT_HEADER header;
}EV_TBL_ELEV_IS_STOPED_TYPE;

/* This message indicates an error message, server sends to client */
typedef struct
{
	EVENT_HEADER header;
	std::string errorMsg; // error message
}EV_TBL_ERROR_REPORT_TYPE;

#endif

#ifdef __cplusplus
}
#endif



