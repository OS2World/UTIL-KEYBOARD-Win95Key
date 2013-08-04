#include "showkey.h"

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

void BeepBeep(int Count)
{ register int i;

  for(i=0;i<Count;i++)
   { DosBeep(500, 200);
     DosBeep(100, 200);
   }
}

