#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "kbdfix.h"
void addkey(char *);
static  char *save_opt(char *);                     /* save conf option    */
int keynum;
int get_config(char *fname)
{
   char *kwords[]= { "Undefined",         /* 0 = undefined keyword       0  */
                     "LogName",           /* Log directory            1  */
                     "Key",           /* Key name              2  */
                     "WatchCode",           /* Key name              2  */
                   };

   FILE *fp;

   char buffer[4096];
   char keyword[32];
   char value[132];
   char *cp1, *cp2;
   int  i,key;
   int num_kwords=sizeof(kwords)/sizeof(char *);
   watchcat_code=999;
   if ( (fp=fopen(fname,"r")) == NULL)
   {
      fprintf(stderr,"Can`t read config file %s\n",fname);
      return(1);
   }
   while ( (fgets(buffer,4096,fp)) != NULL)
   {
      /* skip comments and blank lines */
      if ( (buffer[0]=='#') || isspace((int)buffer[0]) ) continue;

      /* Get keyword */
      cp1=buffer;cp2=keyword;
      while ( isalnum((int)*cp1) ) *cp2++ = *cp1++;
      *cp2='\0';

      /* Get value */
      cp2=value;
      while ( (*cp1!='\n')&&(*cp1!='\0')&&(isspace((int)*cp1)) ) cp1++;
      while ( (*cp1!='\n')&&(*cp1!='\0') ) *cp2++ = *cp1++;
      *cp2--='\0';
      while ( (isspace((int)*cp2)) && (cp2 != value) ) *cp2--='\0';

      /* check if blank keyword/value */
      if ( (keyword[0]=='\0') || (value[0]=='\0') ) continue;

      key=0;
      for (i=0;i<num_kwords;i++)
         if (!strcmp(keyword,kwords[i])) { key=i; break; }

      if (key==0) { printf("Bad keyword '%s' (%s)\n",       /* Invalid keyword       */
                    keyword,fname);
                    continue;
                  }

      switch (key)
      {
        case 1: logname=value;          break;
        case 2: addkey(value);    break;
        case 3: watchcat_code=atol(value);
                if(strcmp(value,"A")) watchcat_alt=1;else watchcat_alt=0;
                if(strcmp(value,"S")) watchcat_shift=1;else watchcat_shift=0;
                if(strcmp(value,"C")) watchcat_ctrl=1;else watchcat_ctrl=0;
                break;
      }
   }
   fclose(fp);
   return(0);
}

void addkey(char *key)
{
 keys[keynum]=atol(key);
 keynum++;
}
