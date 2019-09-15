
/*******************************************************************************************/
/*include                                                                                                                                                       */
/*******************************************************************************************/
#include "Menu_Page.h"
#include "Bsp.h"
#include "Font.h"
#include "Bsp_Ds3231.h"
#include "Bsp_LightMonitor.h"
#include "Menu_If.h"
#include  "Brightness.h"
#include "Audio_Wav.h"
#include "App_Common.h"
#include "fdatefunc.h"
/*******************************************************************************************/
/*Local Macro and Tyedef                                                                                                                                 */
/*******************************************************************************************/

/*******************************************************************************************/
/*Static Function Declaration                                                                                                                            */
/*******************************************************************************************/

/*******************************************************************************************/
/*Public Function Declaration                                                                                                                             */
/*******************************************************************************************/

/*******************************************************************************************/
/*Local variables                                                                                                                                             */
/*******************************************************************************************/
//static uint16_t flg_HoldTimer;
/*******************************************************************************************/
/*Global variables                                                                                                                                           */
/*******************************************************************************************/

/*******************************************************************************************/
/*Static Function Implementation                                                                                                                      */
/*******************************************************************************************/
static void Page_Switch_Prev_Item (MenuRunInfo* MenuRunInfos, uint8_t FirstItem, uint8_t EndItem)
{
    if (MenuRunInfos->CursorMenu->CurrentItem > FirstItem)
    {
        MenuRunInfos->CursorMenu->CurrentItem--;
    }
    else
    {
        MenuRunInfos->CursorMenu->CurrentItem = EndItem;
    }

    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
}
static void Page_Switch_Next_Item (MenuRunInfo* MenuRunInfos, uint8_t FirstItem, uint8_t EndItem)
{
    if (MenuRunInfos->CursorMenu->CurrentItem < EndItem)
    {
        MenuRunInfos->CursorMenu->CurrentItem++;
    }
    else
    {
        MenuRunInfos->CursorMenu->CurrentItem = FirstItem;
    }

    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
}

static void Page_Play_Next_Wav (void)
{
    static DIR dir;
    FILINFO fileinfo;
    FRESULT res;
    char* temp;
    static uint8_t flg = 0;

    if (flg == 0)
    {
        f_opendir (&dir, "");
        flg = 1;
    }

    res = f_readdir (&dir, &fileinfo);

    if ((fileinfo.fname[0]) && ( res == FR_OK))
    {
        temp = strrchr (fileinfo.fname, '.');
        temp ++;

        if (! (fileinfo.fattrib & AM_DIR))
        {
            if (!strcasecmp (temp, "WAV"))
            {
                printf ("wav FileList=%s", fileinfo.fname);
                Audio_Wav_List_Add (fileinfo.fname);
                Audio_Wav_List_Play_Start ();
            }
        }
    }
    else
    {
        f_closedir (&dir);
        flg = 0;
    }
}

uint16_t ReadDirs (char* dir_str)
{
    uint16_t cnt = 0;
    DIR dir;
    FILINFO fileinfo;
    FRESULT res;

    f_opendir (&dir, dir_str);

    res = f_readdir (&dir, &fileinfo);

    while ((fileinfo.fname[0] && res == FR_OK))
    {
        if (fileinfo.fattrib & AM_DIR)
        {
            res = f_readdir (&dir, &fileinfo);
        }
        else
        {
            printf ("FileList=%s", fileinfo.fname);
            cnt ++;
        }

    }

    f_closedir (&dir);

    return cnt;
}
static void Page_Main_DisplayMode0 (MenuRunInfo* MenuRunInfos)
{
#define PAGE_ITEM_FIRST   1
#define PAGE_ITEM_DATE   1
#define PAGE_ITEM_DATE_CN   2
#define PAGE_ITEM_TEMPRA   3
#define PAGE_ITEM_HOLIDAY_INFO    4
#define PAGE_ITEM_BIRTHDAY_INFO    5
#define PAGE_ITEM_END   5

#define PAGE_MAIN_DIS_ON_TIME_MS    (MySettings.Display_Effect.Hold_Timer_ms)
#define PAGE_MAIN_DIS_OFF_TIME_MS    (MySettings.Display_Effect.Off_Timer_ms)
    //#define PAGE_MAIN_DIS_ON_TIME_MS    4000
    //#define PAGE_MAIN_DIS_OFF_TIME_MS    800

#define PAGE_MAIN_DIS_SWITCH_TIMEOUT_MS    ((PAGE_MAIN_DIS_ON_TIME_MS+PAGE_MAIN_DIS_OFF_TIME_MS)*4)
    static uint16_t LastBrightness;

    switch (MenuRunInfos->CursorMenu->CurrentItem)
    {
        case MENU_PAGE_ITEM_DEFAULT:
            MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_DATE;
            MenuRunInfos->Timer_PageHold = PAGE_MAIN_DIS_OFF_TIME_MS;
            Bsp_Screen_Set_Brightness_Immediately (SCREEN_INDEX_TOP, Brightness_GetMaxValue());
            Bsp_Screen_Set_Brightness_Immediately (SCREEN_INDEX_BOTTOM, 0);
            break;

        case PAGE_ITEM_DATE:
            if (MySettings.Display_Effect.Page_Date_En)
            {
                if (Bsp_Screen_Get_Brightness_Current (SCREEN_INDEX_BOTTOM) == 0)
                {
                    MenuRunInfos->Timer_Timerout = 0;

                    if (MenuRunInfos->Timer_PageHold * MENU_LOOP_TIME_MS > PAGE_MAIN_DIS_OFF_TIME_MS)
                    {
                        MenuRunInfos->Timer_PageHold = 0;
                        Screen_Clear (16, 0, 64, 16, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
                        Screen_Printf (16, 0, MySettings.FontCfg.FontType_Info_Num, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%02d", TimeDS3231.Month);
                        Screen_DisplayChar (16, 16, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "Θ");
                        //                    Screen_DisplayChar (0, 16, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "?");
                        Screen_Printf (16, 24, MySettings.FontCfg.FontType_Info_Num, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%02d", TimeDS3231.Day);
                        Screen_DisplayChar (16, 40, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "⊙");

                        //                    Screen_Printf (16, 40, MySettings.FontCfg.FontType_Info_CN, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "周");
                        if ((TimeDS3231.Week > 0) && (TimeDS3231.Week < 8))
                        {
                            Screen_Printf (16, 40 + 8, MySettings.FontCfg.FontType_Info_CN, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%s",
                                           dayNameInWeek_ByWeek (TimeDS3231.Week - 1));
                        }

                        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
                        LastBrightness = Brightness_GetMaxValue();
                        Bsp_Screen_Set_Brightness_Slowly (SCREEN_INDEX_BOTTOM, LastBrightness);
                    }
                    else
                    {
                        MenuRunInfos->Timer_PageHold++;
                    }
                }
                else if (Bsp_Screen_Get_Brightness_Current (SCREEN_INDEX_BOTTOM) >= LastBrightness)
                {
                    MenuRunInfos->Timer_Timerout = 0;

                    if (MenuRunInfos->Timer_PageHold * MENU_LOOP_TIME_MS > PAGE_MAIN_DIS_ON_TIME_MS)
                    {
                        MenuRunInfos->Timer_PageHold = 0;
                        Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                        //                        MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_DATE_CN;
                        Bsp_Screen_Set_Brightness_Slowly (SCREEN_INDEX_BOTTOM, 0);
                    }
                    else
                    {
                        MenuRunInfos->Timer_PageHold++;
                    }
                }
                else
                {
                    if (MenuRunInfos->Timer_Timerout * MENU_LOOP_TIME_MS > PAGE_MAIN_DIS_SWITCH_TIMEOUT_MS)
                    {
                        MenuRunInfos->Timer_Timerout = 0;

                        if (Bsp_Screen_Get_Brightness_New (SCREEN_INDEX_BOTTOM) == 0)
                        {
                            //NOTE:SLF 2018-08-15 判断是要达到最大值还是最小值
                            Bsp_Screen_Set_Brightness_Immediately (SCREEN_INDEX_BOTTOM, 0);
                        }
                        else
                        {
                            Bsp_Screen_Set_Brightness_Immediately (SCREEN_INDEX_BOTTOM, LastBrightness);
                        }
                    }
                    else
                    {
                        MenuRunInfos->Timer_Timerout++;
                    }
                }

            }
            else
            {
                Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
            }

            break;

        case PAGE_ITEM_DATE_CN:
            if (MySettings.Display_Effect.Page_DateCN_En)
            {
                if (Bsp_Screen_Get_Brightness_Current (SCREEN_INDEX_BOTTOM) == 0)
                {
                    MenuRunInfos->Timer_Timerout = 0;

                    if (MenuRunInfos->Timer_PageHold * MENU_LOOP_TIME_MS > PAGE_MAIN_DIS_OFF_TIME_MS)
                    {
                        MenuRunInfos->Timer_PageHold = 0;
                        Screen_Clear (16, 0, 64, 16, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
                        Screen_Printf (16, 0, MySettings.FontCfg.FontType_Info_CN, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%s月%s",
                                       monthNameInYear (TimeDS3231.Month_lunar),
                                       dayNameInMonth (TimeDS3231.Day_lunar));
                        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
                        LastBrightness = Brightness_GetMaxValue();
                        Bsp_Screen_Set_Brightness_Slowly (SCREEN_INDEX_BOTTOM, LastBrightness);
                    }
                    else
                    {
                        MenuRunInfos->Timer_PageHold++;
                    }
                }
                else if (Bsp_Screen_Get_Brightness_Current (SCREEN_INDEX_BOTTOM) >= LastBrightness)
                {
                    MenuRunInfos->Timer_Timerout = 0;

                    if (MenuRunInfos->Timer_PageHold * MENU_LOOP_TIME_MS > PAGE_MAIN_DIS_ON_TIME_MS)
                    {
                        MenuRunInfos->Timer_PageHold = 0;
                        Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                        //                        MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_TEMPRA;
                        Bsp_Screen_Set_Brightness_Slowly (SCREEN_INDEX_BOTTOM, 0);
                    }
                    else
                    {
                        MenuRunInfos->Timer_PageHold++;
                    }
                }
                else
                {
                    if (MenuRunInfos->Timer_Timerout * MENU_LOOP_TIME_MS > PAGE_MAIN_DIS_SWITCH_TIMEOUT_MS)
                    {
                        MenuRunInfos->Timer_Timerout = 0;

                        if (Bsp_Screen_Get_Brightness_New (SCREEN_INDEX_BOTTOM) == 0)
                        {
                            Bsp_Screen_Set_Brightness_Immediately (SCREEN_INDEX_BOTTOM, 0);
                        }
                        else
                        {
                            Bsp_Screen_Set_Brightness_Immediately (SCREEN_INDEX_BOTTOM, LastBrightness);
                        }
                    }
                    else
                    {
                        MenuRunInfos->Timer_Timerout++;
                    }
                }

            }
            else
            {
                Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
            }

            break;

        case PAGE_ITEM_TEMPRA:
            if (MySettings.Display_Effect.Page_Tempra_En)
            {
                if (Bsp_Screen_Get_Brightness_Current (SCREEN_INDEX_BOTTOM) == 0)
                {
                    MenuRunInfos->Timer_Timerout = 0;

                    if (MenuRunInfos->Timer_PageHold * MENU_LOOP_TIME_MS > PAGE_MAIN_DIS_OFF_TIME_MS)
                    {
                        MenuRunInfos->Timer_PageHold = 0;
                        Screen_Clear (16, 0, 64, 16, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);

                        if (MySettings.Senser_Cfg.Senser_Select == 2)
                        {
                            Screen_Printf (16, 0, MySettings.FontCfg.FontType_Info_Num, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%02d℃ %02d%%", TimeDS3231.Temper, 52);
                        }
                        else
                        {
                            Screen_Printf (16, 0, MySettings.FontCfg.FontType_Info_CN, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "室温");
                            Screen_Printf (16, 32, MySettings.FontCfg.FontType_Info_Num, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%02d℃", TimeDS3231.Temper);
                        }

                        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
                        LastBrightness = Brightness_GetMaxValue();
                        Bsp_Screen_Set_Brightness_Slowly (SCREEN_INDEX_BOTTOM, LastBrightness);
                    }
                    else
                    {
                        MenuRunInfos->Timer_PageHold++;
                    }
                }
                else if (Bsp_Screen_Get_Brightness_Current (SCREEN_INDEX_BOTTOM) >= LastBrightness)
                {
                    MenuRunInfos->Timer_Timerout = 0;

                    if (MenuRunInfos->Timer_PageHold * MENU_LOOP_TIME_MS > PAGE_MAIN_DIS_ON_TIME_MS)
                    {
                        MenuRunInfos->Timer_PageHold = 0;
                        Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                        Bsp_Screen_Set_Brightness_Slowly (SCREEN_INDEX_BOTTOM, 0);
                    }
                    else
                    {
                        MenuRunInfos->Timer_PageHold++;
                    }
                }
                else
                {
                    if (MenuRunInfos->Timer_Timerout * MENU_LOOP_TIME_MS > PAGE_MAIN_DIS_SWITCH_TIMEOUT_MS)
                    {
                        MenuRunInfos->Timer_Timerout = 0;

                        if (Bsp_Screen_Get_Brightness_New (SCREEN_INDEX_BOTTOM) == 0)
                        {
                            Bsp_Screen_Set_Brightness_Immediately (SCREEN_INDEX_BOTTOM, 0);
                        }
                        else
                        {
                            Bsp_Screen_Set_Brightness_Immediately (SCREEN_INDEX_BOTTOM, LastBrightness);
                        }
                    }
                    else
                    {
                        MenuRunInfos->Timer_Timerout++;
                    }
                }
            }
            else
            {
                Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
            }

            break;

        case PAGE_ITEM_HOLIDAY_INFO:
            Caculator_Holiday();

            if ((MySettings.Display_Effect.Page_Holiday_En) && (TimeDS3231.HolidayInfo != NULL_MENU) )
            {
                if (Bsp_Screen_Get_Brightness_Current (SCREEN_INDEX_BOTTOM) == 0)
                {
                    MenuRunInfos->Timer_Timerout = 0;

                    if (MenuRunInfos->Timer_PageHold * MENU_LOOP_TIME_MS > PAGE_MAIN_DIS_OFF_TIME_MS)
                    {
                        MenuRunInfos->Timer_PageHold = 0;
                        Screen_Clear (16, 0, 64, 16, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
                        Screen_Printf (16, 0, MySettings.FontCfg.FontType_Info_CN, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%s", TimeDS3231.HolidayInfo);
                        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
                        LastBrightness = Brightness_GetMaxValue();
                        Bsp_Screen_Set_Brightness_Slowly (SCREEN_INDEX_BOTTOM, LastBrightness);
                    }
                    else
                    {
                        MenuRunInfos->Timer_PageHold++;
                    }
                }
                else if (Bsp_Screen_Get_Brightness_Current (SCREEN_INDEX_BOTTOM) >= LastBrightness)
                {
                    MenuRunInfos->Timer_Timerout = 0;

                    if (MenuRunInfos->Timer_PageHold * MENU_LOOP_TIME_MS > PAGE_MAIN_DIS_ON_TIME_MS)
                    {
                        MenuRunInfos->Timer_PageHold = 0;
                        Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                        Bsp_Screen_Set_Brightness_Slowly (SCREEN_INDEX_BOTTOM, 0);
                    }
                    else
                    {
                        MenuRunInfos->Timer_PageHold++;
                    }
                }
                else
                {
                    if (MenuRunInfos->Timer_Timerout * MENU_LOOP_TIME_MS > PAGE_MAIN_DIS_SWITCH_TIMEOUT_MS)
                    {
                        MenuRunInfos->Timer_Timerout = 0;

                        if (Bsp_Screen_Get_Brightness_New (SCREEN_INDEX_BOTTOM) == 0)
                        {
                            //NOTE:SLF 2018-08-15 判断是要达到最大值还是最小值
                            Bsp_Screen_Set_Brightness_Immediately (SCREEN_INDEX_BOTTOM, 0);
                        }
                        else
                        {
                            Bsp_Screen_Set_Brightness_Immediately (SCREEN_INDEX_BOTTOM, LastBrightness);
                        }
                    }
                    else
                    {
                        MenuRunInfos->Timer_Timerout++;
                    }
                }
            }
            else
            {
                Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
            }

            break;

        case PAGE_ITEM_BIRTHDAY_INFO:
            Caculator_Birthday();

            if ((MySettings.Display_Effect.Page_Brithday_En) && (TimeDS3231.BirthDayInfo != NULL_MENU))
            {
                if (Bsp_Screen_Get_Brightness_Current (SCREEN_INDEX_BOTTOM) == 0)
                {
                    MenuRunInfos->Timer_Timerout = 0;

                    if (MenuRunInfos->Timer_PageHold * MENU_LOOP_TIME_MS > PAGE_MAIN_DIS_OFF_TIME_MS)
                    {
                        MenuRunInfos->Timer_PageHold = 0;
                        Screen_Clear (16, 0, 64, 16, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
                        Screen_Printf (16, 0, MySettings.FontCfg.FontType_Info_CN, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%s", TimeDS3231.BirthDayInfo);
                        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
                        LastBrightness = Brightness_GetMaxValue();
                        Bsp_Screen_Set_Brightness_Slowly (SCREEN_INDEX_BOTTOM, LastBrightness);
                    }
                    else
                    {
                        MenuRunInfos->Timer_PageHold++;
                    }
                }
                else if (Bsp_Screen_Get_Brightness_Current (SCREEN_INDEX_BOTTOM) >= LastBrightness)
                {
                    MenuRunInfos->Timer_Timerout = 0;

                    if (MenuRunInfos->Timer_PageHold * MENU_LOOP_TIME_MS > PAGE_MAIN_DIS_ON_TIME_MS)
                    {
                        MenuRunInfos->Timer_PageHold = 0;
                        Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                        Bsp_Screen_Set_Brightness_Slowly (SCREEN_INDEX_BOTTOM, 0);
                    }
                    else
                    {
                        MenuRunInfos->Timer_PageHold++;
                    }
                }
                else
                {
                    if (MenuRunInfos->Timer_Timerout * MENU_LOOP_TIME_MS > PAGE_MAIN_DIS_SWITCH_TIMEOUT_MS)
                    {
                        MenuRunInfos->Timer_Timerout = 0;

                        if (Bsp_Screen_Get_Brightness_New (SCREEN_INDEX_BOTTOM) == 0)
                        {
                            //NOTE:SLF 2018-08-15 判断是要达到最大值还是最小值
                            Bsp_Screen_Set_Brightness_Immediately (SCREEN_INDEX_BOTTOM, 0);
                        }
                        else
                        {
                            Bsp_Screen_Set_Brightness_Immediately (SCREEN_INDEX_BOTTOM, LastBrightness);
                        }
                    }
                    else
                    {
                        MenuRunInfos->Timer_Timerout++;
                    }
                }
            }
            else
            {
                Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
            }

            break;

        default:
            //            MenuRunInfos->Timer_PageHold = 0;
            //            MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_DATE;
            //            Bsp_Screen_Set_Brightness_Slowly (SCREEN_INDEX_BOTTOM, 0);
            break;
    }

    switch (*MenuRunInfos->KeyCode)
    {
        case BT_KEYCODE_MENU:
        case BT_KEYCODE_MENU_RMET:

            break;

        case BT_KEYCODE_UP_LONG:
        case BT_KEYCODE_UP:
        case BT_KEYCODE_UP_RMET:
        case BT_KEYCODE_LEFT:
        case BT_KEYCODE_LEFT_RMET:
            MenuRunInfos->Timer_PageHold = 0;
            Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
            Bsp_Screen_Set_Brightness_Immediately (SCREEN_INDEX_BOTTOM, 0);
            break;

        case BT_KEYCODE_DOWN_LONG:
        case BT_KEYCODE_DOWN:
        case BT_KEYCODE_DOWN_RMET:
        case BT_KEYCODE_RIGHT:
        case BT_KEYCODE_RIGHT_RMET:
            MenuRunInfos->Timer_PageHold = 0;
            Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
            Bsp_Screen_Set_Brightness_Immediately (SCREEN_INDEX_BOTTOM, 0);
            break;

        case BT_KEYCODE_OK:
        case BT_KEYCODE_OK_RMET:

            if (Bsp_Screen_Get_Brightness_Current (SCREEN_INDEX_TOP) != 1)
            {
                Bsp_Screen_Set_Brightness_Immediately (SCREEN_INDEX_TOP | SCREEN_INDEX_BOTTOM, 1);
            }
            else
            {
                Bsp_Screen_Set_Brightness_Immediately (SCREEN_INDEX_TOP | SCREEN_INDEX_BOTTOM, Brightness_GetMaxValue());
            }

            break;

        case BT_KEYCODE_VOICE_RMET:
            Common_BroadcastTime (TimeDS3231.Hour24, TimeDS3231.Minute);
            //            ReadDirs ("");
            //            Page_Play_Next_Wav();
            //            Audio_Wav_List_Add ("现在时间是.wav");
            //            Audio_Wav_List_Add ("早上.wav");
            //            Audio_Wav_List_Add ("8.wav");
            //            Audio_Wav_List_Add ("点.wav");
            //            Audio_Wav_List_Play_Start ();
            break;

        default:
            break;
    }

    if (MenuRunInfos->CursorMenu->Status == MENU_STATUS_HAS_UPATE)
    {

        MenuRunInfos->CursorMenu->Status = MENU_STATUS_NORMAL;
        Screen_Clear (0, 0, 64, 16, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
        Screen_Printf (0, 0, MySettings.FontCfg.FontType_Clock, MySettings.FontCfg.FontColor_Clock, SCREEN_UPDATE_NORMAL, "%02d", TimeDS3231.Hour);
        Screen_Printf (0, 24, MySettings.FontCfg.FontType_Clock, MySettings.FontCfg.FontColor_Clock, SCREEN_UPDATE_NORMAL, "%02d", TimeDS3231.Minute);
        Screen_Printf (0, 48, MySettings.FontCfg.FontType_Clock, MySettings.FontCfg.FontColor_Clock, SCREEN_UPDATE_NORMAL, "%02d", TimeDS3231.Second);

        if (TimeDS3231.Second % 2)
        {
            Screen_Printf (0, 16, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, ":");
            Screen_Printf (0, 40, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, ":");
        }
        else
        {
            Screen_Printf (0, 16, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "∷");
            Screen_Printf (0, 40, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "∷");
        }

        if (TimeDS3231.Is12Hour)
        {
            if (TimeDS3231.IsPM)
            {
                Screen_DisplayChar (0, 16, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "↓");
            }
            else
            {
                Screen_DisplayChar (0, 16, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "↑");
            }
        }


        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
    }

#undef PAGE_ITEM_FIRST
#undef PAGE_ITEM_DATE
#undef PAGE_ITEM_DATE_CN
#undef PAGE_ITEM_TEMPRA
#undef PAGE_ITEM_HOLIDAY_INFO
#undef PAGE_ITEM_BIRTHDAY_INFO
#undef PAGE_MAIN_DIS_ON_TIME_MS
#undef PAGE_MAIN_DIS_OFF_TIME_MS
#undef PAGE_ITEM_END

}
static void Page_Main_DisplayMode1 (MenuRunInfo* MenuRunInfos)
{
    if (MenuRunInfos->CursorMenu->Status == MENU_STATUS_HAS_UPATE)
    {

        MenuRunInfos->CursorMenu->Status = MENU_STATUS_NORMAL;
        Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);

        Screen_Printf (0, 0, FontsType_ColockMode1_Num, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%d", TimeDS3231.Year / 1000);
        Screen_Printf (0, 5, FontsType_ColockMode1_Num, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%d", TimeDS3231.Year % 1000 / 100);
        Screen_Printf (0, 10, FontsType_ColockMode1_Num, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%d", TimeDS3231.Year % 100 / 10);
        Screen_Printf (0, 15, FontsType_ColockMode1_Num, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%d", TimeDS3231.Year % 10);

        Screen_Draw_Point (3, 21, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL);
        Screen_Draw_Point (3, 22, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL);

        Screen_Printf (0, 23, FontsType_ColockMode1_Num, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%d", TimeDS3231.Month / 10);
        Screen_Printf (0, 28, FontsType_ColockMode1_Num, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%d", TimeDS3231.Month % 10);

        Screen_Draw_Point (3, 34, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL);
        Screen_Draw_Point (3, 35, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL);

        Screen_Printf (0, 36, FontsType_ColockMode1_Num, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%d", TimeDS3231.Day / 10);
        Screen_Printf (0, 41, FontsType_ColockMode1_Num, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%d", TimeDS3231.Day % 10);
        //        Screen_Printf (0, 46, FontsType_ColockMode1_Mix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "正");
        Screen_Printf (0, 48, FontsType_ColockMode1_Mix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "周%s", dayNameInWeek_ByWeek (TimeDS3231.Week - 1));

        Screen_Printf (25, 0, FontsType_ColockMode1_Mix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%s月%s",
                       monthNameInYear (TimeDS3231.Month_lunar),
                       dayNameInMonth (TimeDS3231.Day_lunar));

        Screen_Printf (25, 32, FontsType_ColockMode1_Mix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "春分");

        Screen_Printf (25, 48, FontsType_ColockMode1_Num, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%d", TimeDS3231.Temper / 10);
        Screen_Printf (25, 53, FontsType_ColockMode1_Num, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%d", TimeDS3231.Temper % 10);
        Screen_Printf (25, 58, FontsType_ColockMode1_Mix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "℃");

        Screen_Printf (8, 0, MySettings.FontCfg.FontType_Clock, MySettings.FontCfg.FontColor_Clock, SCREEN_UPDATE_NORMAL, "%02d", TimeDS3231.Hour);
        Screen_Printf (8, 24, MySettings.FontCfg.FontType_Clock, MySettings.FontCfg.FontColor_Clock, SCREEN_UPDATE_NORMAL, "%02d", TimeDS3231.Minute);
        Screen_Printf (8, 48, MySettings.FontCfg.FontType_Clock, MySettings.FontCfg.FontColor_Clock, SCREEN_UPDATE_NORMAL, "%02d", TimeDS3231.Second);

        if (TimeDS3231.Second % 2)
        {
            Screen_Printf (8, 16, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, ":");
            Screen_Printf (8, 40, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, ":");
        }
        else
        {
            Screen_Printf (8, 16, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "∷");
            Screen_Printf (8, 40, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "∷");
        }

        if (TimeDS3231.Is12Hour)
        {
            if (TimeDS3231.IsPM)
            {
                Screen_DisplayChar (8, 16, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "↓");
            }
            else
            {
                Screen_DisplayChar (8, 16, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "↑");
            }
        }


        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
    }
}
/*******************************************************************************************/
/*Public Function Implementation                                                                                                                       */
/*******************************************************************************************/
void Page_Main (MenuRunInfo* MenuRunInfos)
{


    static uint8_t LastSecondValue = 0;

    if (MenuRunInfos->CursorMenu->Mode  == MENU_MODE_QUICKVIEW)
    {
        return;
    }

    MenuRunInfos->CursorMenu->Status = MENU_STATUS_NORMAL;

    //    Bsp_Ds3231_GetData (&TimeDS3231);

    if (TimeDS3231.Second != LastSecondValue)
    {
        LastSecondValue = TimeDS3231.Second;
        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
    }

    switch (MySettings.DisplayMode)
    {
        case 0:
            Page_Main_DisplayMode0 (MenuRunInfos);
            break;

        case 1:
            Page_Main_DisplayMode1 (MenuRunInfos);
            break;

        default:
            Page_Main_DisplayMode0 (MenuRunInfos);
            break;
    }

    //    static uint8_t flg_Printf;

    //    if (flg_Printf != TimeDS3231.Second)
    //    {
    //        if (TimeDS3231.Is12Hour)
    //        {
    //            if (TimeDS3231.IsPM)
    //            {
    //                printf ("%04d年%02d月%02d日 星期%d 下午 %02d时%02d分%02d秒", TimeDS3231.Year, TimeDS3231.Month, TimeDS3231.Day, TimeDS3231.Week,
    //                        TimeDS3231.Hour, TimeDS3231.Minute,
    //                        TimeDS3231.Second);
    //            }
    //            else
    //            {
    //                printf ("%04d年%02d月%02d日 星期%d 上午 %02d时%02d分%02d秒", TimeDS3231.Year, TimeDS3231.Month, TimeDS3231.Day, TimeDS3231.Week,
    //                        TimeDS3231.Hour, TimeDS3231.Minute,
    //                        TimeDS3231.Second);
    //            }

    //        }
    //        else
    //        {
    //            printf ("%04d年%02d月%02d日 星期%d %02d时%02d分%02d秒", TimeDS3231.Year, TimeDS3231.Month, TimeDS3231.Day, TimeDS3231.Week, TimeDS3231.Hour,
    //                    TimeDS3231.Minute, TimeDS3231.Second);
    //        }

    //        flg_Printf = TimeDS3231.Second;
    //    }

}


void Page_Set_Time (MenuRunInfo* MenuRunInfos)
{
#define PAGE_ITEM_FIRST   1
#define PAGE_ITEM_HOUR   1
#define PAGE_ITEM_AFTERNON 2
#define PAGE_ITEM_MIN   3
#define PAGE_ITEM_SEC   4
#define PAGE_ITEM_END   4
    static uint8_t LastSecondValue = 0;

    if (MenuRunInfos->CursorMenu->Mode  == MENU_MODE_QUICKVIEW)
    {
        return;
    }

    MenuRunInfos->Timer_SetList = 0; //NOTE:SLF 2018-05-24 时间设置界面不自动退出
    //    Bsp_Ds3231_GetData (&TimeDS3231);

    if (TimeDS3231.Second != LastSecondValue)
    {
        LastSecondValue = TimeDS3231.Second;
        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
    }

    switch (MenuRunInfos->CursorMenu->CurrentItem)
    {
        case MENU_PAGE_ITEM_DEFAULT:
            MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_HOUR;
            MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            break;

        case PAGE_ITEM_HOUR:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    TimeDS3231.Hour++;

                    if (TimeDS3231.Is12Hour == 1)
                    {
                        if (TimeDS3231.Hour > 12)
                        {
                            TimeDS3231.Hour = 0;
                        }
                    }
                    else
                    {
                        if (TimeDS3231.Hour > 23)
                        {
                            TimeDS3231.Hour = 0;
                        }
                    }

                    Bsp_Ds3231_SetHour (&TimeDS3231);
                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    TimeDS3231.Hour--;

                    if (TimeDS3231.Is12Hour == 1)
                    {
                        if ((TimeDS3231.Hour == 0) || (TimeDS3231.Hour > 12))
                        {
                            TimeDS3231.Hour = 12;
                        }
                    }
                    else
                    {
                        if ((TimeDS3231.Hour == 0) || (TimeDS3231.Hour > 23))
                        {
                            TimeDS3231.Hour = 23;
                        }
                    }

                    Bsp_Ds3231_SetHour (&TimeDS3231);

                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;

                    if (TimeDS3231.Is12Hour)
                    {
                        MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_AFTERNON;
                    }
                    else
                    {
                        MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_MIN;
                    }

                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_AFTERNON:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;

                    if (TimeDS3231.IsPM)
                    {
                        TimeDS3231.IsPM = 0;
                    }
                    else
                    {
                        TimeDS3231.IsPM = 1;
                    }

                    Bsp_Ds3231_SetHour (&TimeDS3231);
                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_MIN:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    TimeDS3231.Minute++;

                    if (TimeDS3231.Minute > 59)
                    {
                        TimeDS3231.Minute = 0;
                    }

                    Bsp_Ds3231_SetMin (TimeDS3231.Minute);
                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    TimeDS3231.Minute --;

                    if ((TimeDS3231.Minute == 0) || (TimeDS3231.Minute > 59))
                    {
                        TimeDS3231.Minute = 59;
                    }

                    Bsp_Ds3231_SetMin (TimeDS3231.Minute);
                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_SEC:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    //                    *MenuRunInfos->KeyCode = BT_KEYCODE_NONE;
                    return;

                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:

                //                    TimeDS3231.Second++;

                //                    if (TimeDS3231.Second > 59)
                //                    {
                //                        TimeDS3231.Second = 0;
                //                    }
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    TimeDS3231.Second = 0;
                    Bsp_Ds3231_SetSec (TimeDS3231.Second);
                    //                    if (TimeDS3231.Second == 0)
                    //                    {
                    //                        TimeDS3231.Second = 59;
                    //                    }

                    //                    TimeDS3231.Second--;
                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        default:
            break;
    }

    if (MenuRunInfos->CursorMenu->Status == MENU_STATUS_HAS_UPATE)
    {
        MenuRunInfos->CursorMenu->Status = MENU_STATUS_NORMAL;
        Screen_Clear (16, 0, 64, 16, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
        Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "时间设置");
        Screen_DisplayChar (16, 16, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "∷");
        Screen_DisplayChar (16, 40, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "∷");

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_HOUR)
        {
            Screen_Printf (16, 0, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%02d", TimeDS3231.Hour);
        }
        else
        {
            Screen_Printf (16, 0, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%02d", TimeDS3231.Hour);
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_AFTERNON)
        {
            if (TimeDS3231.Is12Hour)
            {
                if (TimeDS3231.IsPM)
                {
                    Screen_DisplayChar (16, 16, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "↓");
                }
                else
                {
                    Screen_DisplayChar (16, 16, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "↑");
                }
            }
        }
        else
        {
            if (TimeDS3231.Is12Hour)
            {
                if (TimeDS3231.IsPM)
                {
                    Screen_DisplayChar (16, 16, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "↓");
                }
                else
                {
                    Screen_DisplayChar (16, 16, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "↑");
                }
            }
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_MIN)
        {
            Screen_Printf (16, 24, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%02d", TimeDS3231.Minute);
        }
        else
        {
            Screen_Printf (16, 24, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%02d", TimeDS3231.Minute);
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_SEC)
        {
            Screen_Printf (16, 48, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%02d", TimeDS3231.Second);
        }
        else
        {
            Screen_Printf (16, 48, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%02d", TimeDS3231.Second);
        }

        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
    }

#undef PAGE_ITEM_FIRST
#undef PAGE_ITEM_HOUR
#undef PAGE_ITEM_AFTERNON
#undef PAGE_ITEM_MIN
#undef PAGE_ITEM_SEC
#undef PAGE_ITEM_END
}

void Page_Set_Date (MenuRunInfo* MenuRunInfos)
{
#define PAGE_ITEM_FIRST 1
#define PAGE_ITEM_YEAR   1
#define PAGE_ITEM_MONTH   2
#define PAGE_ITEM_DAY   3
#define PAGE_ITEM_WEEK   4
#define PAGE_ITEM_END   4

    if (MenuRunInfos->CursorMenu->Mode  == MENU_MODE_QUICKVIEW)
    {
        return;
    }
    else
    {

    }

    switch (MenuRunInfos->CursorMenu->CurrentItem)
    {
        case MENU_PAGE_ITEM_DEFAULT:

            MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_YEAR;
            //            Bsp_Ds3231_GetData (&TimeDS3231);
            MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            break;

        case PAGE_ITEM_YEAR:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    TimeDS3231.Year++;

                    if (TimeDS3231.Year > 2099)
                    {
                        TimeDS3231.Year = 2000;
                    }

                    Bsp_Ds3231_SetYear (TimeDS3231.Year);
                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    TimeDS3231.Year--;

                    if ((TimeDS3231.Year == 1999) || (TimeDS3231.Year > 2099))
                    {
                        TimeDS3231.Year = 2099;
                    }

                    Bsp_Ds3231_SetYear (TimeDS3231.Year);
                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_MONTH:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    TimeDS3231.Month++;

                    if (TimeDS3231.Month > 12)
                    {
                        TimeDS3231.Month = 1;
                    }

                    Bsp_Ds3231_SetMonth (TimeDS3231.Month);
                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    TimeDS3231.Month--;

                    if ((TimeDS3231.Month == 0) || (TimeDS3231.Month > 12))
                    {
                        TimeDS3231.Month = 12;
                    }

                    Bsp_Ds3231_SetMonth (TimeDS3231.Month);
                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_DAY:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    TimeDS3231.Day++;

                    if ( (TimeDS3231.Month == 1) || (TimeDS3231.Month == 3) || (TimeDS3231.Month == 5) || (TimeDS3231.Month == 7) || (TimeDS3231.Month == 8)
                            || (TimeDS3231.Month == 10)
                            || (TimeDS3231.Month == 12))
                    {
                        if (TimeDS3231.Day > 31)
                        {
                            TimeDS3231.Day = 1;
                        }
                    }
                    else if (TimeDS3231.Month == 2)
                    {
                        if (TimeDS3231.Day > 29)
                        {
                            TimeDS3231.Day = 1;
                        }
                    }
                    else
                    {
                        if (TimeDS3231.Day > 30)
                        {
                            TimeDS3231.Day = 1;
                        }
                    }

                    Bsp_Ds3231_SetDay (TimeDS3231.Day);
                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    TimeDS3231.Day--;

                    if ( (TimeDS3231.Month == 1) || (TimeDS3231.Month == 3) || (TimeDS3231.Month == 5) || (TimeDS3231.Month == 7) || (TimeDS3231.Month == 8)
                            || (TimeDS3231.Month == 10)
                            || (TimeDS3231.Month == 12))
                    {
                        if ((TimeDS3231.Day == 0) || (TimeDS3231.Day > 31))
                        {
                            TimeDS3231.Day = 31;
                        }
                    }
                    else if (TimeDS3231.Month == 2)
                    {
                        if ((TimeDS3231.Day == 0) || (TimeDS3231.Day > 29))
                        {
                            TimeDS3231.Day = 29;
                        }
                    }
                    else
                    {
                        if ((TimeDS3231.Day == 0) || (TimeDS3231.Day > 30))
                        {
                            TimeDS3231.Day = 30;
                        }
                    }

                    Bsp_Ds3231_SetDay (TimeDS3231.Day);
                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_WEEK:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    TimeDS3231.Week++;

                    if (TimeDS3231.Week > 7)
                    {
                        TimeDS3231.Week = 1;
                    }

                    Bsp_Ds3231_SetWeek (TimeDS3231.Week);
                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    TimeDS3231.Week--;

                    if (TimeDS3231.Week == 0)
                    {
                        TimeDS3231.Week = 7;
                    }

                    Bsp_Ds3231_SetWeek (TimeDS3231.Week);
                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        default:
            break;
    }

    if (MenuRunInfos->CursorMenu->Status == MENU_STATUS_HAS_UPATE)
    {
        MenuRunInfos->CursorMenu->Status = MENU_STATUS_NORMAL;
        Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
        Screen_Printf (16, 16, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "-");
        Screen_Printf (16, 16 + 8 + 16, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "周");

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_YEAR)
        {
            Screen_Printf (0, 8, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%04d", TimeDS3231.Year);
            Screen_Printf (0, 8 + 8 * 4, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "年");
        }
        else
        {
            Screen_Printf (0, 8, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%04d年", TimeDS3231.Year);
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_MONTH)
        {
            Screen_Printf (16, 0, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%02d", TimeDS3231.Month);
        }
        else
        {
            Screen_Printf (16, 0, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%02d", TimeDS3231.Month);
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_DAY)
        {
            Screen_Printf (16, 16 + 8, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%02d", TimeDS3231.Day);
        }
        else
        {
            Screen_Printf (16, 16 + 8, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%02d", TimeDS3231.Day);
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_WEEK)
        {
            Screen_Printf (16, 16 + 8 + 16 + 16, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%d", TimeDS3231.Week);
        }
        else
        {
            Screen_Printf (16, 16 + 8 + 16 + 16, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%d", TimeDS3231.Week);
        }

        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
    }

#undef PAGE_ITEM_FIRST
#undef PAGE_ITEM_YEAR
#undef PAGE_ITEM_MONTH
#undef PAGE_ITEM_DAY
#undef PAGE_ITEM_WEEK
#undef PAGE_ITEM_END
}

void Page_Set24Time (MenuRunInfo* MenuRunInfos)
{
#define PAGE_SET_24HOUR     1

    if (MenuRunInfos->CursorMenu->Mode  == MENU_MODE_QUICKVIEW)
    {
        return;
    }
    else
    {

    }

    switch (MenuRunInfos->CursorMenu->CurrentItem)
    {
        case MENU_PAGE_ITEM_DEFAULT:
            MenuRunInfos->CursorMenu->CurrentItem = PAGE_SET_24HOUR;
            //            Bsp_Ds3231_GetData (&TimeDS3231);
            MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            break;

        case PAGE_SET_24HOUR:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;

                    if (TimeDS3231.Is12Hour)
                    {
                        TimeDS3231.Is12Hour = 0;

                        if ((TimeDS3231.IsPM) && (TimeDS3231.Hour != 12))
                        {
                            TimeDS3231.Hour += 12;
                        }
                    }
                    else
                    {
                        TimeDS3231.Is12Hour = 1;

                        if (TimeDS3231.Hour >= 12)
                        {
                            TimeDS3231.IsPM = 1;
                            TimeDS3231.Hour -= 12;
                        }
                    }

                    Bsp_Ds3231_SetHour (&TimeDS3231);
                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                    break;

                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:

                    break;

                default:
                    break;
            }

            if (MenuRunInfos->CursorMenu->Status == MENU_STATUS_HAS_UPATE)
            {
                Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "时制设置");

                if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_SET_24HOUR)
                {
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_NORMAL;
                    Screen_Clear (16, 0, 64, 16, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);

                    if (TimeDS3231.Is12Hour == 1)
                    {
                        Screen_Printf (16, 0, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "12");
                    }
                    else
                    {
                        Screen_Clear (16, 32, 32, 16, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL);
                        Screen_Printf (16, 0, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "24");
                    }

                    Screen_Printf (16, 16, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "时制");
                    Screen_UpdateImmediately (SCREEN_COLOR_ALL);
                }
            }

            break;
    }

#undef PAGE_SET_24HOUR
}
void Page_RemoteKeyLearn (MenuRunInfo* MenuRunInfos)
{
#define PAGE_ITEM_FIRST 1
#define PAGE_ITEM_LEARN_KEY_MENU_REMT     1
#define PAGE_ITEM_LEARN_KEY_UP_REMT    2
#define PAGE_ITEM_LEARN_KEY_DOWN_REMT     3
#define PAGE_ITEM_LEARN_KEY_LEFT_REMT     4
#define PAGE_ITEM_LEARN_KEY_RIGHT_REMT     5
#define PAGE_ITEM_LEARN_KEY_OK_REMT     6
#define PAGE_ITEM_LEARN_KEY_VOICE_REMT     7
#define PAGE_ITEM_END  7
#define PAGE_REMOTEKEYLEARN_TIME_MS    5000
    uint32_t KeyCode_Temp;

    if (MenuRunInfos->CursorMenu->Mode  == MENU_MODE_QUICKVIEW)
    {
        return;
    }
    else
    {

    }

    MenuRunInfos->Timer_SetList = 0; //NOTE:SLF 2018-05-24 时间设置界面不自动退出
    MySettings.RemoteKeyEnable = 1;

    if (MenuRunInfos->Timer_PageHold * MENU_LOOP_TIME_MS % 1000 == 0)
    {
        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
    }

    KeyCode_Temp = Bsp_Remote_Scan ();

    switch (MenuRunInfos->CursorMenu->CurrentItem)
    {
        case MENU_PAGE_ITEM_DEFAULT:
            MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_LEARN_KEY_MENU_REMT;
            KeyCode_Temp = 0; //NOTE:SLF 2018-05-28 需要先清一次码
            MenuRunInfos->Timer_PageHold = PAGE_REMOTEKEYLEARN_TIME_MS / MENU_LOOP_TIME_MS;
            MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            break;

        case PAGE_ITEM_LEARN_KEY_MENU_REMT:
            if (KeyCode_Temp != 0)
            {
                MySettings.Rkey_Menu.KeyCode = KeyCode_Temp;
                MySettings.Rkey_Menu.isLearned = 1;
                MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            }

            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);

                    return;

                case BT_KEYCODE_UP:
                case BT_KEYCODE_LEFT:
                    MenuRunInfos->Timer_PageHold = PAGE_REMOTEKEYLEARN_TIME_MS / MENU_LOOP_TIME_MS;
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_OK:
                    MenuRunInfos->Timer_PageHold = PAGE_REMOTEKEYLEARN_TIME_MS / MENU_LOOP_TIME_MS;
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_LEARN_KEY_UP_REMT:
            if (KeyCode_Temp != 0)
            {
                MySettings.Rkey_Up.KeyCode = KeyCode_Temp;
                MySettings.Rkey_Up.isLearned = 1;
                MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            }

            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);

                    return;

                case BT_KEYCODE_UP:
                case BT_KEYCODE_LEFT:
                    MenuRunInfos->Timer_PageHold = PAGE_REMOTEKEYLEARN_TIME_MS / MENU_LOOP_TIME_MS;
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_OK:
                    MenuRunInfos->Timer_PageHold = PAGE_REMOTEKEYLEARN_TIME_MS / MENU_LOOP_TIME_MS;
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_LEARN_KEY_DOWN_REMT:
            if (KeyCode_Temp != 0)
            {
                MySettings.Rkey_Down.KeyCode = KeyCode_Temp;
                MySettings.Rkey_Down.isLearned = 1;
                MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            }

            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);

                    return;

                case BT_KEYCODE_UP:
                case BT_KEYCODE_LEFT:
                    MenuRunInfos->Timer_PageHold = PAGE_REMOTEKEYLEARN_TIME_MS / MENU_LOOP_TIME_MS;
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_OK:
                    MenuRunInfos->Timer_PageHold = PAGE_REMOTEKEYLEARN_TIME_MS / MENU_LOOP_TIME_MS;
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_LEARN_KEY_LEFT_REMT:
            if (KeyCode_Temp != 0)
            {
                MySettings.Rkey_Left.KeyCode = KeyCode_Temp;
                MySettings.Rkey_Left.isLearned = 1;
                MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            }

            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);

                    return;

                case BT_KEYCODE_UP:
                case BT_KEYCODE_LEFT:
                    MenuRunInfos->Timer_PageHold = PAGE_REMOTEKEYLEARN_TIME_MS / MENU_LOOP_TIME_MS;
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_OK:
                    MenuRunInfos->Timer_PageHold = PAGE_REMOTEKEYLEARN_TIME_MS / MENU_LOOP_TIME_MS;
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_LEARN_KEY_RIGHT_REMT:
            if (KeyCode_Temp != 0)
            {
                MySettings.Rkey_Right.KeyCode = KeyCode_Temp;
                MySettings.Rkey_Right.isLearned = 1;
                MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            }

            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);

                    return;

                case BT_KEYCODE_UP:
                case BT_KEYCODE_LEFT:
                    MenuRunInfos->Timer_PageHold = PAGE_REMOTEKEYLEARN_TIME_MS / MENU_LOOP_TIME_MS;
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_OK:
                    MenuRunInfos->Timer_PageHold = PAGE_REMOTEKEYLEARN_TIME_MS / MENU_LOOP_TIME_MS;
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_LEARN_KEY_OK_REMT:
            if (KeyCode_Temp != 0)
            {
                MySettings.Rkey_Ok.KeyCode = KeyCode_Temp;
                MySettings.Rkey_Ok.isLearned = 1;
                MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            }

            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);

                    return;

                case BT_KEYCODE_UP:
                case BT_KEYCODE_LEFT:
                    MenuRunInfos->Timer_PageHold = PAGE_REMOTEKEYLEARN_TIME_MS / MENU_LOOP_TIME_MS;
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_OK:
                    MenuRunInfos->Timer_PageHold = PAGE_REMOTEKEYLEARN_TIME_MS / MENU_LOOP_TIME_MS;
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_LEARN_KEY_VOICE_REMT:
            if (KeyCode_Temp != 0)
            {
                MySettings.Rkey_Voice.KeyCode = KeyCode_Temp;
                MySettings.Rkey_Voice.isLearned = 1;
                MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            }

            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);

                    return;

                case BT_KEYCODE_UP:
                case BT_KEYCODE_LEFT:
                    MenuRunInfos->Timer_PageHold = PAGE_REMOTEKEYLEARN_TIME_MS / MENU_LOOP_TIME_MS;
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_OK:
                    MenuRunInfos->Timer_PageHold = PAGE_REMOTEKEYLEARN_TIME_MS / MENU_LOOP_TIME_MS;
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        default:
            break;
    }

    if (MenuRunInfos->CursorMenu->Status == MENU_STATUS_HAS_UPATE)
    {
        MenuRunInfos->CursorMenu->Status = MENU_STATUS_NORMAL;
        Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
        Screen_Printf (16, 0, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%d", MenuRunInfos->Timer_PageHold * MENU_LOOP_TIME_MS / 1000);

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_LEARN_KEY_MENU_REMT)
        {
            Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "请按菜单");

            if (MySettings.Rkey_Menu.isLearned)
            {
                if (KeyCode_Temp == 0)
                {
                    Screen_Printf (16, 16, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%6X", MySettings.Rkey_Menu.KeyCode);
                }
                else
                {
                    Screen_Printf (16, 16, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%6X", MySettings.Rkey_Menu.KeyCode);
                }
            }
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_LEARN_KEY_UP_REMT)
        {
            Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "请按上键");

            if (MySettings.Rkey_Up.isLearned)
            {
                if (KeyCode_Temp == 0)
                {
                    Screen_Printf (16, 16, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%6X", MySettings.Rkey_Up.KeyCode);
                }
                else
                {
                    Screen_Printf (16, 16, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%6X", MySettings.Rkey_Up.KeyCode);
                }
            }
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_LEARN_KEY_DOWN_REMT)
        {
            Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "请按下键");

            if (MySettings.Rkey_Down.isLearned)
            {
                if (KeyCode_Temp == 0)
                {
                    Screen_Printf (16, 16, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%6X", MySettings.Rkey_Down.KeyCode);
                }
                else
                {
                    Screen_Printf (16, 16, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%6X", MySettings.Rkey_Down.KeyCode);
                }
            }
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_LEARN_KEY_LEFT_REMT)
        {
            Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "请按左键");

            if (MySettings.Rkey_Left.isLearned)
            {
                if (KeyCode_Temp == 0)
                {
                    Screen_Printf (16, 16, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%6X", MySettings.Rkey_Left.KeyCode);
                }
                else
                {
                    Screen_Printf (16, 16, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%6X", MySettings.Rkey_Left.KeyCode);
                }
            }
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_LEARN_KEY_RIGHT_REMT)
        {
            Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "请按右键");

            if (MySettings.Rkey_Right.isLearned)
            {
                if (KeyCode_Temp == 0)
                {
                    Screen_Printf (16, 16, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%6X", MySettings.Rkey_Right.KeyCode);
                }
                else
                {
                    Screen_Printf (16, 16, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%6X", MySettings.Rkey_Right.KeyCode);
                }
            }
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_LEARN_KEY_OK_REMT)
        {
            Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "请按确认");

            if (MySettings.Rkey_Ok.isLearned)
            {
                if (KeyCode_Temp == 0)
                {
                    Screen_Printf (16, 16, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%6X", MySettings.Rkey_Ok.KeyCode);
                }
                else
                {
                    Screen_Printf (16, 16, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%6X", MySettings.Rkey_Ok.KeyCode);
                }
            }
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_LEARN_KEY_VOICE_REMT)
        {
            Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "请按报时");

            if (MySettings.Rkey_Voice.isLearned)
            {
                if (KeyCode_Temp == 0)
                {
                    Screen_Printf (16, 16, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%6X", MySettings.Rkey_Voice.KeyCode);
                }
                else
                {
                    Screen_Printf (16, 16, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%6X", MySettings.Rkey_Voice.KeyCode);
                }
            }
        }

        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
    }

    MenuRunInfos->Timer_PageHold--;

    if (MenuRunInfos->Timer_PageHold == 0)
    {
        //NOTE:SLF 2018-05-28 倒计时时间到，进入到下一个学习页面或者退出学习界面
        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_END)
        {
            MySettings.RemoteKeyEnable = 1;
            MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
            MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
            MenuIf_ListShow (MenuRunInfos);
            return;
        }
        else
        {
            Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
            MenuRunInfos->Timer_PageHold = PAGE_REMOTEKEYLEARN_TIME_MS / MENU_LOOP_TIME_MS;
        }
    }

#undef PAGE_ITEM_FIRST
#undef PAGE_ITEM_LEARN_KEY_MENU_REMT
#undef PAGE_ITEM_LEARN_KEY_UP_REMT
#undef PAGE_ITEM_LEARN_KEY_DOWN_REMT
#undef PAGE_ITEM_LEARN_KEY_LEFT_REMT
#undef PAGE_ITEM_LEARN_KEY_RIGHT_REMT
#undef PAGE_ITEM_LEARN_KEY_OK_REMT
#undef PAGE_ITEM_LEARN_KEY_VOICE_REMT
#undef PAGE_ITEM_END

#undef PAGE_REMOTEKEYLEARN_TIME_MS
}
void Page_SetVolume (MenuRunInfo* MenuRunInfos)
{
#define PAGE_ITEM_VOLUME    1

    if (MenuRunInfos->CursorMenu->Mode  == MENU_MODE_QUICKVIEW)
    {
        return;
    }
    else
    {

    }

    switch (MenuRunInfos->CursorMenu->CurrentItem)
    {
        case MENU_PAGE_ITEM_DEFAULT:
            MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_VOLUME;

            if (MySettings.Volume > 10)
            {
                MySettings.Volume = 10;
            }

            MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            break;

        case PAGE_ITEM_VOLUME:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);

                    return;

                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    if (MySettings.Volume < 10)
                    {
                        MySettings.Volume++;
                        //FIXME:SLF 2018-05-30 调整音量时播放试听音
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    if (MySettings.Volume > 0)
                    {
                        MySettings.Volume--;
                        //FIXME:SLF 2018-05-30 调整音量时播放试听音
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:

                    break;

                default:
                    break;
            }

            break;
    }

    if (MenuRunInfos->CursorMenu->Status == MENU_STATUS_HAS_UPATE)
    {
        Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "音量调整");
        Screen_Printf (16, 0, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "1-10");

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_VOLUME)
        {
            MenuRunInfos->CursorMenu->Status = MENU_STATUS_NORMAL;
            Screen_Clear (16, 40, 24, 16, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL);
            Screen_Printf (16, 40, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%d", MySettings.Volume);

        }

        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
    }

#undef PAGE_ITEM_VOLUME
}
void Page_SetTimeCalibration (MenuRunInfo* MenuRunInfos)
{
#define PAGE_ITEM_TIME_CALIBRATION    3

    if (MenuRunInfos->CursorMenu->Mode  == MENU_MODE_QUICKVIEW)
    {
        return;
    }
    else
    {

    }

    switch (MenuRunInfos->CursorMenu->CurrentItem)
    {
        case MENU_PAGE_ITEM_DEFAULT:
            MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_TIME_CALIBRATION;
            MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            break;

        case PAGE_ITEM_TIME_CALIBRATION:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);

                    return;

                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    if (MySettings.TimeCorrection < 127)
                    {
                        MySettings.TimeCorrection++;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    if (MySettings.TimeCorrection > -127)
                    {
                        MySettings.TimeCorrection--;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:

                    break;

                default:
                    break;
            }

            if (MenuRunInfos->CursorMenu->Status == MENU_STATUS_HAS_UPATE)
            {
                if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_TIME_CALIBRATION)
                {
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_NORMAL;
                    Screen_Clear (16, 0, 64, 16, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL);
                    Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "每周");

                    if (MySettings.TimeCorrection > 0)
                    {
                        Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "快%d秒", MySettings.TimeCorrection);
                    }
                    else if (MySettings.TimeCorrection < 0)
                    {
                        Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "慢%d秒", 0 - MySettings.TimeCorrection);
                    }
                    else
                    {
                        Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "不校准");
                    }

                    Screen_UpdateImmediately (SCREEN_COLOR_ALL);
                }
            }

            break;
    }

#undef PAGE_ITEM_TIME_CALIBRATION
}
void Page_SetBrightnessMode (MenuRunInfo* MenuRunInfos)
{
#define PAGE_ITEM_FIRST 1
#define PAGE_ITEM_BRIGHTNESS_MODE    1
#define PAGE_ITEM_BRIGHTNESS_FIX    2
#define PAGE_ITEM_END   2

    if (MenuRunInfos->CursorMenu->Mode  == MENU_MODE_QUICKVIEW)
    {
        return;
    }
    else
    {

    }

    switch (MenuRunInfos->CursorMenu->CurrentItem)
    {
        case MENU_PAGE_ITEM_DEFAULT:
            MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_BRIGHTNESS_MODE;
            MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            break;

        case PAGE_ITEM_BRIGHTNESS_MODE:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);

                    return;

                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    if (MySettings.Brightness.isAutoCtr)
                    {
                        MySettings.Brightness.isAutoCtr = 0;
                    }
                    else
                    {
                        MySettings.Brightness.isAutoCtr = 1;

                    }

                    Bsp_Screen_Set_Brightness_Immediately (SCREEN_INDEX_TOP | SCREEN_INDEX_BOTTOM, Brightness_GetMaxValue());
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    if (!MySettings.Brightness.isAutoCtr)
                    {
                        Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    }

                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_BRIGHTNESS_FIX:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);

                    return;

                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    MySettings.Brightness.BrightnessFix = MySettings.Brightness.BrightnessFix - MySettings.Brightness.BrightnessFix % 100;

                    if (MySettings.Brightness.BrightnessFix / 10 < 100)
                    {
                        MySettings.Brightness.BrightnessFix += 10;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    MySettings.Brightness.BrightnessFix = MySettings.Brightness.BrightnessFix - MySettings.Brightness.BrightnessFix % 100;

                    if (MySettings.Brightness.BrightnessFix / 10 > 1)
                    {
                        MySettings.Brightness.BrightnessFix -= 10;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

    }

    if (MenuRunInfos->CursorMenu->Status == MENU_STATUS_HAS_UPATE)
    {
        MenuRunInfos->CursorMenu->Status = MENU_STATUS_NORMAL;
        Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, MENU_STATUS_NORMAL);

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_BRIGHTNESS_MODE)
        {
            if (MySettings.Brightness.isAutoCtr)
            {

                Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "自动亮度");
            }
            else
            {
                Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "固定亮度");
                Screen_Printf (16, 0, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%3d/100", MySettings.Brightness.BrightnessFix / 10);
            }
        }
        else
        {
            if (!MySettings.Brightness.isAutoCtr)
            {
                Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "固定亮度");
            }
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_BRIGHTNESS_FIX)
        {
            Screen_Printf (16, 0, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_IMMEDIATELY, "%3d/100", MySettings.Brightness.BrightnessFix / 10);
            Bsp_Screen_Set_Brightness_Immediately (SCREEN_INDEX_TOP | SCREEN_INDEX_BOTTOM, MySettings.Brightness.BrightnessFix);
        }

        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
    }

#undef PAGE_ITEM_FIRST
#undef PAGE_ITEM_BRIGHTNESS_MODE
#undef PAGE_ITEM_BRIGHTNESS_FIX
#undef PAGE_ITEM_END
}
void Page_SetBrightnessCelibration (MenuRunInfo* MenuRunInfos)
{
#define PAGE_ITEM_FIRST 1
#define PAGE_ITEM_MAX   1
#define PAGE_ITEM_MID   2
#define PAGE_ITEM_MIN   3
#define PAGE_ITEM_END 3

    if (MenuRunInfos->CursorMenu->Mode  == MENU_MODE_QUICKVIEW)
    {
        return;
    }
    else
    {

    }

    switch (MenuRunInfos->CursorMenu->CurrentItem)
    {
        case MENU_PAGE_ITEM_DEFAULT:

            MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_MAX;
            MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            break;

        case PAGE_ITEM_MAX:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    if (MySettings.Brightness.BrightnessMax < 1000)
                    {
                        MySettings.Brightness.BrightnessMax++;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }
                    else
                    {
                        MySettings.Brightness.BrightnessMax = 1000;
                    }

                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    if ((MySettings.Brightness.BrightnessMax > 1) && (MySettings.Brightness.BrightnessMax > MySettings.Brightness.BrightnessMid))
                    {
                        MySettings.Brightness.BrightnessMax--;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_MID:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    if (MySettings.Brightness.BrightnessMid < MySettings.Brightness.BrightnessMax)
                    {
                        MySettings.Brightness.BrightnessMid++;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }
                    else
                    {
                        MySettings.Brightness.BrightnessMid = MySettings.Brightness.BrightnessMax;
                    }

                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    if (MySettings.Brightness.BrightnessMid > MySettings.Brightness.BrightnessMin)
                    {
                        MySettings.Brightness.BrightnessMid--;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_MIN:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    if (MySettings.Brightness.BrightnessMin < MySettings.Brightness.BrightnessMid)
                    {
                        MySettings.Brightness.BrightnessMin++;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }
                    else
                    {
                        MySettings.Brightness.BrightnessMin = MySettings.Brightness.BrightnessMid;
                    }

                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    if (MySettings.Brightness.BrightnessMin > 0)
                    {
                        MySettings.Brightness.BrightnessMin--;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        default:
            break;
    }

    if (MenuRunInfos->CursorMenu->Status == MENU_STATUS_HAS_UPATE)
    {
        MenuRunInfos->CursorMenu->Status = MENU_STATUS_NORMAL;
        Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_MAX)
        {
            Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "高");
            Screen_Printf (0, 16, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%d", MySettings.Brightness.BrightnessMax);
        }
        else
        {
            Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "高");
            Screen_Printf (0, 16, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%d", MySettings.Brightness.BrightnessMax);
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_MID)
        {
            Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "中 ");
            Screen_Printf (16, 16, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%d ", MySettings.Brightness.BrightnessMid);
        }
        else
        {
            Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "中 ");
            Screen_Printf (16, 16, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%d ", MySettings.Brightness.BrightnessMid);
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_MIN)
        {
            Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
            Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "低");
            Screen_Printf (0, 16, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%d", MySettings.Brightness.BrightnessMin);
        }

        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
    }

#undef PAGE_ITEM_FIRST
#undef PAGE_ITEM_END
#undef PAGE_ITEM_MAX
#undef PAGE_ITEM_MID
#undef PAGE_ITEM_MIN
}
void Page_SetBrightness_Sensitivity (MenuRunInfo* MenuRunInfos)
{
#define PAGE_ITEM_FIRST 1
#define PAGE_ITEM_BRIGHTNESS_SENSITIVITY    1
#define PAGE_ITEM_END 1


    if (MenuRunInfos->CursorMenu->Mode  == MENU_MODE_QUICKVIEW)
    {
        return;
    }
    else
    {

    }

    switch (MenuRunInfos->CursorMenu->CurrentItem)
    {
        case MENU_PAGE_ITEM_DEFAULT:
            MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_BRIGHTNESS_SENSITIVITY;

            if (MySettings.Brightness.Sensitivity > 10)
            {
                MySettings.Brightness.Sensitivity = 10;
            }

            MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            break;

        case PAGE_ITEM_BRIGHTNESS_SENSITIVITY:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);

                    return;

                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    if (MySettings.Brightness.Sensitivity < 10)
                    {
                        MySettings.Brightness.Sensitivity++;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    if (MySettings.Brightness.Sensitivity > 0)
                    {
                        MySettings.Brightness.Sensitivity--;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:

                    break;

                default:
                    break;
            }

            break;
    }

    if (MenuRunInfos->CursorMenu->Status == MENU_STATUS_HAS_UPATE)
    {
        Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "灵敏度");
        Screen_Printf (16, 0, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "1-10");

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_BRIGHTNESS_SENSITIVITY)
        {
            MenuRunInfos->CursorMenu->Status = MENU_STATUS_NORMAL;
            Screen_Clear (16, 40, 24, 16, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL);
            Screen_Printf (16, 40, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%d", MySettings.Brightness.Sensitivity);

        }

        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
    }

#undef PAGE_ITEM_FIRST
#undef PAGE_ITEM_BRIGHTNESS_SENSITIVITY
#undef PAGE_ITEM_END
}
void Page_SetVoiceMode (MenuRunInfo* MenuRunInfos)
{
#define PAGE_ITEM_MODE  1

    if (MenuRunInfos->CursorMenu->Mode  == MENU_MODE_QUICKVIEW)
    {
        return;
    }
    else
    {

    }

    switch (MenuRunInfos->CursorMenu->CurrentItem)
    {
        case MENU_PAGE_ITEM_DEFAULT:

            MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_MODE;
            MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            break;

        case PAGE_ITEM_MODE:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    if (MySettings.VoiceBroadcast.Mode < 2)
                    {
                        MySettings.VoiceBroadcast.Mode++;
                    }
                    else
                    {
                        MySettings.VoiceBroadcast.Mode = 2;
                    }

                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    if (MySettings.VoiceBroadcast.Mode > 0)
                    {
                        MySettings.VoiceBroadcast.Mode--;
                    }
                    else
                    {
                        MySettings.VoiceBroadcast.Mode = 0;
                    }

                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    break;

                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    break;

                default:
                    break;
            }

            break;

        default:
            break;
    }

    if (MenuRunInfos->CursorMenu->Status == MENU_STATUS_HAS_UPATE)
    {
        MenuRunInfos->CursorMenu->Status = MENU_STATUS_NORMAL;
        Screen_Clear (16, 0, 64, 16, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL);
        Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "播报模式");

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_MODE)
        {
            switch (MySettings.VoiceBroadcast.Mode)
            {
                case 0:
                    Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "详细模式");
                    break;

                case 1:
                    Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "简洁模式");
                    break;

                case 2:
                    Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "关闭报时");
                    break;

                default:
                    break;
            }
        }

        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
    }

#undef PAGE_ITEM_MODE
}
void Page_SetVoiceStartEnd (MenuRunInfo* MenuRunInfos)
{
#define PAGE_ITEM_FIRST 1
#define PAGE_ITEM_START_TIME   1
#define PAGE_ITEM_END_TIME   2
#define PAGE_ITEM_END 2

    if (MenuRunInfos->CursorMenu->Mode  == MENU_MODE_QUICKVIEW)
    {
        return;
    }
    else
    {

    }

    switch (MenuRunInfos->CursorMenu->CurrentItem)
    {
        case MENU_PAGE_ITEM_DEFAULT:

            MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_START_TIME;
            MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            break;

        case PAGE_ITEM_START_TIME:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:

                    //                    if ((MySettings.VoiceBroadcast.StartTime < 23) && (MySettings.VoiceBroadcast.StartTime < MySettings.VoiceBroadcast.EndTime))
                    if (MySettings.VoiceBroadcast.StartTime < 23)
                    {
                        MySettings.VoiceBroadcast.StartTime++;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    if (MySettings.VoiceBroadcast.StartTime > 0)
                    {
                        MySettings.VoiceBroadcast.StartTime--;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_END_TIME:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    if (MySettings.VoiceBroadcast.EndTime < 23)
                    {
                        MySettings.VoiceBroadcast.EndTime++;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:

                    //                    if ((MySettings.VoiceBroadcast.EndTime > 0) && (MySettings.VoiceBroadcast.StartTime < MySettings.VoiceBroadcast.EndTime))
                    if (MySettings.VoiceBroadcast.EndTime > 0)
                    {
                        MySettings.VoiceBroadcast.EndTime--;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        default:
            break;
    }

    if (MenuRunInfos->CursorMenu->Status == MENU_STATUS_HAS_UPATE)
    {
        MenuRunInfos->CursorMenu->Status = MENU_STATUS_NORMAL;
        Screen_Clear (0, 0, 16, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
        Screen_Printf (0, 16, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "点开始");
        Screen_Printf (16, 16, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "点结束");

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_START_TIME)
        {
            Screen_Printf (0, 0, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%2d", MySettings.VoiceBroadcast.StartTime);
            Screen_Printf (16, 0, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%2d ", MySettings.VoiceBroadcast.EndTime);
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_END_TIME)
        {
            Screen_Printf (0, 0, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%2d", MySettings.VoiceBroadcast.StartTime);
            Screen_Printf (16, 0, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%2d ", MySettings.VoiceBroadcast.EndTime);
        }

        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
    }

#undef PAGE_ITEM_FIRST
#undef PAGE_ITEM_START_TIME
#undef PAGE_ITEM_END_TIME
#undef PAGE_ITEM_END
}
void Page_SetRemoteKeyEnable (MenuRunInfo* MenuRunInfos)
{
#define PAGE_ITEM_REMOTEKEY_ENABLE 1

    if (MenuRunInfos->CursorMenu->Mode  == MENU_MODE_QUICKVIEW)
    {
        return;
    }
    else
    {

    }

    switch (MenuRunInfos->CursorMenu->CurrentItem)
    {
        case MENU_PAGE_ITEM_DEFAULT:

            MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_REMOTEKEY_ENABLE;
            MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            break;

        case PAGE_ITEM_REMOTEKEY_ENABLE:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    MySettings.RemoteKeyEnable = !MySettings.RemoteKeyEnable;
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    break;

                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    break;

                default:
                    break;
            }



            break;

        default:
            break;
    }

    if (MenuRunInfos->CursorMenu->Status == MENU_STATUS_HAS_UPATE)
    {
        Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "遥控开关");
        Screen_Printf (16, 32, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "遥控");

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_REMOTEKEY_ENABLE)
        {
            MenuRunInfos->CursorMenu->Status = MENU_STATUS_NORMAL;
            Screen_Clear (16, 0, 64, 16, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL);

            switch (MySettings.RemoteKeyEnable)
            {
                case 0:
                    Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "关闭");
                    break;

                case 1:
                    Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "使用");
                    break;

                default:
                    break;
            }
        }

        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
    }

#undef PAGE_ITEM_REMOTEKEY_ENABLE
}
void Page_SetAlarmClock (MenuRunInfo* MenuRunInfos)
{
#define PAGE_ITEM_FIRST 1
#define PAGE_ITEM_ENABLE   1
#define PAGE_ITEM_HOUR   2
#define PAGE_ITEM_MIN   3
#define PAGE_ITEM_MON   4
#define PAGE_ITEM_TUE   5
#define PAGE_ITEM_WEN   6
#define PAGE_ITEM_THU   7
#define PAGE_ITEM_FRI   8
#define PAGE_ITEM_SAT   9
#define PAGE_ITEM_SUN   10
#define PAGE_ITEM_MUSIC   11
#define PAGE_ITEM_END 11
    Alarm_Type* Alarm;

    Alarm = &MySettings.Alarms[MenuRunInfos->CursorMenu->ID % 10];

    if (MenuRunInfos->CursorMenu->Mode == MENU_MODE_QUICKVIEW)
    {
        if (Alarm->Enable)
        {
            MenuRunInfos->CursorMenu->Message_String = "开";
        }
        else
        {
            MenuRunInfos->CursorMenu->Message_String = NULL_MENU;
        }

        return;
    }

    switch (MenuRunInfos->CursorMenu->CurrentItem)
    {
        case MENU_PAGE_ITEM_DEFAULT:

            MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_ENABLE;
            MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            break;

        case PAGE_ITEM_ENABLE:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    Alarm->Enable = !Alarm->Enable;
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_HOUR:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    if (Alarm->Hour < 23)
                    {
                        Alarm->Hour++;
                    }
                    else
                    {
                        Alarm->Hour = 23;
                    }

                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    if (Alarm->Hour > 0)
                    {
                        Alarm->Hour--;
                    }
                    else
                    {
                        Alarm->Hour = 0;
                    }

                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_MIN:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    if (Alarm->Minitue < 59)
                    {
                        Alarm->Minitue++;
                    }
                    else
                    {
                        Alarm->Minitue = 59;
                    }

                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    if (Alarm->Minitue > 0)
                    {
                        Alarm->Minitue--;
                    }
                    else
                    {
                        Alarm->Minitue = 0;
                    }

                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_MON:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Alarm->isMonday_Checked = !Alarm->isMonday_Checked;
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_TUE:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Alarm->isTuesday_Checked = !Alarm->isTuesday_Checked;
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_WEN:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Alarm->isWednesday_Checked = !Alarm->isWednesday_Checked;
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_THU:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Alarm->isThursday_Checked = !Alarm->isThursday_Checked;
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_FRI:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Alarm->isFriday_Checked = !Alarm->isFriday_Checked;
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_SAT:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Alarm->isSaturday_Checked = !Alarm->isSaturday_Checked;
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_SUN:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Alarm->isSunday_Checked = !Alarm->isSunday_Checked;
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_MUSIC:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    if (Alarm->Music > 0)
                    {
                        Alarm->Music--;
                    }

                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    if (Alarm->Music < 2)
                    {
                        Alarm->Music++;
                    }

                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    RMP_Thd_Snd (&ThID_Task_oClock_Caculater,  ((0 << 29) | Alarm->Music), 0);
                    break;

                default:
                    break;
            }

            break;

        default:
            break;
    }

    if (MenuRunInfos->CursorMenu->Status == MENU_STATUS_HAS_UPATE)
    {
        MenuRunInfos->CursorMenu->Status = MENU_STATUS_NORMAL;
        Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_ENABLE)
        {
            if (Alarm->Enable)
            {
                Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "开");
                MenuRunInfos->CursorMenu->Message_String = "开";
            }
            else
            {
                Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "关");
                MenuRunInfos->CursorMenu->Message_String = NULL_MENU;
            }
        }
        else
        {
            if (Alarm->Enable)
            {
                Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "开");
            }
            else
            {
                Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "关");
            }
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_HOUR)
        {
            Screen_Printf (0, 16, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%2d", Alarm->Hour);
        }
        else
        {
            Screen_Printf (0, 16, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%2d", Alarm->Hour);
        }

        Screen_Printf (0, 16 + 16, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, ":");

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_MIN)
        {
            Screen_Printf (0, 16 + 16 + 8, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%2d", Alarm->Minitue);
        }
        else
        {
            Screen_Printf (0, 16 + 16 + 8, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%2d", Alarm->Minitue);
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_MON)
        {
            if (Alarm->isMonday_Checked)
            {
                Screen_Printf (16, 0, FontsType_DefaultMix, SCREEN_COLOR_BLACK_BACK_GREEN, SCREEN_UPDATE_NORMAL, "1");
            }
            else
            {
                Screen_Printf (16, 0, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "1");
            }
        }
        else
        {
            if (Alarm->isMonday_Checked)
            {
                Screen_Printf (16, 0, FontsType_DefaultMix, SCREEN_COLOR_BLACK_BACK_RED, SCREEN_UPDATE_NORMAL, "1");
            }
            else
            {
                Screen_Printf (16, 0, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "1");
            }
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_TUE)
        {
            if (Alarm->isTuesday_Checked)
            {
                Screen_Printf (16, 8, FontsType_DefaultMix, SCREEN_COLOR_BLACK_BACK_GREEN, SCREEN_UPDATE_NORMAL, "2");
            }
            else
            {
                Screen_Printf (16, 8, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "2");
            }
        }
        else
        {
            if (Alarm->isTuesday_Checked)
            {
                Screen_Printf (16, 8, FontsType_DefaultMix, SCREEN_COLOR_BLACK_BACK_RED, SCREEN_UPDATE_NORMAL, "2");
            }
            else
            {
                Screen_Printf (16, 8, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "2");
            }
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_WEN)
        {
            if (Alarm->isWednesday_Checked)
            {
                Screen_Printf (16, 16, FontsType_DefaultMix, SCREEN_COLOR_BLACK_BACK_GREEN, SCREEN_UPDATE_NORMAL, "3");
            }
            else
            {
                Screen_Printf (16, 16, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "3");
            }
        }
        else
        {
            if (Alarm->isWednesday_Checked)
            {
                Screen_Printf (16, 16, FontsType_DefaultMix, SCREEN_COLOR_BLACK_BACK_RED, SCREEN_UPDATE_NORMAL, "3");
            }
            else
            {
                Screen_Printf (16, 16, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "3");
            }
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_THU)
        {
            if (Alarm->isThursday_Checked)
            {
                Screen_Printf (16, 24, FontsType_DefaultMix, SCREEN_COLOR_BLACK_BACK_GREEN, SCREEN_UPDATE_NORMAL, "4");
            }
            else
            {
                Screen_Printf (16, 24, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "4");
            }
        }
        else
        {
            if (Alarm->isThursday_Checked)
            {
                Screen_Printf (16, 24, FontsType_DefaultMix, SCREEN_COLOR_BLACK_BACK_RED, SCREEN_UPDATE_NORMAL, "4");
            }
            else
            {
                Screen_Printf (16, 24, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "4");
            }
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_FRI)
        {
            if (Alarm->isFriday_Checked)
            {
                Screen_Printf (16, 32, FontsType_DefaultMix, SCREEN_COLOR_BLACK_BACK_GREEN, SCREEN_UPDATE_NORMAL, "5");
            }
            else
            {
                Screen_Printf (16, 32, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "5");
            }
        }
        else
        {
            if (Alarm->isFriday_Checked)
            {
                Screen_Printf (16, 32, FontsType_DefaultMix, SCREEN_COLOR_BLACK_BACK_RED, SCREEN_UPDATE_NORMAL, "5");
            }
            else
            {
                Screen_Printf (16, 32, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "5");
            }
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_SAT)
        {
            if (Alarm->isSaturday_Checked)
            {
                Screen_Printf (16, 40, FontsType_DefaultMix, SCREEN_COLOR_BLACK_BACK_GREEN, SCREEN_UPDATE_NORMAL, "6");
            }
            else
            {
                Screen_Printf (16, 40, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "6");
            }
        }
        else
        {
            if (Alarm->isSaturday_Checked)
            {
                Screen_Printf (16, 40, FontsType_DefaultMix, SCREEN_COLOR_BLACK_BACK_RED, SCREEN_UPDATE_NORMAL, "6");
            }
            else
            {
                Screen_Printf (16, 40, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "6");
            }
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_SUN)
        {
            if (Alarm->isSunday_Checked)
            {
                Screen_Printf (16, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_BLACK_BACK_GREEN, SCREEN_UPDATE_NORMAL, "日");
            }
            else
            {
                Screen_Printf (16, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "日");
            }
        }
        else
        {
            if (Alarm->isSunday_Checked)
            {
                Screen_Printf (16, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_BLACK_BACK_RED, SCREEN_UPDATE_NORMAL, "日");
            }
            else
            {
                Screen_Printf (16, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "日");
            }
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_MUSIC)
        {
            Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);

            switch (Alarm->Music)
            {
                case 0:
                    Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "音效一");
                    Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "音效二");
                    break;

                case 1:
                    Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "音效一");
                    Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "音效二");
                    break;

                case 2:
                    Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "音效三");
                    break;

                default:
                    break;
            }

            //FIXME:SLF 2018-06-05 播放该音效
        }

        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
    }

#undef PAGE_ITEM_FIRST
#undef PAGE_ITEM_END
#undef PAGE_ITEM_MAX
#undef PAGE_ITEM_MID
#undef PAGE_ITEM_MIN
}
void Page_SetFontColor (MenuRunInfo* MenuRunInfos)
{
#define PAGE_ITEM_FIRST 1
#define PAGE_ITEM_CLOCK_COLOR 1
#define PAGE_ITEM_OTHER_COLOR 2
#define PAGE_ITEM_END  2

    if (MenuRunInfos->CursorMenu->Mode  == MENU_MODE_QUICKVIEW)
    {
        return;
    }
    else
    {

    }

    switch (MenuRunInfos->CursorMenu->CurrentItem)
    {
        case MENU_PAGE_ITEM_DEFAULT:

            MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_CLOCK_COLOR;
            MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            break;

        case PAGE_ITEM_CLOCK_COLOR:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    if (MySettings.FontCfg.FontColor_Clock > SCREEN_COLOR_MIN)
                    {
                        MySettings.FontCfg.FontColor_Clock--;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    if (MySettings.FontCfg.FontColor_Clock < SCREEN_COLOR_MAX)
                    {
                        MySettings.FontCfg.FontColor_Clock++;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_OTHER_COLOR:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    if (MySettings.FontCfg.FontColor_Menu > SCREEN_COLOR_MIN)
                    {
                        MySettings.FontCfg.FontColor_Menu--;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    if (MySettings.FontCfg.FontColor_Menu < SCREEN_COLOR_MAX)
                    {
                        MySettings.FontCfg.FontColor_Menu++;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        default:
            break;
    }

    if (MenuRunInfos->CursorMenu->Status == MENU_STATUS_HAS_UPATE)
    {
        MenuRunInfos->CursorMenu->Status = MENU_STATUS_NORMAL;
        Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_CLOCK_COLOR)
        {
            Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, MySettings.FontCfg.FontColor_Clock, SCREEN_UPDATE_NORMAL, "时钟颜色");

            switch (MySettings.FontCfg.FontColor_Clock)
            {
                case SCREEN_COLOR_RED:
                    Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, MENU_STATUS_NORMAL, "红色");
                    break;

                case SCREEN_COLOR_GREEN:
                    Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, MENU_STATUS_NORMAL, "绿色");
                    break;

                case SCREEN_COLOR_YELLOW:
                    Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, MENU_STATUS_NORMAL, "黄色");
                    break;

                case SCREEN_COLOR_BLACK_BACK_RED:
                    Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, MENU_STATUS_NORMAL, "黑字红底");
                    break;

                case SCREEN_COLOR_BLACK_BACK_GREEN:
                    Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, MENU_STATUS_NORMAL, "黑字绿底");
                    break;

                case SCREEN_COLOR_BLACK_BACK_YELLOW:
                    Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, MENU_STATUS_NORMAL, "黑字黄底");

                    break;

                default:
                    break;
            }
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_OTHER_COLOR)
        {

            Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, MySettings.FontCfg.FontColor_Menu, SCREEN_UPDATE_NORMAL, "信息颜色");

            switch (MySettings.FontCfg.FontColor_Menu)
            {
                case SCREEN_COLOR_RED:
                    Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, MENU_STATUS_NORMAL, "红色");
                    break;

                case SCREEN_COLOR_GREEN:
                    Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, MENU_STATUS_NORMAL, "绿色");
                    break;

                case SCREEN_COLOR_YELLOW:
                    Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, MENU_STATUS_NORMAL, "黄色");
                    break;

                case SCREEN_COLOR_BLACK_BACK_RED:
                    Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, MENU_STATUS_NORMAL, "黑字红底");
                    break;

                case SCREEN_COLOR_BLACK_BACK_GREEN:
                    Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, MENU_STATUS_NORMAL, "黑字绿底");
                    break;

                case SCREEN_COLOR_BLACK_BACK_YELLOW:
                    Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, MENU_STATUS_NORMAL, "黑字黄底");

                    break;

                default:
                    break;
            }
        }

        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
    }

#undef PAGE_ITEM_FIRST
#undef PAGE_ITEM_CLOCK_COLOR
#undef PAGE_ITEM_OTHER_COLOR
#undef PAGE_ITEM_END
}
void Page_SetFontType (MenuRunInfo* MenuRunInfos)
{
#define PAGE_ITEM_FIRST 1
#define PAGE_ITEM_CLOCK_FONT_TYPE 1
#define PAGE_ITEM_MENU_FONT_TYPE 2
#define PAGE_ITEM_INFO_NUM_FONT_TYPE 3
#define PAGE_ITEM_INFO_CN_FONT_TYPE 4
#define PAGE_ITEM_END  4

    if (MenuRunInfos->CursorMenu->Mode  == MENU_MODE_QUICKVIEW)
    {
        return;
    }
    else
    {

    }

    switch (MenuRunInfos->CursorMenu->CurrentItem)
    {
        case MENU_PAGE_ITEM_DEFAULT:

            MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_CLOCK_FONT_TYPE;
            MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            break;

        case PAGE_ITEM_CLOCK_FONT_TYPE:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    if (MySettings.FontCfg.FontType_Clock > FontsType_Start + 1)
                    {
                        MySettings.FontCfg.FontType_Clock--;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    if (MySettings.FontCfg.FontType_Clock < FontsType_DefaultMix)
                    {
                        MySettings.FontCfg.FontType_Clock++;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_MENU_FONT_TYPE:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    if (MySettings.FontCfg.FontType_Menu > FontsType_DefaultMix)
                    {
                        MySettings.FontCfg.FontType_Menu--;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    if (MySettings.FontCfg.FontType_Menu < FontsType_End - 1)
                    {
                        MySettings.FontCfg.FontType_Menu++;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_INFO_NUM_FONT_TYPE:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    if (MySettings.FontCfg.FontType_Info_Num > FontsType_Start + 1)
                    {
                        MySettings.FontCfg.FontType_Info_Num--;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    if (MySettings.FontCfg.FontType_Info_Num < FontsType_DefaultMix)
                    {
                        MySettings.FontCfg.FontType_Info_Num++;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_INFO_CN_FONT_TYPE:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    if (MySettings.FontCfg.FontType_Info_CN > FontsType_DefaultMix)
                    {
                        MySettings.FontCfg.FontType_Info_CN--;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    if (MySettings.FontCfg.FontType_Info_CN < FontsType_End - 1)
                    {
                        MySettings.FontCfg.FontType_Info_CN++;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        default:
            break;
    }

    if (MenuRunInfos->CursorMenu->Status == MENU_STATUS_HAS_UPATE)
    {
        MenuRunInfos->CursorMenu->Status = MENU_STATUS_NORMAL;
        Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_CLOCK_FONT_TYPE)
        {

            Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "时钟字体");
            Screen_Printf (16, 0, MySettings.FontCfg.FontType_Clock, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "11");
            Screen_DisplayChar (16, 16, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, ":");
            Screen_Printf (16, 16 + 8, MySettings.FontCfg.FontType_Clock, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "22");
            Screen_DisplayChar (16, 40, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, ":");
            Screen_Printf (16, 40 + 8, MySettings.FontCfg.FontType_Clock, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "33");
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_MENU_FONT_TYPE)
        {

            Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "菜单字体");
            Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "字体测试");
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_INFO_NUM_FONT_TYPE)
        {

            Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "信息数字");
            Screen_Printf (16, 0, MySettings.FontCfg.FontType_Info_CN, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "  月  日");
            Screen_Printf (16, 0, MySettings.FontCfg.FontType_Info_Num, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "12");
            Screen_Printf (16, 32, MySettings.FontCfg.FontType_Info_Num, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "18");
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_INFO_CN_FONT_TYPE)
        {

            Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "信息字体");
            Screen_Printf (16, 0, MySettings.FontCfg.FontType_Info_CN, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "国庆节");
        }

        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
    }

#undef PAGE_ITEM_FIRST
#undef PAGE_ITEM_CLOCK_FONT_TYPE
#undef PAGE_ITEM_MENU_FONT_TYPE
#undef PAGE_ITEM_INFO_NUM_FONT_TYPE
#undef PAGE_ITEM_INFO_CN_FONT_TYPE
#undef PAGE_ITEM_END

}
void Page_DisplayTimerSet (MenuRunInfo* MenuRunInfos)
{
#define PAGE_ITEM_FIRST 1
#define PAGE_ITEM_CHANGE_SPEED   1
#define PAGE_ITEM_DISPLAY_HOLD_TIME   2
#define PAGE_ITEM_DISPLAY_OFF_TIME   3
#define PAGE_ITEM_END 3

    if (MenuRunInfos->CursorMenu->Mode  == MENU_MODE_QUICKVIEW)
    {
        return;
    }

    switch (MenuRunInfos->CursorMenu->CurrentItem)
    {
        case MENU_PAGE_ITEM_DEFAULT:

            MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_CHANGE_SPEED;
            MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            break;

        case PAGE_ITEM_CHANGE_SPEED:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                    MySettings.Display_Effect.Switch_Timer_ms += 100;
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    break;

                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    MySettings.Display_Effect.Switch_Timer_ms += 10;
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;

                    break;

                case BT_KEYCODE_DOWN_LONG:
                    if  (MySettings.Display_Effect.Switch_Timer_ms > 200)
                    {
                        MySettings.Display_Effect.Switch_Timer_ms -= 100;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    if  (MySettings.Display_Effect.Switch_Timer_ms > 200)
                    {
                        MySettings.Display_Effect.Switch_Timer_ms -= 10;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }
                    else
                    {
                        MySettings.Display_Effect.Hold_Timer_ms = 200;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_DISPLAY_HOLD_TIME:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                    MySettings.Display_Effect.Hold_Timer_ms += 100;
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    break;

                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    MySettings.Display_Effect.Hold_Timer_ms += 10;
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;

                    break;

                case BT_KEYCODE_DOWN_LONG:
                    if  (MySettings.Display_Effect.Hold_Timer_ms > 200)
                    {
                        MySettings.Display_Effect.Hold_Timer_ms -= 100;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    if  (MySettings.Display_Effect.Hold_Timer_ms > 200)
                    {
                        MySettings.Display_Effect.Hold_Timer_ms -= 10;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }
                    else
                    {
                        MySettings.Display_Effect.Hold_Timer_ms = 200;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_DISPLAY_OFF_TIME:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                    MySettings.Display_Effect.Off_Timer_ms += 100;
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    break;

                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    MySettings.Display_Effect.Off_Timer_ms += 10;
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;

                    break;

                case BT_KEYCODE_DOWN_LONG:
                    if  (MySettings.Display_Effect.Off_Timer_ms > 200)
                    {
                        MySettings.Display_Effect.Off_Timer_ms -= 100;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    if  (MySettings.Display_Effect.Off_Timer_ms > 200)
                    {
                        MySettings.Display_Effect.Off_Timer_ms -= 10;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }
                    else
                    {
                        MySettings.Display_Effect.Off_Timer_ms = 200;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        default:
            break;
    }


    if (MenuRunInfos->CursorMenu->Status == MENU_STATUS_HAS_UPATE)
    {
        MenuRunInfos->CursorMenu->Status = MENU_STATUS_NORMAL;
        Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_CHANGE_SPEED)
        {
            Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "变化速率");
            Screen_Printf (16, 0, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%dms", MySettings.Display_Effect.Switch_Timer_ms);
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_DISPLAY_HOLD_TIME)
        {
            Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "显示保持");
            Screen_Printf (16, 0, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%dms", MySettings.Display_Effect.Hold_Timer_ms);
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_DISPLAY_OFF_TIME)
        {
            Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "关闭保持");
            Screen_Printf (16, 0, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%dms", MySettings.Display_Effect.Off_Timer_ms);
        }

        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
    }

#undef PAGE_ITEM_FIRST
#undef PAGE_ITEM_CHANGE_SPEED
#undef PAGE_ITEM_DISPLAY_HOLD_TIME
#undef PAGE_ITEM_DISPLAY_OFF_TIME
#undef PAGE_ITEM_END
}
void Page_DisplayPageEnable (MenuRunInfo* MenuRunInfos)
{
#define PAGE_ITEM_FIRST 1
#define PAGE_ITEM_DATE   1
#define PAGE_ITEM_DATE_CN   2
#define PAGE_ITEM_TEMPRA   3
#define PAGE_ITEM_HUMIDITY 4
#define PAGE_ITEM_HOLIDAY  5
#define PAGE_ITEM_BIRTHDAY   6
#define PAGE_ITEM_END 6

    if (MenuRunInfos->CursorMenu->Mode  == MENU_MODE_QUICKVIEW)
    {
        return;
    }
    else
    {

    }

    switch (MenuRunInfos->CursorMenu->CurrentItem)
    {
        case MENU_PAGE_ITEM_DEFAULT:

            MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_DATE;
            MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            break;

        case PAGE_ITEM_DATE:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    MySettings.Display_Effect.Page_Date_En = !MySettings.Display_Effect.Page_Date_En;
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_DATE_CN:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    MySettings.Display_Effect.Page_DateCN_En = !MySettings.Display_Effect.Page_DateCN_En;
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_TEMPRA:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    MySettings.Display_Effect.Page_Tempra_En = !MySettings.Display_Effect.Page_Tempra_En;
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_HUMIDITY:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    MySettings.Display_Effect.Page_Humidity_En = !MySettings.Display_Effect.Page_Humidity_En;
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_HOLIDAY:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    MySettings.Display_Effect.Page_Holiday_En = !MySettings.Display_Effect.Page_Holiday_En;
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_BIRTHDAY:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    MySettings.Display_Effect.Page_Brithday_En = !MySettings.Display_Effect.Page_Brithday_En;
                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        default:
            break;
    }

    if (MenuRunInfos->CursorMenu->Status == MENU_STATUS_HAS_UPATE)
    {
        MenuRunInfos->CursorMenu->Status = MENU_STATUS_NORMAL;
        Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_DATE)
        {
            Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "阳历");

            if (MySettings.Display_Effect.Page_Date_En)
            {
                Screen_Printf (0, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "开");
            }
            else
            {
                Screen_Printf (0, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "关");
            }
        }
        else if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_DATE_CN)
        {
            Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "阳历");

            if (MySettings.Display_Effect.Page_Date_En)
            {
                Screen_Printf (0, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "开");
            }
            else
            {
                Screen_Printf (0, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "关");
            }
        }


        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_DATE_CN)
        {
            Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "阴历");

            if (MySettings.Display_Effect.Page_DateCN_En)
            {
                Screen_Printf (16, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "开");
            }
            else
            {
                Screen_Printf (16, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "关");
            }
        }
        else if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_DATE)
        {
            Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "阴历");

            if (MySettings.Display_Effect.Page_DateCN_En)
            {
                Screen_Printf (16, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "开");
            }
            else
            {
                Screen_Printf (16, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "关");
            }
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_TEMPRA)
        {
            Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "温度");

            if (MySettings.Display_Effect.Page_Tempra_En)
            {
                Screen_Printf (0, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "开");
            }
            else
            {
                Screen_Printf (0, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "关");
            }
        }
        else if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_HUMIDITY)
        {
            Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "温度");

            if (MySettings.Display_Effect.Page_Tempra_En)
            {
                Screen_Printf (0, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "开");
            }
            else
            {
                Screen_Printf (0, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "关");
            }
        }


        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_HUMIDITY)
        {
            Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "湿度");

            if (MySettings.Display_Effect.Page_Humidity_En)
            {
                Screen_Printf (16, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "开");
            }
            else
            {
                Screen_Printf (16, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "关");
            }
        }
        else if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_TEMPRA)
        {
            Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "湿度");

            if (MySettings.Display_Effect.Page_Humidity_En)
            {
                Screen_Printf (16, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "开");
            }
            else
            {
                Screen_Printf (16, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "关");
            }
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_HOLIDAY)
        {
            Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "节日");

            if (MySettings.Display_Effect.Page_Holiday_En)
            {
                Screen_Printf (0, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "开");
            }
            else
            {
                Screen_Printf (0, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "关");
            }
        }
        else if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_BIRTHDAY)
        {
            Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "节日");

            if (MySettings.Display_Effect.Page_Holiday_En)
            {
                Screen_Printf (0, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "开");
            }
            else
            {
                Screen_Printf (0, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "关");
            }
        }


        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_BIRTHDAY)
        {
            Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "生日");

            if (MySettings.Display_Effect.Page_Brithday_En)
            {
                Screen_Printf (16, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "开");
            }
            else
            {
                Screen_Printf (16, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "关");
            }
        }
        else if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_HOLIDAY)
        {
            Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "生日");

            if (MySettings.Display_Effect.Page_Brithday_En)
            {
                Screen_Printf (16, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "开");
            }
            else
            {
                Screen_Printf (16, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "关");
            }
        }

        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
    }

#undef PAGE_ITEM_FIRST
#undef PAGE_ITEM_DATE
#undef PAGE_ITEM_DATE_CN
#undef PAGE_ITEM_TEMPRA
#undef PAGE_ITEM_HUMIDITY
#undef PAGE_ITEM_HOLIDAY
#undef PAGE_ITEM_BIRTHDAY
#undef PAGE_ITEM_END
}
void Page_PageSwitch (MenuRunInfo* MenuRunInfos)
{
#define PAGE_ITEM_FIRST 1
#define PAGE_ITEM_SWITCH   1
#define PAGE_ITEM_END 1

    if (MenuRunInfos->CursorMenu->Mode  == MENU_MODE_QUICKVIEW)
    {
        return;
    }
    else
    {

    }

    switch (MenuRunInfos->CursorMenu->CurrentItem)
    {
        case MENU_PAGE_ITEM_DEFAULT:
            MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_SWITCH;

            if (MySettings.DisplayMode > 1)
            {
                MySettings.DisplayMode = 0;
            }

            MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            break;

        case PAGE_ITEM_SWITCH:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);

                    return;

                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    if (MySettings.DisplayMode < 1)
                    {
                        MySettings.DisplayMode++;
                        //FIXME:SLF 2018-05-30 调整音量时播放试听音
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    if (MySettings.DisplayMode > 0)
                    {
                        MySettings.DisplayMode--;
                        //FIXME:SLF 2018-05-30 调整音量时播放试听音
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:

                    break;

                default:
                    break;
            }

            break;
    }

    if (MenuRunInfos->CursorMenu->Status == MENU_STATUS_HAS_UPATE)
    {
        Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "版式选择");

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_SWITCH)
        {
            MenuRunInfos->CursorMenu->Status = MENU_STATUS_NORMAL;
            Screen_Clear (16, 0, 64, 16, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL);

            switch (MySettings.DisplayMode)
            {
                case 0:
                    Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "渐变模式");
                    break;

                case 1:
                    Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "单屏模式");
                    break;

                default:

                    break;
            }


        }

        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
    }

#undef PAGE_ITEM_FIRST
#undef PAGE_ITEM_SWITCH
#undef PAGE_ITEM_END
}
void Page_RestoreDefault (MenuRunInfo* MenuRunInfos)
{
#define PAGE_ITEM_FIRST 1
#define PAGE_ITEM_RESTORE_NO 1
#define PAGE_ITEM_RESTORE_YES 2
#define PAGE_ITEM_END  2

    if (MenuRunInfos->CursorMenu->Mode  == MENU_MODE_QUICKVIEW)
    {
        return;
    }
    else
    {

    }

    switch (MenuRunInfos->CursorMenu->CurrentItem)
    {
        case MENU_PAGE_ITEM_DEFAULT:

            MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_RESTORE_NO;
            MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            break;

        case PAGE_ITEM_RESTORE_NO:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_RESTORE_YES:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Setting_LoadDefault();
                    Setting_Save();
                    Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
                    Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_YELLOW, SCREEN_UPDATE_NORMAL, "设置恢复");
                    Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_YELLOW, SCREEN_UPDATE_IMMEDIATELY, "中....");
                    Bsp_Screen_UpdateDelay_50MS (60);
                    Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);

                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:

                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:

                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        default:
            break;
    }

    if (MenuRunInfos->CursorMenu->Status == MENU_STATUS_HAS_UPATE)
    {
        MenuRunInfos->CursorMenu->Status = MENU_STATUS_NORMAL;
        Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
        Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "确定还原");

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_RESTORE_NO)
        {
            Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "否");
        }
        else
        {
            Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "否");
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_RESTORE_YES)
        {
            Screen_Printf (16, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "是");
        }
        else
        {
            Screen_Printf (16, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "是");
        }

        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
    }

#undef PAGE_ITEM_FIRST
#undef PAGE_ITEM_RESTORE_NO
#undef PAGE_ITEM_RESTORE_YES
#undef PAGE_ITEM_END
}
void Page_EnvBightNessLearnAuto (MenuRunInfo* MenuRunInfos)
{
#define PAGE_ITEM_FIRST 1
#define PAGE_ITEM_NO 1
#define PAGE_ITEM_YES 2
#define PAGE_ITEM_END  2

    if (MenuRunInfos->CursorMenu->Mode  == MENU_MODE_QUICKVIEW)
    {
        return;
    }
    else
    {

    }

    switch (MenuRunInfos->CursorMenu->CurrentItem)
    {
        case MENU_PAGE_ITEM_DEFAULT:

            MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_NO;
            MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            break;

        case PAGE_ITEM_NO:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_YES:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Brightness_StartLearnEnv();
                    Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
                    Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_YELLOW, SCREEN_UPDATE_NORMAL, "24小时完");
                    Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_YELLOW, SCREEN_UPDATE_IMMEDIATELY, "成学习");
                    Bsp_Screen_UpdateDelay_50MS (60);
                    Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);

                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:

                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:

                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        default:
            break;
    }

    if (MenuRunInfos->CursorMenu->Status == MENU_STATUS_HAS_UPATE)
    {
        MenuRunInfos->CursorMenu->Status = MENU_STATUS_NORMAL;
        Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
        Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "重新学习");

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_NO)
        {
            Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "否");
        }
        else
        {
            Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "否");
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_YES)
        {
            Screen_Printf (16, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "是");
        }
        else
        {
            Screen_Printf (16, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "是");
        }

        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
    }

#undef PAGE_ITEM_FIRST
#undef PAGE_ITEM_NO
#undef PAGE_ITEM_YES
#undef PAGE_ITEM_END
}
void Page_BrightnessLearnManuel (MenuRunInfo* MenuRunInfos)
{
#define PAGE_ITEM_FIRST 2
#define PAGE_ITEM_CHOOSE   1
#define PAGE_ITEM_MAX   2
#define PAGE_ITEM_MIN   3
#define PAGE_ITEM_END 3

    if (MenuRunInfos->CursorMenu->Mode  == MENU_MODE_QUICKVIEW)
    {
        return;
    }

    MenuRunInfos->Timer_SetList = 0; //NOTE:SLF 2018-05-24 时间设置界面不自动退出
    uint16_t Lightness;
    Lightness = Bsp_Get_BrightnessValue ();

    switch (MenuRunInfos->CursorMenu->CurrentItem)
    {
        case MENU_PAGE_ITEM_DEFAULT:

            MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_CHOOSE;
            break;

        case PAGE_ITEM_CHOOSE:

            MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;

            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_MAX;
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_MAX:
            if (MySettings.Brightness.LightAdcMax != Lightness)
            {
                MySettings.Brightness.LightAdcMax = Lightness;
                MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            }

            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_CHOOSE;
                    break;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:

                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:

                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:

                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_MIN:
            if (MySettings.Brightness.LightAdcMin != Lightness)
            {
                MySettings.Brightness.LightAdcMin = Lightness;
                MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            }

            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_CHOOSE;
                    break;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:

                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:

                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        default:
            break;
    }

    if (MenuRunInfos->CursorMenu->Status == MENU_STATUS_HAS_UPATE)
    {
        MenuRunInfos->CursorMenu->Status = MENU_STATUS_NORMAL;
        Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
        Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "最高");
        Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "最低");

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_MAX)
        {
            Screen_Printf (0, 32, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%4d", MySettings.Brightness.LightAdcMax);
        }
        else
        {
            Screen_Printf (0, 32, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%4d", MySettings.Brightness.LightAdcMax);
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_MIN)
        {
            Screen_Printf (16, 32, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%4d", MySettings.Brightness.LightAdcMin);
        }
        else
        {
            Screen_Printf (16, 32, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%4d", MySettings.Brightness.LightAdcMin);
        }

        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
    }

#undef PAGE_ITEM_FIRST
#undef PAGE_ITEM_END
#undef PAGE_ITEM_MAX
#undef PAGE_ITEM_MIN
}
void Page_BrightnessLearnSet (MenuRunInfo* MenuRunInfos)
{
#define PAGE_ITEM_FIRST 1
#define PAGE_ITEM_MAX   1
#define PAGE_ITEM_MIN   2
#define PAGE_ITEM_END 2

    if (MenuRunInfos->CursorMenu->Mode  == MENU_MODE_QUICKVIEW)
    {
        return;
    }

    switch (MenuRunInfos->CursorMenu->CurrentItem)
    {
        case MENU_PAGE_ITEM_DEFAULT:

            MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_MAX;
            MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            break;

        case PAGE_ITEM_MAX:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    if (MySettings.Brightness.LightAdcMax < 4095)
                    {
                        MySettings.Brightness.LightAdcMax++;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    if  (MySettings.Brightness.LightAdcMax > MySettings.Brightness.LightAdcMin)
                    {
                        MySettings.Brightness.LightAdcMax--;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }
                    else
                    {
                        MySettings.Brightness.LightAdcMax = MySettings.Brightness.LightAdcMin;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_MIN:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    if (MySettings.Brightness.LightAdcMin < MySettings.Brightness.LightAdcMax)
                    {
                        MySettings.Brightness.LightAdcMin++;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }
                    else
                    {
                        MySettings.Brightness.LightAdcMax = MySettings.Brightness.LightAdcMin;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    if (MySettings.Brightness.LightAdcMin > 0)
                    {
                        MySettings.Brightness.LightAdcMin--;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        default:
            break;
    }

    if (MenuRunInfos->CursorMenu->Status == MENU_STATUS_HAS_UPATE)
    {
        MenuRunInfos->CursorMenu->Status = MENU_STATUS_NORMAL;
        Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
        Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "最高");
        Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "最低");

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_MAX)
        {
            Screen_Printf (0, 32, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%4d", MySettings.Brightness.LightAdcMax);
        }
        else
        {
            Screen_Printf (0, 32, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%4d", MySettings.Brightness.LightAdcMax);
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_MIN)
        {
            Screen_Printf (16, 32, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%4d", MySettings.Brightness.LightAdcMin);
        }
        else
        {
            Screen_Printf (16, 32, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%4d", MySettings.Brightness.LightAdcMin);
        }

        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
    }

#undef PAGE_ITEM_FIRST
#undef PAGE_ITEM_END
#undef PAGE_ITEM_MAX
#undef PAGE_ITEM_MIN
}
void Page_VisionInfo (MenuRunInfo* MenuRunInfos)
{
#define PAGE_ITEM_FIRST 1
#define PAGE_ITEM_SOFT_VISION 1
#define PAGE_ITEM_END  1

    if (MenuRunInfos->CursorMenu->Mode  == MENU_MODE_QUICKVIEW)
    {
        return;
    }

    switch (MenuRunInfos->CursorMenu->CurrentItem)
    {
        case MENU_PAGE_ITEM_DEFAULT:

            MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_SOFT_VISION;
            MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            break;

        case PAGE_ITEM_SOFT_VISION:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:


                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:


                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        default:
            break;
    }

    if (MenuRunInfos->CursorMenu->Status == MENU_STATUS_HAS_UPATE)
    {
        MenuRunInfos->CursorMenu->Status = MENU_STATUS_NORMAL;
        Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_SOFT_VISION)
        {

            Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "易水时钟");
            Screen_Printf (16, 0, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "V%d.%d", MySettings.Vision_Main, MySettings.Vision_Sub);
        }

        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
    }

#undef PAGE_ITEM_FIRST
#undef PAGE_ITEM_SOFT_VISION
#undef PAGE_ITEM_END
}
void Page_ScreenLed_Check (MenuRunInfo* MenuRunInfos)
{
#define PAGE_ITEM_FIRST 1
#define PAGE_ITEM_SCREEN_LED_CHECK 1
#define PAGE_ITEM_END  1

    if (MenuRunInfos->CursorMenu->Mode  == MENU_MODE_QUICKVIEW)
    {
        return;
    }

    MenuRunInfos->Timer_SetList = 0; //NOTE:SLF 2018-05-24 测试界面不自动退出

    switch (MenuRunInfos->CursorMenu->CurrentItem)
    {
        case MENU_PAGE_ITEM_DEFAULT:
            MenuRunInfos->Timer_PageHold = 0;
            MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_SCREEN_LED_CHECK;
            MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            break;

        case PAGE_ITEM_SCREEN_LED_CHECK:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:


                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:


                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        default:
            break;
    }

    if (MenuRunInfos->Timer_PageHold * MENU_LOOP_TIME_MS >= 9000)
    {
        MenuRunInfos->Timer_PageHold = 0;
    }

    if (MenuRunInfos->Timer_PageHold * MENU_LOOP_TIME_MS == 0)
    {
        Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
        Screen_Printf (0, 0, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "■■■■");
        Screen_Printf (0, 32, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "■■■■");
        Screen_Printf (16, 0, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "■■■■");
        Screen_Printf (16, 32, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "■■■■");
        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
    }
    else if (MenuRunInfos->Timer_PageHold * MENU_LOOP_TIME_MS == 3000)
    {
        Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
        Screen_Printf (0, 0, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "■■■■");
        Screen_Printf (0, 32, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "■■■■");
        Screen_Printf (16, 0, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "■■■■");
        Screen_Printf (16, 32, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "■■■■");
        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
    }
    else if (MenuRunInfos->Timer_PageHold * MENU_LOOP_TIME_MS == 6000)
    {
        Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
        Screen_Printf (0, 0, FontsType_DefaultMix, SCREEN_COLOR_YELLOW, SCREEN_UPDATE_NORMAL, "■■■■");
        Screen_Printf (0, 32, FontsType_DefaultMix, SCREEN_COLOR_YELLOW, SCREEN_UPDATE_NORMAL, "■■■■");
        Screen_Printf (16, 0, FontsType_DefaultMix, SCREEN_COLOR_YELLOW, SCREEN_UPDATE_NORMAL, "■■■■");
        Screen_Printf (16, 32, FontsType_DefaultMix, SCREEN_COLOR_YELLOW, SCREEN_UPDATE_NORMAL, "■■■■");
        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
    }

    MenuRunInfos->Timer_PageHold++;
#undef PAGE_ITEM_FIRST
#undef PAGE_ITEM_SOFT_VISION
#undef PAGE_ITEM_END
}
void Page_Senser_Ds3231 (MenuRunInfo* MenuRunInfos)
{
#define PAGE_ITEM_FIRST 1
#define PAGE_ITEM_SENSER_ACTIVE 1
#define PAGE_ITEM_SENSER_TEMP_CORRECTION 2
#define PAGE_ITEM_END  2

    if (MenuRunInfos->CursorMenu->Mode  == MENU_MODE_QUICKVIEW)
    {
        return;
    }

    switch (MenuRunInfos->CursorMenu->CurrentItem)
    {
        case MENU_PAGE_ITEM_DEFAULT:

            MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_SENSER_ACTIVE;
            MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            break;

        case PAGE_ITEM_SENSER_ACTIVE:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    if (MySettings.Senser_Cfg.Senser_Select != 0)
                    {
                        MySettings.Senser_Cfg.Senser_Select_Prev = MySettings.Senser_Cfg.Senser_Select;
                        MySettings.Senser_Cfg.Senser_Select = 0;
                    }
                    else
                    {
                        MySettings.Senser_Cfg.Senser_Select = MySettings.Senser_Cfg.Senser_Select_Prev;
                    }

                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_SENSER_TEMP_CORRECTION:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    if (MySettings.Senser_Cfg.TempCorrection_DS3231 < 99)
                    {
                        MySettings.Senser_Cfg.TempCorrection_DS3231++;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    if (MySettings.Senser_Cfg.TempCorrection_DS3231 > -99)
                    {
                        MySettings.Senser_Cfg.TempCorrection_DS3231--;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        default:
            break;
    }

    if (MenuRunInfos->CursorMenu->Status == MENU_STATUS_HAS_UPATE)
    {
        MenuRunInfos->CursorMenu->Status = MENU_STATUS_NORMAL;
        Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_SENSER_ACTIVE)
        {
            if (MySettings.Senser_Cfg.Senser_Select == 0)
            {
                Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "激活");
            }
            else
            {
                Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "未激活");
            }

            Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "温度");


            if (MySettings.Senser_Cfg.TempCorrection_DS3231 > 0)
            {
                Screen_Printf (16, 32, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "+");
            }
            else  if (MySettings.Senser_Cfg.TempCorrection_DS3231 < 0)
            {
                Screen_Printf (16, 32, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "-");
            }
            else
            {
                Screen_Printf (16, 32, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, " ");
            }

            Screen_Printf (16, 40, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%d",
                           ABS (MySettings.Senser_Cfg.TempCorrection_DS3231 / 10));
            Screen_Printf (16, 40, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, ".%d℃",
                           ABS ( MySettings.Senser_Cfg.TempCorrection_DS3231  % 10));
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_SENSER_TEMP_CORRECTION)
        {

            if (MySettings.Senser_Cfg.Senser_Select == 0)
            {
                Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "激活");
            }
            else
            {
                Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "未激活");
            }

            Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "温度");


            if (MySettings.Senser_Cfg.TempCorrection_DS3231 > 0)
            {
                Screen_Printf (16, 32, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "+");
            }
            else  if (MySettings.Senser_Cfg.TempCorrection_DS3231 < 0)
            {
                Screen_Printf (16, 32, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "-");
            }
            else
            {
                Screen_Printf (16, 32, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, " ");
            }

            Screen_Printf (16, 40, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%d",
                           ABS (MySettings.Senser_Cfg.TempCorrection_DS3231 / 10));
            Screen_Printf (16, 40, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, ".%d℃",
                           ABS (MySettings.Senser_Cfg.TempCorrection_DS3231  % 10));
        }

        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
    }

#undef PAGE_ITEM_FIRST
#undef PAGE_ITEM_SENSER_ACTIVE
#undef PAGE_ITEM_SENSER_TEMP_CORRECTION
#undef PAGE_ITEM_END
}

void Page_Senser_Ds18b20 (MenuRunInfo* MenuRunInfos)
{
#define PAGE_ITEM_FIRST 1
#define PAGE_ITEM_SENSER_ACTIVE 1
#define PAGE_ITEM_SENSER_TEMP_CORRECTION 2
#define PAGE_ITEM_END  2

    if (MenuRunInfos->CursorMenu->Mode  == MENU_MODE_QUICKVIEW)
    {
        return;
    }

    switch (MenuRunInfos->CursorMenu->CurrentItem)
    {
        case MENU_PAGE_ITEM_DEFAULT:

            MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_SENSER_ACTIVE;
            MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            break;

        case PAGE_ITEM_SENSER_ACTIVE:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    if (MySettings.Senser_Cfg.Senser_Select != 1)
                    {
                        MySettings.Senser_Cfg.Senser_Select_Prev = MySettings.Senser_Cfg.Senser_Select;
                        MySettings.Senser_Cfg.Senser_Select = 1;
                    }
                    else
                    {
                        MySettings.Senser_Cfg.Senser_Select = MySettings.Senser_Cfg.Senser_Select_Prev;
                    }

                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_SENSER_TEMP_CORRECTION:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    if (MySettings.Senser_Cfg.TempCorrection_DS18B20 < 99)
                    {
                        MySettings.Senser_Cfg.TempCorrection_DS18B20++;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    if (MySettings.Senser_Cfg.TempCorrection_DS18B20 > -99)
                    {
                        MySettings.Senser_Cfg.TempCorrection_DS18B20--;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        default:
            break;
    }

    if (MenuRunInfos->CursorMenu->Status == MENU_STATUS_HAS_UPATE)
    {
        MenuRunInfos->CursorMenu->Status = MENU_STATUS_NORMAL;
        Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_SENSER_ACTIVE)
        {
            if (MySettings.Senser_Cfg.Senser_Select == 1)
            {
                Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "激活");
            }
            else
            {
                Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "未激活");
            }

            Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "温度");


            if (MySettings.Senser_Cfg.TempCorrection_DS18B20 > 0)
            {
                Screen_Printf (16, 32, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "+");
            }
            else  if (MySettings.Senser_Cfg.TempCorrection_DS18B20 < 0)
            {
                Screen_Printf (16, 32, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "-");
            }
            else
            {
                Screen_Printf (16, 32, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, " ");
            }

            Screen_Printf (16, 40, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%d",
                           ABS (MySettings.Senser_Cfg.TempCorrection_DS18B20 / 10));
            Screen_Printf (16, 40, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, ".%d℃",
                           ABS ( MySettings.Senser_Cfg.TempCorrection_DS18B20  % 10));
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_SENSER_TEMP_CORRECTION)
        {

            if (MySettings.Senser_Cfg.Senser_Select == 1)
            {
                Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "激活");
            }
            else
            {
                Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "未激活");
            }

            Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "温度");


            if (MySettings.Senser_Cfg.TempCorrection_DS18B20 > 0)
            {
                Screen_Printf (16, 32, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "+");
            }
            else  if (MySettings.Senser_Cfg.TempCorrection_DS18B20 < 0)
            {
                Screen_Printf (16, 32, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "-");
            }
            else
            {
                Screen_Printf (16, 32, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, " ");
            }

            Screen_Printf (16, 40, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%d",
                           ABS (MySettings.Senser_Cfg.TempCorrection_DS18B20 / 10));
            Screen_Printf (16, 40, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, ".%d℃",
                           ABS (MySettings.Senser_Cfg.TempCorrection_DS18B20  % 10));
        }

        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
    }

#undef PAGE_ITEM_FIRST
#undef PAGE_ITEM_SENSER_ACTIVE
#undef PAGE_ITEM_SENSER_TEMP_CORRECTION
#undef PAGE_ITEM_END
}

void Page_Senser_Sht30 (MenuRunInfo* MenuRunInfos)
{
#define PAGE_ITEM_FIRST 1
#define PAGE_ITEM_SENSER_ACTIVE 1
#define PAGE_ITEM_SENSER_TEMP_CORRECTION 2
#define PAGE_ITEM_SENSER_HUMIDITY_CORRECTION 3
#define PAGE_ITEM_END  3

    if (MenuRunInfos->CursorMenu->Mode  == MENU_MODE_QUICKVIEW)
    {
        return;
    }

    switch (MenuRunInfos->CursorMenu->CurrentItem)
    {
        case MENU_PAGE_ITEM_DEFAULT:

            MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_SENSER_ACTIVE;
            MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            break;

        case PAGE_ITEM_SENSER_ACTIVE:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    if (MySettings.Senser_Cfg.Senser_Select != 2)
                    {
                        MySettings.Senser_Cfg.Senser_Select_Prev = MySettings.Senser_Cfg.Senser_Select;
                        MySettings.Senser_Cfg.Senser_Select = 2;
                    }
                    else
                    {
                        MySettings.Senser_Cfg.Senser_Select = MySettings.Senser_Cfg.Senser_Select_Prev;
                    }

                    MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_SENSER_TEMP_CORRECTION:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    if (MySettings.Senser_Cfg.TempCorrection_SHT30 < 99)
                    {
                        MySettings.Senser_Cfg.TempCorrection_SHT30++;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    if (MySettings.Senser_Cfg.TempCorrection_SHT30 > -99)
                    {
                        MySettings.Senser_Cfg.TempCorrection_SHT30--;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_SENSER_HUMIDITY_CORRECTION:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                case BT_KEYCODE_MENU_RMET:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);
                    return;

                case BT_KEYCODE_UP_LONG:
                case BT_KEYCODE_UP:
                case BT_KEYCODE_UP_RMET:
                    if (MySettings.Senser_Cfg.HumidityCorrection_SHT30 < 99)
                    {
                        MySettings.Senser_Cfg.HumidityCorrection_SHT30++;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_DOWN_LONG:
                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_DOWN_RMET:
                    if (MySettings.Senser_Cfg.HumidityCorrection_SHT30 > -99)
                    {
                        MySettings.Senser_Cfg.HumidityCorrection_SHT30--;
                        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                    }

                    break;

                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_LEFT_RMET:
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_OK_RMET:
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        default:
            break;
    }

    if (MenuRunInfos->CursorMenu->Status == MENU_STATUS_HAS_UPATE)
    {
        MenuRunInfos->CursorMenu->Status = MENU_STATUS_NORMAL;
        Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_SENSER_ACTIVE)
        {
            if (MySettings.Senser_Cfg.Senser_Select == 2)
            {
                Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "激活");
            }
            else
            {
                Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "未激活");
            }

            Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "温度");


            if (MySettings.Senser_Cfg.TempCorrection_SHT30 > 0)
            {
                Screen_Printf (16, 32, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "+");
            }
            else  if (MySettings.Senser_Cfg.TempCorrection_SHT30 < 0)
            {
                Screen_Printf (16, 32, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "-");
            }
            else
            {
                Screen_Printf (16, 32, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, " ");
            }

            Screen_Printf (16, 40, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%d",
                           ABS (MySettings.Senser_Cfg.TempCorrection_SHT30 / 10));
            Screen_Printf (16, 40, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, ".%d℃",
                           ABS ( MySettings.Senser_Cfg.TempCorrection_SHT30  % 10));
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_SENSER_TEMP_CORRECTION)
        {

            if (MySettings.Senser_Cfg.Senser_Select == 2)
            {
                Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "激活");
            }
            else
            {
                Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "未激活");
            }

            Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "温度");


            if (MySettings.Senser_Cfg.TempCorrection_SHT30 > 0)
            {
                Screen_Printf (16, 32, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "+");
            }
            else  if (MySettings.Senser_Cfg.TempCorrection_SHT30 < 0)
            {
                Screen_Printf (16, 32, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "-");
            }
            else
            {
                Screen_Printf (16, 32, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, " ");
            }

            Screen_Printf (16, 40, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%d",
                           ABS (MySettings.Senser_Cfg.TempCorrection_SHT30 / 10));
            Screen_Printf (16, 40, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, ".%d℃",
                           ABS (MySettings.Senser_Cfg.TempCorrection_SHT30  % 10));
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_SENSER_HUMIDITY_CORRECTION)
        {

            Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "湿度");


            if (MySettings.Senser_Cfg.HumidityCorrection_SHT30 > 0)
            {
                Screen_Printf (0, 32, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "+");
            }
            else  if (MySettings.Senser_Cfg.HumidityCorrection_SHT30 < 0)
            {
                Screen_Printf (0, 32, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "-");
            }
            else
            {
                Screen_Printf (0, 32, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, " ");
            }

            Screen_Printf (0, 40, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%d",
                           ABS (MySettings.Senser_Cfg.HumidityCorrection_SHT30 / 10));
            Screen_Printf (0, 40, FontsType_DefaultMix, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, ".%d%%",
                           ABS (MySettings.Senser_Cfg.HumidityCorrection_SHT30  % 10));
        }

        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
    }

#undef PAGE_ITEM_FIRST
#undef PAGE_ITEM_SENSER_ACTIVE
#undef PAGE_ITEM_SENSER_TEMP_CORRECTION
#undef PAGE_ITEM_SENSER_HUMIDITY_CORRECTION
#undef PAGE_ITEM_END
}
void Page_GUI_Test (MenuRunInfo* MenuRunInfos)
{
#define PAGE_ITEM_FIRST 1
#define PAGE_ITEM_POINT     1
#define PAGE_ITEM_LINE    2
#define PAGE_ITEM_DOT_LINE     3
#define PAGE_ITEM_RECTANGLE    4
#define PAGE_ITEM_ROUND_RECT    5
#define PAGE_ITEM_CIRCLE    6
#define PAGE_ITEM_END  6
#define PAGE_HOLD_TIME_MS    10000

    if (MenuRunInfos->CursorMenu->Mode  == MENU_MODE_QUICKVIEW)
    {
        return;
    }


    MenuRunInfos->Timer_SetList = 0; //NOTE:SLF 2018-05-24 时间设置界面不自动退出

    if (MenuRunInfos->Timer_PageHold * MENU_LOOP_TIME_MS % 1000 == 0)
    {
        MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
    }


    switch (MenuRunInfos->CursorMenu->CurrentItem)
    {
        case MENU_PAGE_ITEM_DEFAULT:
            MenuRunInfos->CursorMenu->CurrentItem = PAGE_ITEM_POINT;
            MenuRunInfos->Timer_PageHold = PAGE_HOLD_TIME_MS / MENU_LOOP_TIME_MS;
            MenuRunInfos->CursorMenu->Status = MENU_STATUS_HAS_UPATE;
            break;

        case PAGE_ITEM_POINT:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);

                    return;

                case BT_KEYCODE_UP:
                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_UP_RMET:
                case BT_KEYCODE_LEFT_RMET:
                    Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_DOWN_RMET:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK_RMET:
                    Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_LINE:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);

                    return;

                case BT_KEYCODE_UP:
                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_UP_RMET:
                case BT_KEYCODE_LEFT_RMET:
                    Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_DOWN_RMET:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK_RMET:
                    Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_DOT_LINE:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);

                    return;

                case BT_KEYCODE_UP:
                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_UP_RMET:
                case BT_KEYCODE_LEFT_RMET:
                    Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_DOWN_RMET:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK_RMET:
                    Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_RECTANGLE:

            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);

                    return;

                case BT_KEYCODE_UP:
                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_UP_RMET:
                case BT_KEYCODE_LEFT_RMET:
                    Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_DOWN_RMET:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK_RMET:
                    Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_ROUND_RECT:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);

                    return;

                case BT_KEYCODE_UP:
                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_UP_RMET:
                case BT_KEYCODE_LEFT_RMET:
                    Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_DOWN_RMET:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK_RMET:
                    Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        case PAGE_ITEM_CIRCLE:
            switch (*MenuRunInfos->KeyCode)
            {
                case BT_KEYCODE_MENU:
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    MenuIf_ListShow (MenuRunInfos);

                    return;

                case BT_KEYCODE_UP:
                case BT_KEYCODE_LEFT:
                case BT_KEYCODE_UP_RMET:
                case BT_KEYCODE_LEFT_RMET:
                    Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
                    Page_Switch_Prev_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                case BT_KEYCODE_DOWN:
                case BT_KEYCODE_RIGHT:
                case BT_KEYCODE_OK:
                case BT_KEYCODE_DOWN_RMET:
                case BT_KEYCODE_RIGHT_RMET:
                case BT_KEYCODE_OK_RMET:
                    Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
                    Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
                    break;

                default:
                    break;
            }

            break;

        default:
            break;
    }

    static uint16_t CurrentPosition = 0;

    if (MenuRunInfos->CursorMenu->Status == MENU_STATUS_HAS_UPATE)
    {
        MenuRunInfos->CursorMenu->Status = MENU_STATUS_NORMAL;

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_POINT)
        {
            Screen_Printf (16, 0, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "Point");
            RMP_Point (CurrentPosition / 64, CurrentPosition % 64, SCREEN_COLOR_RED);
            CurrentPosition++;

        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_LINE)
        {
            Screen_Printf (16, 10, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "Line");
            RMP_Line (0, 0, 31, 63, SCREEN_COLOR_RED);
            RMP_Line (0, 63, 31, 0, SCREEN_COLOR_RED);
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_DOT_LINE)
        {
            Screen_Printf (16, 0, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "Dot Line");
            RMP_Dot_Line (1, 0, 1, 63, 2, SCREEN_COLOR_RED);
            RMP_Dot_Line (5, 5, 5, 48, 2, SCREEN_COLOR_RED);
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_RECTANGLE)
        {
            Screen_Printf (16, 0, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "Rectangle");
            RMP_Rectangle (0, 0, 20, 43, SCREEN_COLOR_RED, RMP_TRANS);
            RMP_Rectangle (10, 10, 20, 20, SCREEN_COLOR_RED, RMP_TRANS);
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_ROUND_RECT)
        {
            Screen_Printf (16, 0, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "Round Rect");
            RMP_Round_Rect (0, 0, 20, 43, 2, RMP_TRANS, SCREEN_COLOR_RED);
            RMP_Round_Rect (10, 10, 20, 20, 2, RMP_TRANS, SCREEN_COLOR_RED);
        }

        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_CIRCLE)
        {
            Screen_Printf (16, 0, FontsType_DefaultMix, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "Circle");
            RMP_Circle (15, 32, 14,  SCREEN_COLOR_RED, RMP_TRANS);
            RMP_Circle (15, 32, 10, SCREEN_COLOR_RED, RMP_TRANS);
        }


        Screen_UpdateImmediately (SCREEN_COLOR_ALL);
    }

    MenuRunInfos->Timer_PageHold--;

    if (MenuRunInfos->Timer_PageHold == 0)
    {
        //NOTE:SLF 2018-05-28 倒计时时间到，进入到下一个学习页面或者退出学习界面
        if (MenuRunInfos->CursorMenu->CurrentItem == PAGE_ITEM_END)
        {
            MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
            MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
            MenuIf_ListShow (MenuRunInfos);
            return;
        }
        else
        {
            Page_Switch_Next_Item (MenuRunInfos, PAGE_ITEM_FIRST, PAGE_ITEM_END);
            MenuRunInfos->Timer_PageHold = PAGE_HOLD_TIME_MS / MENU_LOOP_TIME_MS;
            Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
            CurrentPosition = 0;
        }
    }

#undef PAGE_ITEM_FIRST
#undef PAGE_ITEM_POINT
#undef PAGE_ITEM_LINE
#undef PAGE_ITEM_DOT_LINE
#undef PAGE_ITEM_RECTANGLE
#undef PAGE_ITEM_ROUND_RECT
#undef PAGE_ITEM_CIRCLE
#undef PAGE_ITEM_END
#undef PAGE_HOLD_TIME_MS


}
