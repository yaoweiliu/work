

#ifndef MENU_H
#define MENU_H
/*******************************************************************************************/
/*include                                                                                                                                                       */
/*******************************************************************************************/

/*******************************************************************************************/
/*Public Macro and Tyedef                                                                                                                               */
/*******************************************************************************************/
//--------菜单节点结构体
typedef struct _MenuNode
{
    const unsigned char* Title;
    unsigned long int ID: 26;
    unsigned long int Mode: 1;
    unsigned long int Status: 1;
    unsigned long int CurrentItem: 4;
    char* Message_String;
    struct _MenuNode* BrotherNode_First;
    struct _MenuNode* BrotherNode_End;
    struct _MenuNode* BrotherNode_Prev;
    struct _MenuNode* BrotherNode_Next;
    struct _MenuNode* FatherNode;
    struct _MenuNode* SonNode_First;
    void (*SetPageRun) (void* Para);
} MenuNode;
typedef struct _MenuRunInfo
{
    MenuNode* CursorMenu;
    MenuNode* LastMenu;
    volatile unsigned long int* KeyCode;
    ///值范围 MENU_CURRENT_PAGE_
    unsigned long int flg_CurrentPage: 2;
    ///超时计数器
    unsigned long int Timer_SetList: 30;
    unsigned long int Timer_PageHold;
    unsigned long int Timer_Timerout;
} MenuRunInfo;

///以下宏不能修改
#define MENU_MAIN_NODE_INDEX    0
#define MENU_FIRST_MENU_NODE_INDEX    1

#define MENU_CURRENT_PAGE_MAIN       0
#define MENU_CURRENT_PAGE_SET_LIST       1
#define MENU_CURRENT_PAGE_SET_PAGE       2

#define MENU_PAGE_ITEM_DEFAULT       0

#define MENU_STATUS_NORMAL       0
#define MENU_STATUS_HAS_UPATE       1

#define MENU_MODE_NORMAL       0
#define MENU_MODE_QUICKVIEW       1

#define MENU_SETLIST_TIMEOUT_MS      10000
#define MENU_LOOP_TIME_MS             100
/*******************************************************************************************/
/*Public Function Declaration                                                                                                                             */
/*******************************************************************************************/
void Menu_NodesInit (void);
void Menu_Loop (void);
void Menu_QuickEntry (unsigned long int  PageID);
unsigned long Menu_GetCurrentMenuID (void);
/*******************************************************************************************/
/*Public variables                                                                                                                                           */
/*******************************************************************************************/


#endif // MENU_H
