#include "../w95k.h"
#include <rexxsaa.h>


RexxFunctionHandler w95kQueryOneScancode;

HAB habMain;
HMQ hmqMain;
HWND hwndMain, hwndClient;
HMTX    hmtx     = NULLHANDLE; /* Mutex semaphore handle */

MRESULT EXPENTRY w95kRxWinProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);


ULONG APIENTRY w95kQueryOneScancode(PUCHAR Name, ULONG Argc, RXSTRING Argv[], PSZ Queuename, PRXSTRING Retstr)
{ QMSG Message;
  ULONG winAttrs=FCF_DLGBORDER|FCF_SYSMODAL;
  int scancode=-1,rc;

  HWND hwndParent=HWND_DESKTOP;
  int xOrigin=0, yOrigin=0;
  char * Title;

  char aTmp[32];


  if(Argc!=5) /* добавляем 5ый параметр */
    { printf("Invalid arguments number %d\n", Argc);
      return(40);
    }

  if(Argv[0].strptr!=NULL && Argv[0].strlength!=0)
    { hwndParent=strtol(Argv[0].strptr, NULL, 0);
    }

  if((Argv[1].strptr==NULL || Argv[1].strlength==0) || (Argv[2].strptr==NULL || Argv[2].strlength==0))
    { /* winAttrs|=FCF_SHELLPOSITION; */
      xOrigin=1;
      yOrigin=1;
    }
   else
    { xOrigin=strtol(Argv[1].strptr, NULL, 0);
      yOrigin=strtol(Argv[2].strptr, NULL, 0);
    }

  if((Argv[3].strptr!=NULL) && (Argv[3].strlength!=0))
    { Title=Argv[3].strptr;
      winAttrs|=FCF_TITLEBAR;
    }
   else
    { Title=NULL;
    }

  habMain=WinInitialize(0);
  if(!habMain)
    { printf("Can't initialize\n");
      return(40);
    }
  hmqMain=WinCreateMsgQueue(habMain, 0);
  if(!hmqMain)
    { /* printf("Can't create msg queue\n");
      return(40); */
      WinDestroyMsgQueue(HMQ_CURRENT);
      hmqMain=WinCreateMsgQueue(habMain, 0);
    }
  if(!WinRegisterClass(habMain, "w95kRexxQscan", w95kRxWinProc, CS_SIZEREDRAW, 0))
    { printf("Can't register class\n");
      return(40);
    }
  hwndMain=WinCreateStdWindow(hwndParent, WS_ANIMATE, &winAttrs, "w95kRexxQscan", Title, 0L, (HMODULE)NULL, 1, &hwndClient);
  if(hwndMain==NULLHANDLE)
    { printf("Can't create window\n");
      return(40);
    }
  rc = DosOpenMutexSem(SEMNAME, &hmtx);
  rc = DosRequestMutexSem(hmtx,(ULONG) SEM_IMMEDIATE_RETURN); /* disabling win95key*/
  WinSetWindowPos(hwndMain, HWND_TOP, xOrigin, yOrigin, 200, 75, SWP_SIZE|SWP_MOVE|SWP_ZORDER|SWP_ACTIVATE|SWP_SHOW);
 /* для того, чтобы кнопка нажатая для вызова DLL не сработала */
  while(WinGetMsg(habMain, &Message, (HWND)NULL, 0, 0 ))
   { if(Message.msg==WM_CHAR)
       { if((scancode==-1) )
           { scancode=CHAR4FROMMP(Message.mp1);
             WinPostMsg(hwndMain, WM_QUIT, NULL, NULL);
           }
       }
     WinDispatchMsg(habMain, &Message);
   }

  WinDestroyMsgQueue(hmqMain);
  WinTerminate(habMain);
  DosReleaseMutexSem(hmtx);
  DosCloseMutexSem(hmtx);
  if(Scan2Char(scancode)==NULL || strtol(Argv[4].strptr, NULL, 0)==1)
    { sprintf(aTmp, "%d", scancode);
    }
   else
    { strcpy(aTmp, Scan2Char(scancode));
    }

  if(strlen(aTmp)>=Retstr->strlength)
    { DosAllocMem((void**)&Retstr->strptr, strlen(aTmp), PAG_COMMIT | PAG_WRITE | PAG_READ);
    }
  strcpy(Retstr->strptr, aTmp);
  Retstr->strlength=strlen(aTmp);

  return(0);
}

MRESULT EXPENTRY w95kRxWinProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{ HPS hPresSpace;
  RECTL WinRect;


  switch(msg)
   { case WM_CLOSE    : WinPostMsg(hwndMain, WM_QUIT, NULL, NULL);
                        break;
     case WM_CREATE   :
     case WM_PAINT    : hPresSpace=WinBeginPaint(hwndClient, (HPS)NULL, &WinRect);
                        WinFillRect(hPresSpace, &WinRect, SYSCLR_APPWORKSPACE);
                        WinEndPaint(hPresSpace);
                        DosSleep(300);
                        break;
     default          : return(WinDefWindowProc(hwnd, msg, mp1, mp2));
   }

  return((MRESULT)FALSE);
}



