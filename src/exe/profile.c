#include "..\w95k.h"


HINI hiniFile=NULLHANDLE;

struct MainParams mainParams={ 0 };

int ProfileQueryParams(void)
{ ProfileOpen();

  mainParams.x=100;
  mainParams.y=200;
  mainParams.cx=200;
  mainParams.cy=100;
  mainParams.isMinimized=0;
  mainParams.Hide=0;
  mainParams.onKeyPress=0;
  
  ProfileQueryNumber("PARAMETERS", "X",          &mainParams.x);
  ProfileQueryNumber("PARAMETERS", "Y",          &mainParams.y);
  ProfileQueryNumber("PARAMETERS", "XSIZE",      &mainParams.cx);
  ProfileQueryNumber("PARAMETERS", "YSIZE",      &mainParams.cy);
  ProfileQueryNumber("PARAMETERS", "MINIMIZED",  &mainParams.isMinimized);
  ProfileQueryNumber("PARAMETERS", "HIDE",       &mainParams.Hide);
  ProfileQueryNumber("PARAMETERS", "ONKEYPRESS", &mainParams.onKeyPress);

  return(0);
}

int ProfileSaveParams(void)
{ SWP swpWinPos;

  ProfileOpen();

  WinQueryWindowPos(hwndMain, &swpWinPos);

  mainParams.x=swpWinPos.x;
  mainParams.y=swpWinPos.y;
  mainParams.cx=swpWinPos.cx;
  mainParams.cy=swpWinPos.cy;

  mainParams.isMinimized=(swpWinPos.fl&SWP_MINIMIZE)?1:0;

  if(mainParams.isMinimized==0)
    { ProfileSaveNumber("PARAMETERS", "X", mainParams.x);
      ProfileSaveNumber("PARAMETERS", "Y", mainParams.y);
      ProfileSaveNumber("PARAMETERS", "XSIZE", mainParams.cx);
      ProfileSaveNumber("PARAMETERS", "YSIZE", mainParams.cy);
    }

  ProfileSaveNumber("PARAMETERS", "MINIMIZED",  mainParams.isMinimized);
  ProfileSaveNumber("PARAMETERS", "HIDE",       mainParams.Hide);
  ProfileSaveNumber("PARAMETERS", "ONKEYPRESS", mainParams.onKeyPress);

  
  return(0);
}


int ProfileOpen(void)
{ if(hiniFile!=NULLHANDLE)
    { /*Assume that it's already opened*/
      return(0);
    }
  hiniFile=PrfOpenProfile(habMain, PROFILENAME);
  if(hiniFile==NULLHANDLE)
    { WinPerror("Error opening ini file", "Can't open "PROFILENAME);
      return(-1);
    }

  return(0);
}


int ProfileClose(void)
{ PrfCloseProfile(hiniFile);
  hiniFile=NULLHANDLE;  
  
  return(0);
}



int ProfileQuerySection(char * Section, struct Pars * p)
{ char * Buffer;
  ULONG BufferMax=0, OldBufferMax=0;
  
  if(!PrfQueryProfileSize(hiniFile, Section, NULL, &BufferMax))
    { /*sprintf(ErrorBuffer, "Can't query size of section [%s]", Section);
      WinPerror("Error reading ini file", ErrorBuffer);*/
      return(-1);
    }
    
  if(BufferMax==0)
    { /*Empty section*/
      /*sprintf(ErrorBuffer, "Can't determine size of section [%s]", Section);
      WinPerror("Can't read ini file", ErrorBuffer);*/
      return(-1);
    }
    
  Buffer=malloc(BufferMax);
  if(Buffer==NULL)
    { WinPerror("Error reading ini file", "Can't allocate memory");
      return(-1);
    }

  memset(Buffer, 0, BufferMax);
  OldBufferMax=BufferMax;

  if(!PrfQueryProfileData(hiniFile, Section, NULL, Buffer, &BufferMax))
    { sprintf(ErrorBuffer, "Can't query section [%s]", Section);
      WinPerror("Error reading ini file", ErrorBuffer);
      free(Buffer);
      return(-1);
    }
  BufferMax=OldBufferMax;
  if((Buffer[BufferMax-1]!=0) || (Buffer[BufferMax-2]!=0))
    { WinPerror("Error reading ini file", "Section too large 8-0");
      free(Buffer);
      return(-1);
    }

  if(ParsParseString(p, Buffer, BufferMax, 0, 1)!=0)
    { sprintf(ErrorBuffer, "Can't parse %s section", Section);
      WinPerror("Error reading ini file", ErrorBuffer);
      free(Buffer);
      return(-1);
    }

  free(Buffer);
  
  return(0);
}


int ProfileQueryNumber(char * Section, char * Key, int * Res)
{ ULONG Result, ResLen;
  BOOL Rc;

  Result=0;
  
  ResLen=sizeof(Result);
  Rc=PrfQueryProfileData(hiniFile, Section, Key, &Result, &ResLen);
  if((!Rc) || (ResLen!=sizeof(Result)))
    { sprintf(ErrorBuffer, "Can't query value for key [%s] in section [%s]", Key, Section);
      WinPerror("Error while querying key", ErrorBuffer);
      return(-1);
    }

  *Res=(int)Result;
  
  return(0);
}


int ProfileSaveNumber(char * Section, char * Key, int Res)
{
  if(PrfWriteProfileData(hiniFile, Section, Key, &Res, sizeof(Res)))
    { return(0);
    }

  return(-1);
}


int ProfileQueryString(char * Section, char * Key, char ** Res)
{ ULONG ResLen;
  BOOL Rc;
  char *tmpRes;

  *Res=NULL;

  if(!PrfQueryProfileSize(hiniFile, Section, Key, &ResLen))
    { sprintf(ErrorBuffer, "Can't query size of value for key [%s] in section [%s]", Key, Section);
      WinPerror("Error while querying key", ErrorBuffer);
      return(-1);
    }

  tmpRes=malloc(ResLen);
  if(tmpRes==NULL)
    { sprintf(ErrorBuffer, "Can't allocate %d bytes of memory while reading value of key [%s] in section [%s]", ResLen, Key, Section);
      WinPerror("Error while querying key", ErrorBuffer);
      return(-1);
    }

  if(!PrfQueryProfileString(hiniFile, Section, Key, "", tmpRes, ResLen))
    { sprintf(ErrorBuffer, "Can't query string value for key [%s] in section [%s]", Key, Section);
      WinPerror("Error while querying key", ErrorBuffer);
      return(-1);
    }
  
  *Res=tmpRes;

  return(0);
}


