#include "../w95k.h"

HAB habMain;

extern struct Modifier * Modifiers;
extern int ModifiersCount;

HWND hwndMain=NULLHANDLE;

int main(void)
{ BOOL bRes;
  register int i;
  struct Pars p;
  char *Buffer;
  MPARAM mp1, mp2;
  
  habMain=WinInitialize(0);
  if(!habMain)
    { BeepBeep(1);
      printf("Can't WinInitialize\n");
      return(1);
    }

  if(Init()!=0)
    { printf("Can't init\n");
      return(-1);
    }
    
  printf("At least it looks like it running\n");

/*
  mp1=MPFROMSH2CH(flags, repeat, scan);
  mp2=MPFROM2SH(char, virtual);
*/
  mp1=MPFROMSH2CH(KC_CTRL, 1, 0x7e);
  ProcessKey(mp1, mp2);

  mp1=MPFROMSH2CH(KC_KEYUP|KC_CTRL, 1, 0x7e);
  ProcessKey(mp1, mp2);

  printf("=========================================================\n");

  Buffer="eee qqq yyy aaa zzz ddd 111";
  ParsInit(&p);
  printf("Parse res %d\n", ParsParseString(&p, Buffer, strlen(Buffer), ' ', 0));
  for(i=0;i<p.argc;i++)
   { printf("[%s]\n", p.argv[i]);
   }
  printf("=========================================================\n");
  ParsSort(&p);
  for(i=0;i<p.argc;i++)
   { printf("[%s]\n", p.argv[i]);
   }

  printf("At least it looks like it running - 2\n");
  
  
  for(i=0;i<ModifiersCount;i++)
   { printf("Name [%s] Code %2.2Xh Status %d\n", Modifiers[i].Name, Modifiers[i].Code, Modifiers[i].Status);
   }
  
  
  Shutdown();
  
  WinTerminate(habMain);
  
  return(0);
}




