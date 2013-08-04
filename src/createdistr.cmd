/* building package */
/* Destanation dir, must NOT be in win95key tree */
dstdir="D:\apps\prg\mysrc\win95key";
say "Building dll ..."
 rc=directory("dll");
 call build
say "Building exe ..."
'@cd ..\exe'
 call build
say "Building kbdfixd ..."
 '@cd  ..\kbdfix\kbdfixd'
  call build
say "Building kbdfixrestart  ..."
 '@cd  ..\kbdfixrestart'
  call build 
say "Building testmon   ..."  
 '@cd  ..\testmon'
  call build  
say "Building APM plugin ..."
 'cd ..\..\plugins\apm'
  call build   
say "Building OD plugin ..."
 'cd ..\od'
  call build   
say "Building pipe plugin ..."
 'cd ..\pipe'
  call build   
say "Building sem plugin ..."
 'cd ..\sem'
  call build   
say "Building std plugin ..."
 'cd ..\std'
  call build   
say "Building plugin tester ..."
 'cd ..\..\plugintest'
  call build   
say "Building rexxdll ..."
 'cd ..\rexxdll'
  call build     
say "Building SHOWKEY ..."
 'cd ..\showkey'
  call build   
say "Creating installation package:"
say "copy sources..."
'mkdir -p '||dstdir
'rm -rf '||dstdir
'cp -f -R ../../ '||dstdir
'cp -f ..\win95key.ini.dist '||dstdir||"/bin/win95key.ini"
'cp -f ..\kbdfix\kbdfixd\kbdfixd.cfg  '||dstdir||"/bin/kbdfixd.cfg"
'rm -f 'dstdir'/bin/pmconf.inf'
'rm -f 'dstdir'/bin/*.ori'
'cd 'dstdir
"zip -R -9 -S  win95key.zip *.*"
say "Done :)"
exit;
build:
'@nmake clean  >nul'
'@nmake >nul'
'@nmake clean  >nul'
return;