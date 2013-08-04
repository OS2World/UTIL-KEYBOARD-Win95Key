#
# (C) viktor remennik vik@avi.kiev.ua 1997-2000
#

.SUFFIXES: .c .obj .exe .dll .rc .res

CC=icc /W2 /Ge+ /Wcns+ /Wdcl+ /Weff+ /Wenu+ /Wext+ /Wgnr+ /Wgot+   \
       /Wini+ /Wlan+ /Wobs+ /Word+ /Wpar+ /Wpor+ /Wppc+ /Wpro+ /Wrea+ \
       /Wret+ /Wtrd+ /Wtru+ /Wund+ /Wuni+ /Wuse+ /Wvft+ /Ss+ /G5 /Gi+\
       /Gm+ /Gl+ /O+ /Oi+ /Op-

#CC=icc /Ge+ /Gi /Ol /Ti+ /Tm+ /DDEBUG
CCJUSTCOMP=/C
SETNAME=/Fe
COPY=cp


RCC=rc -r
RC=rc
RM=rm -f


LINK=


OBJS=startstop.obj modifiers.obj other.obj plugins.obj processkey.obj chartable.obj sysmenu.obj\
     profile.obj w95k_exe.obj windproc.obj pars.obj err.obj events.obj hook.obj tasklist.obj

DOBJS=startstop.obj modifiers.obj other.obj plugins.obj processkey.obj chartable.obj\
     profile.obj _debug.obj pars.obj d_err.obj events.obj hook.obj tasklist.obj
     

DEF=w95k_exe.def
RES=w95k_exe.res
BIN=win95key.exe

$INCLD=../w95k.h

DESTDIR=../../bin


binary: $(OBJS) $(RES)
	$(CC) $(LINK) $(SETNAME)$(BIN) $(OBJS) $(DEF)
	$(RC) $(RES) $(BIN)
	$(COPY) $(BIN) $(DESTDIR)

debug:  $(DOBJS) $(RES)
	$(CC) $(LINK) $(SETNAME)debug_$(BIN) $(DOBJS)
	$(RC) $(RES) debug_$(BIN)
	
.rc.res:$(INCLD) $*.rc
	$(RCC) $*.rc
.c.obj: $(INCLD) $*.c
	$(CC) $(CCJUSTCOMP) $*.c

clean:
	$(RM) $(OBJS) $(DOBJS) $(RES) $(BIN) debug_$(BIN)
