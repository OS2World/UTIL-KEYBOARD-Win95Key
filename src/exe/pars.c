#include "../w95k.h"

int ParsParseString(struct Pars * p, char * Str, int Len, char Separator, int TillFirstEmpty)
{ register int i;
  int LastStart;

/* Is it really nesessary? */
  ParsFree(p);

  LastStart=0;

  if(Len!=0)
    { for(i=0;i<=Len;i++)
       { if((Str[i]==Separator) || (i==Len))
           { if(LastStart==i)
               { if(TillFirstEmpty)
                   { break; /*First empty argument*/
                   }
               }
             p->argv=realloc(p->argv, (sizeof (char*))*(p->argc+1));
             if(p->argv==NULL)
               { WinPerror("Error parsing string to parameters stru", "Can't allocate memory for result");
                 ParsFree(p);
                 return(-1);
               }
             p->argv[p->argc]=malloc(i-LastStart+1);

             if(p->argv[p->argc]==NULL)
               { WinPerror("Error parsing string to parameters stru", "Can't allocate memory for result");
                 ParsFree(p);
                 return(-1);
               }
             memcpy(p->argv[p->argc], &Str[LastStart], i-LastStart);
             p->argv[p->argc][i-LastStart]=0;
             LastStart=i+1;
             p->argc++;
           }
       }
    }

  p->argv=realloc(p->argv, (sizeof (char*))*(p->argc+1));
  if(p->argv==NULL)
    { WinPerror("Error parsing string to parameters stru", "Can't allocate memory for result");
      ParsFree(p);
      return(-1);
    }
  p->argv[p->argc]=NULL;

  return(0);
}

int ParsInit(struct Pars * p)
{ p->argv=NULL;
  p->argc=0;

  return(0);
}

int ParsFree(struct Pars * p)
{ for(;p->argc>0;p->argc--)
   { if(p->argv[p->argc-1])
       { free(p->argv[p->argc-1]);
       }
   }
  if(p->argv!=NULL)
    { if(p->argv)
        { free(p->argv);
        }
      p->argv=NULL;
    }
  p->argc=0;

  return(0);
}


static int psort(const void * p1, const void * p2)
{ char ** sp1;
  char ** sp2;
  
  sp1=(char**)p1;
  sp2=(char**)p2;
  
  return(strcmp(sp1[0], sp2[0]));
}

int ParsSort(struct Pars * p)
{ qsort(p->argv, p->argc, sizeof(char*), psort);
  return(0);
}
