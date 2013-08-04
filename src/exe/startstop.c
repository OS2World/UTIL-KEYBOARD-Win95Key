#include "../w95k.h"


int w95kStartup(void)
{ static int firstTime=1;

  if(ProfileOpen()!=0)
    { WinPerror("Can't initialize", "Can't open profile");
      return(-1);
    }

  if(ModifierInit()!=0)
    { WinPerror("Can't initialize", "Can't initialize modifiers");
      ProfileClose();
      return(-1);
    }

  if(PluginInit()!=0)
    { WinPerror("Can't initialize", "Can't initialize plugins");
      ModifierFree();
      ProfileClose();
      return(-1);
    }

  if(EventInit()!=0)
    { WinPerror("Can't initialize", "Can't initialize events");
      PluginFree();
      ModifierFree();
      ProfileClose();
      return(-1);
    }

  if(HookSet()!=0)
    { WinPerror("Can't initialize", "Can't register hook");
      PluginFree();
      ModifierFree();
      ProfileClose();
      return(-1);
    }

  if(SemSet()!=0)
    { WinPerror("Can't initialize", "Can't register semaphore");
      PluginFree();
      ModifierFree();
      ProfileClose();
      return(-1);
    }

  ProfileQueryParams();

  if(firstTime)

    { if(w95kAddItemsToSysmenu()!=0)
        { WinPerror("Error", "Can't add items to sysmenu");
          return(-1);
        }
    }
  WinCheckMenuItem(hwndMenu, MENU_PRESS, mainParams.onKeyPress);
  WinCheckMenuItem(hwndMenu, MENU_HIDE, mainParams.Hide);
  WinCheckMenuItem(hwndSysSubMenu, MENU_PRESS, mainParams.onKeyPress);
  WinCheckMenuItem(hwndSysSubMenu, MENU_HIDE, mainParams.Hide);
  if(w95kHide(mainParams.Hide)!=0)
    { WinPerror("Can't initialize", "Can't set application visibility");
      PluginFree();
      ModifierFree();
      ProfileClose();
      return(-1);
    }

  ProfileClose();

  if(firstTime)
    { WinSetWindowPos(hwndMain, HWND_TOP, mainParams.x, mainParams.y, mainParams.cx, mainParams.cy, SWP_SIZE|SWP_MOVE|SWP_ZORDER|SWP_ACTIVATE);
      if(mainParams.isMinimized)
        { WinSetWindowPos(hwndMain, HWND_BOTTOM, 0, 0, 0, 0, SWP_MINIMIZE|SWP_SHOW|SWP_ZORDER);
        }
       else
        { WinSetWindowPos(hwndMain, HWND_TOP, mainParams.x, mainParams.y, mainParams.cx, mainParams.cy, SWP_SIZE|SWP_MOVE|SWP_ZORDER|SWP_ACTIVATE|SWP_SHOW);
        }
    }

  firstTime=0;
  return(0);
}

int w95kShutdown(int SavePars)
{ if(SavePars)
    { if(ProfileOpen()==0)
        { ProfileSaveParams();
          ProfileClose();
        }
    }

  HookRemove();
  SemRemove();

  ModifierFree();
  PluginFree();
  EventFree();

  return(0);
}


