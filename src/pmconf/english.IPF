:userdoc.
:prolog.
:title.Configuration tool for WIN95KEY 
:eprolog.
.nameit symbol=product text='PM Conf'
.nameit symbol=Version text='0.91'
.*--------------------------------
:h1 x=right y=top .About PM Conf
:p.
I was always upset with the absence of normal possibility to assign events 
on the user defined hotkey comnbinations in OS/2. All programs of this kind which
I have seen earlier were extremely poor or very buggy...
But not long ago
:hp2. Vik :ehp2. has issued a wonderful software win95key which posesses a lot of 
possibilities and useful features.
:p.
For now win95key has following features:  
:ul.  
:li. Allows assigning combinations of :hp2.any:ehp2. keys on your keyboard 
(including WIN95 keys, ATX extension etc)
:li. Has plugin support which are responsible for reaction on key pressing event.
Now following plugins are available:
:ul.
:li.:hp1.SwitchTo:ehp1. switches to a program chosen by a user.
:li.:hp1.WindowList:ehp1. shows the task list.
:li.:hp1.DesktopMenu:ehp1. shows the desktop menu.
:li.:hp1.ContextMenu:ehp1. shows the active application menu.
:li.:hp1.WarpCenterMenu:ehp1. allows controlling the warpcenter from the keyboard.
In particular to 'press' the OS2WARP button, to change the current tray and so on.
:li.:hp1.OpenObject:ehp1. Launches an application or opens an object.
:li.:hp1.Volume:ehp1. Allows controlling the system volume.
:li.:hp1.SendToPipe:ehp1. Allow to send user defined text to pipe (e.g. useful for Z! MP3 player).
:eul.
:li. Need no rebooting and can reload configurations without program restarting.
:eul.   
:p. 
With all off these wonderful features the program is completely lacking 
the GUI interface,
so all values should have been written down manually in ini file...

Thus I decided to write the PM Conf program.

:h1  x=right y=top  .Possibilities of PM Conf program

At present PM Conf 0.91 allows:
  
:ul.  
:li.To show assigned hotkeys, to edit them and to add new.
:li. To edit the parameters of a plugin assigned for a hotkey combination.
The plugin parameters are taken from its resources.
:li. To support Drag &amp. Drop (for example, in the OpenObject plugin)
:li. To add and remove plugins from external libraries.
:li.To edit and assign Scan codes by special keys (modifiers), for example, 
LWIN.
:li.To reload the win95key program settings.
:eul. 

:h1.Using PM Conf
:h2 x=right y=top  .
Assigning hotkeys
:p.
After launching the program you will see the "Hotkeys" bookmark where you 
can browse the list of assigned keys and response on them. :p.
To add a new combination:
:ol.
:li. In the field "Code" (scan-code) type the scan-code of a key or its title
(for example  "F").
:li. In the "Modifiers" field (modifiers) select the keys with which this key
is intended to deal
(for example CTRL and Shift). In the "Settings" bookmark
you can change or add modifiers. If you want to use a scan-code without modifiers,
just leave modifiers unselected.
:li. After this select in the "Plugin" window the plugin which you want to 
assign for chosen combination of keys and press the "Settings" button.
You can add or remove plugins in the "Settings" bookmark.
:eol.
:p. To change parameters select in the "Hotkey" field the combination you want to
chamge, thereafter change the parameters and press the "Change" button.:p. To 
delete a key combination choose it in the "Hotkey" list and press "Remove" button.
:p. To activise the changes made press the "Apply" button.

:h2 x=right y=top  .
Adding and removing plugins.
:p. If you want to add a plugin press the button "Add new" in the plugin section
of the "Settings" bookmark. Then in the dialog window press the "Locate" button and
choose the DLL with plugins. 
.br

In the "Add plugin" window there appears the plugin list stored in the 
chosen library. 
.br Select the plugins you want to setup into the system 
and press the "Add" button. After this the selected plugins will appear in the 
"Function" window.:p.
To delete a plugin select it in the "Function" window and press the "Remove" 
button.

:h2 x=right y=top .Handling modifiers.    
:p.  The modifiers section describes keys which are used in addition to standard 
modifiers "Ctrl", "Alt" and "Shift". To add a modifier press the button "Add new" 
in the section "Modifiers", then type the scan-code of the modifier and 
its name. 
.br
After that the new modifier will be available in the "Hotkeys" bookmark when adding
new key combinations.:p.
To remove or to edit a mnodifier select it in the "Modifiers" window and
press "Remove" or "Edit".

:h2 x=right y=top .win95key parameters.
:p.  The section "global" of the "Settings" bookmark contains three checkboxes 
for setting win95 parameters.

.br The "Immediately key reaction"  checkbox is responsible for keyboard reaction.
If it is on, the keyboard reaction is initiated on keystroke (immediately
after a key is pressed). If this checkbox is off, the keyboard reaction 
initiates only after releasing a key.

.br If the checkbox "Run minimized" is on, win95key will be run minimized at 
next launch.

.br The checkbox "hide in tasklist" allows to hide win95key from the
OS/2 tasklist.

.br After setting these parameters it is necessary to click on "Apply" button
 to activate them.

:h1 x=right y=top .Information for developers.
:p. This section contains information about creating win95key plugins
supported by PM Conf.:p.
How to write plugins please refer to the win95key documentation. If you want 
your plugins to be recognized by pmconf programs you should perform following 
manipulations:

:ol.
:li. Create the plugin description file (for example dll.dat). It has the 
following format: 
.br
First line:
:color fc=blue.
:xmp.
Plugin DLL=:hp1.<name of a library>:ehp1.,:hp1.<numbers of plugins>:ehp1. :color fc=default.
:exmp.
.br
Second and following lines:
:color fc=blue.
:xmp.
:hp1.<Name of function>:ehp1.=:hp1.<function id>:ehp1. :color fc=default.
.br
:exmp.
:hp1.<Plugin name>:ehp1. is the name of a function in your dll, id 
is an integer > 1 unique for each of the functions. 
:li. Create the description file to every function contained in your plugin.
It has the following format:
.br 
First line
:xmp.
:color fc=blue.
Plugin :hp1.<Function name>:ehp1.=:hp1.<plugin description>:ehp1.,:hp1.<number of parameters>:ehp1. :color fc=default.
:exmp.
.br
Second line
.br
:color fc=blue.
:xmp.
HELPTEXT=:hp1.<help text>:ehp1. :color fc=default.
:exmp.
.br
:hp1.<help text>:ehp1. is displayed in the Settings... window  
when pressing the help button. If the text contains several lines
use the '|' symbol to start a new line.
.br
Third and fourth lines:
:color fc=blue.
:xmp.
TYPE=:hp1.<type of parameter>:ehp1., DESCR=:hp1.<description>:ehp1., MANDATORY=:hp1.<n>:ehp1., VALUE=:hp1.<default value>,:ehp1., :color fc=default.
:exmp.
.br
The lines beginning with TYPE= are plugin parameter descriptions.
The parameter <type of parameter> can be either SELECTOR or INPUTFIELD.
Maximal number of parameters is 2. The parameter MANDATORY=1 indicates that the 
user should fill in with the value of this parameter. For additional information
see the descriptions in the 
'\src\plugins\std\dat' directory - everything there seems to be clear. 
:li. Create the .rc file of descriptions and  add the resources created to your dll.
.br
The .rc file should look like as follows:
.br
:xmp.
RESOURCE 100 1 LOADONCALL FIXED :hp1.<plugin description file>:ehp1.
RESOURCE 100 :hp1.<function ID>:ehp1. LOADONCALL FIXED :hp1.
<function description file>:ehp1.
:exmp.
.br
Second and following lines contain links to plugin description files.
<function ID> is taken from the description file.
.br After you have created the .rc file add your resources to the 
dll with plugins: 
In the OS/2 command line type "RC.EXE <rc file> <dll file>.
OK, now you can use dll with pmconf.
:eol.
:p. Attention! Don't compress the resource containing dll with compression utilities 
like lxlite ! 
:h1 x=right y=top . Acknowledgments.
Special thanks:
:sl.
:li.:hp2.Viktor Remennik:ehp2. - for win95key.
.br
:hp2.Martin Lafaix:ehp2. - for rdc resources decompiler. A part of the
program code was taken from it.
:esl.
:h1 x=right y=top  . Author
:p.
I would appreciate your notes and comments.
:p.
:p.
:p.
:p.
.br
Alex Samorukov
.br
   e-mail :hp2.samm@os2.ru:ehp2.
.br   
   Fidonet :hp2.2&colon.463/598@Fidonet :ehp2.
.br
:h1. List of changes 
.im history.txt
.*
.*

:euserdoc.