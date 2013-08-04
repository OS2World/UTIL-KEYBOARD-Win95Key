/*(C)'1997-2000 vr vik@avi.kiev.ua*/

#include "..\..\w95k.h"


HWND hwndFolder=NULLHANDLE;

HWND findByClassName(HWND hwnd, char * className)
{ HENUM henum;
  HWND hwnd2;
  HWND hwndFound;
  char tmpClassName[65];

  henum=WinBeginEnumWindows(hwnd);
  
  WinQueryClassName(hwnd, 64, tmpClassName);

  if(strcmp(tmpClassName, "wpFolder window")==0)
    { hwndFolder=hwnd;
    }

  if(strcmpi(tmpClassName, className)==0)
    { return(hwnd);
    }

  while((hwnd2=WinGetNextWindow(henum))!=NULLHANDLE)
   { hwndFound=findByClassName(hwnd2, className);
     if(hwndFound!=NULLHANDLE)
       { return(hwndFound);
       }
   }

  WinEndEnumWindows(henum);
  return(NULLHANDLE);
}



void EXPENTRY SwitchToVirtualDesktop(int argc, char * argv[])
{
  HWND hwndCc;
  HAB habMain;
  HMQ hmqLocal=NULLHANDLE;
  HSWITCH hswCc, hswDesk;
  unsigned long mp1;


  habMain=WinQueryAnchorBlock(HWND_DESKTOP);
  hmqLocal=WinCreateMsgQueue(habMain, 0);

  if(argc<1)
    { WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, "Not enough args", "w95k Object Desktop", 40, MB_OK|MB_MOVEABLE|MB_ERROR);
      if(hmqLocal!=NULLHANDLE)
        { WinDestroyMsgQueue(hmqLocal);
        }
      return;
    }

  switch(argv[0][0])
   { case '+' : mp1=0x7FC3;
                break;
     case '-' : mp1=0x7FC4;
                break;
     case '1' : mp1=0x7FC6;
                break;
     case '2' : mp1=0x7FC7;
                break;
     case '3' : mp1=0x7FC8;
                break;
     case '4' : mp1=0x7FC9;
                break;
     case '5' : mp1=0x7FCA;
                break;
     case '6' : mp1=0x7FCB;
                break;
     case '7' : mp1=0x7FCC;
                break;
     case '8' : mp1=0x7FCD;
                break;
     case '9' : mp1=0x7FCE;
                break;
     case '0' : mp1=0x7FCF;
                break;
     default  : WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, "Invalid argument", "w95k Object Desktop", 40, MB_OK|MB_MOVEABLE|MB_ERROR);
                if(hmqLocal!=NULLHANDLE)
                  { WinDestroyMsgQueue(hmqLocal);
                  }
                return;
   }


  hwndCc=findByClassName(HWND_DESKTOP, "OBJDControlCenterVDesk");
  if(hwndCc==NULLHANDLE)
    { if(hmqLocal!=NULLHANDLE)
        { WinDestroyMsgQueue(hmqLocal);
        }
      return;
    }


  hswCc=WinQuerySwitchHandle(hwndFolder, 0);
  if(hswCc!=NULLHANDLE)
    { WinSwitchToProgram(hswCc);
    }

  WinPostMsg(hwndFolder, WM_COMMAND, MPFROMLONG(mp1), MPFROMLONG(3));

  findByClassName(HWND_DESKTOP, "\x01\x02\x03");
  hswDesk=WinQuerySwitchHandle(hwndFolder, 0);
  WinSwitchToProgram(hswDesk);

  if(hmqLocal!=NULLHANDLE)
    { WinDestroyMsgQueue(hmqLocal);
    }
}


