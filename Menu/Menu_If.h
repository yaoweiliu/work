

#ifndef MENU_IF_H
#define MENU_IF_H
/*******************************************************************************************/
/*include                                                                                                                                                       */
/*******************************************************************************************/
#include "Menu.h"
#include "stdint.h"
#include "Screen.h"
#include "Bsp_Screen.h"
#include "Bsp.h"
#include "Menu_Page.h"
#include "Brightness.h"
#include "Bsp_Remote.h"
#include "Settings.h"
/*******************************************************************************************/
/*Public Macro and Tyedef                                                                                                                               */
/*******************************************************************************************/
#define MENU_NODES_COUNT    47
#define MENU_DEBUG_CFG      0
#define NULL_MENU  ((void*)0)
/*******************************************************************************************/
/*Public Function Declaration                                                                                                                             */
/*******************************************************************************************/
void MenuIf_ListShow (MenuRunInfo* MenuRunInfos);
void MenuIf_MenuDispatcher (MenuRunInfo* MenuRunInfos);
/*******************************************************************************************/
/*Public variables                                                                                                                                           */
/*******************************************************************************************/
extern MenuNode MenuNodes[MENU_NODES_COUNT];

#endif // MENU_IF_H
