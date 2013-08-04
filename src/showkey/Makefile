#
# (C) viktor remennik vik@avi.kiev.ua 1997-2000
#

.SUFFIXES: .c .obj .exe .dll .rc .res

CC=icc /Ge+ /Gi /Ol
CCJUSTCOMP=/C
SETNAME=/Fe
COPY=cp


RCC=rc -r
RC=rc
RM=rm -f


LINK=


OBJS=showkey.obj windproc.obj other.obj
DEF=showkey.def
RES=showkey.res
BIN=showkey.exe

DESTDIR=../../tools


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
