#include "../w95k.h"

struct
{ char * Name;
  int Code;
} TransTable[]={ { "Esc",        0x01 },
                 { "F1",         0x3B },
                 { "F2",         0x3C },
                 { "F3",         0x3D },
                 { "F4",         0x3E },
                 { "F5",         0x3F },
                 { "F6",         0x40 },
                 { "F7",         0x41 },
                 { "F8",         0x42 },
                 { "F9",         0x43 },
                 { "F10",        0x44 },
                 { "F11",        0x57 },
                 { "F12",        0x58 },
                 { "ScrollLock", 0x46 },
                 { "Pause",      0x5F },
                 { "\x60",       0x29 },
                 { "k1",         0x02 },
                 { "k2",         0x03 },
                 { "k3",         0x04 },
                 { "k4",         0x05 },
                 { "k5",         0x06 },
                 { "k6",         0x07 },
                 { "k7",         0x08 },
                 { "k8",         0x09 },
                 { "k9",         0x0A },
                 { "k0",         0x0B },
                 { "\x2D",       0x0C },
                 { "\x3D",       0x0D },
                 { "BackSpace",  0x0E },
                 { "Tab",        0x0F },
                 { "\x71",       0x10 },
                 { "\x77",       0x11 },
                 { "\x65",       0x12 },
                 { "\x72",       0x13 },
                 { "\x74",       0x14 },
                 { "\x79",       0x15 },
                 { "\x75",       0x16 },
                 { "\x69",       0x17 },
                 { "\x6F",       0x18 },
                 { "\x70",       0x19 },
                 { "\x5B",       0x1A },
                 { "\x5D",       0x1B },
                 { "BigEnter",   0x1C },
                 { "CapsLock",   0x3A },
                 { "\x61",       0x1E },
                 { "\x73",       0x1F },
                 { "\x64",       0x20 },
                 { "\x66",       0x21 },
                 { "\x67",       0x22 },
                 { "\x68",       0x23 },
                 { "\x6A",       0x24 },
                 { "\x6B",       0x25 },
                 { "\x6C",       0x26 },
                 { "\x3B",       0x27 },
                 { "\x27",       0x28 },
                 { "LShift",     0x2A },
                 { "\x7A",       0x2C },
                 { "\x78",       0x2D },
                 { "\x63",       0x2E },
                 { "\x76",       0x2F },
                 { "\x62",       0x30 },
                 { "\x6E",       0x31 },
                 { "\x6D",       0x32 },
                 { "\x2C",       0x33 },
                 { "\x2E",       0x34 },
                 { "\x2F",       0x35 },
                 { "RShift",     0x36 },
                 { "\x5C",       0x2B },
                 { "LCtrl",      0x1D },
                 { "LWin",       0x7E },
                 { "LAlt",       0x38 },
                 { "Space",      0x39 },
                 { "RAlt",       0x5E },
                 { "RWin",       0x7F },
                 { "WinMenu",    0x7C },
                 { "RCtrl",      0x5B },
                 { "s_left",     0x63 },
                 { "s_down",     0x66 },
                 { "s_right",    0x64 },
                 { "s_up",       0x61 },
                 { "s_ins",      0x68 },
                 { "s_home",     0x60 },
                 { "s_pgup",     0x62 },
                 { "s_del",      0x69 },
                 { "s_end",      0x65 },
                 { "s_pgdn",     0x67 },
                 { "numlock",    0x45 },
                 { "\x2F",       0x5C },
                 { "\x2A",       0x37 },
                 { "\x2D",       0x4A },
                 { "home",       0x47 },
                 { "up",         0x48 },
                 { "pgup",       0x49 },
                 { "left",       0x4B },
                 { "center",     0x4C },
                 { "right",      0x4D },
                 { "end",        0x4F },
                 { "down",       0x50 },
                 { "pgdn",       0x51 },
                 { "ins",        0x52 },
                 { "del",        0x53 },
                 { "SmallEnter", 0x5A },
                 { "Plus",       0x4E },
                 { "Minus",      0x4A },
                 { "Power",      0xAE },
                 { "Sleep",      0xAF },
                 { "WakeUp",     0x89 },
                 { "\x2A",       0x37 },
                 { NULL,         0 }
               };

int Char2Scan(char * ChrName)
{ register int i;

  for(i=0;TransTable[i].Name!=NULL;i++)
   { if(strcmpi(ChrName, TransTable[i].Name)==0)
       { return(TransTable[i].Code);
       }
   }

  return(0);
}




char * Scan2Char(int scancode)
{ register int i;

  for(i=0;TransTable[i].Name!=NULL;i++)
   { if(scancode== TransTable[i].Code)
       { return(TransTable[i].Name);
       }
   }

  return(NULL);
}


