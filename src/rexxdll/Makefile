#
# (C) viktor remennik vik@avi.kiev.ua 1997-2000
#

.SUFFIXES: .c .obj .exe .dll .rc .res

CC=icc /W2 /Ge- /O+
CCJUSTCOMP=/C
SETNAME=/Fe
COPY=cp


RCC=rc -r
RC=rc
RM=rm -f


LINK=


OBJS=w95k_rx.obj chartable.obj
DEF=w95k_rx.def
RES=w95k_rx.res
BIN=w95k_rx.dll

DESTDIR=../..


binary: $(OBJS) $(RES)
	$(CC) $(LINK) $(SETNAME)$(BIN) $(OBJS) $(DEF) rexx.lib
	$(RC) $(RES) $(BIN)
	$(COPY) $(BIN) $(DESTDIR)

.rc.res:
	$(RCC) $*.rc
chartable.obj: ../exe/chartable.c
	$(CC) $(CCJUSTCOMP) ../exe/chartable.c
.c.obj:
	$(CC) $(CCJUSTCOMP) $*.c

clean:
	$(RM) $(OBJS) $(RES) $(BIN)
