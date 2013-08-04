#define INCL_DOSSEMAPHORES
#include "..\..\w95k.h"

void EXPENTRY SemPoster(int argc, char * argv[])
{
	unsigned long rc;
	HEV sem = 0L;

  	if (argc < 1)
 {
		// Too few params
		return;
 }

	rc = DosOpenEventSem(argv[0], &sem);

	if (rc)
 {
		return;
 }

	DosPostEventSem(sem);
	DosCloseEventSem(sem);
}
