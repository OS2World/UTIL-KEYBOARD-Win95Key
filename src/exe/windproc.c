#include "../w95k.h"



MRESULT EXPENTRY w95kWindowProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{ HPS hPresSpace;
  static int First=1;
  POWNERITEM pItemOwner;
  char * Title="Win95key";
  RECTL itemRect, winRect;
  SWP menuPos;
  
  switch(msg)
   { case WM_MEASUREITEM : pItemOwner=(POWNERITEM)mp2;
                           pItemOwner->rclItem.yTop=25;
                           pItemOwner->rclItem.xRight=10;
                           break;

     case WM_DRAWITEM    : pItemOwner=(POWNERITEM)mp2;
                           
                           memcpy(&itemRect, &pItemOwner->rclItem, sizeof(RECTL));
                           WinQueryWindowPos(hwndSysSubMenu, &menuPos);
                           
                           itemRect.xLeft=1;
                           itemRect.xRight=menuPos.cx-1;
                           WinFillRect(pItemOwner->hps, &itemRect, SYSCLR_MENU);
                           
                           itemRect.xLeft=1;
                           itemRect.xRight=2;
                           WinFillRect(pItemOwner->hps, &itemRect, SYSCLR_BUTTONLIGHT);
                           
                           itemRect.xLeft=menuPos.cx-2;
                           itemRect.xRight=menuPos.cx-1;
                           WinFillRect(pItemOwner->hps, &itemRect, SYSCLR_BUTTONDARK);
                           
                           /****************************/
                           
                           memcpy(&itemRect, &pItemOwner->rclItem, sizeof(RECTL));
                           itemRect.xLeft+=2;
                           itemRect.xRight-=1;
                           itemRect.yBottom+=3;
                           
                           WinFillRect(pItemOwner->hps, &itemRect, SYSCLR_BUTTONLIGHT);
                           
                           itemRect.xRight--;
                           itemRect.yBottom++;
                           WinFillRect(pItemOwner->hps, &itemRect, SYSCLR_BUTTONDARK);
                           itemRect.xRight++;
                           itemRect.yBottom--;
                           
                           itemRect.xRight--;
                           itemRect.yBottom++;
                           itemRect.xLeft++;
                           itemRect.yTop--;
                           
                           WinFillRect(pItemOwner->hps, &itemRect, SYSCLR_MENUHILITEBGND);
                           WinDrawText(pItemOwner->hps, strlen(Title), Title, &itemRect, SYSCLR_MENUHILITE, SYSCLR_MENUHILITEBGND, DT_CENTER|DT_VCENTER);
                           return((MRESULT)TRUE);

     case WM_USER+10     : if(ProcessKey(mp1, mp2)==0) /*We've found event*/
                             { return((MRESULT)TRUE);
                             }
                           break;
                           
     case WM_USER+11     : if(w95kStartup()!=0)
                             { WinPerror("Can't re-read config", "Can't read settings");
                               WinPostMsg(hwndMain, WM_QUIT, NULL, NULL);
                             }
                           break;

     case WM_COMMAND     : switch(SHORT1FROMMP(mp1))
                           { case MENU_REREADCFG : if(w95kShutdown()!=0)
                                                     { WinPerror("Can't re-read config", "Can't remove old settings");
                                                     }
                                                   if(w95kStartup()!=0)
                                                     { WinPerror("Can't re-read config", "Can't read settings");
                                                       WinPostMsg(hwndMain, WM_QUIT, NULL, NULL);
                                                     }
                                                   break;
                                                   
                             case MENU_PRESS     : mainParams.onKeyPress=!mainParams.onKeyPress;
                                                   WinCheckMenuItem(hwndMenu, MENU_PRESS, mainParams.onKeyPress);
                                                   WinCheckMenuItem(hwndSysSubMenu, MENU_PRESS, mainParams.onKeyPress);
                                                   break;
                                                   
                             case MENU_HIDE      : mainParams.Hide=!mainParams.Hide;
                                                   WinCheckMenuItem(hwndMenu, MENU_HIDE, mainParams.Hide);
                                                   WinCheckMenuItem(hwndSysSubMenu, MENU_HIDE, mainParams.Hide);
                                                   if(w95kHide(mainParams.Hide)!=0)
                                                     { WinPostMsg(hwndMain, WM_QUIT, NULL, NULL);
                                                     }
                                                   break;
                                                   
                             case MENU_EXIT      : WinPostMsg(hwndMain, WM_QUIT, NULL, NULL);
                                                   break;
                                                   
                             case MENU_ABOUT     : WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, VERSION, "Win95key 2.0", 55,
                                                      MB_OK | MB_APPLMODAL | MB_MOVEABLE | MB_INFORMATION);
                                                   break;
                           }
                           break;
     case WM_CLOSE       : WinPostMsg(hwndMain, WM_QUIT, NULL, NULL);
                           break;
     case WM_CREATE      :      
     case WM_PAINT       : hPresSpace=WinBeginPaint(hwndClient, (HPS)NULL, &winRect);
                           WinFillRect(hPresSpace, &winRect, SYSCLR_APPWORKSPACE);
                           WinEndPaint(hPresSpace);
                           break;
     default             : return(WinDefWindowProc(hwnd, msg, mp1, mp2));
   }

  return((MRESULT)FALSE);
}



