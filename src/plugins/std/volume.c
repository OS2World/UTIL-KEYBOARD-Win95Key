#include "volume.h"
#include "..\..\w95k.h"

int GetMasterVolume(void)
{
    ULONG VolumeL,Volume,VolumeR,Mute,Fn;
    if (volume_interface==0) // mmos2
    {
      SendStringF("MASTERAUDIO QUERY VOLUME WAIT", Buff, 32, 0, 0);
      return strtoul(Buff, NULL, 10);
    }
    if (volume_interface==1) // lbmix
    {
      _itoa(0x11,Buff,16);
      rc=LbmixPipeCommand(Buff,31);
      rc=sscanf(Buff,"%x %d %d %x",&Fn,&VolumeL,&VolumeR, &Mute);
      return LR2V(VolumeL, VolumeR);
    }
    return 0;
}

int SetMasterVolume(int vol)
{
    if (volume_interface==0) // mmos2
    {
      sprintf(Buff,"MASTERAUDIO VOLUME %d",vol);
      SendStringF(Buff,NULL,0,0,0);
    }
    if (volume_interface==1) // LBMIX
    {
      sprintf(Buff,"01 %d %d",vol,vol);
      LbmixPipeCommand(Buff,31);
    }
    return 0;
}

int LbmixPipeCommand(char *Buff,ULONG Size) {
   UCHAR szPipeName[100]="\\PIPE\\MIXER";
   ULONG ulRC;
   HFILE hFile;
   ULONG A;
   ULONG rc;
   rc=DosWaitNPipe(szPipeName,PIPE_WAIT);
   if (rc) return rc;
   rc=DosOpen(szPipeName,&hFile,&A,0,0,OPEN_ACTION_OPEN_IF_EXISTS,
                OPEN_ACCESS_READWRITE|OPEN_SHARE_DENYNONE,NULL);
   if (rc) return rc;
   rc=DosWrite(hFile,Buff,strlen(Buff),&A);
   if (rc) return rc;
   rc=DosRead(hFile,Buff,Size,&A);
   if (rc) return rc;
   Buff[A]=0;
   return DosClose(hFile);
   }

int LR2V(int L, int R) {
   if (L>R) return L;
   else return R;
}

