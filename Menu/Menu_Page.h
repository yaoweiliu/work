
#ifndef MENU_PAGE_H
#define MENU_PAGE_H
/*******************************************************************************************/
/*include                                                                                                                                                       */
/*******************************************************************************************/
#include "Menu.h"
#include "stdint.h"
/*******************************************************************************************/
/*Public Macro and Tyedef                                                                                                                               */
/*******************************************************************************************/

/*******************************************************************************************/
/*Public Function Declaration                                                                                                                             */
/*******************************************************************************************/
void Page_Main (MenuRunInfo* MenuRunInfos);
void Page_Set_Time (MenuRunInfo* MenuRunInfos);
void Page_Set_Date (MenuRunInfo* MenuRunInfos);
void Page_Set24Time (MenuRunInfo* MenuRunInfos);
void Page_RemoteKeyLearn (MenuRunInfo* MenuRunInfos);
void Page_SetVolume (MenuRunInfo* MenuRunInfos);
void Page_SetTimeCalibration (MenuRunInfo* MenuRunInfos);
void Page_SetBrightnessMode (MenuRunInfo* MenuRunInfos);
void Page_SetBrightnessCelibration (MenuRunInfo* MenuRunInfos);
void Page_SetBrightness_Sensitivity (MenuRunInfo* MenuRunInfos);
void Page_SetVoiceMode (MenuRunInfo* MenuRunInfos);
void Page_SetVoiceStartEnd (MenuRunInfo* MenuRunInfos);
void Page_SetRemoteKeyEnable (MenuRunInfo* MenuRunInfos);
void Page_SetAlarmClock (MenuRunInfo* MenuRunInfos);
void Page_SetFontColor (MenuRunInfo* MenuRunInfos);
void Page_SetFontType (MenuRunInfo* MenuRunInfos);
void Page_VisionInfo (MenuRunInfo* MenuRunInfos);
void Page_RestoreDefault (MenuRunInfo* MenuRunInfos);
void Page_DisplayTimerSet (MenuRunInfo* MenuRunInfos);
void Page_DisplayPageEnable (MenuRunInfo* MenuRunInfos);
void Page_PageSwitch (MenuRunInfo* MenuRunInfos);
void Page_EnvBightNessLearnAuto (MenuRunInfo* MenuRunInfos);
void Page_BrightnessLearnManuel (MenuRunInfo* MenuRunInfos);
void Page_BrightnessLearnSet (MenuRunInfo* MenuRunInfos);
void Page_ScreenLed_Check (MenuRunInfo* MenuRunInfos);
void Page_Senser_Ds3231 (MenuRunInfo* MenuRunInfos);
void Page_Senser_Ds18b20 (MenuRunInfo* MenuRunInfos);
void Page_Senser_Sht30 (MenuRunInfo* MenuRunInfos);
void Page_GUI_Test (MenuRunInfo* MenuRunInfos);
/*******************************************************************************************/
/*Public variables                                                                                                                                           */
/*******************************************************************************************/


#endif // MENU_PAGE_H
