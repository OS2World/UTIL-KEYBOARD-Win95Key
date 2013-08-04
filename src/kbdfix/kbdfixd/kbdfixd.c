#define INCL_DOS
#define INCL_KBD
#define INCL_NOPM
#define INCL_DOSSEMAPHORES
#include<os2.h>
#include<stdio.h>
#include<conio.h>
#include<process.h>
#include<stdlib.h>
#include<string.h>
#include "kbdfix.h"
#include "../../w95k.h"
#define DosMonOpen  DOS16MONOPEN
#define DosMonClose DOS16MONCLOSE
#define DosMonReg   DOS16MONREG
#define DosMonRead  DOS16MONREAD
#define DosMonWrite DOS16MONWRITE

#define MONITOR_DEFAULT 0x0000
#define MONITOR_BEGIN   0x0001
#define MONITOR_END     0x0002
#define MONITOR_SESSION 1

typedef SHANDLE HMONITOR;       /* hmon */
typedef HMONITOR *PHMONITOR;

#pragma pack(2)
typedef struct _MONIN {         /* mnin */
        USHORT cb;
        BYTE abReserved[18];
        BYTE abBuffer[108];
} MONIN;
typedef MONIN *PMONIN;

#pragma pack(2)
typedef struct _MONOUT {        /* mnout */
        USHORT cb;
        UCHAR buffer[18];
        BYTE abBuf[108];
} MONOUT;
typedef MONOUT *PMONOUT;

APIRET16 APIENTRY16 DosMonOpen(PSZ pszDevName, PHMONITOR phmon);
APIRET16 APIENTRY16 DosMonClose(HMONITOR hmon);
APIRET16 APIENTRY16 DosMonReg(HMONITOR hmon, PBYTE pbInBuf,
             PBYTE pbOutBuf, USHORT fPosition, USHORT usIndex);
APIRET16 APIENTRY16 DosMonRead(PBYTE pbInBuf, USHORT fWait,
                               PBYTE pbDataBuf,
                               PUSHORT pcbData);
APIRET16 APIENTRY16 DosMonWrite(PBYTE pbOutBuf, PBYTE pbDataBuf,
                                USHORT cbData);

#pragma pack(2)
typedef struct _keypacket
  {
   USHORT     mnflags;
   KBDKEYINFO cp;
   USHORT     ddflags;
  } KEYPACKET;

#pragma stack16(8192)
#pragma seg16(HKBD)
#pragma seg16(MONIN)
#pragma seg16(MONOUT)
#pragma seg16(KEYPACKET)

static HKBD KBDHandle = (HKBD)0;
static MONIN monInbuf = {0};
static MONOUT monOutbuf = {0};
static HEV hevThreadDone = (HEV)0;

static void _System keyboard_monitor(ULONG Dummy);
static void CallCat(void);
int *keynum;
int keys[255];
int main(int argc, char *argv[], char *envp[])
{
 USHORT count,rc,i;
 PID pidKeybrd;
 HEV sem = 0;
 char path_buffer[_MAX_PATH];
 char drive[_MAX_DRIVE];
 char dir[_MAX_DIR];
 char fname[_MAX_FNAME];
 char ext[_MAX_EXT];
 _splitpath(argv[0], drive, dir, fname, ext);
 sprintf(path_buffer,"%s%skbdfixd.cfg",drive,dir);
 monInbuf.cb  = sizeof(MONIN);
 monOutbuf.cb = sizeof(MONOUT);
 rc=DosMonOpen ( "KBD$", &KBDHandle );
 if(rc)
  {
    printf("DosMonOpenError = %d\n",rc);
    return(1);
  }
 rc=DosCreateEventSem("\\SEM32\\WIN95KEY\\reload", &hevThreadDone,0x1000,FALSE);
 if (rc==285) /* program already in memory, reloading config */
  {
    return(1);
  }
 if(get_config(path_buffer)) return(1);
 if (DosCreateThread(&pidKeybrd, &keyboard_monitor, 0L, 2L, 42000L))
  {
     printf("DosCreateThread Error\n");
     DosExit(EXIT_PROCESS,0);
  }
 for(;;)
 {
   DosWaitEventSem(hevThreadDone, (ULONG)SEM_INDEFINITE_WAIT);
   DosBeep(400,100);
   keynum=0;
   if(get_config(path_buffer)) return(1);
 }
 DosMonClose(KBDHandle);
 return(0);
}


static void _System keyboard_monitor(ULONG Dummy)
{
 KEYPACKET keybuff;
 UCHAR    oldchScan=0;
 USHORT count,rc;
 INT i;
 BOOL watchkey=FALSE;
 DosSetPrty(PRTYS_THREAD, PRTYC_TIMECRITICAL,0, 0);
 printf("%s\nKBDFIXD 0.0.1 alpha started\n",VERSION);
 rc=DosMonReg( KBDHandle, (PBYTE)&monInbuf, (PBYTE)&monOutbuf,
             MONITOR_BEGIN, MONITOR_SESSION);
 if (rc) {
    fprintf(stderr,"Error: DosMonReg rc=%d\n",rc);
    return;
 }
 for(keybuff.cp.chChar = 0; ; )
 {
  count = sizeof(keybuff);
  DosMonRead( (PBYTE)&monInbuf, IO_WAIT, (PBYTE)&keybuff, &count);
  for(i=0;i<(int)keynum;i++)
    if (keys[i]==keybuff.cp.chScan)
    {
        keybuff.ddflags=keybuff.ddflags^56;
        break;
    }
     if (keybuff.cp.chScan==watchcat_code && (keybuff.ddflags&64)==0) CallCat();
     DosMonWrite((PBYTE)&monOutbuf,(PBYTE)&keybuff,count);
 }
}
static void CallCat()
 {
       HEV hev;
       int rc=0;
       DosBeep(400,100);
       rc=DosOpenEventSem("\\Sem32\\WatchCat", &hev);
       DosPostEventSem(hev);
       DosCloseEventSem(hev);
       return;
    }

