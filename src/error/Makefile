#
# (C) viktor remennik vik@avi.kiev.ua 1997-2000
#

.SUFFIXES: .c .obj .exe .dll .rc .res

CC=icc /Ge+ /Gi /Ol /Ti+ /Tm+ /DDEBUG

#CC=icc /W2 /Ge+ /Wcns+ /Wdcl+ /Weff+ /Wenu+ /Wext+ /Wgnr+ /Wgot+   \
        /Wini+ /Wlan+ /Wobs+ /Word+ /Wpar+ /Wpor+ /Wppc+ /Wpro+ /Wrea+ \
        /Wret+ /Wtrd+ /Wtru+ /Wund+ /Wuni+ /Wuse+ /Wvft+ /Ss+ /G5 /Gi+ \
        /Gm+ /Gl+ /O+ /Oi+ /Op-

CCJUSTCOMP=/C
SETNAME=/Fe
COPY=cp


RCC=rc -r
RC=rc
RM=rm -f


LINK=


OBJS=error.obj


DEF=error.def
RES=error.res
BIN=error.exe

$INCLD=../w95k.h

DESTDIR=../../tools


binary: $(OBJS) $(RES)
	$(CC) $(LINK) $(SETNAME)$(BIN) $(OBJS) $(DEF)
	$(RC) $(RES) $(BIN)
	$(COPY) $(BIN) $(DESTDIR)

debug:  $(DOBJS) $(RES)
	$(CC) $(LINK) $(SETNAME)debug_$(BIN) $(DOBJS)
	$(RC) $(RES) debug_$(BIN)
	
.rc.res:
	$(RCC) $*.rc
.c.obj: $(INCLD) $*.c
	$(CC) $(CCJUSTCOMP) $*.c

clean:
	$(RM) $(OBJS) $(DOBJS) $(RES) $(BIN) debug_$(BIN)
