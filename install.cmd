/*

  REXX Script to install Win 95 Key

*/

if RxFuncQuery('SysLoadFuncs') then do
   call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
   call SysLoadFuncs
end

say "Installing..."

dir = directory() || '\'

say "Creating folder"
call SysCreateObject "WPFolder", "Win95Key", "<WP_DESKTOP>", "OBJECTID=<WIN95KEY>;ICONVIEW=FLOWED,NORMAL;ALWAYSSORT=NO;ICONFONT=10.Helv;BACKGROUND=" || dir || "back.bmp,T;ICONFILE=" || dir || "", "replace"
call SysCreateObject "WPFolder", "Tools", "<WIN95KEY>", "OBJECTID=<WIN95KEYUTILFOLDER>;ICONVIEW=FLOWED,NORMAL;ALWAYSSORT=NO;ICONFONT=8.Helv;BACKGROUND=" || dir || "back.bmp,T", "replace"
call SysCreateObject "WPShadow", "Source Code","<WIN95KEY>","SHADOWID="||dir||"SRC;ICONVIEW=FLOWED,NORMAL;ALWAYSSORT=NO;ICONFONT=8.Helv;BACKGROUND=" || dir || "back.bmp,T", "replace"
say "Creating program objects"

call SysCreateObject "WPProgram", "Key Info", "<WIN95KEYUTILFOLDER>", "EXENAME=" || dir || "tools\showkey.exe;STARTUPDIR=" || dir ||"\tools\"||";ICONVIEW=FLOWED,NORMAL;ALWAYSSORT=YES;"
call SysCreateObject "WPProgram", "Remove win95key <2.xx dll", "<WIN95KEYUTILFOLDER>", "NOAUTOCLOSE=YES;EXENAME=" || dir || "tools\remove.cmd;STARTUPDIR=" || dir ||"\tools\"||";ICONVIEW=FLOWED,NORMAL;ALWAYSSORT=YES;"
call SysCreateObject "WPProgram", "Keyboard DosMon* tester", "<WIN95KEYUTILFOLDER>", "EXENAME=" || dir || "tools\testmon.exe;STARTUPDIR=" || dir ||"\tools\"||";ICONVIEW=FLOWED,NORMAL;ALWAYSSORT=YES;"
call SysCreateObject "WPProgram", "Reload KBDfixD configuration", "<WIN95KEYUTILFOLDER>", "NOAUTOCLOSE=YES;EXENAME=" || dir || "tools\reloadcfg.exe;STARTUPDIR=" || dir ||"\tools\"||";ICONVIEW=FLOWED,NORMAL;ALWAYSSORT=YES;"
call SysCreateObject "WPUrl", "Win95Key "||d2c(10)||" Documentation (English)", "<WIN95KEY>","URL="||dir||"doc\manual_eng.html", "REPLACE"
call SysCreateObject "WPUrl", "Win95Key "||d2c(10)||" Documentation (Russian)", "<WIN95KEY>","URL="||dir||"doc\manual_rus.html", "REPLACE"
call SysCreateObject "WPProgram", "Win 95 Key", "<WIN95KEY>", "OBJECTID=<WIN95KEY_EXE>;EXENAME=" || dir || "bin\win95key.exe;STARTUPDIR=" || dir ||"\bin\;ICONVIEW=FLOWED,NORMAL;ALWAYSSORT=YES;"
call SysCreateObject "WPProgram","PM Conf"||d2c(10)||"Documentation", "<WIN95KEY>", "EXENAME=view.exe;PARAMETERS=pmconf.inf;STARTUPDIR=" || dir ||"\bin\;ICONVIEW=FLOWED,NORMAL;ALWAYSSORT=YES;"
call SysCreateObject "WPProgram","PM Conf"||d2c(10)||"Configuration program", "<WIN95KEY>", "OBJECTID=<WP_PMCONF>;EXENAME=" || dir || "\bin\pm_conf.exe;STARTUPDIR=" || dir ||"\bin\;ICONVIEW=FLOWED,NORMAL;ALWAYSSORT=YES;"
say 'Do you want to add Win95Key object to WPS startup folder ? (Y/N).'
Pull key
if key="Y" then call SysCreateObject "WPShadow" , "Win95Key", "<WP_START>","SHADOWID=<WIN95KEY_EXE>;"

sel:
say 'Please select program help language.'
say '1 - English'
say '2 - Russian'

Pull key
select
when key=1 then do
'@copy bin\english.inf bin\pmconf.inf'
end
when key=2 then do
'@copy bin\russian.inf bin\pmconf.inf'
end

otherwise
say 'Incorrect choice !' d2c(007)
 signal sel
end

say "Installation completed"
