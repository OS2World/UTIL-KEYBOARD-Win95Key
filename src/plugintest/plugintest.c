#include "../w95k.h"

HAB habMain;
HWND hwndMain, hwndClient;
HINI hiniFile;
ULONG TimerId;

MRESULT EXPENTRY w95kTestWindowProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
int LoadTestDll(char * moduleName, char * ProcName, int argCount, char * Args[]);
VOID APIENTRY Start(void);

char *** Argv;
int Argc;

int main (int argc, char * argv[])
{ ULONG CreateFlags=FCF_TITLEBAR|FCF_SIZEBORDER|FCF_DLGBORDER|FCF_SHELLPOSITION|FCF_TASKLIST|FCF_ICON|FCF_AUTOICON|FCF_SYSMENU|FCF_MINMAX|FCF_HIDEBUTTON;
  HMQ hmqMain;
  QMSG Message;
  BOOL bRes;
  TID tidFoo;
  
  Argv=&argv;
  Argc=argc;
  
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

  bRes=WinRegisterClass(habMain, "Win95keyTestClass", w95kTestWindowProc, CS_SIZEREDRAW, 0);
  if(!bRes)
    { BeepBeep(3); 
      WinPerror("Error", "Error while registering window class");
      return(3);
    }

  hwndMain=WinCreateStdWindow(HWND_DESKTOP, 0, &CreateFlags, "Win95keyTestClass", "Dll test", 0L, (HMODULE)NULL, 1, &hwndClient);
  if(hwndMain==NULLHANDLE)
    { BeepBeep(4);
      WinPerror("Error", "Error while creating window");
      return(4);
    }

  WinSetWindowText(hwndMain, "Plugin tester for Win95key");

  
  if(argc<3)
    { WinPerror("Error", "Usage : plugintest <dll name> <foo name> [parameters]");
      WinDestroyMsgQueue(hmqMain);
      WinTerminate(habMain);
      return(0);
    }
  
  TimerId=WinStartTimer(habMain, hwndClient, 5, 1000l);
  
  DosCreateThread(&tidFoo, (PFNTHREAD)Start, NULL, 0, 65535);
  
  
  while(WinGetMsg(habMain, &Message, (HWND)NULL, 0, 0 ))
   {  WinDispatchMsg(habMain, &Message);
   }

  WinStopTimer(habMain, hwndClient, TimerId);
  
  
  WinDestroyMsgQueue(hmqMain);
  WinTerminate(habMain);

  return(0);
}

VOID APIENTRY Start(void)
{ DosSleep(1000);
  LoadTestDll((*Argv)[1], (*Argv)[2], Argc-3, &(*Argv)[3]);
}

MRESULT EXPENTRY w95kTestWindowProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{ HPS hPresSpace;
  RECTL WinRect;
  static int Count=0;

  switch(msg)
   { case WM_TIMER       : Count++;
                           if(Count<10) break;
     case WM_CLOSE       : WinPostMsg(hwndMain, WM_QUIT, NULL, NULL);
                           break;
     case WM_PAINT       : hPresSpace=WinBeginPaint(hwndClient, (HPS)NULL, &WinRect);
                           WinFillRect(hPresSpace, &WinRect, SYSCLR_DIALOGBACKGROUND);
                           WinEndPaint(hPresSpace);
                           break;
     default             : return(WinDefWindowProc(hwnd, msg, mp1, mp2));
   }

  return((MRESULT)FALSE);
}




int LoadTestDll(char * moduleName, char * ProcName, int argCount, char * Args[])
{ int Res;
  char ErrorBuffer[256];
  HMODULE hModule=NULLHANDLE;
  PFN pFoo=NULL;
  int Number;

  Res=DosLoadModule(ErrorBuffer, 200, moduleName, &hModule);
  if(Res!=0)
    { strcat(ErrorBuffer, " - can't load module");
      WinPerror("Error loading module", ErrorBuffer);
      return(-1);
    }

  
  if(IsNumber(ProcName))
    { Number=atol(ProcName);
    }
   else
    { Number=0;
    }
    
    
  Res=DosQueryProcAddr(hModule, Number, ProcName, &pFoo);
  if(Res!=0)
    { sprintf(ErrorBuffer, "Can't get foo [%s][%d] entry point in module [%s]", ProcName, Number, moduleName);
      WinPerror("Error loading module", ErrorBuffer);
      DosFreeModule(hModule);
      return(-1);
    }
    
  pFoo(argCount, Args);
  
  DosFreeModule(hModule);
  return(0);
}


