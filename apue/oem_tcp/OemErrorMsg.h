#ifndef __OEMERRORMSG_H__
#define __OEMERRORMSG_H__

typedef struct errorMsg
{
	//int id; //0 is cradle, 1 is elev.
	int num;
	const char *info;
}ErrorMsgType;

ErrorMsgType cradleErrorTable[21] = 
{
	{1, "Target cradle speed is invalid"},
	{2, "Target cradle position is invalid"},
	{3, "Cradle expose start position is invalid"},
	{4, "Cradle scan start position is invalid"},
	{5, "Cradle motion lost step error happened"},
	{6, "Cradle stop position mismatched with target"},
	{7, "Uncommanded cradle motion detected"},
	{8, "Cradle speed does not match with the target"},
	{9, "Cradle motion start timeout after receiving move command"},
	{10, "Cradle motion stop timeout after receiving stop command"},
	{11, "Cradle calibration data is invalid"},
	{12, "Cradle config file is invalid"},
	{13, "Cradle reach target speed timeout since moving"},
	{14, "Cradle pot value mismatch with encoder"},
	{15, "SCU to Table communication error when read pot value"},
	{16, "SCU to Table communication error when read table e2prom"},
	{17, "Cradle motion stopped as button release action not executed"},
	{18, "Cradle over current protection"},
	{19, "Cradle motor driver alarm"},
	{20, "Cradle motor driver alarm recover"},
	{21, "Cradle reach the limit switch"},
};

ErrorMsgType elevErrorTable[15] = 
{
	{27, "Elevation convert height to elevation position fail"},
	{28, "SCU FPGA register access error"},
	{29, "Elevation target height is invalid"},
	{30, "Elevation motion start timeout after receiving move command"},
	{31, "Elevation motion stop timeout after receiving stop command"},
	{32, "Elevation calibration data is invalid"},
	{33, "Elevation config file is invalid"},
	{35, "Communication error when read elevation pot value"},
	{36, "Table elevation stop height mismatched with target"},
	{37, "Table elevation driver is over current"},
	{38, "Uncommanded table elevation motion detected"},
	{40, "SCU to Table communication error when read table e2prom"},
	{41, "Table elevation over current recovered"},
	{43, "Table elevation fpga state change timeout"},
	{44, "Elev litmit switch error, and stop elev"},
};

#endif



