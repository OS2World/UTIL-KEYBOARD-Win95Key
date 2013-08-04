#include "../w95k.h"


int w95kHide(int Hide)
{ HSWITCH hswMy;
  SWCNTRL swcntrlMy;
/*  PID pidMy;
  TID tidMy;

  if(!WinQueryWindowProcess(hwndMain, &pidMy, &tidMy))
    { WinPerror("Can't get window process id", "Fatal error");
      return(-1);
    }*/

  hswMy=WinQuerySwitchHandle(hwndMain, 0);
  if(hswMy==NULLHANDLE)
    { WinPerror("Can't find switch handle", "Fatal error");
      return(-1);
    }
  if(WinQuerySwitchEntry(hswMy, &swcntrlMy)!=0)
    { WinPerror("Can't obtain switch entry", "Fatal error");
      return(-1);
    }
    
  if(Hide)
    { /*swcntrlMy.fbJump=SWL_NOTJUMPABLE;*/
      if(swcntrlMy.uchVisibility!=SWL_INVISIBLE)
        { swcntrlMy.uchVisibility=SWL_INVISIBLE;
          WinChangeSwitchEntry(hswMy, &swcntrlMy);
        }
    }
   else
    { /*swcntrlMy.fbJump=SWL_JUMPABLE;*/
      if(swcntrlMy.uchVisibility!=SWL_VISIBLE)
        { swcntrlMy.uchVisibility=SWL_VISIBLE;
          WinChangeSwitchEntry(hswMy, &swcntrlMy);
        }
    }
    
  return(0);
}


