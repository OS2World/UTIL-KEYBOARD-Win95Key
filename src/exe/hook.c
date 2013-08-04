#include "../w95k.h"

HMODULE HookModule=NULLHANDLE;
PFN HookFoo;
PFN RegisterFoo;
int (*EXPENTRY w95kRegisterTarget)(HWND hw);

int HookSet(void)
{ int Res;

  Res=DosLoadModule(ErrorBuffer, 200, "win95key.dll", &HookModule);
  if(Res!=0)
    { strcat(ErrorBuffer, " - can't load module");
      WinPerror("Error loading hook module", ErrorBuffer);
      return(-1);
    }

  Res=DosQueryProcAddr(HookModule, 0, "w95kRegisterTarget", &RegisterFoo);
  if(Res!=0)
    { WinPerror("Error loading hook module", "Can't get RegisterTarget entry point");
      DosFreeModule(HookModule);
      return(-1);
    }
  Res=DosQueryProcAddr(HookModule, 0, "w95kHook",           &HookFoo);
  if(Res!=0)
    { WinPerror("Error loading hook module", "Can't get hook entry point");
      DosFreeModule(HookModule);
      return(-1);
    }


  w95kRegisterTarget=(int (*APIENTRY)(HWND hw))RegisterFoo;
  if(w95kRegisterTarget(hwndMain)!=0)
    { DosFreeModule(HookModule);
      WinPerror("Error loading", "Already running");
      HookModule=NULLHANDLE;
      return(-1);
    }

  WinSetHook(habMain, NULLHANDLE, HK_INPUT, HookFoo, HookModule);

  return(0);
}


int HookRemove(void)
{ if(HookModule!=NULLHANDLE)
    { w95kRegisterTarget(NULLHANDLE);
      if(!WinReleaseHook(habMain, NULLHANDLE, HK_INPUT, HookFoo, HookModule))
        { WinPerror("Can't release hook!", "Fatal error");
        }
      DosFreeModule(HookModule);
    }
  return(0);
}

