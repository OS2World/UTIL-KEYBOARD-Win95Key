#define INCL_DOSPROCESS			   /* For init function */
#define INCL_BASE
#define INCL_ERRORS
#define INCL_DOSMODULEMGR
#define INCL_DOSSEMAPHORES
#define INCL_DOSDEVICES
#define INCL_DOSFILEMGR
#define INCL_DOSQUEUES
#define INCL_DOSPROCESS
#define INCL_DOSMEMMGR
#define INCL_DOSDATETIME
#define INCL_DOSRESOURCES
#define INCL_MCIOS2
#define INCL_OS2MM
#define INCL_WINWORKPLACE

#include <stddef.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <os2.h>
#include <audio.h>
char Buff[255];
int volume_interface,mute; /* 0 - MMOS2, 1 - LBMIX, 2 - WPS */
ULONG (* APIENTRY SendStringF) (PSZ,PSZ,USHORT,HWND,USHORT);
APIRET rc;
int LR2V(int L, int R);
ULONG (* APIENTRY SendStringF) (PSZ,PSZ,USHORT,HWND,USHORT);
int SetMasterVolume(int vol);
int GetMasterVolume(void);
INT	LbmixPipeCommand(char *Buff,ULONG Size);
#define PIPE_WAIT 200
