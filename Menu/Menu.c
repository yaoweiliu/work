
/*******************************************************************************************/
/*include                                                                                                                                                       */
/*******************************************************************************************/
#include "Menu.h"
#include "Menu_If.h"
/*******************************************************************************************/
/*Local Macro and Tyedef                                                                                                                                 */
/*******************************************************************************************/
//--------菜单索引类型
#define NodeTP_BrotherNode_First          1
#define NodeTP_BrotherNode_End           2
#define NodeTP_BrotherNode_Prev         3
#define NodeTP_BrotherNode_Next          4
#define NodeTP_FatherNode           5
#define NodeTP_SonNode_First           6

/*******************************************************************************************/
/*Static Function Declaration                                                                                                                            */
/*******************************************************************************************/

/*******************************************************************************************/
/*Public Function Declaration                                                                                                                             */
/*******************************************************************************************/

/*******************************************************************************************/
/*Local variables                                                                                                                                             */
/*******************************************************************************************/
static MenuRunInfo MenuRunInfos;
/*******************************************************************************************/
/*Global variables                                                                                                                                           */
/*******************************************************************************************/

/*******************************************************************************************/
/*Static Function Implementation                                                                                                                      */
/*******************************************************************************************/
static MenuNode* Menu_SearchNode_ById (unsigned int nodeid)
{
    unsigned char i;
    MenuNode* SearchMenuNode = NULL_MENU;

    for (i = 0; i < MENU_NODES_COUNT; i++)
    {
        SearchMenuNode = &MenuNodes[i];

        if (((*SearchMenuNode).ID == nodeid) && (nodeid != 0))
        {
            //NOTE:SLF 2018-05-29 0号菜单被定义为主界面，不属于菜单系统
            return SearchMenuNode;
        }
        else

        {
            SearchMenuNode = NULL_MENU;
        }
    }

    return SearchMenuNode;
}
static MenuNode* Menu_SearchNode_ByType (unsigned int nodeid, unsigned char searchtype)
{
    unsigned char i;
    MenuNode* ReturnMenuNode = NULL_MENU;
    unsigned int searchnodeid = 0;

    switch (searchtype)
    {
        case NodeTP_BrotherNode_First:
            searchnodeid = nodeid - nodeid % 100 + 1;
            break;

        case NodeTP_BrotherNode_End:
            searchnodeid = nodeid;

            for (i = 0; i < MENU_NODES_COUNT; i++)
            {
                searchnodeid++;
                ReturnMenuNode = Menu_SearchNode_ById (searchnodeid);

                if (ReturnMenuNode == NULL_MENU)
                {
                    searchnodeid -= 1;
                    break;
                }
            }

            break;

        case NodeTP_BrotherNode_Prev:
            searchnodeid = nodeid - 1;
            break;

        case NodeTP_BrotherNode_Next:
            searchnodeid = nodeid + 1;
            break;

        case NodeTP_FatherNode:
            searchnodeid = nodeid / 100;
            break;

        case NodeTP_SonNode_First:
            searchnodeid = nodeid * 100 + 1;
            break;

        default:
            break;
    }

    ReturnMenuNode = Menu_SearchNode_ById (searchnodeid);
    return ReturnMenuNode;
}

/*******************************************************************************************/
/*Public Function Implementation                                                                                                                       */
/*******************************************************************************************/
//--------菜单初始化
void Menu_NodesInit (void)
{
    unsigned char i;
    MenuNode* InitMenuNode;
    memset (&MenuRunInfos, 0x00, sizeof (MenuRunInfo));

    for (i = 0; i < MENU_NODES_COUNT; i++)
    {
        InitMenuNode = &MenuNodes[i];
        (*InitMenuNode).BrotherNode_First = Menu_SearchNode_ByType ((*InitMenuNode).ID, NodeTP_BrotherNode_First);
        (*InitMenuNode).BrotherNode_End = Menu_SearchNode_ByType ((*InitMenuNode).ID, NodeTP_BrotherNode_End);
        (*InitMenuNode).BrotherNode_Prev = Menu_SearchNode_ByType ((*InitMenuNode).ID, NodeTP_BrotherNode_Prev);
        (*InitMenuNode).BrotherNode_Next = Menu_SearchNode_ByType ((*InitMenuNode).ID, NodeTP_BrotherNode_Next);
        (*InitMenuNode).FatherNode = Menu_SearchNode_ByType ((*InitMenuNode).ID, NodeTP_FatherNode);
        (*InitMenuNode).SonNode_First = Menu_SearchNode_ByType ((*InitMenuNode).ID, NodeTP_SonNode_First);

        if (InitMenuNode->SetPageRun != NULL_MENU)
        {
            MenuRunInfos.CursorMenu = InitMenuNode;
            InitMenuNode->Mode = MENU_MODE_QUICKVIEW;
            InitMenuNode->SetPageRun (&MenuRunInfos);//NOTE:SLF 2018-08-10 通过此操作读取一次菜单内的预览值

        }

        InitMenuNode->Mode = MENU_MODE_NORMAL;

#if MENU_DEBUG_CFG
        printf ("%s", MenuNodes[i].Title);

        if (MenuNodes[i].BrotherNode_First != NULL_MENU)
        {
            printf ("BrotherNode_First=%s", MenuNodes[i].BrotherNode_First->Title);
        }

        if (MenuNodes[i].BrotherNode_End != NULL_MENU)
        {
            printf ("BrotherNode_End=%s", MenuNodes[i].BrotherNode_End->Title);
        }

        if (MenuNodes[i].BrotherNode_Prev != NULL_MENU)
        {
            printf ("BrotherNode_Prev=%s", MenuNodes[i].BrotherNode_Prev->Title);
        }

        if (MenuNodes[i].BrotherNode_Next != NULL_MENU)
        {
            printf ("BrotherNode_Next=%s", MenuNodes[i].BrotherNode_Next->Title);
        }

        if (MenuNodes[i].FatherNode != NULL_MENU)
        {
            printf ("FatherNode=%s", MenuNodes[i].FatherNode->Title);
        }

        if (MenuNodes[i].SonNode_First != NULL_MENU)
        {
            printf ("SonNode_First=%s", MenuNodes[i].SonNode_First->Title);
        }

#endif
    }

    MenuRunInfos .CursorMenu = &MenuNodes[MENU_MAIN_NODE_INDEX];
    MenuRunInfos.flg_CurrentPage = MENU_CURRENT_PAGE_MAIN;
}

//--------菜单主操作
void Menu_Loop (void)
{
    Bsp_Remote_Convert();
    MenuRunInfos.KeyCode = &KeyCode;

    if (MenuRunInfos.flg_CurrentPage != MENU_CURRENT_PAGE_SET_PAGE)
    {
        MenuIf_MenuDispatcher (&MenuRunInfos);
    }
    else
    {
        if (MenuRunInfos.CursorMenu != NULL_MENU)
        {
            if (MenuRunInfos.CursorMenu->SetPageRun != NULL_MENU)
            {
                MenuRunInfos.CursorMenu->SetPageRun (&MenuRunInfos);
            }
            else
            {
                //NOTE:SLF 2018-08-06 Nothing
            }
        }
        else
        {
            //NOTE:SLF 2018-08-06 Nothing
        }
    }

    if (*MenuRunInfos.KeyCode != BT_KEYCODE_NONE)
    {
        *MenuRunInfos.KeyCode = BT_KEYCODE_NONE;

        if (MenuRunInfos.flg_CurrentPage != MENU_CURRENT_PAGE_MAIN)
        {
            MenuRunInfos.Timer_SetList = 0;
        }
    }
    else
    {
        if (MenuRunInfos.flg_CurrentPage != MENU_CURRENT_PAGE_MAIN)
        {
            MenuRunInfos.Timer_SetList++;

            if (MenuRunInfos.Timer_SetList > MENU_SETLIST_TIMEOUT_MS / MENU_LOOP_TIME_MS)
            {
                //NOTE:SLF 2018-08-06 设置 超时界面 下一次进入的地方 为默认值或上次退出的值
                //                MenuRunInfos.CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                MenuRunInfos.CursorMenu->Status = MENU_STATUS_HAS_UPATE;

                //NOTE:SLF 2018-05-24 设置超时进入主界面
                MenuRunInfos.flg_CurrentPage = MENU_CURRENT_PAGE_MAIN;
                MenuRunInfos.LastMenu = MenuRunInfos.CursorMenu;
                MenuRunInfos.CursorMenu = &MenuNodes[MENU_MAIN_NODE_INDEX];
                //NOTE:SLF 2018-08-06 设置 主界面 下一次进入的地方 为默认值或上次退出的值
                //                MenuRunInfos.CursorMenu->CurrentItem = MENU_PAGE_ITEM_DEFAULT;
                MenuRunInfos.CursorMenu->Status = MENU_STATUS_HAS_UPATE;
                Screen_Clear (0, 0, 64, 32, SCREEN_COLOR_ALL, SCREEN_UPDATE_NORMAL);
                Setting_Save ();
                MenuRunInfos.CursorMenu->SetPageRun (&MenuRunInfos);
            }
        }
    }
}
///
/// \brief Menu_QuickEntry 快捷进入某一菜单页面内
/// \param PageID
///
void Menu_QuickEntry (unsigned long PageID)
{
    uint16_t  PageIndex = 0;

    for (PageIndex = 0; PageIndex < MENU_NODES_COUNT; ++PageIndex)
    {
        if (MenuNodes[PageIndex].ID == PageID)
        {
            if (MenuNodes[PageIndex].SetPageRun != NULL_MENU)
            {
                if (PageIndex == 0)
                {
                    MenuRunInfos.flg_CurrentPage = MENU_CURRENT_PAGE_MAIN;
                }
                else
                {
                    MenuRunInfos.flg_CurrentPage = MENU_CURRENT_PAGE_SET_PAGE;
                }

                MenuRunInfos.CursorMenu = &MenuNodes[PageIndex];
            }

            Bsp_Screen_Set_Brightness_Immediately (SCREEN_INDEX_TOP, Brightness_GetMaxValue());
            Bsp_Screen_Set_Brightness_Immediately (SCREEN_INDEX_BOTTOM, Brightness_GetMaxValue());
            return;
        }
    }
}
unsigned long Menu_GetCurrentMenuID (void)
{
    return MenuRunInfos.CursorMenu->ID;
}
