/*(C)'1997-2000 vr vik@avi.kiev.ua*/
#define INCL_BASE
#define INCL_DOSSEMAPHORES
#define INCL_DOSNMPIPES
#include "..\..\w95k.h"
static void writePipe(const char *,const char *pipeName);

void EXPENTRY SendToPipe(int argc, char * argv[])
{
 writePipe(argv[0],argv[1]);
} 

 static void writePipe(const char *text,const char *pipeName)
 {
    FILE *f = fopen(pipeName,"w");
    if(f)
    {
       fprintf(f,"%s\n",text);
       fclose(f);
    }
 }
