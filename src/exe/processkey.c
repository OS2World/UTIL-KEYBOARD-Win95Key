#include "../w95k.h"
HMTX    hmtxsem;
struct PendingEvent
{ int scancode;
  struct Event e;
};

int ProcessKey(MPARAM mp1, MPARAM mp2)
{ unsigned int flags;
  unsigned char repeat;
  unsigned char scancode;
  unsigned char character;
  int virtualKey;

  static struct PendingEvent pendingEvent={-1};

  struct Event *pe, e;
  ULONG p;
  TID tidFoo;

  int retval=1;


  flags=SHORT1FROMMP(mp1);
  repeat=CHAR3FROMMP(mp1);
  scancode=CHAR4FROMMP(mp1);
  character=(SHORT1FROMMP(mp2)) &0xFF;
  virtualKey=SHORT2FROMMP(mp2);

  pe=NULL;
  if(!mainParams.onKeyPress)
    { /*Process key releases*/
      ModifierSet(scancode, flags, 0);
      if(EventFindFoo(scancode, flags, &e)==0)
        { retval=0;
          if((flags & KC_KEYUP) &&                     /*And key released               */
             (flags & KC_LONEKEY))                     /*And no other keys has been used*/
            { pe=malloc(sizeof(struct Event));
              if(pe==NULL)
                { return(-1);
                }
              memcpy(pe, &e, sizeof(struct Event));
            }
        }
    }
   else
    { /*Process key presses*/
      if(EventFindFoo(scancode, flags, &e)==0)
        { retval=0;
          if(((flags & KC_KEYUP)==0)                /*And key pressed          */
             && ((flags & KC_PREVDOWN)==0)  /*And key pressed only once*/
             )
            { if(ModifierSet(scancode, flags, 1)==1)
                { /*Modifier*/
                  pendingEvent.scancode=scancode;
                  memcpy(&pendingEvent.e, &e, sizeof(struct Event));
                }
               else
                { pe=malloc(sizeof(struct Event));
                  if(pe==NULL)
                    { return(-1);
                    }
                  memcpy(pe, &e, sizeof(struct Event));
                }
            }
        }
      ModifierSet(scancode, flags, 0);

      if((pendingEvent.scancode==scancode) && (flags & KC_KEYUP))
        { if(flags & KC_LONEKEY)
            { pe=malloc(sizeof(struct Event));
              if(pe==NULL)
                { return(-1);
                }
              memcpy(pe, &pendingEvent.e, sizeof(struct Event));
            }
          pendingEvent.scancode=-1;
        }
    }

  if(pe!=NULL)
    { p=(ULONG)pe;
      DosCreateThread(&tidFoo, (PFNTHREAD)EventStart, p, 0, 65535);
      return(0); /* Remove event */
    }

  return(retval);
}

VOID APIENTRY EventStart(ULONG p)
{ struct Event * e=(struct Event *) p;

  e->Foo(e->Parameters.argc, e->Parameters.argv);
  free(e);

}




