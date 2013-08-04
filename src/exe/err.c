#include "../w95k.h"

char ErrorBuffer[2048];

void WinPerror(char * Title, char * Text)
{ char Buf[2048];
  ERRORID ErrCode=0;
  static int Code=111;

  ErrCode=WinGetLastError(habMain);
  if(strlen(Text)>512)
    { Text[512]=0;
    }


  sprintf(Buf, "%s\nError number is %lXh(%ld)", Text, ErrCode, ErrCode);

  WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, Buf, Title, Code++,
                    MB_OK | MB_MOVEABLE | MB_ERROR);

}


