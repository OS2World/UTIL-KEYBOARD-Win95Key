#include "../w95k.h"



struct Event * Events=NULL;
int EventsCount=0;


int EventFree(void)
{ register int i;

  for(i=0;i<EventsCount;i++)
   { ParsFree(&Events[i].Modifiers);
     ParsFree(&Events[i].Parameters);
   }
   
  if(Events)
    { free(Events);
      Events=NULL;
    }
  EventsCount=0;
  
  return(0);
}


int EventInit(void)
{ struct Pars p;
  register int i, j;
  char * KeyMacro;
  struct Event NewEvent;
  char * Foo;
  char * FooParams;
  char * Mod;
  char * Key;

  
  ParsInit(&p);

  if(ProfileOpen()!=0)
    { WinPerror("Error initializing keylist", "Can't open profile");
      return(-1);
    }

  if(ProfileQuerySection("KEYS", &p)!=0)
    { /* No keys defined*/
      WinPerror("Can't initialize keylist", "No keyboard combinations found\nwhile scanning KEYS section");
      return(-1);
    }
    
  for(i=0;i<p.argc;i++)
   {
     ParsInit(&NewEvent.Parameters);
     ParsInit(&NewEvent.Modifiers);

     if(ProfileQueryString("KEYS", p.argv[i], &KeyMacro)!=0)
       { sprintf(ErrorBuffer, "Can't read key definition [%s]", p.argv[i]);
         WinPerror("Error initializing keylist", ErrorBuffer);
         ParsFree(&p);
         return(-1);
       }

     /* Modifier1+Modifier2+ModifierN&Keydef = foo("arg1", "arg2", "argN") */

     Foo=KeyMacro;
     DelSpaces(Foo);
     if(IsEmpty(Foo))
       { sprintf(ErrorBuffer, "Error while reading key definition [%s]\nEmpty definition for this key combination", p.argv[i]);
         WinPerror("Error initializing keylist", ErrorBuffer);
         ParsFree(&p);
         free(KeyMacro);
         return(-1);
       }
     FooParams=strchr(Foo, '(');
     if(FooParams==NULL)
       { sprintf(ErrorBuffer, "Error while reading key definition [%s]\nFunction parameters are absent in macro [%s]", p.argv[i], KeyMacro);
         WinPerror("Error initializing keylist", ErrorBuffer);
         ParsFree(&p);
         free(KeyMacro);
         return(-1);
       }
     *FooParams++=0;
     DelSpaces(FooParams);
     if(FooParams[strlen(FooParams)-1]!=')')
       { sprintf(ErrorBuffer, "Error while reading key definition [%s]\nFunction parameters not terminated correctly", p.argv[i]);
         WinPerror("Error initializing keylist", ErrorBuffer);
         ParsFree(&p);
         free(KeyMacro);
         return(-1);
       }
     FooParams[strlen(FooParams)-1]=0;
     
     NewEvent.Foo=PluginGetByName(Foo);
     if(NewEvent.Foo==NULL)
       { sprintf(ErrorBuffer, "Error while reading key definition [%s]\nUndefined function [%s] called", p.argv[i], Foo);
         WinPerror("Error initializing keylist", ErrorBuffer);
         ParsFree(&p);
         free(KeyMacro);
         return(-1);
       }
     if(ParsParseString(&NewEvent.Parameters, FooParams, strlen(FooParams), ',', 0)!=0)
       { sprintf(ErrorBuffer, "Error while reading key definition [%s]\nCan't parse parameters [%s] of function [%s]", p.argv[i], FooParams, Foo);
         WinPerror("Error initializing keylist", ErrorBuffer);
         ParsFree(&p);
         free(KeyMacro);
         return(-1);
       }

     for(j=0;j<NewEvent.Parameters.argc;j++)
      { DelSpaces(NewEvent.Parameters.argv[j]);
        switch(NewEvent.Parameters.argv[j][0])
         { case '\'' :
           case '"'  : NewEvent.Parameters.argv[j][0]=' ';
                       DelSpaces(NewEvent.Parameters.argv[j]);
                       break;
           default   : sprintf(ErrorBuffer, "Error while reading key definition [%s]\nCan't parse parameters [%s] of function [%s]\nInvalid string parameter %d [%s]", p.argv[i], FooParams, Foo, j, NewEvent.Parameters.argv[j]);
                       WinPerror("Error initializing keylist", ErrorBuffer);
                       ParsFree(&p);
                       free(KeyMacro);
                       return(-1);
         }
        switch(NewEvent.Parameters.argv[j][strlen(NewEvent.Parameters.argv[j])-1])
         { case '\'' :
           case '"'  : NewEvent.Parameters.argv[j][strlen(NewEvent.Parameters.argv[j])-1]=0;
                       break;
           default   : sprintf(ErrorBuffer, "Error while reading key definition [%s]\nCan't parse parameters [%s] of function [%s]\nEnd of string parameter %d [%s] not found", p.argv[i], FooParams, Foo, j, NewEvent.Parameters.argv[j]);
                       WinPerror("Error initializing keylist", ErrorBuffer);
                       ParsFree(&p);
                       free(KeyMacro);
                       return(-1);
         }
      }

     Mod=p.argv[i];
     Key=strchr(Mod, '&');
     if(Key==NULL)
       { Mod="";
         Key=p.argv[i];
       }
      else
       { *Key++=0;
       }
       
     if(ParsParseString(&NewEvent.Modifiers, Mod, strlen(Mod), '+', 0)!=0)
       { sprintf(ErrorBuffer, "Error while reading key definition [%s]\nCan't parse modifiers [%s] for key [%s]", p.argv[i], Mod, Key);
         WinPerror("Error initializing keylist", ErrorBuffer);
         ParsFree(&p);
         free(KeyMacro);
         return(-1);
       }
       
     NewEvent.ScanCode=strtol(Key, NULL, 0);
     if(NewEvent.ScanCode==0)
       { NewEvent.ScanCode=Char2Scan(Key);
       }
     
     if(NewEvent.ScanCode<=0)
       { sprintf(ErrorBuffer, "Error while reading key definition [%s]\nInvalid key [%s]", p.argv[i], Key);
         WinPerror("Error initializing keylist", ErrorBuffer);
         ParsFree(&p);
         free(KeyMacro);
         return(-1);
       }
       
     ParsSort(&NewEvent.Modifiers);

     for(j=0;j<NewEvent.Modifiers.argc;j++)
      { if(ModifierIsExist(NewEvent.Modifiers.argv[j])!=0)
          { sprintf(ErrorBuffer, "Can't add event : Modifier [%s] not found", NewEvent.Modifiers.argv[j]);
            WinPerror("Error reading events", ErrorBuffer);
            EventsCount=0;
            ParsFree(&p);
            free(KeyMacro);
            return(-1);
          }
      }
  
     Events=realloc(Events, (EventsCount+1)*sizeof(struct Event));
     if(Events==NULL)
       { EventsCount=0;
         sprintf(ErrorBuffer, "Error while reading key definition [%s]\nCan't allocate memory for new event", p.argv[i]);
         WinPerror("Error initializing keylist", ErrorBuffer);
         ParsFree(&p);
         free(KeyMacro);
         return(-1);
       }
     memcpy(&Events[EventsCount], &NewEvent, sizeof(struct Event));
     EventsCount++;

     free(KeyMacro);

   }
   
  ParsFree(&p);

  if(EventsCount==0)
    { WinPerror("Error initializing keylist", "No keymacro definitions found");
      return(-1);
    }

  
  return(0);
}


int EventFindFoo(unsigned char Scancode, int Flags, struct Event * res)
{ register int i;

  for(i=0;i<EventsCount;i++)
   { if(Scancode==Events[i].ScanCode) /*Scancode match - look for modifiers*/
       { if(ModifierQuery(&Events[i].Modifiers, Flags)==0)
           { memcpy(res, &Events[i], sizeof(struct Event));
             return(0);
           }
       }
   }

  return(-1);
}








