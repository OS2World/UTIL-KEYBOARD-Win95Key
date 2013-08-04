#include "../w95k.h"


void BeepBeep(int Count)
{ register int i;

  for(i=0;i<Count;i++)
   { DosBeep(500, 200);
     DosBeep(100, 200);
   }
}


int IsNumber(char * Str)
{ register int i;

  for(i=0;i<strlen(Str);i++)
   { if(!isdigit(Str[i]))
       { return(0);
       }
   }
   
  return(1);
}


int IsSpace(char c)
{ switch(c)
   { case ' '  :
     case 0x08 :
     case 0x09 :
     case 0x0A :
     case 0x0D : return(1);
   }
  return(0);
}

int IsEmpty (char * Str)
{ register int i;
  for(i=0;i<strlen(Str);i++)
   { if(!IsSpace(Str[i])) return(0);
   }
  return(1);
}

void DelEndSpaces(char * Str)
{ register int End;

  if(Str[0]==0) return;

  for(End=strlen(Str)-1;IsSpace(Str[End]) && (End>=0);End--);
  Str[End+1]=0;
}

void DelBegSpaces(char * Str)
{ register int Beg, i;

  if(Str[0]==0) return;

  for(Beg=0;IsSpace(Str[Beg]) && Beg<strlen(Str);Beg++);
  for(i=0;Str[i+Beg-1]!=0;i++)
   { Str[i]=Str[i+Beg];
   }
}

void DelSpaces(char * Str)
{ DelEndSpaces(Str);
  DelBegSpaces(Str);
}
