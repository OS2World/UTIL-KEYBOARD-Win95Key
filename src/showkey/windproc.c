#include "showkey.h"


struct key
{ MPARAM mp1;
  MPARAM mp2;
};

#define ARRAYSIZE 20

struct key Keys[ARRAYSIZE]={{0,0}};
int CurPos=ARRAYSIZE-1;


void ProcessKey(MPARAM mp1, MPARAM mp2)
{ register int i;

  if(CurPos>=(ARRAYSIZE-1))
    { for(i=0;i<(ARRAYSIZE-1);i++)
       { memcpy(&Keys[i], &Keys[i+1], sizeof(struct key));
       }
    }
  Keys[CurPos].mp1=mp1;
  Keys[CurPos].mp2=mp2;

  if(CurPos<(ARRAYSIZE-1)) CurPos++;

  return;
}

void DrawBuffer(HWND hwnd, int Fill)
{ HPS    hps;
  RECTL  rc;
  char Str[20240];
  register int i;
  static int First=1;

  int kc_char, kc_scancode, kc_virtualkey;
  int kc_keyup, kc_prevdown, kc_deadkey;
  int kc_composite, kc_invalidcomp, kc_lonekey;
  int kc_shift, kc_alt, kc_ctrl;
  unsigned short fsflags;
  unsigned char ucrepeat, ucscancode;
  unsigned short usch, usvk;

  long CurrentY;
  POINTL PointList[TXTBOX_COUNT];
  RECTL TextRect;

  hps=WinBeginPaint(hwnd, (HPS)NULL, &rc);
  if(Fill) WinFillRect(hps, &rc, SYSCLR_DIALOGBACKGROUND);

  Str[0]=0;
  CurrentY=1;

  for(i=ARRAYSIZE-1;i>=0;i--)
   { if((Keys[i].mp1==0) && (Keys[i].mp2==0))
       { sprintf(Str, " ");
       }
     else
       { fsflags=SHORT1FROMMP(Keys[i].mp1);
         ucrepeat=CHAR3FROMMP(Keys[i].mp1);
         ucscancode=CHAR4FROMMP(Keys[i].mp1);
         usch=(SHORT1FROMMP(Keys[i].mp2)) &0xFF;
         usvk=SHORT2FROMMP(Keys[i].mp2);

         sprintf(Str, "[%X][%X] repeated[%d], "
                       "scan %Xh, "
                       "character %Xh(%c), "
                       "virtual %Xh; Flags [%X]:", 
                       LONGFROMMP(Keys[i].mp1),
                       LONGFROMMP(Keys[i].mp2),
                       (unsigned int)ucrepeat, 
                       (unsigned int)ucscancode, 
                       (unsigned int)usch, usch==0?32:usch, 
                       (unsigned int)usvk, fsflags);



         kc_char=fsflags & KC_CHAR;               if(kc_char)        strcat(Str, "[char]");
         kc_scancode=fsflags & KC_SCANCODE;       if(kc_scancode)    strcat(Str, "[scancode]");
         kc_virtualkey=fsflags & KC_VIRTUALKEY;   if(kc_virtualkey)  strcat(Str, "[virtualkey]");
         kc_keyup=fsflags & KC_KEYUP;             if(kc_keyup)       strcat(Str, "[keyup]");
         kc_prevdown=fsflags & KC_PREVDOWN;       if(kc_prevdown)    strcat(Str, "[prevdown]");
         kc_deadkey=fsflags & KC_DEADKEY;         if(kc_deadkey)     strcat(Str, "[deadkey]");
         kc_composite=fsflags & KC_COMPOSITE;     if(kc_composite)   strcat(Str, "[composite]");
         kc_invalidcomp=fsflags & KC_INVALIDCOMP; if(kc_invalidcomp) strcat(Str, "[invalidcomp]"); 
         kc_lonekey=fsflags & KC_LONEKEY;         if(kc_lonekey)     strcat(Str, "[lonekey]");
         kc_shift=fsflags & KC_SHIFT;             if(kc_shift)       strcat(Str, "[shift]");
         kc_alt=fsflags & KC_ALT;                 if(kc_alt)         strcat(Str, "[alt]");
         kc_ctrl=fsflags & KC_CTRL;               if(kc_ctrl)        strcat(Str, "[ctrl]");
       }

         printf("repeated[%d], "
                       "scan %Xh, "
                       "character %Xh(%c), "
                       "virtual %Xh; Flags:%s\n", 
                       (unsigned int)ucrepeat, 
                       (unsigned int)ucscancode, 
                       (unsigned int)usch, usch==0?32:usch, 
                       (unsigned int)usvk, Str);


     GpiQueryTextBox(hps, strlen(Str), Str, TXTBOX_COUNT, PointList);
     TextRect.xLeft=5;
     TextRect.xRight=2048; /* PointList[TXTBOX_TOPRIGHT].x-PointList[TXTBOX_TOPLEFT].x+10; */
     TextRect.yBottom=CurrentY;
     CurrentY+=PointList[TXTBOX_TOPLEFT].y-PointList[TXTBOX_BOTTOMLEFT].y;
     TextRect.yTop=CurrentY;
     CurrentY+=5;

     WinDrawText(hps, strlen(Str), Str, &TextRect, SYSCLR_WINDOWTEXT, SYSCLR_DIALOGBACKGROUND, DT_TEXTATTRS|DT_ERASERECT);


     if(First)
       { First=0;
         WinSetWindowPos(hwndMain, 0, 0, 30, 700, (TextRect.yTop-TextRect.yBottom+5)*20+7, SWP_SIZE);
       }
   }


  WinEndPaint(hps);

  return;
}

MRESULT EXPENTRY WindowProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{ RECTL  rc;

  switch(msg)
   { case WM_CHAR        : ProcessKey(mp1, mp2);
                           WinQueryWindowRect(hwnd, &rc);
                           WinInvalidateRect(hwnd, &rc, TRUE);
                           DrawBuffer(hwnd, 0);
                           break;
     case WM_PAINT       : DrawBuffer(hwnd, 1);
                           break;
     case WM_CLOSE       : WinPostMsg(hwndMain, WM_QUIT, NULL, NULL);
                           break;
     default             : return(WinDefWindowProc(hwnd, msg, mp1, mp2));
   }

  return((MRESULT)FALSE);
}



