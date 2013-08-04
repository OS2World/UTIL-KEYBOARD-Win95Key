#include "../w95k.h"
HMTX hmtxsem;

int SemSet(void)
{ int Res=0;
  Res=DosCreateMutexSem(SEMNAME, &hmtxsem, 0,FALSE);
  if(Res!=0)
    { sprintf(ErrorBuffer,"DosCreateMutexSem error=%d",Res);
      WinPerror("Error Creating semaphore", ErrorBuffer);
      return(-1);
    }
  return(0);
}


int SemRemove(void)
{ int Res;
  if(hmtxsem)
  {
     Res = DosCloseMutexSem(hmtxsem);
     if(Res!=0)
      { sprintf(ErrorBuffer,"DosCloseMutexSem error=%d",Res);
        WinPerror("Error Clossing semaphore", ErrorBuffer);
        return(-1);
      }
   }
  return(0);
}
