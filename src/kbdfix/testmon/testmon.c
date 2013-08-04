#define INCL_DOS
#define INCL_KBD

#include<os2.h>
#include "../../w95k.h"
#include<stdio.h>
#include<conio.h>
#include<process.h>
#include<stdlib.h>
#include<string.h>

#define DosMonOpen  DOS16MONOPEN
#define DosMonClose DOS16MONCLOSE
#define DosMonReg   DOS16MONREG
#define DosMonRead  DOS16MONREAD
#define DosMonWrite DOS16MONWRITE

#define MONITOR_DEFAULT 0x0000
#define MONITOR_BEGIN   0x0001
#define MONITOR_END     0x0002

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

int main(int argc, char *argv[], char *envp[])
{
 USHORT count,rc;
 KEYPACKET keybuff;
 CHAR buffer[10],result[255];
 FILE *fp;
 monInbuf.cb  = sizeof(MONIN);
 monOutbuf.cb = sizeof(MONOUT);
 rc=DosMonOpen ( "KBD$", &KBDHandle );
 if(rc)
  {
    printf("DosMonOpenError = %d",rc);
    return(1);
  }
 rc=DosMonReg( KBDHandle, (PBYTE)&monInbuf, (PBYTE)&monOutbuf,
               MONITOR_BEGIN, -1);
 if(rc)
  {
    printf("DosMonReg Error = %d",rc);
    return(1);
  }
 count = sizeof(keybuff);
 printf("%s\nDosMon tester, v 0.0.1\nPress ESC to exit\n",VERSION);
 fp=fopen("testmon.log","a");
 for(keybuff.cp.chScan=0;keybuff.cp.chScan!=1;){
    DosMonRead( (PBYTE)&monInbuf, IO_WAIT, (PBYTE)&keybuff, &count);
    sprintf(result,"Char=%d,Scan=%d,Status=%d,State=%d,ddflgs=%#08s,mnflgs=%d,time=%d\n",
            keybuff.cp.chChar,keybuff.cp.chScan,keybuff.cp.fbStatus,
            keybuff.cp.fsState,_ultoa(keybuff.ddflags,buffer,2),keybuff.mnflags,
            keybuff.cp.time);
    printf(result);
    fprintf(fp,result);
        }
 fclose(fp);
 DosMonClose(KBDHandle);
 DosBeep(100,100);
 return(0);
}


