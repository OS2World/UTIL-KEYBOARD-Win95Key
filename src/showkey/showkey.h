#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>


#define INCL_GPI
#define INCL_WIN
#define INCL_PM
#define INCL_WINSYS
#define INCL_WINFRAMEMGR
#define INCL_WINMENUS
#define INCL_WINDIALOGS
#define INCL_WINWINDOWMGR
#define INCL_DOSMODULEMGR
#define INCL_DOSPROCESS
#define INCL_DOSMISC
#define INCL_DOSERRORS
#define INCL_DOSSESMGR

#include <os2.h>


#define PROFILENAME "win95key.ini"

extern HAB habMain;
extern HWND hwndMain, hwndClient;
extern HINI hiniFile;

extern MRESULT EXPENTRY WindowProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

void WinPerror(char * Title, char * Text);
void BeepBeep(int Count);




