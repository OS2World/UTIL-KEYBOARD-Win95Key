/*(C)'1997-2000 vr vik@avi.kiev.ua*/

#include "..\..\w95k.h"


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
       { if(!memcmp(pswBlock->aswentry[i].swctl.szSwtitle, Title, strlen(Title)))
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


int Send2Class(ULONG msg, MPARAM mp1, MPARAM mp2, HWND hwnd, char * reqName)
{ HENUM henum;
  HWND hwnd2;
  char ClassName[101];

  henum=WinBeginEnumWindows(hwnd);
  
  WinQueryClassName(hwnd, 100, ClassName);

  if(strcmpi(ClassName, reqName)==0)
    { WinPostMsg(hwnd, msg, mp1, mp2);
      return(0);
    }

  while((hwnd2=WinGetNextWindow(henum))!=NULLHANDLE)
   { if(Send2Class(msg, mp1, mp2, hwnd2, reqName)==0)
       { WinEndEnumWindows(henum);
         return(0);
       }
   }
  WinEndEnumWindows(henum);
  return(1);
}



void EXPENTRY SwitchTo(int argc, char * argv[])
{ HSWITCH hswPrg;
  HWND    hwndPrg;
  HMQ hmqLocal=NULLHANDLE;
  HAB hab;

  if(argc<1) return;

  hab=WinQueryAnchorBlock(HWND_DESKTOP);
  hmqLocal=WinCreateMsgQueue(hab, 0);

  if(findProgram(argv[0], &hswPrg, &hwndPrg, hab)==0)
    { WinSwitchToProgram(hswPrg);
    }

  if(hmqLocal!=NULLHANDLE)
    { WinDestroyMsgQueue(hmqLocal);
    }
}


void EXPENTRY WindowList(int argc, char * argv[])
{ HMQ hmqLocal=NULLHANDLE;
  PSWBLOCK pswBlock;
  int Total;
  HAB hab;

  hab=WinQueryAnchorBlock(HWND_DESKTOP);
  hmqLocal=WinCreateMsgQueue(hab, 0);

  Total=WinQuerySwitchList(hab,NULL,0);
  pswBlock=(PSWBLOCK) _alloca(Total* sizeof(SWENTRY)+ sizeof(ULONG));
  Total=WinQuerySwitchList(hab, pswBlock, Total* sizeof(SWENTRY)+ sizeof(ULONG));
  if(pswBlock != NULL)
    { WinSwitchToProgram(pswBlock->aswentry[0].hswitch);
    }

  if(hmqLocal!=NULLHANDLE)
    { WinDestroyMsgQueue(hmqLocal);
    }
}


void EXPENTRY DesktopMenu(int argc, char * argv[])
{ HMQ hmqLocal=NULLHANDLE;
  HAB hab;
  HSWITCH hswPrg;
  HWND    hwndPrg;
  register int i;
  char * DesktopNames[]={ "Desktop",
                          " ¡®ç¨© áâ®«",
                          "\x83\x66\x83\x58\x83\x4E\x83\x67\x83\x62\x83\x76",
                          "Escritorio",
                          "Werkplek",
                          "Arbeitsoberfl„che",
                          "Area di lavoro",
                          "Scrivania",
                          NULL
                        };
                        
  hab=WinQueryAnchorBlock(HWND_DESKTOP);
  hmqLocal=WinCreateMsgQueue(hab, 0);

  if(argc>0)
    { if(findProgram(argv[0], &hswPrg, &hwndPrg, hab)==0)
        { WinPostMsg(hwndPrg, WM_CONTEXTMENU, MPFROM2SHORT(1, 1), MPFROMLONG(0));
        }
    }
   else
    { for(i=0;DesktopNames[i]!=NULL;i++)
       { if(findProgram(DesktopNames[i], &hswPrg, &hwndPrg, hab)==0)
           { /*WinSetPointerPos(HWND_DESKTOP, 20, 20);*/
             WinPostMsg(hwndPrg, WM_CONTEXTMENU, MPFROM2SHORT(1, 1), MPFROMLONG(0));
           }
       }

    }
  if(hmqLocal!=NULLHANDLE)
    { WinDestroyMsgQueue(hmqLocal);
    }
}


void EXPENTRY ContextMenu(int argc, char * argv[])
{ HMQ hmqLocal=NULLHANDLE;
  HAB hab;
  HWND hWin, hOld;
  register int i;
  SWP swp;

  hab=WinQueryAnchorBlock(HWND_DESKTOP);
  hmqLocal=WinCreateMsgQueue(hab, 0);

  hOld=NULLHANDLE;
  hWin=HWND_DESKTOP;
  i=0;
  while( hWin != NULLHANDLE)
   { if(hOld==hWin) break;
     hOld=hWin;
     hWin=WinQueryFocus(hWin);
     i++;
     if(i>256)
       { break;
       }
   }

  WinQueryWindowPos(hOld, &swp);
/*  WinSetPointerPos(HWND_DESKTOP, swp.x+(swp.cx/3), swp.y+(swp.cy/3));*/

  if(Send2Class(WM_BUTTON2CLICK, MPFROM2SHORT(10, 10), MPFROM2SHORT(0, KC_CTRL|KC_SHIFT|KC_ALT), hOld, "Shield")!=0)
    { WinSendMsg(hOld, WM_CONTEXTMENU, MPFROM2SHORT(0,0), MPFROM2SHORT(0, TRUE));
    }

}


/******************************************************************************/
/**            ****************************************************************/
/** WarpCenter ****************************************************************/
/**            ****************************************************************/
/******************************************************************************/

/* 10x to Sergey Yevtushenko */
#define SMALLCENTER 22
#define BIGCENTER   26
int WarpCenterPoints[2][8]={ {10, 52, 75, 100, 125, 160, 320, 0},
                             {10, 65, 90, 115, 145, 180, 380, 0}};

void EXPENTRY WarpCenterMenu(int argc, char * argv[])
{ HMQ hmqLocal=NULLHANDLE;
  HAB hab;
  SWP swp;
  HSWITCH hswPrg;
  HWND    hwndPrg;
  int ourPoint=-1;
  ULONG msgId1=WM_BUTTON1DOWN;
  ULONG msgId2=WM_BUTTON1UP;
  int firstIndex=0;
  POINTL pPos;
  int MovePointer=1;

  char str[1111];

  hab=WinQueryAnchorBlock(HWND_DESKTOP);
  hmqLocal=WinCreateMsgQueue(hab, 0);


  if(argc<1)
    { WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, "Error : required argument missing", "WarpCenterMenu", 9, MB_OK | MB_MOVEABLE | MB_INFORMATION);
      if(hmqLocal!=NULLHANDLE)
        { WinDestroyMsgQueue(hmqLocal);
        }
      return;
    }

  if(strcmpi(argv[0], "menu")==0)       { ourPoint=0; MovePointer=1; }
  if(strcmpi(argv[0], "windowlist")==0) { ourPoint=1; MovePointer=1; }
  if(strcmpi(argv[0], "lockup")==0)     { ourPoint=2; MovePointer=0; }
  if(strcmpi(argv[0], "find")==0)       { ourPoint=3; MovePointer=0; }
  if(strcmpi(argv[0], "shutdown")==0)   { ourPoint=4; MovePointer=0; }
  if(strcmpi(argv[0], "info")==0)       { ourPoint=5; MovePointer=0; }
  if(strcmpi(argv[0], "trays")==0)      { ourPoint=6; MovePointer=1; }
  if(strcmpi(argv[0], "clock")==0)      { ourPoint=7; MovePointer=0; }
  if(strcmpi(argv[0], "properties")==0)
    { ourPoint=0;
      msgId1=WM_CONTEXTMENU;
      msgId2=0;
      MovePointer=1;
    }

  if(argc>1)
    { if(strcmpi(argv[1], "nomove")==0)
        { MovePointer=0;
        }
      if(strcmpi(argv[1], "move")==0)
        { MovePointer=1;
        }
    }

  if(ourPoint<0)
    { WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, argv[1], "WarpCenterMenu : unrecognized command", 9, MB_OK | MB_MOVEABLE | MB_INFORMATION);
    }
   else
    { if(findProgram("WarpCenter", &hswPrg, &hwndPrg, hab)==0)
        { if(MovePointer)
            { WinSwitchToProgram(hswPrg);
            }
          WinQueryWindowPos(hwndPrg, &swp);
          if(swp.cy==SMALLCENTER)
            { firstIndex=0;
            }
           else
            { firstIndex=1;
            }

          /*Clock*/
          WarpCenterPoints[firstIndex][7]=swp.cx-2;

          WinQueryPointerPos(HWND_DESKTOP, &pPos);
          WinSetPointerPos(HWND_DESKTOP, WarpCenterPoints[firstIndex][ourPoint], 10+swp.y);
          WinPostMsg(hwndPrg, msgId1, MPFROM2SHORT(WarpCenterPoints[firstIndex][ourPoint], 10), MPFROMLONG(0));
          if(msgId2) WinPostMsg(hwndPrg, msgId2, MPFROM2SHORT(WarpCenterPoints[firstIndex][ourPoint], 10), MPFROMLONG(0));
          if(MovePointer)
            { WinSetPointerPos(HWND_DESKTOP, pPos.x, pPos.y);
            }
        }
       else
        { WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, "WarpCenter not found", "WarpCenterMenu", 27, MB_OK | MB_MOVEABLE | MB_INFORMATION);
        }
    }

  if(hmqLocal!=NULLHANDLE)
    { WinDestroyMsgQueue(hmqLocal);
    }

}








int cmpSwEntry(SWENTRY * se1, SWENTRY * se2)
{ return(se1->swctl.hwnd-se2->swctl.hwnd);
}

/* 8-0 */
#define OPEN_UNKNOWN      -1
#define OPEN_DEFAULT       0
#define OPEN_CONTENTS      1
#define OPEN_SETTINGS      2
#define OPEN_HELP          3
#define OPEN_RUNNING       4
#define OPEN_PROMPTDLG     5
#define OPEN_TREE          101
#define OPEN_DETAILS       102
#define OPEN_PALETTE       121                                   /*SPLIT*/
#define OPEN_USER          0x6500

void EXPENTRY OpenObject(int argc, char * argv[])
{ HMQ hmqLocal=NULLHANDLE;
  HOBJECT Obj;
  ULONG OpenMode;
  HAB hab;
  PSWBLOCK pswBefore, pswAfter;
  int swCountBefore, swCountAfter;
  register int i, j;
  BOOL useExistingView=TRUE;
  
  /* (What, Mode) */
  
  hab=WinQueryAnchorBlock(HWND_DESKTOP);
  hmqLocal=WinCreateMsgQueue(hab, 0);

  if(argc<1)
    { if(hmqLocal!=NULLHANDLE)
        { WinDestroyMsgQueue(hmqLocal);
        }
      return;
    }
    
  if(argc>2)
    { if(strcmpi(argv[2], "NEWVIEW")==0)
        { useExistingView=FALSE;
        }
    }
    
  OpenMode=OPEN_DEFAULT;
  
  if(argc>1)
    { if(strcmpi(argv[1], "HELP")==0)
        { OpenMode=OPEN_HELP;
        }
      if(strcmpi(argv[1], "PALETTE")==0)
        { OpenMode=OPEN_PALETTE;
        }
      if(strcmpi(argv[1], "PROMPTDLG")==0)
        { OpenMode=OPEN_PROMPTDLG;
        }
      if(strcmpi(argv[1], "RUNNING")==0)
        { OpenMode=OPEN_RUNNING;
        }
      if(strcmpi(argv[1], "SETTINGS")==0)
        { OpenMode=OPEN_SETTINGS;
        }
      if(strcmpi(argv[1], "TREE")==0)
        { OpenMode=OPEN_TREE;
        }
      if(strcmpi(argv[1], "CONTENTS")==0)
        { OpenMode=OPEN_CONTENTS;
        }
      if(strcmpi(argv[1], "DETAILS")==0)
        { OpenMode=OPEN_DETAILS;
        }
    }

  Obj=WinQueryObject(argv[0]);
  if(Obj!=NULLHANDLE)
    { swCountBefore=WinQuerySwitchList(hab,NULL,0);
      pswBefore=(PSWBLOCK)_alloca(swCountBefore*sizeof(SWENTRY)+sizeof(ULONG));
      swCountBefore=WinQuerySwitchList(hab, pswBefore, swCountBefore*sizeof(SWENTRY)+sizeof(ULONG));

      WinOpenObject(Obj, OpenMode, useExistingView);

      qsort(pswBefore->aswentry, swCountBefore, sizeof(SWENTRY), (int(*)(const void*, const void*))cmpSwEntry);

      swCountAfter=WinQuerySwitchList(hab,NULL,0);
      pswAfter=(PSWBLOCK)_alloca(swCountAfter*sizeof(SWENTRY)+sizeof(ULONG));
      swCountAfter=WinQuerySwitchList(hab, pswAfter, swCountAfter*sizeof(SWENTRY)+sizeof(ULONG));

      qsort(pswAfter->aswentry, swCountAfter, sizeof(SWENTRY), (int(*)(const void*, const void*))cmpSwEntry);

      for(i=0;i<swCountAfter;i++)
       { if(i>=swCountBefore)
           { break;
           }
         if(pswAfter->aswentry[i].swctl.hwnd != pswBefore->aswentry[i].swctl.hwnd)
           { 
/*             WinSetFocus(HWND_DESKTOP, pswAfter->aswentry[i].swctl.hwnd);*/
             WinSetWindowPos(pswAfter->aswentry[i].swctl.hwnd, HWND_TOP, 0, 0, 0, 0, SWP_ZORDER|SWP_ACTIVATE);
/*                WinPostMsg(pswAfter->aswentry[i].swctl.hwnd, WM_ACTIVATE, MPFROMLONG(TRUE), MPFROMLONG(pswAfter->aswentry[i].swctl.hwnd));
                WinPostMsg(pswAfter->aswentry[i].swctl.hwnd, WM_SHOW, MPFROMLONG(0), MPFROMLONG(0));*/
             break;
           }
       }
    }
   else
    { WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, argv[0], "Open Object : can't get object handle", 27, MB_OK | MB_MOVEABLE | MB_INFORMATION);
    }

  if(hmqLocal!=NULLHANDLE)
    { WinDestroyMsgQueue(hmqLocal);
    }

}




void EXPENTRY Volume(int argc, char * argv[])
{ HMQ hmqLocal=NULLHANDLE;
  HAB hab;
  HSWITCH hswPrg;
  HWND    hwndPrg;
  int isUp=0;
  char * ObjectName="<MMPM2_MMVOLUME>";
  
  hab=WinQueryAnchorBlock(HWND_DESKTOP);
  hmqLocal=WinCreateMsgQueue(hab, 0);

  if(argc>0)
    { if(strcmpi(argv[0], "up")==0)
        { isUp=1;
        }
    }
    
    
  if(findProgram("Volume", &hswPrg, &hwndPrg, hab)!=0)
    { OpenObject(1, &ObjectName);
    }
  if(findProgram("Volume", &hswPrg, &hwndPrg, hab)==0)
    { if(isUp)
        { Send2Class(WM_CHAR, MPFROMLONG(0x64011006), MPFROMLONG(0x174DE0), hwndPrg, "#68");
          Send2Class(WM_CHAR, MPFROMLONG(0x64011146), MPFROMLONG(0x174DE0), hwndPrg, "#68");
        }
       else
        { Send2Class(WM_CHAR, MPFROMLONG(0x4B011006), MPFROMLONG(0x154B00), hwndPrg, "#68");
          Send2Class(WM_CHAR, MPFROMLONG(0x4B011146), MPFROMLONG(0x154B00), hwndPrg, "#68");
        }
    }
   else
    { /*Change via MCI*/
      ;
    }

  if(hmqLocal!=NULLHANDLE)
    { WinDestroyMsgQueue(hmqLocal);
    }
}

void EXPENTRY MinimizeAll(int argc, char * argv[])
{ HMQ hmqLocal=NULLHANDLE;
  HAB hab;
  PSWBLOCK pswBlk;
  SWP swp;
  int swCount;
  register int i, j;
  int Exception;

  hab=WinQueryAnchorBlock(HWND_DESKTOP);
  hmqLocal=WinCreateMsgQueue(hab, 0);

  swCount=WinQuerySwitchList(hab,NULL,0);
  pswBlk=(PSWBLOCK)_alloca(swCount*sizeof(SWENTRY)+sizeof(ULONG));
  swCount=WinQuerySwitchList(hab, pswBlk, swCount*sizeof(SWENTRY)+sizeof(ULONG));

  for(i=0;i<swCount;i++)
   { Exception=0;
     for(j=0;j<argc;j++)
      { if(strnicmp(argv[j], pswBlk->aswentry[i].swctl.szSwtitle, strlen(argv[j])>strlen(pswBlk->aswentry[i].swctl.szSwtitle)?strlen(argv[j]):strlen(pswBlk->aswentry[i].swctl.szSwtitle))==0)
          { Exception=1;
            break;
          }
      }
     if(!Exception)
       { if(WinQueryWindowPos(pswBlk->aswentry[i].swctl.hwnd, &swp))
           { if((!(swp.fl&SWP_MINIMIZE)) && (!(swp.fl&SWP_HIDE)))
               { WinSendMsg(pswBlk->aswentry[i].swctl.hwnd, WM_SYSCOMMAND, MPFROMSHORT(SC_MINIMIZE), MPFROM2SHORT(CMDSRC_OTHER, FALSE));
               }
           }
       }
   }

  if(hmqLocal!=NULLHANDLE)
    { WinDestroyMsgQueue(hmqLocal);
    }
}

