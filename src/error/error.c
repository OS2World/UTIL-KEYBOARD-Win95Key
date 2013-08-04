#include "../w95k.h"


void EXPENTRY MinimizeAll(int argc, char * argv[]);



int main (int argc, char * argv[])
{ HAB habMain;
  HMQ hmqMain;
  int Res;
  HMODULE HookModule=NULLHANDLE;
  PFN HookFoo=NULL;
  char ErrorBuffer[300];
  char * Str[]={ NULL
               };

  habMain=WinInitialize(0);
  if(!habMain)
    { return(1);
    }

  hmqMain=WinCreateMsgQueue(habMain, 0);
  if(!hmqMain)
    { return(2);
    }



  MinimizeAll(0, Str);
  
  WinDestroyMsgQueue(hmqMain);
  WinTerminate(habMain);

  return(0);
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
               { WinSetWindowPos(pswBlk->aswentry[i].swctl.hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_MINIMIZE|SWP_ZORDER);
               }
           }
         
/*         WinSetWindowPos(pswBlk->aswentry[i].swctl.hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_MINIMIZE|SWP_ZORDER);*/
/*         WinSendMsg(pswBlk->aswentry[i].swctl.hwnd, WM_COMMAND, MPFROMSHORT(SC_MINIMIZE), 0);*/
       }
   }

  if(hmqLocal!=NULLHANDLE)
    { WinDestroyMsgQueue(hmqLocal);
    }
}

