

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
    { (const unsigned char*)"������", 0, MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_Main},
    { (const unsigned char*)"��������", 1,   MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_Set_Date},
    { (const unsigned char*)"ʱ������", 2,   MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU},
    { (const unsigned char*)"ʱ�����", 201,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_Set_Time},
    { (const unsigned char*)"24Сʱ��", 202,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_Set24Time},
    { (const unsigned char*)"ʱ��У׼", 203,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetTimeCalibration},
    { (const unsigned char*)"��������", 3,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU},
    { (const unsigned char*)"����ģʽ", 301,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetVoiceMode},
    { (const unsigned char*)"����ʱ��", 302,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetVoiceStartEnd},
    { (const unsigned char*)"����", 303,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetVolume},
    { (const unsigned char*)"�¶�ѡ��", 4,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU},
    { (const unsigned char*)"DS3231", 401,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_Senser_Ds3231},
    { (const unsigned char*)"DS18B20", 402,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_Senser_Ds18b20},
    { (const unsigned char*)"SHT30", 403,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_Senser_Sht30},
    { (const unsigned char*)"��ʾЧ��", 5,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU},
    { (const unsigned char*)"ʱ�����", 501,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_DisplayTimerSet},
    { (const unsigned char*)"ҳ�濪��", 502,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_DisplayPageEnable},
    { (const unsigned char*)"��ʽѡ��", 503,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_PageSwitch},
    { (const unsigned char*)"��������", 6,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU},
    { (const unsigned char*)"����ģʽ", 601,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetBrightnessMode},
    { (const unsigned char*)"���ȱ궨", 602,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetBrightnessCelibration},
    { (const unsigned char*)"��������", 603,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU},
    { (const unsigned char*)"�Զ��ɼ�", 60301,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_EnvBightNessLearnAuto},
    { (const unsigned char*)"�ֶ��ɼ�", 60302,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_BrightnessLearnManuel},
    { (const unsigned char*)"�ֶ�����", 60303,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_BrightnessLearnSet},
    { (const unsigned char*)"������", 604,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetBrightness_Sensitivity},
    { (const unsigned char*)"ң������", 7,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU},
    { (const unsigned char*)"ң�ؿ���", 701,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetRemoteKeyEnable},
    { (const unsigned char*)"ң��ѧϰ", 702,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_RemoteKeyLearn},
    { (const unsigned char*)"��������", 8,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU},
    { (const unsigned char*)"����ѡ��", 801,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetFontType},
    { (const unsigned char*)"������ɫ", 802,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetFontColor},
    { (const unsigned char*)"Ƶ��", 9,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU},
    { (const unsigned char*)"��ɫ����", 901,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU},
    { (const unsigned char*)"��Ƶ����", 902,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU},
    { (const unsigned char*)"��Ƶ��", 903,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU},
    { (const unsigned char*)"��ʱ����", 10,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_GUI_Test},
    { (const unsigned char*)"��������", 11,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU},
    { (const unsigned char*)"����һ", 1101,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetAlarmClock},
    { (const unsigned char*)"���Ӷ�", 1102,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetAlarmClock},
    { (const unsigned char*)"������", 1103,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetAlarmClock},
    { (const unsigned char*)"������", 1104,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetAlarmClock},
    { (const unsigned char*)"������", 1105,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetAlarmClock},
    { (const unsigned char*)"������", 1106,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_SetAlarmClock},
    { (const unsigned char*)"��Ļ���", 12,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_ScreenLed_Check},
    { (const unsigned char*)"��ԭ����", 13,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_RestoreDefault},
    { (const unsigned char*)"�汾��Ϣ", 14,  MENU_MODE_QUICKVIEW, MENU_STATUS_NORMAL, MENU_PAGE_ITEM_DEFAULT, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, NULL_MENU, &Page_VisionInfo},
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
    //    Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_IMMEDIATELY);//NOTE:SLF 2018-06-07 �˴�������������
    Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);//NOTE:SLF 2018-06-07 �˴�������������

    if (MenuRunInfos->CursorMenu->ID % 2 != 0)
    {

        Screen_Printf (0, 0, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_GREEN, SCREEN_UPDATE_NORMAL, "%s", MenuRunInfos->CursorMenu->Title);

        if (MenuRunInfos->CursorMenu->Message_String != NULL_MENU)
        {
            Screen_Printf (0, 48, MySettings.FontCfg.FontType_Menu, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL, "%s", MenuRunInfos->CursorMenu->Message_String);
        }

        if (MenuRunInfos->CursorMenu->BrotherNode_Next != NULL_MENU)
        {
            //            Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL);//NOTE:SLF 2018-06-07 �˴�������������
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
            //            Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_RED, SCREEN_UPDATE_NORMAL);//NOTE:SLF 2018-06-07 �˴�������������
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
//                //NOTE:SLF 2018-05-23 �������ò˵��б�
//                MenuRunInfos->CursorMenu = &MenuNodes[1];
//                MenuIf_ListShow (MenuRunInfos);
//            }
//            else
//            {
//                //NOTE:SLF 2018-05-23 ������һ������˳��˵�
//                if (MenuRunInfos->CursorMenu->FatherNode == NULL_MENU)
//                {
//                    //NOTE:SLF 2018-05-23 �˳��˵�������������
//                    MenuRunInfos->CursorMenu = NULL_MENU;
//                    Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_YELLOW);
//                    //                    *MenuRunInfos->KeyCode = BT_KEYCODE_NONE;
//                    MenuRunInfos->flg_CurrentPage = MENU_CURRENT_PAGE_MAIN;
//                    Page_Main (1);
//                }
//                else
//                {
//                    //NOTE:SLF 2018-05-23 ������һ��˵�
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
//                    //NOTE:SLF 2018-05-23 ������һ��˵�
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
//                    //NOTE:SLF 2018-05-23 û���Ӳ˵������ȷ�����������ҳ��
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
                //NOTE:SLF 2018-05-23 �������ò˵��б�
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
                //NOTE:SLF 2018-05-23 ������һ������˳��˵�
                if (MenuRunInfos->CursorMenu->FatherNode == NULL_MENU)
                {
                    //NOTE:SLF 2018-05-23 �˳��˵�������������
                    //NOTE:SLF 2018-05-30 ��ʱ����һ������
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
                    //NOTE:SLF 2018-05-23 ������һ��˵�
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
                    //NOTE:SLF 2018-05-23 ������һ��˵�
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
                        //NOTE:SLF 2018-05-23 û���Ӳ˵������ȷ�����������ҳ��
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
