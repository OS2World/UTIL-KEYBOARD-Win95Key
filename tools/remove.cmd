/* Uninstalling win95key ver <2 */

ModuleName="WIN95KEY"

call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

bootDrive = Left(VALUE('RUNWORKPLACE',,'OS2ENVIRONMENT'),2)

KeyValue=SysIni("USER", "SYS_DLLS", "LoadOneTime")

Installed=0

do i=1 to Length(KeyValue)
   if Substr(KeyValue, i, Length(ModuleName)) = ModuleName then
      Installed=i
end

i=Installed

if Installed <> 0 then
 do

   if (i < 2) then
     NewKey1=""
    else
     NewKey1=Substr(KeyValue, 1, i-1)

   if (i+Length(ModuleName)+1) > Length(KeyValue) then
     do
      if (i < 3) then
        NewKey1=""
       else
        NewKey1=Substr(KeyValue, 1, i-2)
       NewKey2=""
     end
    else
     NewKey2=Substr(KeyValue, i+Length(ModuleName)+1, Length(KeyValue)-i-Length(ModuleName))

   NewKey=NewKey1||NewKey2

   say "Do you REALLY want to remove "ModuleName" ? ( y | n )"
   Answer=charin()
   if Answer="y" then
     do 
       if SysIni("USER", "SYS_DLLS", "LoadOneTime", NewKey) = '' then
          do
           say ModuleName" deregistered."
           "@unlock" bootDrive || "\os2\dll\win95key.dll >nul 2>nul"
           say "Please reboot. You may delete "bootDrive"\os2\dll\win95key.dll manually"
          end
         else
          do
           say "Failed for unknown reason :("
          end
     end

 end
else
 say "Win95key does not seem to be installed."
exit


