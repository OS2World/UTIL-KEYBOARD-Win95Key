#
# (C) viktor remennik vik@avi.kiev.ua 1997-2000
#

.SUFFIXES: .c .obj .exe .dll .rc .res

CC=icc /W2 /Ge- /Wcns+ /Wdcl+ /Weff+ /Wenu+ /Wext+ /Wgnr+ /Wgot+   \
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


OBJS=w95k_dll.obj
DEF=w95k_dll.def
RES=w95k_dll.res
BIN=win95key.dll

DESTDIR=../../bin


binary: $(OBJS) $(RES)
	$(CC) $(LINK) $(SETNAME)$(BIN) $(OBJS) $(DEF)
	$(RC) $(RES) $(BIN)
	$(COPY) $(BIN) $(DESTDIR)

.rc.res:
	$(RCC) $*.rc
.c.obj:
	$(CC) $(CCJUSTCOMP) $*.c

clean:
	$(RM) $(OBJS) $(RES) $(BIN)
