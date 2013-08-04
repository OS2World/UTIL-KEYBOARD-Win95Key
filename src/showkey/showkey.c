#include "showkey.h"

HAB habMain;
HWND hwndMain, hwndClient;
HINI hiniFile;

int main (int argc, char * argv[])
{ ULONG CreateFlags=FCF_TITLEBAR|FCF_SIZEBORDER|FCF_DLGBORDER|FCF_SHELLPOSITION|FCF_TASKLIST|FCF_ICON|FCF_AUTOICON|FCF_SYSMENU|FCF_MINMAX|FCF_HIDEBUTTON;
  HMQ hmqMain;
  QMSG Message;
  BOOL bRes;

  habMain=WinInitialize(0);
  if(!habMain)
    { BeepBeep(1);
      return(1);
    }

  hmqMain=WinCreateMsgQueue(habMain, 0);
  if(!hmqMain)
    { BeepBeep(2);
      WinTerminate(habMain);
      return(2);
    }

  bRes=WinRegisterClass(habMain, "ShKyWindoClass", WindowProc, CS_SIZEREDRAW, 0);
  if(!bRes)
    { BeepBeep(3); 
      WinPerror("Error", "Error while registering window class");
      return(3);
    }

  hwndMain=WinCreateStdWindow(HWND_DESKTOP, WS_VISIBLE, &CreateFlags, "ShKyWindoClass", "Key info", 0L, (HMODULE)NULL, 1, &hwndClient);
  if(hwndMain==NULLHANDLE)
    { BeepBeep(4);
      WinPerror("Error", "Error while creating window");
      return(4);
    }

  WinSetWindowText(hwndMain, "Key info");
  WinSetWindowPos(hwndMain, 0, 30, 30, 600, 400, SWP_SIZE);

  while(WinGetMsg(habMain, &Message, (HWND)NULL, 0, 0 ))
   { WinDispatchMsg(habMain, &Message);
   }

  WinDestroyMsgQueue(hmqMain);
  WinTerminate(habMain);

  return(0);
}




