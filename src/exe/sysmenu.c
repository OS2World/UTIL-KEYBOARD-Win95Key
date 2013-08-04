#include "../w95k.h"


HWND hwndSysMenu=NULLHANDLE, hwndSysSubMenu=NULLHANDLE;

#define NEWITEMSCOUNT 7

int w95kAddItemsToSysmenu(void)
{ struct mItem
  { MENUITEM item;
    char * text;
  } additionalSysMenu[NEWITEMSCOUNT];
  MENUITEM sysMenuItem, menuItemQueried;
  USHORT idSysMenu;
  register int i;
  SHORT sRes;
  int itemCount;

  additionalSysMenu[0].item.iPosition=0;
  additionalSysMenu[0].item.afStyle=MIS_OWNERDRAW|MIS_STATIC;
  additionalSysMenu[0].item.afAttribute=0;
  additionalSysMenu[0].item.id=MENU_ABOUT;
  additionalSysMenu[0].item.hwndSubMenu=NULL;
  additionalSysMenu[0].item.hItem=0;
  additionalSysMenu[0].text="";
  
  additionalSysMenu[1].item.iPosition=MIT_END;
  additionalSysMenu[1].item.afStyle=MIS_SEPARATOR;
  additionalSysMenu[1].item.afAttribute=0;
  additionalSysMenu[1].item.id=0;
  additionalSysMenu[1].item.hwndSubMenu=NULL;
  additionalSysMenu[1].item.hItem=0;
  additionalSysMenu[1].text="";
  
  additionalSysMenu[2].item.iPosition=MIT_END;
  additionalSysMenu[2].item.afStyle=MIS_TEXT;
  additionalSysMenu[2].item.afAttribute=0;
  additionalSysMenu[2].item.id=MENU_REREADCFG;
  additionalSysMenu[2].item.hwndSubMenu=NULL;
  additionalSysMenu[2].item.hItem=0;
  additionalSysMenu[2].text="~Re-read configuration";
  
  additionalSysMenu[3].item.iPosition=MIT_END;
  additionalSysMenu[3].item.afStyle=MIS_TEXT;
  additionalSysMenu[3].item.afAttribute=MIA_NODISMISS;
  additionalSysMenu[3].item.id=MENU_PRESS;
  additionalSysMenu[3].item.hwndSubMenu=NULL;
  additionalSysMenu[3].item.hItem=0;
  additionalSysMenu[3].text="Use key ~presses instead releases";
  
  additionalSysMenu[4].item.iPosition=MIT_END;
  additionalSysMenu[4].item.afStyle=MIS_TEXT;
  additionalSysMenu[4].item.afAttribute=MIA_NODISMISS;
  additionalSysMenu[4].item.id=MENU_HIDE;
  additionalSysMenu[4].item.hwndSubMenu=NULL;
  additionalSysMenu[4].item.hItem=0;
  additionalSysMenu[4].text="~Hide in task list";

  additionalSysMenu[5].item.iPosition=MIT_END;
  additionalSysMenu[5].item.afStyle=MIS_SEPARATOR;
  additionalSysMenu[5].item.afAttribute=0;
  additionalSysMenu[5].item.id=0;
  additionalSysMenu[5].item.hwndSubMenu=NULL;
  additionalSysMenu[5].item.hItem=0;
  additionalSysMenu[5].text="";

  additionalSysMenu[6].item.iPosition=MIT_END;
  additionalSysMenu[6].item.afStyle=MIS_TEXT;
  additionalSysMenu[6].item.afAttribute=0;
  additionalSysMenu[6].item.id=MENU_ABOUT;
  additionalSysMenu[6].item.hwndSubMenu=NULL;
  additionalSysMenu[6].item.hItem=0;
  additionalSysMenu[6].text="~About";


  hwndSysMenu=WinWindowFromID(hwndMain, FID_SYSMENU);
  if(hwndSysMenu==NULLHANDLE)
    { return(-1);
    }
  idSysMenu=SHORT1FROMMR(WinSendMsg(hwndSysMenu, MM_ITEMIDFROMPOSITION, 0L, 0L));
  WinSendMsg(hwndSysMenu, MM_QUERYITEM, MPFROM2SHORT(idSysMenu, FALSE), MPFROMP(&sysMenuItem));
  hwndSysSubMenu=sysMenuItem.hwndSubMenu;


  itemCount=(SHORT)WinSendMsg(hwndSysSubMenu, MM_QUERYITEMCOUNT, MPFROMLONG(0), MPFROMLONG(0));
  for(i=0;i<itemCount;i++)
   { sRes=(SHORT)WinSendMsg(hwndSysSubMenu, MM_ITEMIDFROMPOSITION, MPFROMSHORT(i), MPFROMLONG(0));
     WinSendMsg(hwndSysSubMenu, MM_QUERYITEM, MPFROM2SHORT(sRes, FALSE), (MPARAM)&menuItemQueried);
     if((menuItemQueried.afStyle & MIS_SEPARATOR) || (menuItemQueried.id==SC_HIDE) || (menuItemQueried.id==SC_TASKMANAGER))
       { WinSendMsg(hwndSysSubMenu, MM_DELETEITEM, MPFROM2SHORT(sRes, FALSE), MPFROMLONG(0));
         i--;
       }
     itemCount=(SHORT)WinSendMsg(hwndSysSubMenu, MM_QUERYITEMCOUNT, MPFROMLONG(0), MPFROMLONG(0));
   }
  
  for(i=0;i<NEWITEMSCOUNT;i++)
   { sRes=(SHORT)WinSendMsg(hwndSysSubMenu, MM_INSERTITEM, (MPARAM)&additionalSysMenu[i].item, (MPARAM)additionalSysMenu[i].text);
     if(sRes==MIT_ERROR)
       { return(-1);
       }
     if(sRes==MIT_MEMERROR)
       { return(-1);
       }
   }
    
  return(0);
}
