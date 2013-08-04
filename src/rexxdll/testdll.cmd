/* testdll.cmd */

say 'Started'

call RXFuncAdd 'w95kQueryOneScancode', 'W95K_RX', 'w95kQueryOneScancode'

/* w95kQueryOneScancode(ParentWindowHandle, x position, y position, title)*/
say w95kQueryOneScancode('', '', '', 'Test - press key');

