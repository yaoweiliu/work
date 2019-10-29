/**@file 
 * Copyright (C) 2013, Sinovision Tech Ltd.
 * All rights reserved.
 *
 * @brief   This file defines the event communication interface
 *    for inter-thread communication, will use a queue directly
 *    for inter-process communication, will use tcp scoket
 *
 * @author  
 * @date 2013-3-1
 *
 */


 
#ifndef _ECL_EVENT_H_
#define _ECL_EVENT_H_

#include "ev_base.h"





/** Common PCL events.
 */
enum
{
    EV_ECL_BASE = SYSTEM_ECL_CODE_BASE,
    EV_ECL_CONNECT, //use fro connect probe
    EV_ECL_DISCONNECT,
    EV_ECL_CONNECTED, //notice a connection connected
    EV_ECL_DISCONNECTED, //notice a connection disconnected

    EV_FW_LOGGER,
    EV_LOG_DUMP,
    EV_TEST,
    EV_TEST_TIMER,
	EV_FW_STATISTICS,
	EV_STATISTICS_DUMP,

	EV_SS_POWERED = 1000, // this is subsystem to notify the subsystem control
	EV_SS_START,
	EV_SS_SELF_TEST_FAIL,
	EV_SS_INIT_DONE,
	EV_SS_INIT_FAIL,	
	EV_SS_READY,
	EV_SS_SHUTDOWN,
	EV_SS_FAULT,
	EV_SS_FAULT_RECOVER,	// event subsystem control to subsystem
	EV_SS_FAULT_RECOVERED,	
	EV_SS_ENTER_FAULT,		// subsystem to notify the subsystem control
	EV_SS_LEAVE_FAULT,        // subsystem to notify the subsystem control
	EV_SS_ENTER_SHUTDOWN,     // subsystem to notify the subsystem control
	EV_SS_POWER_RESUMED,	

	EV_SS_QUERY_VER,
	EV_SS_RESP_VER,
	EV_SS_REPORT_VER,
	EV_SS_DEV_REPORT_VER,
	EV_SS_FPGA_REPORT_VER,
    EV_SS_DUMP_LOG_TO_SVC,
    EV_BD_DUMP_APP_LOG,
    //add for shutdown cmd
    EV_SS_SHUTDOWN_FINISHED,
    EV_ALL_SS_SHUTDOWN_FINISHED,
    EV_ALL_SS_SHUTDOWN_FINISHED_TMO,
    EV_SEND_SHUTDOWN_TO_SCU_BS_TMO,
    EV_SS_HD_VER_QUERY,
    EV_SS_HD_VER_RESP,
	EV_SS_DUMP_STATISTICS,

	//those event code for document and test only
	EV_SS_ACTION_SETUP = 1100,
	EV_SS_ACTION_SETUP_DONE,	
	EV_SS_ACTION_READY,
	EV_SS_ACTION_END,
	EV_SS_ACTION_RESULT,	
	EV_SS_ACTION_CANCEL,
	EV_SVC_SC_FAULT_RECOVER,


	EV_RD_MEM = 1200,
	EV_RD_MEM_RES,
	EV_WR_MEM,
	EV_RD_FPGA_REG,
	EV_RD_FPGA_REG_RES,
	EV_WR_FPGA_REG,
	EV_RD_ADC_COMM,
	EV_RD_ADC_COMM_RES,
	EV_RD_FPGA_KEY,
	EV_RD_FPGA_KEY_RES,
	EV_WR_FPGA_KEY,

	EV_WR_SLIPRING_REG,
	EV_RD_SLIPRING_STATUS,
	EV_RD_SLIPRING_STATUS_RES,
	EV_EVENT_TEST,//numbers of event test
	EV_XML_VERSION,
	EV_SV_REQUEST_STATUS,  //request service running status
	EV_SV_RESPONSE_STATUS, //response service running status
	/* events for deamon timers in board service [Add][liufei][2015.01.15] */
	EV_BS_DEAMON_TMR_BEGIN,
	EV_BS_DEAMON_TMR_START,
	EV_BS_DEAMON_TMR_STOP,
	EV_BS_DEAMON_TMR_NETWORK,
	EV_BS_DEAMON_TMR_PROCESS,
	EV_BS_DEAMON_TMR_BOARD,
	EV_BS_DEAMON_TMR_HARDWARE,
	EV_BS_DEAMON_TMR_END = EV_BS_DEAMON_TMR_BEGIN + 8,
	/* event for slipring error report */
	EV_SS_SLIPRING_REPORT_ERR,
	EV_BS_START_READY_TIMER,
	
	/* event base for fw update/reboot tool */
	EV_FW_TOOL_BASE = 1300,
	/* events for component diagnose service */
	EV_SS_ENTER_DIAGNOSE = 1400,
	EV_SS_LEAVE_DIAGNOSE,
	EV_SS_NOTIFY_ENTER_DIAGNOSE,
	EV_SS_NOTIFY_LEAVE_DIAGNOSE,

	EV_OC_HD_VER_QUERY = 1500,
	EV_OC_HD_VER_RESP,

    EV_OC_SYS_MODEL,
};
#define SOFT_VER_NUM (15)
typedef enum
{
	scVer,
	axialVer,
	cradleVer,
	tiltElevVer,
	genVer,
	dasVer,
	collVer,
	disp1PanelVer,
	disp1LcdVer,
	disp1AudioVer,
	disp2PanelVer,
	disp2LcdVer,
	ctbPanelVer,
	ctbLcdVer,
	ctbAudioVer,
}SOFT_VER_ENUM;

typedef enum
{
    Insitum_16,
    Insitum_32,
    Insitum_64,
    Insitum_64S,
    InsitumCT_338,
    InsitumCT_568,
    InsitumCT_768,
}SYS_MODEL_ENUM;

typedef struct
{
    EVENT_HEADER_TYPE header;
	char scuSerialNumber[64];
	char rcuSerialNumber[64];
} EV_OC_HD_VER_RESP_TYPE;

typedef struct
{
    EVENT_HEADER_TYPE header;
    SYS_MODEL_ENUM system_model;
} EV_OC_SYS_MODEL_TYPE;

typedef struct
{
    EVENT_HEADER_TYPE header;
} EV_SS_QUERY_VER_TYPE;

/**
 * request service running status
 */
typedef struct
{
    EVENT_HEADER_TYPE header;
}EV_SV_REQUEST_STATUS_TYPE;
/**
 * response service running status
 */
typedef struct
{
    EVENT_HEADER_TYPE header;
}EV_SV_RESPONSE_STATUS_TYPE;

typedef struct
{
    EVENT_HEADER_TYPE header;
    SVCID target;
    unsigned int spare;
}EV_LOG_DUMP_TYPE;

typedef struct
{
    EVENT_HEADER_TYPE header;
    SVCID target;
    unsigned int spare;
}EV_SS_DUMP_LOG_TO_SVC_TYPE;

typedef struct
{
    EVENT_HEADER_TYPE header;
    SVCID target;
    unsigned int spare;
}EV_SS_DUMP_STATISTICS_TYPE;

typedef struct
{
    EVENT_HEADER_TYPE header;
    SVCID target;
    unsigned int spare;
}EV_STATISTICS_DUMP_TYPE;

typedef struct
{
    EVENT_HEADER_TYPE header;
	char* data;
} EV_FW_STATISTICS_TYPE;

typedef struct
{
    EVENT_HEADER_TYPE header;
	char svnVer[32];
	char buildTime[64];
} EV_SS_RESP_VER_TYPE;

typedef struct
{
    EVENT_HEADER_TYPE header;
	char xmlStr[512];
} EV_SS_REPORT_VER_TYPE;

typedef struct
{
    EVENT_HEADER_TYPE header;
	char devName[32];
	char buildTime[64];
	char svnVer[32];
	char builder[32];
} EV_SS_DEV_REPORT_VER_TYPE;

typedef struct
{
    EVENT_HEADER_TYPE header;
	char fpgaName[32];
	char buildTime[64];
	char svnVer[32];
} EV_SS_FPGA_REPORT_VER_TYPE;

typedef struct
{
	EVENT_HEADER_TYPE header;
	uint32_t errcode;
	uint32_t errcnt;
} EV_SS_SLIPRING_REPORT_ERR_TYPE;

//lyw, for test 2018-11-12
typedef struct
{
	EVENT_HEADER_TYPE header;
}EV_SS_SLIPRING_DISCONNECT_TYPE;

typedef struct
{
	EVENT_HEADER_TYPE header;
	uint32_t subcode;
	uint32_t reserved;
	char optional[648];
} EV_FW_TOOL_TYPE;

typedef struct
{
    EVENT_HEADER_TYPE header;
} EV_TEST_TYPE;


/**the numbers of event test event
 */
typedef struct
{
    EVENT_HEADER_TYPE header;
   	SVCID from;
	SVCID to;
	int item;
} EV_EVENT_TEST_TYPE;


/** The connect event.
 */
typedef struct
{
    EVENT_HEADER_TYPE header;
   	SVCID from;
	SVCID to;
} EV_ECL_CONNECTED_TYPE;

/** The disconnect event.
 */
typedef struct
{
    EVENT_HEADER_TYPE header;
   	SVCID from;
	SVCID to;
} EV_ECL_DISCONNECTED_TYPE;

typedef struct
{
    EVENT_HEADER_TYPE header;
	int type;
    int conn_status;
	int en_connected_probe;
   	SVCID from;
	SVCID to;
	int fd;
} EV_ECL_CONNECT_TYPE;



typedef struct
{
    EVENT_HEADER_TYPE header;
	unsigned int phy_addr;
} EV_RD_MEM_TYPE;

typedef struct
{
    EVENT_HEADER_TYPE header;
	unsigned int phy_addr;
	unsigned int val;
} EV_RD_MEM_RES_TYPE;

typedef struct
{
    EVENT_HEADER_TYPE header;
	unsigned int phy_addr;
	unsigned int val;
} EV_WR_MEM_TYPE;


typedef struct
{
    EVENT_HEADER_TYPE header;
	unsigned int reg_offset;
	unsigned int spare;
} EV_RD_FPGA_REG_TYPE;

typedef struct
{
    EVENT_HEADER_TYPE header;
	unsigned int reg_offset;
	unsigned int val;
} EV_RD_FPGA_REG_RES_TYPE;

typedef struct
{
    EVENT_HEADER_TYPE header;
	unsigned int reg_offset;
	unsigned int val;
} EV_WR_FPGA_REG_TYPE;

typedef struct
{
    EVENT_HEADER_TYPE header;
	unsigned int chan_mask;
} EV_RD_ADC_COMM_TYPE;

typedef struct
{
    EVENT_HEADER_TYPE header;
	unsigned int channel_val[8];
} EV_RD_ADC_COMM_RES_TYPE;

typedef struct
{
    EVENT_HEADER_TYPE header;
	unsigned short reg_offset;
} EV_RD_FPGA_KEY_TYPE;

typedef struct
{
    EVENT_HEADER_TYPE header;
	unsigned short reg_offset;
	unsigned short val;
} EV_RD_FPGA_KEY_RES_TYPE;

typedef struct
{
    EVENT_HEADER_TYPE header;
	unsigned short reg_offset;
	unsigned short val;
} EV_WR_FPGA_KEY_TYPE;


typedef struct
{
    EVENT_HEADER_TYPE header;
	char* data;
} EV_FW_LOGGER_TYPE;

typedef struct
{
    EVENT_HEADER_TYPE header;
	char data[0];
}EV_XML_VERSION_TYPE;

typedef struct
{
    EVENT_HEADER_TYPE header;
	char data[0];
}EV_SS_HD_VER_RESP_TYPE;


typedef struct
{
    EVENT_HEADER_TYPE header; 
} EV_SS_ENTER_DIAGNOSE_TYPE;


typedef struct
{
    EVENT_HEADER_TYPE header; 
} EV_SS_LEAVE_DIAGNOSE_TYPE;

typedef struct
{
    EVENT_HEADER_TYPE header; 
} EV_SS_NOTIFY_ENTER_DIAGNOSE_TYPE;

typedef struct
{
    EVENT_HEADER_TYPE header; 
} EV_SS_NOTIFY_LEAVE_DIAGNOSE_TYPE;

typedef struct
{
	EVENT_HEADER_TYPE header;
} EV_FW_ITEST_EXIT_TYPE;

#endif



