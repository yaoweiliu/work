

/*******************************************************************************************/
/*include                                                                                                                                                       */
/*******************************************************************************************/
#include "Menu_If.h"
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

/*******************************************************************************************/
/*Global variables                                                                                                                                           */
/*******************************************************************************************/
MenuNode MenuNodes[MENU_NODES_COUNT] =
{
    { (const unsigned char*)"主界面", 0, MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_Main},
    { (const unsigned char*)"日期设置", 1,   MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_Set_Date},
    { (const unsigned char*)"时间设置", 2,   MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU},
    { (const unsigned char*)"时间调整", 201,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_Set_Time},
    { (const unsigned char*)"24小时制", 202,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_Set24Time},
    { (const unsigned char*)"时间校准", 203,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetTimeCalibration},
    { (const unsigned char*)"语音设置", 3,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU},
    { (const unsigned char*)"播报模式", 301,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetVoiceMode},
    { (const unsigned char*)"播报时间", 302,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetVoiceStartEnd},
    { (const unsigned char*)"音量", 303,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetVolume},
    { (const unsigned char*)"温度选择", 4,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU},
    { (const unsigned char*)"DS3231", 401,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_Senser_Ds3231},
    { (const unsigned char*)"DS18B20", 402,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_Senser_Ds18b20},
    { (const unsigned char*)"SHT30", 403,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_Senser_Sht30},
    { (const unsigned char*)"显示效果", 5,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU},
    { (const unsigned char*)"时间参数", 501,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_DisplayTimerSet},
    { (const unsigned char*)"页面开关", 502,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_DisplayPageEnable},
    { (const unsigned char*)"版式选择", 503,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_PageSwitch},
    { (const unsigned char*)"亮度设置", 6,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU},
    { (const unsigned char*)"亮度模式", 601,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetBrightnessMode},
    { (const unsigned char*)"亮度标定", 602,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetBrightnessCelibration},
    { (const unsigned char*)"环境亮度", 603,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU},
    { (const unsigned char*)"自动采集", 60301,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_EnvBightNessLearnAuto},
    { (const unsigned char*)"手动采集", 60302,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_BrightnessLearnManuel},
    { (const unsigned char*)"手动设置", 60303,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_BrightnessLearnSet},
    { (const unsigned char*)"灵敏度", 604,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetBrightness_Sensitivity},
    { (const unsigned char*)"遥控设置", 7,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU},
    { (const unsigned char*)"遥控开关", 701,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetRemoteKeyEnable},
    { (const unsigned char*)"遥控学习", 702,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_RemoteKeyLearn},
    { (const unsigned char*)"字体设置", 8,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU},
    { (const unsigned char*)"字体选择", 801,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetFontType},
    { (const unsigned char*)"字体颜色", 802,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetFontColor},
    { (const unsigned char*)"频谱", 9,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU},
    { (const unsigned char*)"颜色设置", 901,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU},
    { (const unsigned char*)"分频设置", 902,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU},
    { (const unsigned char*)"打开频谱", 903,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU},
    { (const unsigned char*)"授时设置", 10,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_GUI_Test},
    { (const unsigned char*)"闹钟设置", 11,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU},
    { (const unsigned char*)"闹钟一", 1101,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetAlarmClock},
    { (const unsigned char*)"闹钟二", 1102,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetAlarmClock},
    { (const unsigned char*)"闹钟三", 1103,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetAlarmClock},
    { (const unsigned char*)"闹钟四", 1104,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetAlarmClock},
    { (const unsigned char*)"闹钟五", 1105,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetAlarmClock},
    { (const unsigned char*)"闹钟六", 1106,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetAlarmClock},
    { (const unsigned char*)"屏幕检测", 12,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_ScreenLed_Check},
    { (const unsigned char*)"还原设置", 13,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_RestoreDefault},
    { (const unsigned char*)"版本信息", 14,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_VisionInfo},
};

/*******************************************************************************************/
/*Static Function Implementation                                                                                                                      */
/*******************************************************************************************/


/*******************************************************************************************/
/*Public Function Implementation                                                                                                                       */
/*******************************************************************************************/
void MenuIf_ListShow (MenuRunInfo* MenuRunInfos)
{
    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_LIST;
    Bsp_Screen_Set_Brightness_Immediately (SCREEN_INDEX_BOTTOM, Brightness_GetMaxValue());
    //    Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_IMMEDIATELY);//NOTE:SLF 2018-06-07 此处必须立即更新
    Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);//NOTE:SLF 2018-06-07 此处必须立即更新

    if (MenuRunInfos->CursorMenu->ID % 2 != 0)
    {

        Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%s", MenuRunInfos->CursorMenu->Title);

        if (MenuRunInfos->CursorMenu->Message_String != NULL_MENU)
        {
            Screen_Printf (0, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%s", MenuRunInfos->CursorMenu->Message_String);
        }

        if (MenuRunInfos->CursorMenu->BrotherNode_Next != NULL_MENU)
        {
            //            Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL);//NOTE:SLF 2018-06-07 此处必须立即更新
            Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%s",
                           MenuRunInfos->CursorMenu->BrotherNode_Next->Title);

            if (MenuRunInfos->CursorMenu->BrotherNode_Next->Message_String != NULL_MENU)
            {
                Screen_Printf (16, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%s",
                               MenuRunInfos->CursorMenu->BrotherNode_Next->Message_String);
            }
        }
    }
    else
    {
        Screen_Printf (16, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%s", MenuRunInfos->CursorMenu->Title);

        if (MenuRunInfos->CursorMenu->Message_String != NULL_MENU)
        {
            Screen_Printf (16, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%s", MenuRunInfos->CursorMenu->Message_String);
        }

        if (MenuRunInfos->CursorMenu->BrotherNode_Prev != NULL_MENU)
        {
            //            Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL);//NOTE:SLF 2018-06-07 此处必须立即更新
            Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%s",
                           MenuRunInfos->CursorMenu->BrotherNode_Prev->Title);

            if (MenuRunInfos->CursorMenu->BrotherNode_Prev->Message_String != NULL_MENU)
            {
                Screen_Printf (0, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%s",
                               MenuRunInfos->CursorMenu->BrotherNode_Prev->Message_String);
            }
        }
    }

    Screen_UpdateImmediately (SCREEN_COLOR_ALL);
}

//void MenuIf_MenuDispatcher (MenuRunInfo* MenuRunInfos)
//{
//    switch (*MenuRunInfos->KeyCode)
//    {
//        case BT_KEYCODE_MENU:
//        case BT_KEYCODE_MENU_RMET:
//            if (MenuRunInfos->CursorMenu == NULL_MENU)
//            {
//                //NOTE:SLF 2018-05-23 进入设置菜单列表
//                MenuRunInfos->CursorMenu = &MenuNodes[1];
//                MenuIf_ListShow (MenuRunInfos);
//            }
//            else
//            {
//                //NOTE:SLF 2018-05-23 返回上一层或者退出菜单
//                if (MenuRunInfos->CursorMenu->FatherNode == NULL_MENU)
//                {
//                    //NOTE:SLF 2018-05-23 退出菜单，进入主界面
//                    MenuRunInfos->CursorMenu = NULL_MENU;
//                    Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_YELLOW);
//                    //                    *MenuRunInfos->KeyCode = BT_KEYCODE_NONE;
//                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_MAIN;
//                    Page_Main (1);
//                }
//                else
//                {
//                    //NOTE:SLF 2018-05-23 返回上一层菜单
//                    MenuRunInfos->CursorMenu = MenuRunInfos->CursorMenu->FatherNode;
//                    MenuIf_ListShow (MenuRunInfos);
//                }
//            }

//            break;

//        case BT_KEYCODE_UP:
//        case BT_KEYCODE_UP_RMET:
//            if (MenuRunInfos->CursorMenu == NULL_MENU)
//            {

//            }
//            else
//            {
//                if (MenuRunInfos->CursorMenu->BrotherNode_Prev == NULL_MENU)
//                {

//                }
//                else
//                {
//                    MenuRunInfos->CursorMenu = MenuRunInfos->CursorMenu->BrotherNode_Prev;
//                    MenuIf_ListShow (MenuRunInfos);
//                }
//            }

//            break;

//        case BT_KEYCODE_DOWN:
//        case BT_KEYCODE_DOWN_RMET:
//            if (MenuRunInfos->CursorMenu == NULL_MENU)
//            {

//            }
//            else
//            {
//                if (MenuRunInfos->CursorMenu->BrotherNode_Next == NULL_MENU)
//                {

//                }
//                else
//                {
//                    MenuRunInfos->CursorMenu = MenuRunInfos->CursorMenu->BrotherNode_Next;
//                    MenuIf_ListShow (MenuRunInfos);
//                }
//            }

//            break;

//        case BT_KEYCODE_LEFT:
//        case BT_KEYCODE_LEFT_RMET:
//            if (MenuRunInfos->CursorMenu != NULL_MENU)
//            {
//                if (MenuRunInfos->CursorMenu->FatherNode != NULL_MENU)
//                {
//                    //NOTE:SLF 2018-05-23 返回上一层菜单
//                    MenuRunInfos->CursorMenu = MenuRunInfos->CursorMenu->FatherNode;
//                    MenuIf_ListShow (MenuRunInfos);
//                }
//            }

//            break;

//        case BT_KEYCODE_RIGHT:
//        case BT_KEYCODE_RIGHT_RMET:
//        case BT_KEYCODE_OK:
//        case BT_KEYCODE_OK_RMET:
//            if (MenuRunInfos->CursorMenu == NULL_MENU)
//            {

//            }
//            else
//            {
//                if (MenuRunInfos->CursorMenu->SonNode_First == NULL_MENU)
//                {
//                    //NOTE:SLF 2018-05-23 没有子菜单，点击确定则进入设置页面
//                    if (MenuRunInfos->CursorMenu->SetPageRun != NULL_MENU)
//                    {
//                        //                        *MenuRunInfos->KeyCode = BT_KEYCODE_NONE;
//                        Bsp_Screen_Set_Brightness_Immediately (SCREEN_INDEX_BOTTOM, Brightness_GetMaxValue());
//                        MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_PAGE;
//                        Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_YELLOW);
//                        MenuRunInfos->CursorMenu->SetPageRun (MenuRunInfos);
//                    }
//                }
//                else
//                {
//                    MenuRunInfos->CursorMenu = MenuRunInfos->CursorMenu->SonNode_First;
//                    MenuIf_ListShow (MenuRunInfos);
//                }
//            }

//            break;

//        default:
//            if (MenuRunInfos->flg_CurrentPage == MENU_CURRENT_PAGE_MAIN)
//            {
//                Page_Main (0);
//            }

//            break;
//    }
//}

void MenuIf_MenuDispatcher (MenuRunInfo* MenuRunInfos)
{
    switch (*MenuRunInfos->KeyCode)
    {
        case BT_KEYCODE_MENU:
        case BT_KEYCODE_MENU_RMET:
            if (MenuRunInfos->CursorMenu->ID == MENU_MAIN_NODE_INDEX)
            {
                //NOTE:SLF 2018-05-23 进入设置菜单列表
                if (MenuRunInfos->LastMenu == NULL_MENU)
                {
                    MenuRunInfos->CursorMenu = &MenuNodes[MENU_FIRST_MENU_NODE_INDEX];
                }
                else
                {
                    MenuRunInfos->CursorMenu = MenuRunInfos->LastMenu;
                }

                MenuIf_ListShow (MenuRunInfos);
            }
            else
            {
                //NOTE:SLF 2018-05-23 返回上一层或者退出菜单
                if (MenuRunInfos->CursorMenu->FatherNode == NULL_MENU)
                {
                    //NOTE:SLF 2018-05-23 退出菜单，进入主界面
                    //NOTE:SLF 2018-05-30 此时保存一次配置
                    Setting_Save ();
                    MenuRunInfos->LastMenu = MenuRunInfos->CursorMenu;
                    MenuRunInfos->CursorMenu = &MenuNodes[MENU_MAIN_NODE_INDEX];
                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_MAIN;
                    MenuRunInfos->CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                    Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
                    MenuRunInfos->CursorMenu->SetPageRun (MenuRunInfos);
                }
                else
                {
                    //NOTE:SLF 2018-05-23 返回上一层菜单
                    MenuRunInfos->LastMenu = MenuRunInfos->CursorMenu;
                    MenuRunInfos->CursorMenu = MenuRunInfos->CursorMenu->FatherNode;
                    MenuIf_ListShow (MenuRunInfos);
                }
            }

            break;

        case BT_KEYCODE_UP:
        case BT_KEYCODE_UP_RMET:
        case BT_KEYCODE_UP_LONG:
            if (MenuRunInfos->CursorMenu != NULL_MENU)
            {
                if ((MenuRunInfos->CursorMenu->BrotherNode_Prev != NULL_MENU) && (MenuRunInfos->CursorMenu->ID != MENU_MAIN_NODE_INDEX))
                {
                    MenuRunInfos->CursorMenu = MenuRunInfos->CursorMenu->BrotherNode_Prev;
                    MenuIf_ListShow (MenuRunInfos);
                }
            }

            break;

        case BT_KEYCODE_DOWN:
        case BT_KEYCODE_DOWN_RMET:
        case BT_KEYCODE_DOWN_LONG:
            if (MenuRunInfos->CursorMenu != NULL_MENU)
            {
                if ((MenuRunInfos->CursorMenu->BrotherNode_Next != NULL_MENU) && (MenuRunInfos->CursorMenu->ID != MENU_MAIN_NODE_INDEX))
                {
                    MenuRunInfos->CursorMenu = MenuRunInfos->CursorMenu->BrotherNode_Next;
                    MenuIf_ListShow (MenuRunInfos);
                }
            }

            break;

        case BT_KEYCODE_LEFT:
        case BT_KEYCODE_LEFT_RMET:
            if (MenuRunInfos->CursorMenu != NULL_MENU)
            {
                if ((MenuRunInfos->CursorMenu->FatherNode != NULL_MENU) && (MenuRunInfos->CursorMenu->ID != MENU_MAIN_NODE_INDEX))
                {
                    //NOTE:SLF 2018-05-23 返回上一层菜单
                    MenuRunInfos->CursorMenu = MenuRunInfos->CursorMenu->FatherNode;
                    MenuIf_ListShow (MenuRunInfos);
                }
            }

            break;

        case BT_KEYCODE_RIGHT:
        case BT_KEYCODE_RIGHT_RMET:
        case BT_KEYCODE_OK:
        case BT_KEYCODE_OK_RMET:
            if (MenuRunInfos->CursorMenu != NULL_MENU)
            {
                if  (MenuRunInfos->CursorMenu->ID != MENU_MAIN_NODE_INDEX)
                {
                    if (MenuRunInfos->CursorMenu->SonNode_First == NULL_MENU)
                    {
                        //NOTE:SLF 2018-05-23 没有子菜单，点击确定则进入设置页面
                        if (MenuRunInfos->CursorMenu->SetPageRun != NULL_MENU)
                        {
                            Bsp_Screen_Set_Brightness_Immediately (SCREEN_INDEX_BOTTOM, Brightness_GetMaxValue());
                            MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_SET_PAGE;
                            Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
                            MenuRunInfos->CursorMenu->SetPageRun (MenuRunInfos);
                        }
                    }
                    else
                    {
                        MenuRunInfos->CursorMenu = MenuRunInfos->CursorMenu->SonNode_First;
                        MenuIf_ListShow (MenuRunInfos);
                    }
                }
            }

            break;

        default:


            break;
    }

    if (MenuRunInfos->flg_CurrentPage == MENU_CURRENT_PAGE_MAIN)
    {
        //        MenuRunInfos->LastMenu = MenuRunInfos->CursorMenu;
        MenuRunInfos->CursorMenu = &MenuNodes[MENU_MAIN_NODE_INDEX];
        MenuRunInfos->CursorMenu->SetPageRun (MenuRunInfos);
    }
}
