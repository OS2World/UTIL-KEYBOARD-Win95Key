#include "../w95k.h"

void WinPerror(char * Title, char * Text)
{ char Buf[1024];
  ERRORID ErrCode;
  short sErr;
  int iErr;

  ErrCode=WinGetLastError(habMain);
  if(strlen(Text)>512)
    { Text[512]=0;
    }

  sErr=ErrCode;
  iErr=sErr;

  sprintf(Buf, "%s\nError number is %Xh(%d)\nerrno=%d", Text, sErr, iErr, errno);

  printf("---[%s]---\n%s\n----------\n", Title, Buf);
}

