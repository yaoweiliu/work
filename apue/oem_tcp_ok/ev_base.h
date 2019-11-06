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
#ifndef _EV_BASE_H_
#define _EV_BASE_H_

typedef enum 
{
    PROC_ID_UNKNOWN = 0,

    PROC_ID_SCANMGR, // =1
    PROC_ID_ALARM_CENTER,
    PROC_ID_STATUS_MGR,
    PROC_ID_STATISTIC_MGR,
    PROC_ID_PET_INTF, 
    PROC_ID_COMP_MGR,
    PROC_ID_SC = 10,  // scan control, start of subsystem on scu
    PROC_ID_AX,  //= 11 axial control
    PROC_ID_TBL, //= 12
    PROC_ID_GUIF, //= 13
    PROC_ID_AUDIO,//= 14
    PROC_ID_TILT_ELEV, // = 15 tilt and elevation
    PROC_ID_DAS = 30, //  start of subsystem on rcu
    PROC_ID_GEN,  // = 31
    PROC_ID_COLL, // = 32
    PROC_ID_DET_THERMAL, // = 33
    PROC_ID_TOOLS,
    PROC_ID_SCU_BOARD_SERVICE,
    PROC_ID_RCU_BOARD_SERVICE,
    PROC_ID_DISP1_BOARD_SERVICE,
    PROC_ID_DISP2_BOARD_SERVICE,
    PROC_ID_CTBOX_BOARD_SERVICE,


    PROC_ID_DISP = 40, // start of subsystem on lcd display board
    PROC_ID_NOUSED, // = 41
    PROC_ID_SCB,   // = 42

    PROC_ID_CTBOX,

    PROC_TEST1,    // = 43
    PROC_TEST2,    // = 44
    PROC_ID_CTBDISP,
    PROC_ID_CTBAUDIO,
    PROC_ID_DISP1,
    PROC_ID_GANTAUDIO,
    PROC_ID_DISP2,
    PROC_ID_DISP1_PANEL,
    PROC_ID_DISP2_PANEL,
    PROC_ID_LAST

}PROCESSID;


typedef enum 
{
    SVC_ID_UNKNOWN = 0,
    SVC_ID_OC_SCANMGR,
    SVC_ID_OC_ALARM_CENTER,
    SVC_ID_OC_STATUS_MGR,
    SVC_ID_OC_STATISTIC_MGR,
    SVC_ID_OC_SEASONING_MGR,
    SVC_ID_OC_ALIGNMENT_MGR,
    SVC_ID_OC_FWUPGRADE_MGR = 7,
    SVC_ID_OC_AX_DIAGNOSE_MGR,
    SVC_ID_OC_COLL_DIAGNOSE_MGR,
    SVC_ID_OC_CRADLE_DIAGNOSE_MGR,
    SVC_ID_OC_ELEV_DIAGNOSE_MGR,
    SVC_ID_OC_CTBOX_RECORD_MGR,

    SVC_ID_SC = 50, // scan control
    SVC_ID_SC_OC_COMM,
    SVC_ID_SC_SEQUENCE,
    SVC_ID_SC_HW_CTRL,
    SVC_ID_SC_DAS_CTRL,
    SVC_ID_SC_AX_CTRL,
    SVC_ID_SC_CRADLE_CTRL,
    SVC_ID_SC_GEN_CTRL,
    SVC_ID_SC_COLL_CTRL,
    SVC_ID_SC_PANEL_CTRL,
    SVC_ID_SC_TILT_CTRL,
    SVC_ID_SC_ELEV_CTRL,
    SVC_ID_SC_DISP_CTL,
    SVC_ID_SC_ERR_SVC,
    SVC_ID_SC_AUDIO,
    SVC_ID_SC_ECG,


    SVC_ID_AXIAL = 100,
    SVC_ID_CRADLE,
    SVC_ID_GANTRY_UIF,
    SVC_ID_TILT_ELEV,
    SVC_ID_AUDIO_CTBOX,
    SVC_ID_AUDIO_GANTRY,
    SVC_ID_AXIAL_DRV, 

    SVC_ID_DAS= 150,
    SVC_ID_GEN_MNG,
    SVC_ID_GEN_STARTER,
    SVC_ID_GEN_FLAM,
    SVC_ID_COLL,
    SVC_ID_DET_THERMAL,

    SVC_ID_GANTRY_PANEL1 = 200,
    SVC_ID_GANTRY_PANEL2,
    SVC_ID_GANTRY_DISP1,
    SVC_ID_GANTRY_DISP2,
    SVC_ID_CTBOX_BTN = 230,
    SVC_ID_CTBOX_DISP,

    SVC_ID_TILT_ELEV_CTRL = 260,
    SVC_ID_TILT_MGR,
    SVC_ID_ELEV_MGR,


    SVC_ID_TEST1 =300,
    SVC_ID_TEST2,
    SVC_ID_TEST3,
    SVC_ID_TEST4,
    SVC_ID_TEST5,
    SVC_ID_TEST6,
    SVC_ID_TEST7,
    SVC_ID_TEST8,

    SVC_ID_SCU_BOARD_SERVICE = 400,
    SVC_ID_RCU_BOARD_SERVICE,
    SVC_ID_DISP1_BOARD_SERVICE,
    SVC_ID_DISP2_BOARD_SERVICE,
    SVC_ID_CTBOX_BOARD_SERVICE,
    /* task board */
    SVC_ID_SCU_BS_TASK_BOARD,
    SVC_ID_RCU_BS_TASK_BOARD,
    SVC_ID_DISP1_BS_TASK_BOARD,
    SVC_ID_DISP2_BS_TASK_BOARD,
    SVC_ID_CTBOX_BS_TASK_BOARD,
    /* task hardware */
    SVC_ID_SCU_BS_TASK_HARDWARE,
    SVC_ID_RCU_BS_TASK_HARDWARE,
    SVC_ID_DISP1_BS_TASK_HARDWARE,
    SVC_ID_DISP2_BS_TASK_HARDWARE,
    SVC_ID_CTBOX_BS_TASK_HARDWARE,
    /* task network */
    SVC_ID_SCU_BS_TASK_NETWORK, 
    SVC_ID_RCU_BS_TASK_NETWORK, 
    SVC_ID_DISP1_BS_TASK_NETWORK,
    SVC_ID_DISP2_BS_TASK_NETWORK,
    SVC_ID_CTBOX_BS_TASK_NETWORK,
    /* task process */
    SVC_ID_SCU_BS_TASK_PROCESS,
    SVC_ID_RCU_BS_TASK_PROCESS,
    SVC_ID_DISP1_BS_TASK_PROCESS,
    SVC_ID_DISP2_BS_TASK_PROCESS,
    SVC_ID_CTBOX_BS_TASK_PROCESS,
    /* board test */
    SVC_ID_SCU_BOARD_TEST,
    SVC_ID_RCU_BOARD_TEST,
    SVC_ID_DISP1_BOARD_TEST,
    SVC_ID_DISP2_BOARD_TEST,
    SVC_ID_CTBOX_BOARD_TEST,

    SVC_ID_TOOLS,

    SVC_ID_PET_SCANMGR = 500,
    SVC_ID_PET_END = 599,

    SVC_ID_PET_CTRL = 600,
    SVC_ID_PET_CTRL_END = 699,

    SVC_ID_FW_TOOL = 700,

    /*below id are for device only*/    
    SVC_ID_DEV_BASE = 0x8000,    
    SVC_ID_DEV_MEM,
    SVC_ID_DEV_FPGATEST,
    SVC_ID_DEV_ADC_COMM,
    SVC_ID_DEV_FPGA_KEY,
    SVC_ID_DEV_AX,
    SVC_ID_DEV_FPGA_COMM,//add by wjw for gpga_comm device.
    SVC_ID_DEV_SC   = SVC_ID_DEV_BASE + 200,
    SVC_ID_DEV_DAS  = SVC_ID_DEV_BASE + 300,
    SVC_ID_DEV_DAS_RCU,
    SVC_ID_DEV_DAS_ADC,
    SVC_ID_DEV_TCB_TP,
    SVC_ID_DEV_CRADLE,
    SVC_ID_DEV_GANTRY_PANEL,
    SVC_ID_DEV_CTBOX_PANEL,
    SVC_ID_DEV_GEN  = SVC_ID_DEV_BASE + 400,
    SVC_ID_DEV_COLL  = SVC_ID_DEV_BASE + 500,


    SVC_ID_DEV_SLIPRING_STAT,
    SVC_ID_DEV_TRIGGER,
    SVC_ID_DEV_POWER,
    SVC_ID_DEV_TILT_ELEV,
    SVC_ID_DEV_ELEV_TILT,
    SVC_ID_DEV_PET_SHIFTER,
    SVC_ID_DEV_TCB,
    SVC_ID_DEV_FPGA_CONF,
    SVC_ID_DEV_ECG,
    SVC_ID_DEV_NAVI,
    SVC_ID_DEV_FPGA_SPI,
    SVC_ID_DEV_AUDIO,
	
	EV_ERROR_REPORTING,
    SVC_ID_DEV_END  = 0xFFFF
}SVCID;


typedef struct {
	uint32_t code;     // event code
	uint32_t length;   // total  size of header and payload
	SVCID sid;         // sender id
	SVCID rid;         // receiver
	uint32_t checksum; // code + length + sid + rid 
	uint32_t not_log;     //not log event
	int64_t sent;       // sent time, in nanoseconds since Jan. 1, 1970.
	int64_t received;  // received time, in nanoseconds since Jan. 1, 1970.
	int64_t complete;  // complete time, in nanoseconds since Jan. 1, 1970.
} EVENT_HEADER_TYPE;



enum SYSTEM_EVENT_SEGMENT
{
	SYSTEM_ECL_CODE_BASE = 0,
	SYSTEM_COMMON_CODE_BASE = 0x5000,
	SYSTEM_HOST_CODE_BASE = 0x10000,
	SYSTEM_SC_CODE_BASE = 0x20000,
	SYSTEM_AX_CODE_BASE = 0x22000,
	SYSTEM_CRADLE_CODE_BASE = 0x24000,
	SYSTEM_GUIF_CODE_BASE = 0x26000,//forr gantry pannel
	SYSTEM_AUDIO_CODE_BASE = 0x28000,
	SYSTEM_TILT_ELEV_CODE_BASE = 0x30000,
	SYSTEM_DAS_CODE_BASE = 0x32000,
	SYSTEM_GEN_CODE_BASE = 0x34000,
	SYSTEM_COLL_CODE_BASE = 0x36000,
	SYSTEM_DISP_CODE_BASE = 0x38000,
	SYSTEM_CTB_CODE_BASE = 0x40000,//for ct box dev.
	SYSTEM_GP_CODE_BASE	= 0x42000,//for gantry pannel dev
	SYSTEM_TCB_CODE_BASE = 0x44000,

	SYSTEM_PET_CTRL_CODE_BASE = 0x60000,//for pet control.
    SYSTEM_PET_CTRL_CODE_END = 0x6FFFF,
    SYSTEM_FPGA_CONF_CODE_BASE = 0x70000,
    SYSTEM_FPGA_CONF_CODE_END = 0x70500,
    SYSTEM_ECG_CODE_BASE = 0x70600,
    SYSTEM_ECG_CODE_END = 0x70800,
};


typedef struct
{
    EVENT_HEADER_TYPE header;
} EV_GENERIC_TYPE;


#endif// _EV_BASE_H_

