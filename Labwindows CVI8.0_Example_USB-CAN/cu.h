/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2009. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  CANTooLS                        1
#define  CANTooLS_quit                   2       /* callback function: quit */
#define  CANTooLS_frame_type             3
#define  CANTooLS_frame_format           4
#define  CANTooLS_frame_id               5
#define  CANTooLS_frame_data             6
#define  CANTooLS_DECORATION             7
#define  CANTooLS_clear                  8       /* callback function: clear */
#define  CANTooLS_COMMANDBUTTON_2        9       /* callback function: send */
#define  CANTooLS_receive                10
#define  CANTooLS_TEXTMSG                11


     /* Menu Bars, Menus, and Menu Items: */

#define  MENUBAR                         1
#define  MENUBAR_Menu1                   2
#define  MENUBAR_Menu1_open              3       /* callback function: open_can */
#define  MENUBAR_Menu1_close             4       /* callback function: close_can */
#define  MENUBAR_MENU2                   5
#define  MENUBAR_MENU2_ITEM4             6       /* callback function: set_can */
#define  MENUBAR_MENU2_ITEM5             7       /* callback function: reset_can */
#define  MENUBAR_MENU2_ITEM1_2           8
#define  MENUBAR_MENU3                   9
#define  MENUBAR_MENU3_ITEM7             10
#define  MENUBAR_MENU3_ITEM6             11
#define  MENUBAR_MENU4                   12
#define  MENUBAR_MENU4_ITEM1             13      /* callback function: clear_can */
#define  MENUBAR_MENU4_ITEM2             14      /* callback function: quit_can */


     /* Callback Prototypes: */ 

int  CVICALLBACK clear(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK clear_can(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK close_can(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK open_can(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK quit_can(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK reset_can(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK send(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK set_can(int menubar, int menuItem, void *callbackData, int panel);


#ifdef __cplusplus
    }
#endif
