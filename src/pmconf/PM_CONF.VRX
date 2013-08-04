/*:VRX         Main
*/
/*  Main
*/
Main:
/*  Process the arguments.
    Get the parent window.
*/

    parse source . calledAs .
    parent = ""
    argCount = arg()
    argOff = 0
    if( calledAs \= "COMMAND" )then do
        if argCount >= 1 then do
            parent = arg(1)
            argCount = argCount - 1
            argOff = 1
        end
    end; else do
        call VROptions 'ImplicitNames'
    end
    InitArgs.0 = argCount
    if( argCount > 0 )then do i = 1 to argCount
        InitArgs.i = arg( i + argOff )
    end
    drop calledAs argCount argOff

/*  Load the windows
*/
    call VRInit
    parse source . . spec
    _VREPrimaryWindowPath = ,
        VRParseFileName( spec, "dpn" ) || ".VRW"
    _VREPrimaryWindow = ,
        VRLoad( parent, _VREPrimaryWindowPath )
    drop parent spec
    if( _VREPrimaryWindow == "" )then do
        call VRMessage "", "Cannot load window:" VRError(), ,
            "Error!"
        _VREReturnValue = 32000
        signal _VRELeaveMain
    end

/*  Process events
*/
    call Init
    signal on halt
    do while( \ VRGet( _VREPrimaryWindow, "Shutdown" ) )
        _VREEvent = VREvent()
        interpret _VREEvent
    end
_VREHalt:
    _VREReturnValue = Fini()
    call VRDestroy _VREPrimaryWindow
_VRELeaveMain:
    call VRFini
exit _VREReturnValue

VRLoadSecondary:
    __vrlsWait = abbrev( 'WAIT', translate(arg(2)), 1 )
    if __vrlsWait then do
        call VRFlush
    end
    __vrlsHWnd = VRLoad( VRWindow(), VRWindowPath(), arg(1) )
    if __vrlsHWnd = '' then signal __vrlsDone
    if __vrlsWait \= 1 then signal __vrlsDone
    call VRSet __vrlsHWnd, 'WindowMode', 'Modal' 
    __vrlsTmp = __vrlsWindows.0
    if( DataType(__vrlsTmp) \= 'NUM' ) then do
        __vrlsTmp = 1
    end
    else do
        __vrlsTmp = __vrlsTmp + 1
    end
    __vrlsWindows.__vrlsTmp = VRWindow( __vrlsHWnd )
    __vrlsWindows.0 = __vrlsTmp
    do while( VRIsValidObject( VRWindow() ) = 1 )
        __vrlsEvent = VREvent()
        interpret __vrlsEvent
    end
    __vrlsTmp = __vrlsWindows.0
    __vrlsWindows.0 = __vrlsTmp - 1
    call VRWindow __vrlsWindows.__vrlsTmp 
    __vrlsHWnd = ''
__vrlsDone:
return __vrlsHWnd

/*:VRX         __VXREXX____APPENDS__
*/
__VXREXX____APPENDS__:
/*
*/
return
/*:VRX         About_b_Click
*/
About_b_Click: 
    if( \VRIsValidObject( "About" ) ) then do
        call VRLoadSecondary "About"
    end

return

/*:VRX         About_Close
*/
About_Close: 
    call About_Fini
return

/*:VRX         About_Create
*/
About_Create: 
    call About_Init
return

/*:VRX         About_Fini
*/
About_Fini: 
    window = VRInfo( "Window" )
    call VRDestroy window
    drop window
return
/*:VRX         About_Init
*/
About_Init: 
    window = VRInfo( "Object" )
    if( \VRIsChildOf( window, "Notebook" ) ) then do
        call VRMethod window, "CenterWindow"
        call VRSet window, "Visible", 1
        call VRMethod window, "Activate"
    end
    drop window
return

/*:VRX         Add_modifier_b_Click
*/
Add_modifier_b_Click: 
 if (datatype(VRGet( "EF_ScanCode", "Value" )) <> 'NUM')|(VRGet( "EF_ScanCode", "Value" )>255)|(VRGet( "EF_ScanCode", "Value" )<0) then 
do
 id = VRMessage( VRWindow(), "ScanCode field Value must be from 0 to 255", "Warning", "Error", , ,  )
 return
end
if VRGet( "EF_Key", "Value" )='' then 
do
 id = VRMessage( VRWindow(), "Key field is empty", "Warning", "Error", , ,  )
 return
end
 if mode_edit=1 then result=SysIni('WIN95KEY.INI','MODIFIERS',word(translate(VRMethod( "modifiers_listbox", "GetString", modifiers_sel.1),' ','='),1),'DELETE:')
 result=SysIni('WIN95KEY.INI','MODIFIERS',VRGet( "EF_Key", "Value" ),substr(d2c(VRGet( "EF_ScanCode", "Value" )),1,4,d2c(0))) 
call modifiers_listbox_refresh
call Add_Plugin_win_close
return

/*:VRX         Add_modifier_Close
*/
Add_modifier_Close: 
    call Add_modifier_Fini
return

/*:VRX         Add_modifier_Create
*/
Add_modifier_Create: 
    call Add_modifier_Init
return

/*:VRX         Add_modifier_Fini
*/
Add_modifier_Fini: 
    window = VRInfo( "Window" )
    call VRDestroy window
    drop window
return
/*:VRX         Add_modifier_help_Click
*/
Add_modifier_help_Click: 
HelpLines.1 = 'Describes special keys treated as modifiers in addition to standard Ctrl, Alt and '
HelpLines.2 = 'Shift. Syntax is simple :'
HelpLines.3 = 'key_name=key_scancode '
HelpLines.4 = 'For example if you want to use left windoze key as modifier with the name LWIN, '
HelpLines.5 = 'just create following entry : key=LWIN, Scancode=128'
HelpLines.6 = 'Where 0x mean hexadecimal notation (126 in Dec) '
HelpLines.7 = 'You can omit scancode (just set it to 0) for 'standard' keys. Just use mnemonic name,'
HelpLines.8 = 'for example A for 'A' key. '
HelpLines.0 = 8
call display_help 
return

/*:VRX         Add_modifier_Init
*/
Add_modifier_Init: 
    window = VRInfo( "Object" )
    if( \VRIsChildOf( window, "Notebook" ) ) then do
        call VRMethod window, "CenterWindow"
        call VRSet window, "Visible", 1
        call VRMethod window, "Activate"
    end

if mode_edit=1 then do
 call VRSet window, "Caption", 'Edit modifier'
 call VRSet 'EF_Key','Value',word(translate(VRMethod( "modifiers_listbox", "GetString", modifiers_sel.1),' ','='),1)
 call VRSet 'EF_ScanCode','Value', word(translate(VRMethod( "modifiers_listbox", "GetString", modifiers_sel.1),' ','='),2)
end

    drop window
return

/*:VRX         Add_new_plug_button_Click
*/
Add_new_plug_button_Click: 
    edit=0
    if( \VRIsValidObject( "Add_Plugin_win" ) ) then do
        call VRLoadSecondary "Add_Plugin_win"
    end
return



/*:VRX         add_plugin_Cancel_b111_Click
*/
add_plugin_Cancel_b111_Click: 
call Add_Plugin_win_close
return

/*:VRX         add_plugin_Cancel_b11_Click
*/
add_plugin_Cancel_b11_Click: 
call Add_Plugin_win_close
return

/*:VRX         add_plugin_Cancel_b1_Click
*/
add_plugin_Cancel_b1_Click: 
call Add_Plugin_win_close
return

/*:VRX         add_plugin_Help_b_Click
*/
add_plugin_Help_b_Click: 
HelpLines.1 = "Choose plugin DLL."
HelpLines.2 = "Select plugins that you want to register. "
HelpLines.0 = 2
call display_help 
return

/*:VRX         Add_plugin_ok_b_Click
*/
Add_plugin_ok_b_Click: 
call VRMethod "Function_lb", "GetSelectedStringList", "tmp7." 
if tmp7.0=0 then do 
 call VRMessage VRWindow(), "No plugins selected.", "Error", "Error", , ,   
 return
 return
end
if pos(directory(),infile)==1 then do
 infile=substr(infile,2+length(directory()))
end

do key=1 to tmp7.0 
 call SysIni  'WIN95KEY.INI','PLUGINS',tmp7.key,infile||','||tmp7.key||d2c(0)
end
drop tmp7. key
  call plugin_listbox_refresh 
call Add_Plugin_win_Close
return

/*:VRX         Add_Plugin_win_Close
*/
Add_Plugin_win_Close: 
    call Add_Plugin_win_Fini
return

/*:VRX         Add_Plugin_win_Create
*/
Add_Plugin_win_Create: 
    call Add_Plugin_win_Init
return

/*:VRX         Add_Plugin_win_Fini
*/
Add_Plugin_win_Fini: 
    window = VRInfo( "Window" )
    call VRDestroy window
    drop window
return
/*:VRX         Add_Plugin_win_Init
*/
Add_Plugin_win_Init: 
    window = VRInfo( "Object" )
    if( \VRIsChildOf( window, "Notebook" ) ) then do
        call VRMethod window, "CenterWindow"
        call VRSet window, "Visible", 1
        call VRMethod window, "Activate"
    end
if edit=1 then do
 call VRSet window, "Caption", 'Edit plugin'
 call VRSet 'EF_DLL','Value',word(translate(string,' ',','),1)
 call VRSet 'EF_FUNC','Value',VRMethod( "plugin_listbox", "GetString", plugin_sel.1)
end
    drop window
return

/*:VRX         CB_Hide_Click
*/
CB_Hide_Click: 
If VRGet( "CB_Hide", "Set" )=1 then call SysIni 'WIN95KEY.INI','PARAMETERS','HIDE',substr(d2c(1),1,4,d2c(0))
If VRGet( "CB_Hide", "Set" )=0 then call SysIni 'WIN95KEY.INI','PARAMETERS','HIDE',substr(d2c(0),1,4,d2c(0))
return

/*:VRX         CB_KeyReact_Click
*/
CB_KeyReact_Click: 
If VRGet( "CB_KeyReact", "Set" )=1 then call SysIni 'WIN95KEY.INI','PARAMETERS','ONKEYPRESS',substr(d2c(1),1,4,d2c(0))
If VRGet( "CB_KeyReact", "Set" )=0 then call SysIni 'WIN95KEY.INI','PARAMETERS','ONKEYPRESS',substr(d2c(0),1,4,d2c(0))
return

/*:VRX         CB_RunMinimized_Click
*/
CB_RunMinimized_Click: 
If VRGet( "CB_RunMinimized", "Set" )=1 then call SysIni 'WIN95KEY.INI','PARAMETERS','MINIMIZED',substr(d2c(1),1,4,d2c(0))
If VRGet( "CB_RunMinimized", "Set" )=0 then call SysIni 'WIN95KEY.INI','PARAMETERS','MINIMIZED',substr(d2c(0),1,4,d2c(0))
return

/*:VRX         Display_help
*/
Display_help: 
Buttons.1 = "Ok"
Buttons.0 = 1
id = VRMessageStem( VRWindow(), "HelpLines.", "Help", "Information", "Buttons.", Ok,  )
drop HelpLines
return

/*:VRX         Drag_and_drop_DragDrop
*/
Drag_and_drop_DragDrop: 
 /* Display all the record information
    */

    srcFile = VRInfo( "SourceFile" )
    targetCtn = VRInfo( "TargetObject" )
    targetRec = VRInfo( "TargetRecord" )
    left = VRInfo( "Left" )
    bottom = VRInfo( "Bottom" )
    operation = VRInfo( "Operation" )

    if( srcFile <> "" ) then do
        call VRSet 'Plugin_input_field','Value',srcFile
    end 
return

/*:VRX         Fini
*/
Fini:
    call RXFuncdrop 'w95kQueryOneScancode'
    window = VRWindow()
    call VRSet window, "Visible", 0
    drop window
return 0

/*:VRX         Get_code_b_Click
*/
Get_code_b_Click: procedure expose scancode_field kbd. active_kbd
x=VRMethod("Screen","TwipsToPixels",VRGet("window1","left")) + (VRMethod("Screen","TwipsToPixels",VRGet("window1","width")))/2-100
y=VRMethod("Screen","TwipsToPixels",VRGet("Screen","height"))-VRMethod("Screen","TwipsToPixels",VRGet("Window1","top"))-(VRMethod("Screen","TwipsToPixels",VRGet("window1","height")))/2-75
scancode_field=w95kQueryOneScancode(VRInfo( "PseudoWin" ), x,y, 'Press a key',0)
visible_code=scancode_field;

 if datatype(visible_code,"w")=1 & active_kbd<>0 then do /* it seems that we have uknown scancode there */
 do i=1 to kbd.active_kbd.scancode.0
   if kbd.active_kbd.scancode.i.code=scancode_field then do
     visible_code=kbd.active_kbd.scancode.i.name;
   end
  end 
 end
call VRSet  "CODE_FIELD", "Value", visible_code
ok = VRMethod( "Get_code_b","SetFocus" )
return
/*:VRX         GetMod_n_Click
*/
GetMod_n_Click: 

x=VRMethod("Screen","TwipsToPixels",VRGet("Add_modifier","left")) + (VRMethod("Screen","TwipsToPixels",VRGet("Add_modifier","width")))/2-100
y=VRMethod("Screen","TwipsToPixels",VRGet("Screen","height"))-VRMethod("Screen","TwipsToPixels",VRGet("Add_modifier","top"))-(VRMethod("Screen","TwipsToPixels",VRGet("Add_modifier","height")))/2-35
call VRSet  "EF_Scancode", "Value", w95kQueryOneScancode(, x,y, 'Press a key',1) 
call VRMethod "GetMod_n","SetFocus" 
return

/*:VRX         Halt
*/
Halt:
/*   signal _VREHalt */
return

/*:VRX         Init
*/
Init:
debug=0;
signal on halt
if VRFileExists( "WIN95KEY.INI" )<>1 then do
 Buttons.1 = "Exit"
 Buttons.0 = 1
 signal on halt
 id = VRMessage( VRWindow(), "Can`t read WIN95KEY.INI !", "Warning", "Error", "Buttons.", ,  )
 exit
end
call RxFuncAdd 'SysLoadFuncs','RexxUtil','SysLoadFuncs'
call RXFuncAdd 'w95kQueryOneScancode', 'W95K_RX', 'w95kQueryOneScancode'
call SysLoadFuncs
if debug=0 then do
 ok = VRRedirectStdIO( "Off") /* debug console */
end
call keyboard_def_read;
    window = VRWindow()
    call VRMethod window, "CenterWindow"
    call VRSet window, "Visible", 1
    call VRMethod window, "Activate"
    drop window
return

/*:VRX         kbd_lb_Change
*/
kbd_lb_Change: procedure expose kbd. active_kbd
 active_kbd=VRMethod("kbd_lb","GetItemData",VRGet("kbd_lb","Selected"))
 call key_listbox_refresh;
 call SysIni 'WIN95KEY.INI','PARAMETERS','KEYBOARD',kbd.active_kbd.brand||" "||kbd.active_kbd.model||d2c(0)
return
/*:VRX         key_listbox_Click
*/
key_listbox_Click: 
ok = VRMethod( "key_listbox", "GetSelectedList", "key_sel." )
tmp=VRMethod( "key_listbox", "GetItemData", key_sel.1 )
string=SysIni('WIN95KEY.INI','KEYS',tmp)
if tmp='New_hotkey' then do 
 string='Select Plugin()'
 tmp=''
end
ok = VRSet( "string_field", "Value",translate(word(translate(translate(string,'%',' '),' ','()'),2),' ','%'))
ok = VRSet( "Plugin_combo_box", "Value",word(translate(string,' ','('),1))
ok = VRMethod( "Modifiers_lb", "GetStringList", "tmp1." )

count=0

/* парсим строку */
do mod_words=1 to words(translate(word(translate(tmp,' ','&'),1),' ','+'))
 mod_word=word(translate(word(translate(tmp,' ','&'),1),' ','+'),mod_words) 
 do tmp2=1 to tmp1.0
  if translate(tmp1.tmp2)=translate(mod_word) then 
  do
   count=count+1
   select.count=tmp2
  end
 end
end /* не парсим строку :) */

select.0=count
ok = VRMethod( "Modifiers_lb", "SetSelectedList", "select." )
drop tmp2 tmp1 mod_word

if lastpos('&',tmp)=0  then scancode_field=tmp 
if lastpos('&',tmp)>0 then scancode_field=substr(tmp,lastpos('&',tmp)+1,length(tmp))
drop tmp
tmp=VRMethod( "key_listbox", "GetString", key_sel.1 )
if lastpos('&',tmp)=0  then call VRSet "CODE_FIELD", "Value", tmp 
if lastpos('&',tmp)>0 then call VRSet "CODE_FIELD", "Value", substr(tmp,lastpos('&',tmp)+1,length(tmp))
parametr_string='' /* переменная проверки установленных сеттингов */
combo_reset=0
 /*переменная показывающая, что пользователь не менял трогал */
drop tmp
return

/*:VRX         key_listbox_refresh
*/
key_listbox_refresh: 
call VRSet "key_listbox", "painting", 0
ok = VRMethod( "key_listbox", "Clear" )
call SysIni 'WIN95KEY.INI','KEYS','ALL:','keystem'
do key=1 to keystem.0
 listname=keystem.key;
 if pos("&",keystem.key)=0 then curkeyname=keystem.key;
 else curkeyname= substr(keystem.key,1+lastpos("&",keystem.key));
 if datatype(curkeyname,"w")=1 & active_kbd<>0 then do /* it seems that we have uknown scancode there */
 do i=1 to kbd.active_kbd.scancode.0
   if kbd.active_kbd.scancode.i.code=curkeyname then do
     listname=substr(keystem.key,1,lastpos("&",keystem.key))||kbd.active_kbd.scancode.i.name;
   end
  end 
 end
 call VRMethod  "key_listbox", "AddString", listname, ,keystem.key;
end
call VRSet "key_listbox", "painting", 1
ok = VRMethod( "Plugin_combo_box", "Clear" )
call VRSet "Plugin_combo_box", "Value", 'Select plugin'
call VRSet "string_field" , "Value", ''
call SysIni 'WIN95KEY.INI','PLUGINS','ALL:','pluginstem'
 do key=1 to pluginstem.0
 call VRMethod  "Plugin_combo_box", "AddString", pluginstem.key  
end
call VRSet "Modifiers_lb", "painting", 0
ok = VRMethod( "Modifiers_lb", "Clear" )
position = VRMethod( "Modifiers_lb", "AddString", "Ctrl", 1 )
position = VRMethod( "Modifiers_lb", "AddString", "Shift", 2 )
position = VRMethod( "Modifiers_lb", "AddString", "Alt", 3 )
call SysIni 'WIN95KEY.INI','MODIFIERS','ALL:','modstem'
do key=1 to (modstem.0)
 call VRMethod "Modifiers_lb", "AddString", modstem.key,key+3
end
call VRSet "Modifiers_lb", "painting", 1
drop modstem
key_sel.1=0
parametr_string='' /* переменная проверки установленных сеттингов */
combo_reset=0
ok = VRSet( "CODE_FIELD", "Value", "" )
 /*переменная показывающая, что пользователь не менял трогал */
return

/*:VRX         keyboard_def_read
*/
keyboard_def_read: procedure expose kbd. 
cfg_file="keyboards.def";

kbd.0=0; /* count of the keyboard fefenition */
do while lines(cfg_file)=1
 str=space(linein(cfg_file))
 if (str="" | substr(str,1,1)="#")  then iterate
 if pos("[keyboard]",str)=1 then do 
  kbd.0=kbd.0+1;
  currentkbd=kbd.0;
  kbd.currentkbd.scancode.0=0; /* number of defined scancodes */
  kbd.currentkbd.brand=""; 
  kbd.currentkbd.model="";
 end
 if pos("brand=",str)=1 then do
  parse value str with temp 'brand=' brandtext;
  kbd.currentkbd.brand=brandtext;
 end 
 if pos("model=",str)=1 then do
  parse value str with temp 'model=' modeltext;
  kbd.currentkbd.model=modeltext;
 end 
 if pos("code=",str)=1 then do
  kbd.currentkbd.scancode.0=kbd.currentkbd.scancode.0+1;
  curscancode=kbd.currentkbd.scancode.0;
  parse value str with temp 'code=' codetext;  
  kbd.currentkbd.scancode.curscancode.code=codetext;
 end 
 if pos("name=",str)=1 then do 
   parse value str with temp 'name=' nametext;  
   kbd.currentkbd.scancode.curscancode.name=nametext;   
 end
end
call stream cfg_file,"c","close"
return 0

/*:VRX         Keys_win_Close
*/
Keys_win_Close: 
    call Keys_win_Fini
return

/*:VRX         Keys_win_Create
*/
Keys_win_Create: 
    call Keys_win_Init
return

/*:VRX         Keys_win_Fini
*/
Keys_win_Fini: 
    window = VRInfo( "Window" )
    call VRDestroy window
    drop window
return
/*:VRX         Keys_win_Init
*/
Keys_win_Init: 
    window = VRInfo( "Object" )
    if( \VRIsChildOf( window, "Notebook" ) ) then do
        call VRMethod window, "CenterWindow"
        call VRSet window, "Visible", 1
        call VRMethod window, "Activate"
    end
    drop window
active_kbd=0;
kbdlist.0=kbd.0;
kbddata.0=kbd.0;
inikbd=SysIni('WIN95KEY.INI','PARAMETERS','KEYBOARD');
inikbd=substr(inikbd,1,length(inikbd)-1);

do curkbd=1 to kbd.0
 kbddata.curkbd=curkbd;
 kbdlist.curkbd=kbd.curkbd.brand||" "||kbd.curkbd.model
 if kbdlist.curkbd=inikbd then active_kbd=curkbd;
end

call VRMethod  "kbd_lb", "AddStringList","kbdlist.",1,kbddata
call VRMethod  "kbd_lb", "AddString","<default>",1,0
if VRMethod("kbd_lb", "FindString",inikbd)>0 then
call VRSet "kbd_lb", "Selected", VRMethod("kbd_lb", "FindString",inikbd)
else
call key_listbox_refresh

return

/*:VRX         LoadRes
*/
LoadRes: 
/* convert from .EXE to resource */
step1: 
type100=0
call stream infile, 'c', 'close'
plugin.name=''
if charin(infile,,2) = 'MZ' then
   base = 1+l2d(charin(infile,61,4))
else 
   base = 1
type = charin(infile,base,2)
if  type \= 'LX' & type \= 'NE' then
   do
         return(99) /* incorrect header */
   end
if type = 'NE' then
   do
      cseg = w2d(charin(infile,base+28,2))
      call skip 4
      segtab = readw()
      rsrctab = readw()
      restab = readw()
      call skip 10
      segshift = readw()
      rsrccnt = readw()
      tabsize = 4
      /* Reading OS/2 v1.x .EXE file */
   end
else
   do
      call charin infile,base+44,0
      pageshift = readl()
      call skip 16
      objtab = readl()
      call skip 4
      objmap = readl()
      call skip 4
      rsrctab = readl()
      rsrccnt = readl()
      restab = readl()
      call skip 36
      datapage = readl()
      tabsize = 14
       /* Reading OS/2 v2.x .EXE file' */
   end
if rsrccnt = 0 then
   do
      return(2) /* 'No resources!' */
   end
do cnt = 0 to rsrccnt-1
   call charin infile,base+rsrctab+cnt*tabsize,0
   if type = 'NE' then call tab16in; else do
   if tab32in()=1 then return(5)    
   end
      if resout()=6 then return(6)    
end /* do */
if type100<>1 then return(2)

step1a:  /* conversion done */
call stream infile, 'c', 'close'
if plugin=0 then return(4) /* no plugin found */

return(0)

l2d:     /* littleendian long to decimal */
   l = c2x(arg(1))
   return x2d(substr(l,7,2)substr(l,5,2)substr(l,3,2)substr(l,1,2))
readl:   /* read one long from infile */
   return l2d(charin(infile,,4))
skip:    /* skip arg(1) chars */
   return charin(infile,,arg(1))          
   
tab32in: /* read resource table entry (32bits) */
   etype = readw()
   ename = readw()
   cb = readl()
   eobj = readw()
   eoffset = readl()   
   if objin(eobj)=1 then return(1)
   return(0)
   
readw:   /* read one word from infile */
   return w2d(charin(infile,,2))        
w2d:     /* littleendian word to decimal */
   w = c2x(arg(1))
   return x2d(substr(w,3,2)substr(w,1,2))  

objin:   /* read object map entry */
   call charin infile,base+objtab+(arg(1)-1)*24,8
   oflags = readl()
   opagemap = readl()
   omapsize = readl()
   opagedataoffset = l2d(charin(infile,base+objmap+(opagemap-1)*8,4))
   opagedatasize = readw()
   opagedataflags = readw()
   if opagedataflags \= 0 then  
   do 
    return(1);
   end
   pos = 1+datapage+eoffset+(2**pageshift)*opagedataoffset
   flags = 0
   if bit(oflags,10) then flags = flags+64
   if bit(oflags,11) then flags = flags+16
   if bit(oflags,12) then flags = flags+4096
   if \ bit(oflags,15) then flags = flags+32
   return(0)
bit:     /* return bit arg(2) of arg(1) */
   return substr(x2b(d2x(arg(1),4)), arg(2),1)

d2w:     /* decimal to littleendian word */
   w = d2x(arg(1),4)
   return x2c(substr(w,3,2)substr(w,1,2))  
   
d2l:     /* decimal to littleindian long */
   l = d2x(arg(1),8)
   return x2c(substr(l,7,2)substr(l,5,2)substr(l,3,2)substr(l,1,2))
   
resout:  /* write resources to stem */
if etype=100 then  type100=1
/* тут мы просто вернём список при resid=0 */
if (etype=100)&(ename=1)&(resid=0) then 
do    
 result_rc=charin(infile,pos,cb);cur_line=1
 parse value readoneline(result_rc,1)  with 'DLL='plugin.name','
 do until readoneline(result_rc,cur_line+1)=''
  parse value readoneline(result_rc,cur_line+1)  with plugin.cur_line'='
  plugin.0=cur_line
  cur_line=cur_line+1
 end /* цикл */
end    /* вернули список */
   
   /* определяем плагин и читаем параметры */
   if (etype=100)&(resid<>0) then 
   do
    if ename=1 then do
    result_rc=charin(infile,pos,cb);cur_line=2
     do until (readoneline(result_rc,cur_line)='')|(translate(plugin.name)=translate(resid))
      parse value readoneline(result_rc,cur_line) with plugin.name '=' plugin.id                 
      cur_line=cur_line+1
     end /* цикл */
     if translate(resid)<>translate(plugin.name) then return(6) /* плагин не найден */
    end /* имя ресурса*/
    /* if plugin.id читаем параметры */
     if ename=plugin.id then do
      result_rc=charin(infile,pos,cb)
      parse value readoneline(result_rc,1) with 'Plugin 'plugin.name '=' plugin.descr','plugin.param.0
      parse value readoneline(result_rc,2) with 'HELPTEXT='plugin.help
      /* парсим параметры управления */
      tmp_plugin=1
     plugin.mandatory.0=0
      do plugin.param.0
       parse value readoneline(result_rc,tmp_plugin+2) with 'TYPE='plugin.param.tmp_plugin
       parse value readoneline(result_rc,tmp_plugin+2) with 'MANDATORY=' plugin.mandatory.tmp_plugin ','
       plugin.mandatory.0=plugin.mandatory.0+plugin.mandatory.tmp_plugin
       tmp_plugin=tmp_plugin+1
      end
     end
     drop  tmp_plugin
   end  /* не опр. параметры */       
   return(0)
 
/* ReadOneLine */
readoneline:
return translate(word(translate(translate(arg(1),'%',' '),' ',d2c(13)||d2c(10)),arg(2)),' ','%')

return

/*:VRX         Locate_plugin_b_Click
*/
Locate_plugin_b_Click: 
curdir=directory()
call directory 'plugins'
infile = VRFileDialog(VRInfo( "Window" ), "Select DLL", "Open", '*.dll', , ,  )
call directory curdir;drop curdir 
resid=0
if infile<>'' then do
 if step1()<>0 then do
   call VRMessage VRWindow(), "This is not win95key plugin DLL.", "Error", "Error", , ,   
   return
   return
 end
 call VRMethod  "Function_lb", "Clear" 
 do key=1 to plugin.0
   call VRMethod  "Function_lb", "AddString", plugin.key
 end
 drop key
 call VRSet 'EF_DLL', 'Caption',plugin.name
end
return

/*:VRX         Main_Help_b_Click
*/
Main_Help_b_Click: 
'view.exe pmconf.inf'
return

/*:VRX         Mod_Add_new_b_Click
*/
Mod_Add_new_b_Click: 
mode_edit=0
  edit=0
    if( \VRIsValidObject( "Add_Modifier" ) ) then do
        call VRLoadSecondary "Add_Modifier"
    end
return

/*:VRX         Mod_Edit_b_Click
*/
Mod_Edit_b_Click: 
if VRMethod( "modifiers_listbox", "GetString", modifiers_sel.1)<>'' then do
 mode_edit=1
 if( \VRIsValidObject( "Add_Modifier" ) ) then do
  call VRLoadSecondary "Add_Modifier"
 end
end

return

/*:VRX         Modifier_remove_b_Click
*/
Modifier_remove_b_Click: 
if VRMethod( "modifiers_listbox", "GetString", modifiers_sel.1)<>'' then do
 Buttons.1 = "Yes,"
 Buttons.2 = "No"
 Buttons.0 = 2
  tmp=word(translate(VRMethod( "modifiers_listbox", "GetString", modifiers_sel.1),' ','='),1)
 id = VRMessage( VRWindow(), "Are you sure that you want delete "||tmp||" modifier ?", "Warning !", "Query", "Buttons.", 2,  )
 If  id=1 then do
  result=SysIni('WIN95KEY.INI','MODIFIERS',tmp,'DELETE:')
  call modifiers_listbox_refresh 
 end 
end

return

/*:VRX         modifiers_listbox_Click
*/
modifiers_listbox_Click: 
 ok = VRMethod( "modifiers_listbox", "GetSelectedList", "modifiers_sel." )
return

/*:VRX         modifiers_listbox_refresh
*/
modifiers_listbox_refresh: 
ok = VRMethod( "modifiers_listbox", "Clear" )
call SysIni 'WIN95KEY.INI','MODIFIERS','ALL:','modstem'
do key=1 to modstem.0
  call VRMethod  "modifiers_listbox", "AddString", modstem.key||'='||c2d(reverse(SysIni('WIN95KEY.INI','MODIFIERS',modstem.key)))
end
modifiers_sel.1=0
return

/*:VRX         NB_2_PageSelected
*/
NB_2_PageSelected: 
IF  VRGet( "NB_2", "Selected" )<>VRInfo( "OldPage" ) then do
 if VRGet( "NB_2", "Selected" )=1 then call key_listbox_refresh
end
return

/*:VRX         New_plugin_b_click_Click
*/
New_plugin_b_click_Click: 

if VRMethod( "key_listbox", "GetString", key_sel.1)='' then do
 Buttons.1 = "Ok"
 Buttons.0 = 1
 call VRMessage VRWindow(), "Key not saved. You must select hotkey !", "Error", "Error", "Buttons.", ,  
 return
 return
end
/* Удаляем старое значение */
  result=SysIni('WIN95KEY.INI','KEYS',VRMethod( "key_listbox", "GetItemData", key_sel.1),'DELETE:')
/* Теперь проверяем, выбран ли плагин  */
if VRGet( "Plugin_combo_box", "Selected" )=0 then do
 Buttons.1 = "Ok"
 Buttons.0 = 1
 call VRMessage VRWindow(), "Key not saved. You must select plugin !", "Error", "Error", "Buttons.", ,  
 return
 return
end

modvalue.1=''
ok = VRMethod( "Modifiers_lb", "GetSelectedStringList", "modvalue." )
count=0
result_string.1=modvalue.1
do tmp=2 to modvalue.0
 result_string.1=result_string.1||'+'||modvalue.tmp
end
if result_string.1<>'' then result_string.1=result_string.1||'&'
if VRGet( "CODE_FIELD", "Value" )='' then do
 Buttons.1 = "Ok"
 Buttons.0 = 1
 call VRMessage VRWindow(), "Key not saved. You must specify key code !", "Error", "Error", "Buttons.", ,  
 return
return
end

result_string.1=result_string.1||scancode_field

/* string.1 содержит имя ключа ! */

/* Если парам<>'' , то всё установлено руками и параметры берём 
из парам.
Если комбо=1 и парам='' , то плагин выбран, но не откофигурирован. Проверим,
а надо ли онор ему...
если комбо=0 и парам='', то плагин не изменялся (точнее не трогался :))
Берём параметры из string ! */
result_string.2=''

if (parametr_string<>'') then 
do
 result_string.2=parametr_string
end
if (combo_reset=1)&(parametr_string='') then do
resid=translate(VRGet( "Plugin_combo_box", "Value" ))
parse value SysIni('WIN95KEY.INI','PLUGINS',VRGet( "Plugin_combo_box", "Value" )) with infile','
if step1()<>0 then do
  call VRMessage VRWindow(), "Key not saved. I can`n read pluggin settings :(", "Error", "Error", , ,   
  return
  return
end
 if plugin.mandatory.0=1 then do
  call VRMessage VRWindow(), "Key not saved. You must specify plugin settings !", "Error", "Error", , ,   
  return
  return
 end 

 result_string.2=VRGet( "Plugin_combo_box", "Value" )||'("")'
end
if (combo_reset=0)&(parametr_string='') then result_string.2=string
result=SysIni('WIN95KEY.INI','KEYS',space(result_string.1),space(result_string.2)||d2c(0))
call key_listbox_refresh
drop value1
drop modvalue.
drop result_string.
return

/*:VRX         Open_obj_sett_Close
*/
Open_obj_sett_Close: 
    call Open_obj_sett_Fini
return

/*:VRX         Open_obj_sett_Create
*/
Open_obj_sett_Create: 
    call Open_obj_sett_Init
return

/*:VRX         Open_obj_sett_Fini
*/
Open_obj_sett_Fini: 
    window = VRInfo( "Window" )
    call VRDestroy window
    drop window
return
/*:VRX         Open_obj_sett_Init
*/
Open_obj_sett_Init: 
    window = VRInfo( "Object" )
    if( \VRIsChildOf( window, "Notebook" ) ) then do
        call VRMethod window, "CenterWindow"
        call VRSet window, "Visible", 1
        call VRMethod window, "Activate"
    end
    drop window

call VRSet 'Plug_descr', 'Caption', plugin.descr
pluginnumber=1
if (plugin.help<>'') then   call VRSet 'Plug_settings_help_b','Enabled', 1
do plugin.param.0
 parse upper value plugin.param.pluginnumber with PluginType ','
 if PluginType="INPUTFIELD" then do   
  parse value plugin.param.pluginnumber with ',DESCR='inputfield.descr','
  call VRSet 'plugin_input_desc','Caption', inputfield.descr
  if pos('DRAG=1',plugin.param.pluginnumber)>0 then do
  call VRSet 'drag_and_drop','Enabled', 1
  call VRSet 'drag_and_drop','visible', 1
 end /* drag */
 if (combo_reset<>1) then do
  call VRSet 'Plugin_input_field', 'Value', space(translate(translate(word(translate(word(translate(translate(string,'%',' '),' ','()'),2),' ',','),pluginnumber),' ','%'),'','"'))
 end
  call VRSet 'plugin_input_desc', 'Enabled', 1
  call VRSet 'plugin_input_desc','visible', 1
end /* inputfield */
if PluginType="SELECTOR" then do   
 parse value plugin.param.pluginnumber with ',DESCR='selector.desc','
  call VRSet 'Selector_desc', 'Caption', selector.desc
parse value plugin.param.pluginnumber with 'VALUES=' values','
selector_string=1
parse value plugin.param.pluginnumber with 'VALUE0=' selector.0','
parse value  plugin.param.pluginnumber with 'VALUES=' selector.data ','
do selector.0
   call VRMethod 'Plugin_Selector', "AddString", word(translate(selector.data,' ','|'),selector_string)
  selector_string=selector_string+1
end
 if combo_reset<>1 then call  VRSet 'Plugin_selector', 'Value',space(translate(translate(word(translate(word(translate(translate(string,'|',' '),' ','()'),2),' ',','),pluginnumber),' ','|'),'','"'))
 if combo_reset=1 then call VRSet 'Plugin_Selector', 'Value', word(translate(selector.data,' ','|'),1)
  call VRSet 'Selector_desc', 'Enabled', 1
  call VRSet 'Selector_desc','visible', 1
end /* selector */
 pluginnumber=pluginnumber+1
end
drop pluginnumber PluginType
return

/*:VRX         Pict_1_Click
*/
Pict_1_Click: 
 call Add_Plugin_win_close
return

/*:VRX         Pict_1_KeyPress
*/
Pict_1_KeyPress: 
call Pict_1_Click
return

/*:VRX         Plug_settings_help_b_Click
*/
Plug_settings_help_b_Click: 
n1=1
do helpwords()
 helplines.n1=readhelpline(n1)
 helplines.0=n1
 n1=n1+1
end
call Display_help
drop n1
return

readhelpline:
return translate(word(translate(translate(plugin.help,'%',' '),' ','^'),arg(1)),' ','%')
helpwords:
return words(translate(translate(plugin.help,'%',' '),' ','^'))
/*:VRX         Plug_settings_ok_b_Click
*/
Plug_settings_ok_b_Click: 
/* А тут мы запишем полученные данные */

tmp_plugin=1
parametr_string=''
do plugin.param.0
 parse upper value plugin.param.tmp_plugin with PluginType ','
 if plugin.mandatory.tmp_plugin=1 then do
   parse value plugin.param.tmp_plugin with ',DESCR='mydescr','
  if  ((PluginType="INPUTFIELD")&(VRGet('Plugin_Input_Field','Value')='')) then do
   call VRMessage VRWindow(), 'Please specify "'mydescr'" value !', "Warning", "Information", , ,
   return
   return
  end
  if  ((PluginType="SELECTOR")&(VRGet('Plugin_selector','Value')='')) then  do 
  call VRMessage VRWindow(), 'Please specify "'mydescr'" value !', "Warning", "Information", , ,
   return
   return
   end
 end /* mandatory */
 if  PluginType="SELECTOR" then do 
  if tmp_plugin=1 then parametr_string='"'||VRGet('Plugin_selector','Value')||'"'
  if (tmp_plugin=2)&(VRGet('Plugin_selector','Value')<>'') then parametr_string=parametr_string||','||'"'||VRGet('Plugin_selector','Value')||'"'
 end
 if  PluginType="INPUTFIELD" then do 
  if tmp_plugin=1 then parametr_string='"'||VRGet('Plugin_Input_Field','Value')||'"'
  if (tmp_plugin=2)&(VRGet('Plugin_Input_Field','Value')<>'') then parametr_string=parametr_string||','||'"'||VRGet('Plugin_Input_Field','Value')||'"'
 end
 tmp_plugin=tmp_plugin+1
end
drop tmp_plugin
drop mydescr
parametr_string=resid||'('||parametr_string||')' 
call Add_Plugin_win_close

return
/*:VRX         Plugin_combo_box_Change
*/
Plugin_combo_box_Change: 
parametr_string=''
return

/*:VRX         Plugin_combo_box_GotFocus
*/
Plugin_combo_box_GotFocus: 
combo_reset=1
return

/*:VRX         plugin_listbox_Click
*/
plugin_listbox_Click: 
 ok = VRMethod( "plugin_listbox", "GetSelectedList", "plugin_sel." )
string=SysIni('WIN95KEY.INI','PLUGINS',VRMethod( "plugin_listbox", "GetString", plugin_sel.1))
parse value string with dllstring','
ok = VRSet( "plug_string", "Value", dllstring);drop dllstring
return

/*:VRX         plugin_listbox_refresh
*/
plugin_listbox_refresh: 
call VRSet "plugin_listbox", "painting", 0
ok = VRMethod( "plugin_listbox", "Clear" )
call SysIni 'WIN95KEY.INI','PLUGINS','ALL:','plugstem'
do key=1 to plugstem.0
 call VRMethod  "plugin_listbox", "AddString", plugstem.key 
end
call VRSet "plugin_listbox", "painting", 1
call VRSet "plug_string", "Value", ''
return

/*:VRX         Plugin_settings_b_Click
*/
Plugin_settings_b_Click: 

if VRGet( "Plugin_combo_box", "Selected" )<>0 then do

resid=VRGet( "Plugin_combo_box", "Value" )
parse value SysIni('WIN95KEY.INI','PLUGINS',VRGet( "Plugin_combo_box", "Value" )) with infile','

if step1()<>0 then do
  call VRMessage VRWindow(), "INI error. I can`t read plugin settings :(", "Error", "Error", , ,   
  return
  return
end
/* Description='Uknown Plugin '||Plugin
 inputfield.descr='Options'
 inputfield.value=VRGet( "string_field", "Value",word(translate(string,' ','()'),2))
 selector.0=0r
 dragdrop=0
 SELECT
  WHEN Plugin='SwitchTo'
  THEN 
  do
   Description='Switch to (bring up) window'
   inputfield.descr='Title of the window'
   selector.desc=''
  end 
  WHEN Plugin='ContextMenu'
  THEN 
  do
   Description='Shows the context menu'||d2c(13)||' of the current application'
   inputfield.descr=''
   selector.desc=''
  end 

  WHEN Plugin='DesktopMenu'
  THEN 
  do
   Description="Shows the desktop's context menu"
   inputfield.descr='Title of the desctop (optional)'
   selector.desc=''
  end 
 WHEN Plugin='WarpCenterMenu'
 THEN 
 do
  Description="Shows the WarpCenter's menus"
  inputfield.descr=''
  selector.desc=''
  Selector.desc='Action to perform'
  selector.0=9
  selector.Value=translate(word(translate(word(translate(translate(string,'|',' '),' ','(")'),2),' ',','),1),' ','|')
  selector.1='MENU'
  selector.2='WINDOWLIST'
  selector.3='LOCKUP'
  selector.4='FIND'
  selector.5='SHUTDOWN'
  selector.6='INFO'
  selector.7='TRAYS'
  selector.8='CLOCK'
  selector.9='PROPERTIES'
 end 
 WHEN Plugin='WindowList'
 THEN 
 do
  Description='Shows the window list.'
  inputfield.descr=''
  inputfield.value=VRGet( "string_field", "Value",word(translate(string,' ','()'),2))
  selector.desc=''
 end 
 WHEN Plugin='OpenObject'
 THEN 
 do 
  dragdrop=1
  inputfield.value=translate(word(translate(word(translate(translate(string,'|',' '),' ','(")'),2),' ',','),1),' ','|')
  Description='Opens an object or launch program'
  Selector.desc='Type of view'
  selector.0=9
  selector.Value=translate(word(translate(word(translate(translate(string,'|',' '),' ','(")'),2),' ',','),2),' ','|')
  if selector.Value='' then selector.Value='DEFAULT'
  selector.1='DEFAULT'
  selector.2='HELP'
  selector.3='PALETTE'
  selector.4='PROMPTDLG'
  selector.5='RUNNING'
  selector.6='SETTINGS'
  selector.7='TREE'
  selector.8='CONTENTS'
  selector.9='DETAILS'
  inputfield.descr='Path or object name'
  dragdrop=1
 end

 WHEN Plugin='Volume'
 THEN 
 do 
  Description="Controls OS/2 MM Volume"
  inputfield.descr=''
  selector.desc=''
  Selector.desc='Action to perform'
  selector.0=2
  selector.Value=translate(word(translate(word(translate(translate(string,'|',' '),' ','(")'),2),' ',','),1),' ','|')
  selector.1='UP'
  selector.2='DOWN'
end

 OTHERWISE 
  Selector.desc=''
  inputfield.descr='Plugin Options'
  inputfield.value=translate((word(translate(translate(string,'|',' '),' ','"()'),2)),' ','|')
 END
*/
 if( \VRIsValidObject( "Open_obj_sett" ) ) then do
  call VRLoadSecondary "Open_obj_sett"
 end
end
return

/*:VRX         plugin_win_Close
*/
plugin_win_Close: 
    call plugin_win_Fini
return

/*:VRX         plugin_win_Create
*/
plugin_win_Create: 
    call plugin_win_Init
return

/*:VRX         plugin_win_Fini
*/
plugin_win_Fini: 
    window = VRInfo( "Window" )
    call VRDestroy window
    drop window
return
/*:VRX         plugin_win_Init
*/
plugin_win_Init: 
    window = VRInfo( "Object" )
    if( \VRIsChildOf( window, "Notebook" ) ) then do
        call VRMethod window, "CenterWindow"
        call VRSet window, "Visible", 1
        call VRMethod window, "Activate"
    end
    drop window
call plugin_listbox_refresh
call modifiers_listbox_refresh
call global_checkbox_refresh
return

global_checkbox_refresh:
if SysIni('WIN95KEY.INI','PARAMETERS','HIDE')<>"ERROR:" then do
 if c2d(reverse(SysIni('WIN95KEY.INI','PARAMETERS','HIDE')))=1 then call VRSet  "CB_Hide", "Set", 1 
end
if SysIni('WIN95KEY.INI','PARAMETERS','MINIMIZED')<>"ERROR:" then do
 if c2d(reverse(SysIni('WIN95KEY.INI','PARAMETERS','MINIMIZED')))=1 then call VRSet  "CB_RunMinimized", "Set", 1 
end
if SysIni('WIN95KEY.INI','PARAMETERS','ONKEYPRESS')<>"ERROR:" then do
 if c2d(reverse(SysIni('WIN95KEY.INI','PARAMETERS','ONKEYPRESS')))=1 then call VRSet  "CB_KeyReact", "Set", 1  
end
return

/*:VRX         Quit
*/
Quit:
    window = VRWindow()
    call VRSet window, "Shutdown", 1
    drop window
return

/*:VRX         Reload_b_Click
*/
Reload_b_Click: 
    w = VRWindow()
    MyWin=w
    window = "?HWND1"
    firstChild = VRGet( window, "FirstChild" )
call VRSet "Reload_b", "Enabled", 0 
    win95_hd=AddChildren(firstChild, "") 
 call VRSet "Reload_b", "Enabled", 1 
if win95_hd=0 then do
 id = VRMessage( VRWindow(), "I can`t reload settings ! Are you sure that win95key is running ?", Error !, "Error", "", ,  )
 return
 return
end
 call VRMethod "Application", "SendKeyString", win95_hd, '{Alt}R'
 call VRMethod "Application", "SendKeyString", MyWin, '' 
drop mywin

return


AddChildren: procedure
    parse arg w, parentRec
    do while w <> ""
        if VRGet( w, "Visible" ) = 1 then do          
            caption = w || " " || VRGet( w, "ClassName" ) || "0a"x || VRGet( w, "Caption" ) 
            if VRGet( w, "Caption" )='Win95key' then do        
              return(w)
              end
            firstChild = VRGet( w, "FirstChild" )
            call AddChildren firstChild, rec
        end
        w = VRGet( w, "Sibling" )
    end
return(0)



/* reload_b_Click:
win95_hd=VRMethod("Screen","FindWindow","Win95key","Desktop",,"exact","exact")
call VRMethod "Screen","WinSendMsg",win95_hd,"0x000b","0x1","0x1"
if win95_hd=0 then do
 id = VRMessage( VRWindow(), "I can`t reload settings ! Are you sure that win95key is running ?", Error !, "Error", "", ,  )
 return
 return
end */


return

/*:VRX         Remove_hotkey_b_Click
*/
Remove_hotkey_b_Click: 
if VRMethod( "key_listbox", "GetString", key_sel.1)='' then do
 Buttons.1 = "Ok"
 Buttons.0 = 1
 call VRMessage VRWindow(), "You must select hotkey !", "Error", "Error", "Buttons.", ,  
 return
 return
end

if VRMethod( "key_listbox", "GetItemData", key_sel.1)<>'' then do
 Buttons.1 = "Yes,"
 Buttons.2 = "No"
 Buttons.0 = 2
 id = VRMessage( VRWindow(), "Are you sure that you want delete "||VRMethod( "key_listbox", "GetString", key_sel.1)||" hotkey ?", "Warning !", "Query", "Buttons.", 2,  )
 If  id=1 then do
  result=SysIni('WIN95KEY.INI','KEYS',VRMethod( "key_listbox", "GetItemData", key_sel.1),'DELETE:')
  call key_listbox_refresh 
 end 
end

return

/*:VRX         Remove_Plagin_b_Click
*/
Remove_Plagin_b_Click: 
if VRMethod( "plugin_listbox", "GetString", plugin_sel.1)<>'' then do
 Buttons.1 = "Yes,"
 Buttons.2 = "No"
 Buttons.0 = 2
 id = VRMessage( VRWindow(), "Are you sure that you want delete "||VRMethod( "plugin_listbox", "GetString", plugin_sel.1)||" function ?", "Warning !", "Query", "Buttons.", 2,  )
 If  id=1 then do
  result=SysIni('WIN95KEY.INI','PLUGINS',VRMethod( "plugin_listbox", "GetString", plugin_sel.1),'DELETE:')
  call plugin_listbox_refresh 
 end 
end
return

/*:VRX         Save_key_pb_Click
*/
Save_key_pb_Click: 
/* формируем имя ключа */

/* Теперь проверяем, выбран ли плагин  */
if VRGet( "Plugin_combo_box", "Selected" )=0 then do
 Buttons.1 = "Ok"
 Buttons.0 = 1
 call VRMessage VRWindow(), "Key not saved. You must select plugin !", "Error", "Error", "Buttons.", ,  
 return
 return
end

modvalue.1=''
ok = VRMethod( "Modifiers_lb", "GetSelectedStringList", "modvalue." )
count=0
result_string.1=modvalue.1
do tmp=2 to modvalue.0
 result_string.1=result_string.1||'+'||modvalue.tmp
end
if result_string.1<>'' then result_string.1=result_string.1||'&'
if VRGet( "CODE_FIELD", "Value" )='' then do
 Buttons.1 = "Ok"
 Buttons.0 = 1
 call VRMessage VRWindow(), "Key not saved. You must specify key code !", "Error", "Error", "Buttons.", ,  
 return
return
end

result_string.1=result_string.1||scancode_field

/* string.1 содержит имя ключа ! */

/* Если парам<>'' , то всё установлено руками и параметры берём 
из парам.
Если комбо=1 и парам='' , то плагин выбран, но не откофигурирован. Проверим,
а надо ли онор ему...
если комбо=0 и парам='', то плагин не изменялся (точнее не трогался :))
Берём параметры из string ! */
result_string.2=''

if (parametr_string<>'') then 
do
 result_string.2=parametr_string
end
if (combo_reset=1)&(parametr_string='') then do

resid=translate(VRGet( "Plugin_combo_box", "Value" ))
parse value SysIni('WIN95KEY.INI','PLUGINS',VRGet( "Plugin_combo_box", "Value" )) with infile','
if step1()<>0 then do
  call VRMessage VRWindow(), "Key not saved. I can`n read pluggin settings :(", "Error", "Error", , ,   
  return
  return
end
 if plugin.mandatory.0=1 then do
  call VRMessage VRWindow(), "Key not saved. You must specify plugin settings !", "Error", "Error", , ,   
  return
  return
 end 

 result_string.2=VRGet( "Plugin_combo_box", "Value" )||'("")'
end
if (combo_reset=0)&(parametr_string='') then result_string.2=string
result=SysIni('WIN95KEY.INI','KEYS',space(result_string.1),space(result_string.2)||d2c(0))
call key_listbox_refresh
drop modvalue.
drop result_string.
return

/*:VRX         Save_keys
*/
Save_keys: 

return

/*:VRX         Window1_Close
*/
Window1_Close:
    call Quit
return

