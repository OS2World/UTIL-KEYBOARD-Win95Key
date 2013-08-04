/* */
if arg(1)="clean" then call cleanup

'ipfc -i russian.ipf ..\..\bin\russian.inf'
'ipfc -i english.ipf ..\..\bin\english.inf'
'lxlite pm_conf.exe'
'packvxrx.exe PM_Conf.EXE /T+ /D+'
'cp PM_Conf.EXE ../../bin/'
'exit'

cleanup:
'rm PM_Conf.EXE *.res *.$$$'