#include "../w95k.h"


char *pchVersion=VERSION;

HWND hTargetWin=NULLHANDLE;

int EXPENTRY w95kRegisterTarget(HWND hw)
{ if(hw==NULLHANDLE)
    { hTargetWin=NULLHANDLE;
      return(0);
    }

  if(hTargetWin!=NULLHANDLE)
    { /* Already registered */
      return(-1);
    }


  hTargetWin=hw;

  return(0);
}

BOOL EXPENTRY w95kHook(HAB hab, PQMSG pQmsg, ULONG fs)
{ BOOL Res;


  if(hTargetWin==NULLHANDLE)
    { return(FALSE);
    }


  if(pQmsg->msg == WM_CHAR)
    { if(WinSendMsg(hTargetWin, WM_USER+10, pQmsg->mp1, pQmsg->mp2))
        { pQmsg->msg=WM_NULL;
          pQmsg->mp1=MPFROMLONG(0);
          pQmsg->mp2=MPFROMLONG(0);
/*          Bug? Bug!!!   */
/*          return(TRUE); */
        }
    }

  return(FALSE);
}




