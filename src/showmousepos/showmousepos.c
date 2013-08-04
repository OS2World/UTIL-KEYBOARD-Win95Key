#define INCL_WIN
#define INCL_DOSMODULEMGR
#define INCL_DOSERRORS
#define INCL_DOSPROCESS
#define INCL_WINSYS
#define INCL_WINFRAMEMGR
#define INCL_WINMENUS
#define INCL_WINERRORS
#define INCL_WINDIALOGS
#define INCL_GPI
#define INCL_WINWINDOWMGR

#include <os2.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>





int findProgram(char * Title, HSWITCH * hSw, HWND * hWn, HAB hab)
{ PSWBLOCK pswBlock;
  int Found=0;
  int Total;
  register int i;

  Total=WinQuerySwitchList(hab,NULL,0);
  pswBlock=(PSWBLOCK) _alloca(Total* sizeof(SWENTRY)+ sizeof(ULONG));
  Total=WinQuerySwitchList(hab, pswBlock, Total* sizeof(SWENTRY)+ sizeof(ULONG));
  if(pswBlock != NULL)
    { for(i=0;i<Total;i++)
       { 
         if(!strncmp(pswBlock->aswentry[i].swctl.szSwtitle, Title, strlen(Title)))
           {  *hWn = pswBlock->aswentry[i].swctl.hwnd;
              *hSw = pswBlock->aswentry[i].hswitch;
              Found=1;
              break;
           }
       }
    }

  if(Found) return(0);
  return(-1);
}


void Query(HAB hab)
{ HSWITCH hsw;
  HWND hwnd;
  SWP swp;
  
  if(findProgram("WarpCenter", &hsw, &hwnd, hab)==0)
    { WinQueryWindowPos(hwnd, &swp);
      printf("x=%d, y=%d, cx=%d, cy=%d\n", swp.x, swp.y, swp.cx, swp.cy);
    }
   else
    { printf("WarpCenter not found\n");
    }

}

MRESULT EXPENTRY MyWindowProc (HWND hWnd, ULONG Msg, MPARAM Mp1, MPARAM Mp2);

HWND  hWin, hWndClient, hwndMenu;
int Distance=0;
HAB Hab;
ULONG TimerId;

void WinPError(char * Title, char * Text)
{ char Buf[1024];
  ERRORID ErrCode;

  ErrCode=WinGetLastError(Hab);
  sprintf(Buf, "%s\nError number is -%Xh", Text, (int)(ErrCode&0x7FFF));

  WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, Buf, Title, 11,
                    MB_OK | MB_APPLMODAL | MB_MOVEABLE | MB_ERROR);
  
}

int main (void)
{ QMSG  qMsg;
  BOOL Res;
  HMQ Hmq;
/*  ULONG frmFlag=FCF_TITLEBAR | FCF_SYSMENU | FCF_SIZEBORDER | FCF_MINMAX | FCF_ICON | FCF_SHELLPOSITION | FCF_TASKLIST;*/
  ULONG frmFlag=FCF_BORDER|FCF_SIZEBORDER|FCF_SHELLPOSITION|FCF_TASKLIST|FCF_ICON|FCF_AUTOICON;
  UCHAR ErrorMsg[512]={0};
  HMODULE hModule;
  PFN Proc=0;
  PFN Reg=0;

  UCHAR Code[512];
  int iRes;


  Hab=WinInitialize(0);
  if(Hab==NULLHANDLE)
    { DosBeep(500, 500);
      exit(1);
    }

  Hmq=WinCreateMsgQueue(Hab,0);
  if(Hmq==NULLHANDLE)
    { DosBeep(900, 500);
      WinTerminate(Hab);
      exit(1);
    }

  Res=WinRegisterClass(           /* Register window class        */
       Hab,                       /* Anchor block handle          */
       "shmou",                   /* Window class name            */
       MyWindowProc,              /* Address of window procedure  */
       CS_SIZEREDRAW,             /* Class style                  */
       0                          /* No extra window words        */
       );
  if(!Res)
    { DosBeep(500, 500);
      DosBeep(200, 500);
      exit(1);
    }


  hWin=WinCreateStdWindow(
        HWND_DESKTOP,                   /* Desktop window is parent     */
        WS_ANIMATE,                     /* Frame style                  */
        &frmFlag,                       /* Frame control flag           */
        "shmou",                        /* Window class name            */
        "Mouse pos",                    /* Window title                 */
        0L,                             /* Client style of VISIBLE      */
        (HMODULE)NULL,                  /* Resource is in .EXE file     */
        1,                              /* Frame window identifier      */
        &hWndClient);                   /* Client window handle         */

  if(!hWin)
    { WinPError("Error", "Can't create window");
      exit(1);
    }

  WinSetWindowPos(hWin, hWin, 0, 0, 160, 27, SWP_SIZE|SWP_SHOW);
  WinSetWindowText(hWin, "Mouse pos");
  hwndMenu=WinLoadMenu(hWndClient, NULLHANDLE, 100);


  WinPostMsg(hWin, WM_SHOW, MPFROMLONG(0l), MPFROMLONG(0l));
  TimerId=WinStartTimer(Hab, hWndClient, 5, 10l);
  Query(Hab);

  while(WinGetMsg(Hab, &qMsg, (HWND)NULL, 0, 0 ))
   {  WinDispatchMsg(Hab, &qMsg);
   }

  WinStopTimer(Hab, hWndClient, TimerId);
  WinDestroyMsgQueue(Hmq);
  WinTerminate(Hab);


  return(0);
}

MRESULT EXPENTRY MyWindowProc (HWND hWnd, ULONG Msg, MPARAM Mp1, MPARAM Mp2)
{ HPS hPresSpace;
  RECTL WinRect;
  RECTL fWinRect;
  char Text[128];
  char * FontName="8.Arial";
  POINTL MousePos;
  SWP WindowPos;
  
  static POINTL oldMousePos={0, 0};

  switch(Msg)
   { case WM_TIMER       : WinQueryPointerPos(HWND_DESKTOP, &MousePos);
                           if((MousePos.x!=oldMousePos.x) || (MousePos.y!=oldMousePos.y))
                             { oldMousePos.x=MousePos.x;
                               oldMousePos.y=MousePos.y;
                               sprintf(Text, "x=[%4.4d] y=[%4.4d]", oldMousePos.x, oldMousePos.y);
                               WinSetPresParam(hWndClient, PP_FONTNAMESIZE, strlen(FontName), FontName);
                               hPresSpace=WinGetPS(hWndClient);
                               WinQueryWindowRect(hWndClient, &WinRect);
                               WinFillRect(hPresSpace, &WinRect, SYSCLR_DIALOGBACKGROUND);
                               WinDrawText(hPresSpace, strlen(Text), Text, &WinRect, SYSCLR_WINDOWSTATICTEXT, SYSCLR_DIALOGBACKGROUND, DT_CENTER|DT_VCENTER|DT_ERASERECT);
                               WinReleasePS(hPresSpace);
                             }
                           break;
     case WM_CLOSE:
                           WinPostMsg(hWnd, WM_QUIT, NULL, NULL);
                           break;


     case WM_BUTTON1DOWN : WinPostMsg(hWin, WM_TRACKFRAME, MPFROMSHORT(TF_MOVE), MPFROMLONG(0));
                           break;
     case WM_BUTTON2DOWN :
     case WM_CONTEXTMENU :
                           WinQueryPointerPos(HWND_DESKTOP, &MousePos);
                           WinQueryWindowPos(hWin, &WindowPos);
                           WinPopupMenu(hWin, hWin, hwndMenu, MousePos.x-WindowPos.x, MousePos.y-WindowPos.y, 0, PU_KEYBOARD|PU_MOUSEBUTTON1);
                           break;

     case WM_CREATE      : WinSetPresParam(hWnd, PP_FONTNAMESIZE, strlen(FontName), FontName);
                           WinSetPresParam(hWndClient, PP_FONTNAMESIZE, strlen(FontName), FontName);
                           
     case WM_PAINT       : WinSetPresParam(hWndClient, PP_FONTNAMESIZE, strlen(FontName), FontName);
     
                           hPresSpace=WinBeginPaint(hWndClient, (HPS)NULL, &WinRect);
                           sprintf(Text, "x=[%4.4d] y=[%4.4d]", oldMousePos.x, oldMousePos.y);
                           
                           WinFillRect(hPresSpace, &WinRect, SYSCLR_DIALOGBACKGROUND);
                           WinQueryWindowRect(hWndClient, &WinRect);
                           WinDrawText(hPresSpace, strlen(Text), Text, &WinRect, SYSCLR_WINDOWSTATICTEXT, SYSCLR_DIALOGBACKGROUND, DT_CENTER|DT_VCENTER|DT_ERASERECT);
                           WinEndPaint(hPresSpace);
                           break;

     case WM_COMMAND     : switch(SHORT1FROMMP(Mp1))
                            { case 901 : WinPostMsg(hWin, WM_TRACKFRAME, MPFROMSHORT(TF_MOVE), MPFROMLONG(0));
                                         break;
                              case 902 : WinPostMsg(hWin, WM_QUIT, NULL, NULL);
                                         break;
                              case 903 : WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, "Mouse position", "About", 11,
                                                       MB_OK | MB_APPLMODAL | MB_MOVEABLE | MB_INFORMATION);
                                         break;

                            }
                           break;
     default:
                           return (WinDefWindowProc( hWnd, Msg, Mp1, Mp2 ));
   }

  return (MRESULT) FALSE;
}







