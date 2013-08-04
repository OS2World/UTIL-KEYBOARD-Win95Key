#include "../w95k.h"


struct Plugin *Plugins=NULL;
int PluginsCount=0;

struct Dll *Dlls=NULL;
int DllsCount=0;


int DllGetHandle(char * Name, HMODULE * hmDll)
{ APIRET Res;
  register int i;
  char ErrorString[256];
  
  if(strlen(Name)>128) Name[128]=0;

  for(i=0;i<DllsCount;i++)
   { if(stricmp(Name, Dlls[i].Name)==0)
       { *hmDll=Dlls[i].Handle;
         return(0);
       }
   }

  Res=DosLoadModule(ErrorString, sizeof(ErrorString)-1, Name, hmDll);
  if(Res!=0)
    { sprintf(ErrorBuffer, "Can't load module [%s]. Error %d while loading [%s]", Name, Res, ErrorString);
      WinPerror("Can't load plugin dll", ErrorBuffer);
      return(-1);
    }
    
  Dlls=realloc(Dlls, sizeof(struct Dll)*(DllsCount+1));
  if(Dlls==NULL)
    { WinPerror("Can't load plugin", "Can't allocate memory");
      DosFreeModule(*hmDll);
      DllsCount=0;
      return(-1);
    }
    
  Dlls[DllsCount].Handle=*hmDll;
  strcpy(Dlls[DllsCount].Name, Name);
  
  DllsCount++;
  
  return(0);
}

int DllFree(void)
{ register int i;

  for(i=0;i<DllsCount;i++)
   { DosFreeModule(Dlls[i].Handle);
   }
   
  if(Dlls!=NULL)
    { free(Dlls);
      Dlls=NULL;
    }
  DllsCount=0;
  
  return(0);
}


int PluginAdd(char * Name, char * dllName, char * EntryName)
{ struct Plugin newPlugin;
  long Number=0;
  HMODULE hmDll;
  APIRET Res;
  PFN NewFoo;
  
  if(IsNumber(EntryName))
    { Number=atol(EntryName);
    }

  if(DllGetHandle(dllName, &hmDll)!=0)
    { WinPerror("Error loading plugin", "Can't get dll module handle");
      return(-1);
    }
  
  if(strlen(Name)>10) Name[10]=0;
  strcpy(newPlugin.Name, Name);
  
  Res=DosQueryProcAddr(hmDll, Number, EntryName, &NewFoo);
  if(Res!=0)
    { WinPerror("Entry not found in dll", EntryName);
      return(-1);
    }

  newPlugin.Foo=(PluginFoo)NewFoo;
  
  Plugins=realloc(Plugins, sizeof(struct Plugin)*(PluginsCount+1));
  if(Plugins==NULL)
    { PluginsCount=0;
      WinPerror("Can't add plugin", "Can't allocate memory");
      return(-1);
    }
    
  memcpy(&Plugins[PluginsCount], &newPlugin, sizeof(newPlugin));
  PluginsCount++;
  
  return(0);
}


int PluginFree(void)
{ if(Plugins!=NULL)
    { free(Plugins);
      Plugins=NULL;
    }
    
  PluginsCount=0;
  
  DllFree();
  
  return(0);
}


int PluginInit(void)
{ struct Pars Pars;
  register int i;
  unsigned int ScanCode;
  char * WholeKey;
  char * DllName;
  char * FooName;

  ParsInit(&Pars);
  
  if(ProfileOpen()!=0)
    { WinPerror("Error initializing plugins", "Can't open profile");
      return(-1);
    }
    
  if(ProfileQuerySection("PLUGINS", &Pars)!=0)
    { /* No plugins */
      return(1);
    }
    
  for(i=0;i<Pars.argc;i++)
   { if(ProfileQueryString("PLUGINS", Pars.argv[i], &WholeKey)!=0)
       { sprintf(ErrorBuffer, "Can't add plugin [%s]", Pars.argv[i]);
         WinPerror("Error initializing plugins", ErrorBuffer);
         ParsFree(&Pars);
         return(-1);
       }
       
     DelSpaces(WholeKey);
     if(IsEmpty(WholeKey))
       { sprintf(ErrorBuffer, "Can't add plugin [%s]. Dll name and entry point are empty.", Pars.argv[i]);
         WinPerror("Error initializing plugins", ErrorBuffer);
         ParsFree(&Pars);
         free(WholeKey);
         return(-1);
       }
       
     DllName=WholeKey;
     FooName=strchr(WholeKey, ',');
     if(FooName==NULL)
       { sprintf(ErrorBuffer, "Can't add plugin [%s]. No comma in description [%s] found", Pars.argv[i], WholeKey);
         WinPerror("Error initializing plugins", ErrorBuffer);
         ParsFree(&Pars);
         free(WholeKey);
         return(-1);
       }
     *FooName++=0;
     
     DelSpaces(DllName);
     if(IsEmpty(DllName))
       { sprintf(ErrorBuffer, "Can't add plugin [%s]. Dll name is empty.", Pars.argv[i]);
         WinPerror("Error initializing plugins", ErrorBuffer);
         ParsFree(&Pars);
         free(WholeKey);
         return(-1);
       }
     DelSpaces(FooName);
     if(IsEmpty(FooName))
       { sprintf(ErrorBuffer, "Can't add plugin [%s]. Entry point missing.", Pars.argv[i]);
         WinPerror("Error initializing plugins", ErrorBuffer);
         ParsFree(&Pars);
         free(WholeKey);
         return(-1);
       }
     
     if(PluginAdd(Pars.argv[i], DllName, FooName)!=0)
       { sprintf(ErrorBuffer, "Can't add new plugin [%s], dll [%s], entry [%s]", Pars.argv[i], DllName, FooName);
         WinPerror("Error initializing modifiers", ErrorBuffer);
         ParsFree(&Pars);
         free(WholeKey);
         return(-1);
       }
     free(WholeKey);
   }
   
   
  ParsFree(&Pars);
  
  
  return(0);
}


PluginFoo PluginGetByName(char * Name)
{ register int i;

  if(strlen(Name)>10)
    { Name[10]=0;
    }

  for(i=0;i<PluginsCount;i++)
   { if(strcmp(Plugins[i].Name, Name)==0)
       { return(Plugins[i].Foo);
       }
   }

  return(NULL);
}



