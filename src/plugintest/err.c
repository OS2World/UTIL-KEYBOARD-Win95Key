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

  sprintf(Buf, "%s\nError number is %Xh(%d)", Text, sErr, iErr);

  WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, Buf, Title, 11,
                    MB_OK | MB_MOVEABLE | MB_ERROR);

}

