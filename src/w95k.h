#define VERSION "Win95Key 2.10\n(C)'1996-2000 vik@avi.kiev.ua"

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <search.h>
#include <ctype.h>
#include <errno.h>


#define INCL_GPI
#define INCL_WIN
#define INCL_PM
#define INCL_WINSYS
#define INCL_WINWORKPLACE
#define INCL_WINFRAMEMGR
#define INCL_WINMENUS
#define INCL_WINSTDCNR
#define INCL_WINDIALOGS
#define INCL_WINWINDOWMGR
#define INCL_DOSMODULEMGR
#define INCL_DOSPROCESS
#define INCL_DOSMISC
#define INCL_DOSERRORS
#define INCL_DOSSESMGR
#define INCL_REXXSAA

#include <os2.h>

#define PROFILENAME "win95key.ini"

extern HAB habMain;
extern HWND hwndMain, hwndClient, hwndMenu;
extern HWND hwndSysMenu, hwndSysSubMenu;

MRESULT EXPENTRY w95kWindowProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);


int w95kStartup(void);
int w95kShutdown(void);

extern char ErrorBuffer[2048];
void WinPerror(char * Title, char * Text);

int IsNumber(char * Str);
int IsEmpty (char * Str);
void DelEndSpaces( char * Str);
void DelBegSpaces( char * Str);
void DelSpaces (char * Str);

int ProcessKey(MPARAM mp1, MPARAM mp2);
void BeepBeep(int Count);


struct Pars
{ int argc;
  char ** argv;
};


int ParsInit(struct Pars * p);
int ParsFree(struct Pars * p);
int ParsParseString(struct Pars * p, char * Str, int Len, char Separator, int TillFirstEmpty);
int ParsSort(struct Pars * p);



int ProfileOpen(void);
int ProfileClose(void);
int ProfileQuerySection(char * Section, struct Pars * p);
int ProfileQueryNumber(char * Section, char * Key, int * Res);
int ProfileQueryString(char * Section, char * Key, char ** Res);
int ProfileSaveNumber(char * Section, char * Key, int Res);

int ProfileQueryParams(void);
int ProfileSaveParams(void);



struct Modifier
{ char Name[11];
  unsigned char Code;
  char Status;
};

int ModifierAdd(char * Name, unsigned char Code);
int ModifierFree(void);
int ModifierInit(void);
int ModifierSet(unsigned int ScanCode, unsigned int Flags, int justCheck);
int ModifierQuery(struct Pars * mod, unsigned int Flags);
void ModifierFillBuffer(void);
int ModifierIsExist(char * Name);





struct Dll
{ char Name[129];
  HMODULE Handle;
};

int DllGetHandle(char * Name, HMODULE * hmDll);
int DllFree(void);

/*
typedef void ( APIENTRY _PluginFoo) (int argc, char * argv[]);
typedef _PluginFoo * PluginFoo;
*/

typedef PFN PluginFoo;

struct Plugin
{ char Name[11];
  PluginFoo Foo;
};

int PluginAdd(char * Name, char * dllName, char * EntryName);
int PluginFree(void);
int PluginInit(void);
PluginFoo PluginGetByName(char * Name);


struct Event
{ /* Event description */
  struct Pars Modifiers;     /* User defined modifiers */
  
  int ScanCode;              /* Scancode */
  
  /* What to do */
  PluginFoo Foo;             /* Foo to call */
  struct Pars Parameters;    /* Parameters  */
};

int EventFree(void);
int EventInit(void);
int EventFindFoo(unsigned char Scancode, int Flags, struct Event * res);
VOID APIENTRY EventStart(ULONG p);



int HookSet(void);
int HookRemove(void);


int Char2Scan(char * ChrName);
char * Scan2Char(int scancode);



#define MENU_REREADCFG 330
#define MENU_EXIT      331
#define MENU_ABOUT     332
#define MENU_HIDE      333
#define MENU_PRESS     334


struct MainParams
{ int x;
  int y;
  int cx;
  int cy;
  int isMinimized;
  int Hide;
  int onKeyPress;
};

extern struct MainParams mainParams;

int w95kHide(int Hide);
int w95kAddItemsToSysmenu(void);





