/* (C)'2003 samm samm@os2.ru */
#define INCL_BASE
#define INCL_DOSSEMAPHORES
#define INCL_DOSNMPIPES
#include "..\..\w95k.h"
#include "apm.h"
void EXPENTRY ProcessAPM(int argc, char * argv[])
{
  HMQ hmqLocal=NULLHANDLE;
  HFILE               hfileAPM;
  HAB hab;
  HSWITCH hswPrg;
  HWND    hwndPrg;
  APIRET          apiretRc=NO_ERROR;
  ULONG           ulAction=0;
  ULONG           ulPacketSize;
  ULONG           ulDataSize;
  struct POWERRETURNCODE powerRC;               /* Just be sure and enable APM */
  struct SENDPOWEREVENT  sendpowereventAPM;

  int i=1;
  UCHAR       LoadError[CCHMAXPATH] = {0};
  UCHAR         Args[400];
  PSZ         Envs             = NULL;
  RESULTCODES ChildRC          = {0};
  APIRET      rc               = NO_ERROR;  /* Return code */
  hab=WinQueryAnchorBlock(HWND_DESKTOP);
  hmqLocal=WinCreateMsgQueue(hab, 0);
  rc=DosOpen("\\DEV\\APM$", &hfileAPM, &ulAction, 0, FILE_NORMAL,
             OPEN_ACTION_OPEN_IF_EXISTS,
             OPEN_FLAGS_FAIL_ON_ERROR | OPEN_SHARE_DENYNONE |
             OPEN_ACCESS_READWRITE, NULL);
  if(rc){
    WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, "Can`t open APM$ device.", "OS/2 APM", 9, MB_OK | MB_MOVEABLE | MB_WARNING);
    return;
   }
    memset(&sendpowereventAPM, 0, sizeof(sendpowereventAPM));
    powerRC.usReturnCode=0;                                    /* Enable PWR MGMT function */
    sendpowereventAPM.usSubID=SUBID_ENABLE_POWER_MANAGEMENT;
    ulPacketSize=sizeof(sendpowereventAPM);
    ulDataSize=sizeof(powerRC);
    rc=DosDevIOCtl(hfileAPM, IOCTL_POWER, POWER_SENDPOWEREVENT,
        &sendpowereventAPM, ulPacketSize, &ulPacketSize,
        &powerRC, ulDataSize, &ulDataSize);
  if(rc){
    WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, "IOCTL failed.", "OS/2 APM", 9, MB_OK | MB_MOVEABLE | MB_WARNING);
    return;
   }
    memset(&sendpowereventAPM, 0, sizeof(sendpowereventAPM));
    powerRC.usReturnCode=0;
    sendpowereventAPM.usSubID=SUBID_SET_POWER_STATE;
    sendpowereventAPM.usData1=DEVID_ALL_DEVICES;
    /* type of request */
    if(strcmpi(argv[0], "standby")==0)
        sendpowereventAPM.usData2=POWERSTATE_STANDBY;
    if(strcmpi(argv[0], "suspend")==0)
        sendpowereventAPM.usData2=POWERSTATE_SUSPEND;
    if(strcmpi(argv[0], "power_off")==0)
        sendpowereventAPM.usData2=POWERSTATE_OFF;
   if(!sendpowereventAPM.usData2){
    WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, "Incorrect args, check configuration.", "OS/2 APM", 9, MB_OK | MB_MOVEABLE | MB_WARNING);
    return;
   }
    ulPacketSize=sizeof(sendpowereventAPM);
    ulDataSize=sizeof(powerRC);
    sprintf (Args,"Do you really want to %s your system ?",argv[0]);
    if (MBID_OK==WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, Args, "OS/2 APM", 9, MB_OKCANCEL | MB_MOVEABLE | MB_WARNING))
    {
    apiretRc=DosDevIOCtl(hfileAPM, IOCTL_POWER, POWER_SENDPOWEREVENT,
        &sendpowereventAPM, ulPacketSize, &ulPacketSize,
        &powerRC, ulDataSize, &ulDataSize);
    }
    DosClose(hfileAPM);
    if(hmqLocal!=NULLHANDLE)   { WinDestroyMsgQueue(hmqLocal); }
  return;
}


