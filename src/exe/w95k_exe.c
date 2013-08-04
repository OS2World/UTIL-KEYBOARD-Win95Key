#include "../w95k.h"

HAB habMain;
HWND hwndMain, hwndClient, hwndMenu;
HINI hiniFile;

int main (int argc, char * argv[])
{ ULONG CreateFlags=FCF_TITLEBAR | FCF_SYSMENU | FCF_MENU | FCF_SIZEBORDER | FCF_MINMAX | FCF_ICON | FCF_ACCELTABLE | FCF_TASKLIST ;
  HMQ hmqMain;
  QMSG Message;
  BOOL bRes;

  habMain=WinInitialize(0);
  if(!habMain)
    { BeepBeep(1);
      return(1);
    }

  setbuf(stdout, NULL);
  setbuf(stderr, NULL);
/*  printf("%s\n", VERSION);*/

  hmqMain=WinCreateMsgQueue(habMain, 0);
  if(!hmqMain)
    { BeepBeep(2);
      WinTerminate(habMain);
      return(2);
    }

  bRes=WinRegisterClass(habMain, "Win95keyWindoClass", w95kWindowProc, CS_SIZEREDRAW, 0);
  if(!bRes)
    { BeepBeep(3); 
      WinPerror("Error", "Error while registering window class");
      return(3);
    }


  hwndMain=WinCreateStdWindow(HWND_DESKTOP, WS_ANIMATE, &CreateFlags, "Win95keyWindoClass", "Win95key", 0L, (HMODULE)NULL, 1, &hwndClient);
  if(hwndMain==NULLHANDLE)
    { WinPerror("Error", "Error while creating window");
      return(4);
    }


  hwndMenu=WinWindowFromID(hwndMain, FID_MENU); 
  if(hwndMenu==NULLHANDLE)
    { WinPerror("Error", "Error while querying window's menu");
      WinDestroyMsgQueue(hmqMain);
      WinTerminate(habMain);
      return(6);
    }

  if(w95kStartup()!=0)
    { WinPerror("Error", "Error while initializing application");
      return(7);
    }
    
  while(WinGetMsg(habMain, &Message, (HWND)NULL, 0, 0 ))
   { WinDispatchMsg(habMain, &Message);
   }

  w95kShutdown(1);
  
  
  WinDestroyMsgQueue(hmqMain);
  WinTerminate(habMain);

  return(0);
}




