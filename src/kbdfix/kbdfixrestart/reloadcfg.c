#define INCL_DOSSEMAPHORES
#include<os2.h>
#include<stdio.h>
#include "../../w95k.h"
void main (void)
{
    HEV      hev                    = NULLHANDLE;
    int rc;
    rc = DosOpenEventSem("\\SEM32\\WIN95KEY\\reload", &hev);
    if (rc)
    {
        fprintf(stderr,"ERROR: Can`t open semaphore. rc=%d",rc);
        return;
    }
    DosPostEventSem(hev);
    printf("%s\nRestart semaphore posted. \n",VERSION);
}
