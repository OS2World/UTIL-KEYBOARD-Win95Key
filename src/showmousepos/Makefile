#
# (C) viktor remennik vik@avi.kiev.ua 1997-2000
#

.SUFFIXES: .c .obj .exe .dll .rc .res

#CC=icc /Ge+ /Gi /Ol
CC=icc /Ge+ /Gi /Ol /Ti+ /Tm+
CCJUSTCOMP=/C
SETNAME=/Fe
COPY=cp


RCC=rc -r
RC=rc
RM=rm -f


LINK=


OBJS=showmousepos.obj


DEF=showmousepos.def
RES=showmousepos.res
BIN=showmousepos.exe

$INCLD=..\w95k.h

DESTDIR=../../tools


binary: $(OBJS) $(RES)
	$(CC) $(LINK) $(SETNAME)$(BIN) $(OBJS) $(DEF)
	$(RC) $(RES) $(BIN)
	$(COPY) $(BIN) $(DESTDIR)

.rc.res:
	$(RCC) $*.rc
.c.obj: $(INCLD) $*.c
	$(CC) $(CCJUSTCOMP) $*.c

clean:
	$(RM) $(OBJS) $(DOBJS) $(RES) $(BIN)
