#include "..\w95k.h"

static int MaxModStrLen=0;

static char * ModBuf=NULL;
static char * ModBufForCmp=NULL;

struct Modifier * Modifiers=NULL;
int ModifiersCount=0;

int ModifierCmp(struct Modifier * m1, struct Modifier * m2)
{ return(strcmp(m1->Name, m2->Name));
}

int ModifierAdd(char * Name, unsigned char Code)
{
  if(strlen(Name)>10) Name[10]=0;

  Modifiers=realloc(Modifiers, (ModifiersCount+1)*sizeof(struct Modifier));
  if(Modifiers==NULL)
    { WinPerror("Error loading modifiers list", "Can't allocate memory");
      ModifiersCount=0;
      return(-1);
    }
  strcpy(Modifiers[ModifiersCount].Name, Name);
  Modifiers[ModifiersCount].Code=Code;
  Modifiers[ModifiersCount].Status=0;
  ModifiersCount++;

  qsort(Modifiers, ModifiersCount, sizeof(struct Modifier), (int(*)(const void*, const void*))&ModifierCmp);

  return(0);
}


int ModifierFree(void)
{ if(Modifiers) free(Modifiers);
  ModifiersCount=0;
  Modifiers=NULL;

  if(ModBuf) free(ModBuf);
  if(ModBufForCmp) free(ModBufForCmp);
  
  ModBufForCmp=ModBuf=NULL;
  
  return(0);
}


int ModFillFirstTime=1;
int ModifierSet(unsigned int ScanCode, unsigned int Flags, int justCheck)
{ register int i;

  
  for(i=0;i<ModifiersCount;i++)
   { if(ScanCode==Modifiers[i].Code)
       { if(!justCheck)
           { Modifiers[i].Status=(!(Flags & KC_KEYUP));
             ModifierFillBuffer();
           }
         return(1);
       }
   }

  if(justCheck)
    { switch(ScanCode)
       { case 0x2A :
         case 0x1D :
         case 0x38 :
         case 0x5E :
         case 0x36 :
         case 0x5B : return(1);
       }
    }

  if(ModFillFirstTime)
    { /*To fill with real flags*/
      ModifierFillBuffer();
      ModFillFirstTime=0;
    }

  return(0);
}



int ModifierInit(void)
{ struct Pars p;
  register int i;
  unsigned int Scancode;

  ModFillFirstTime=1;
  
  ParsInit(&p);

  if(ProfileOpen()!=0)
    { WinPerror("Error initializing modifiers", "Can't open profile");
      return(-1);
    }

  if(ProfileQuerySection("MODIFIERS", &p)!=0)
    { /* No additional modifiers */
      ModBuf=malloc(1);
      if(ModBuf==NULL)
        { WinPerror("Error initializing modifiers", "Can't allocate memory");
          return(-1);
        }
      ModBufForCmp=malloc(1);
      if(ModBufForCmp==NULL)
        { WinPerror("Error initializing modifiers", "Can't allocate memory");
          return(-1);
        }
      ModBuf[0]=ModBufForCmp[0]=0;
      return(0);
    }

  ParsSort(&p);

  for(i=0;i<p.argc;i++)
   { MaxModStrLen+=strlen(p.argv[i])+2;

     ProfileQueryNumber("MODIFIERS", p.argv[i], (int *)&Scancode);
     if(Scancode==0)
       { Scancode=Char2Scan(p.argv[i]);
         if(Scancode==0)
           { sprintf(ErrorBuffer, "Can't add modifier [%s] with Scancode %d", p.argv[i], Scancode);
             WinPerror("Error initializing modifiers", ErrorBuffer);
             ParsFree(&p);
             return(-1);
           }
       }
     if(ModifierAdd(p.argv[i], (unsigned char)Scancode)!=0)
       { sprintf(ErrorBuffer, "Can't add new modifier [%s]", p.argv[i]);
         WinPerror("Error initializing modifiers", ErrorBuffer);
         ParsFree(&p);
         return(-1);
       }
   }

  ParsFree(&p);

  ModBuf=realloc(ModBuf, MaxModStrLen+1);
  if(ModBuf==NULL)
    { WinPerror("Error initializing modifiers", "Can't allocate buffer");
      return(-1);
    }
  ModBuf[0]=0;
  ModBufForCmp=realloc(ModBufForCmp, MaxModStrLen+1);
  if(ModBufForCmp==NULL)
    { WinPerror("Error initializing modifiers", "Can't allocate temporary buffer");
      return(-1);
    }
  ModBufForCmp[0]=0;

  return(0);
}


int ModifierQuery(struct Pars * mod, unsigned int Flags)
{ register int i;
#define K_CTRL  0
#define K_ALT   1
#define K_SHIFT 2
  char needMap[4]="000";
  char isMap[4]  ="000";
  int ModBufLen=0;

  ModBufForCmp[0]=0;
  ModBufLen=0;
  for(i=0;i<mod->argc;i++)
   { if(strcmpi(mod->argv[i], "ctrl")==0)
       { needMap[K_CTRL]='1';
         continue;
       }
     if(strcmpi(mod->argv[i], "alt")==0)
       { needMap[K_ALT]='1';
         continue;
       }
     if(strcmpi(mod->argv[i], "shift")==0)
       { needMap[K_SHIFT]='1';
         continue;
       }

     ModBufLen+=strlen(mod->argv[i]);
     if(ModBufLen>MaxModStrLen)
       { return(-1);
       }
     strcat(ModBufForCmp, mod->argv[i]);
     strcat(ModBufForCmp, ".");
   }
   
  if(strcmpi(ModBuf, ModBufForCmp)==0)
    { if((Flags&KC_CTRL)!=0)
        { isMap[K_CTRL]='1';
        }
      if((Flags&KC_ALT)!=0)
        { isMap[K_ALT]='1';
        }
      if((Flags&KC_SHIFT)!=0)
        { isMap[K_SHIFT]='1';
        }
      if(strcmp(isMap, needMap)==0)
        { 
          return(0);
        }
    }
    
  return(-1);
}

void ModifierFillBuffer(void)
{ register int i;

  ModBuf[0]=0;
  for(i=0;i<ModifiersCount;i++)
   { if(Modifiers[i].Status)
       { strcat(ModBuf, Modifiers[i].Name);
         strcat(ModBuf, ".");
       }
   }
}




int ModifierIsExist(char * Name)
{ register int i;

  if((strcmpi(Name, "ctrl")==0) || (strcmpi(Name, "alt")==0) || (strcmpi(Name, "shift")==0))
    { return(0);
    }
  
  for(i=0;i<ModifiersCount;i++)
   { if(strcmpi(Modifiers[i].Name, Name)==0)
       { return(0);
       }
   }
  
  return(-1);
}


