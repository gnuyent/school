Windows CMD Commands:

Note that command flags in Windows are denoted by

forward slashes by convention rather than hyphens,

and though the hyphen notation is supported by

most commands, it is not by all.

i.e.

>time /t

VS

>time -t

Also, while Unix based commands all have manual pages that can

be accessed by either the info or man commands, Windows uses the

format:

>specificCommand /?

for command info.

========================================================================

General:

>dir

Lists current directory contents.

>md yourFolder1 yourFolder2 yourFolder3

mkdir still works as well; both make the folders specified.

>rd yourFolder1 yourFolder2 yourFolder3

rmdir also works here.  Removes specified folder(s).

>cd yourFolder

Changes directory (thank goodness it's still the same!)

>type yourFile.txt

The cat of Windows.

>ren yourOldFileName yourNewFileName

Rename.

>move yourFile yourFolder

Self explanatory..

>copy yourFile yourFolder

Yep.  Shoulda just copied Unix here, Windows.

>del yourFile

Deletes your file.  Erase should also work.

>time

>date

By default, these commands will display their items,

but also prompt the user to change them.  Pressing

Enter without inputting any changes will not alter

these two, but know that the /t flag can be used

to only display these items.  i.e. time /t

>where someFileName

Returns the path of the specified file name.

>color ##

i.e. color 02

Changes the screen and text coloration respectively.

Here are some common numbers:

0 black, 2 green, 9 bright blue, C bright red, F bright white.

>echo whatever

Still works.  The old < > | operators also work, so

we could take some brief notes down with:

>echo I am a potato! > quack.txt

>notepad whatever2.txt

Uses notepad.exe to open/create whatever2.txt

>someExecutable

i.e. quack.txt

Yep.  That's all there is to it!

This runs the specified executable, and can also be used to edit

existing text files.  It works similar in concept to Unix's

"./someExecutable"

>cls

Clear the terminal screen.

>exit

Closes the terminal window.

========================================================================

Being administrative:

shutdown:

This is gonna be a fun one. Note that:

/r              Restarts

/l                  Logs off

/m \\desiredIP      Specifies a comp.

/t #                Specifies the time in seconds

/s                  Actually shuts down

/a                  Aborts a currently scheduled shutdown

/c                  Leaves a nice little message to boot

Soo if someone somehow got in and ran something like:

>shutdown /s /c "lolol urfked"

If we get a CMD prompt up within 60 secs, we can simply abort it with:

>shutdown /a

Though our most practical application with shutdown will be rebooting.

>shutdown /r /t 0

>net user yourUsername yourPassword

Changes the username and password for a user.

>hostname

Tells us the name of the local machine.

>ver

Tells us the OS version.

>whoami

Tells us who we are.

>ipconfig

Equivalent of the renowned ifconfig.  Displays IP.

>systeminfo

Hmm.  I wonder what it does..

>tasklist

Lists current processes.  For only those actively running, use

>tasklist /fi "status eq running"

>taskkill /pid desiredNumber

Self explanatory.

>taskmgr

Opens up the graphical Task Manager.  You can also use CTRL+SHIFT+ESC

>control panel

Take a guess.

>msinfo32

The graphical version of systeminfo!

>netstat /a /n /o

(Or -ano if you prefer)

Grab the network statistics of -a all connections and listening ports with

the -n numerical form of port numbers and addresses, along with the -o owning

process ID utilizing each connection.

If you close all windows, and you still see some states as

"Established", note the PIDs, open up task manager or

use taskkill, and destroy those processes.  In more times than not,

these are active processes/connections used by hackers.  You can also

use

>netstat /b

To display executables invovled with certain connections, so this

can be also used to help track connected malware.

>eventvwr

Opens up the Event Viewer logs.

From here, if you go into

Event Viewer (Local) > Windows Logs > System

You'd also be able to get the details of that recent shutdown duel,

among other things.

>chkdsk /f C:

Fixes errors on the C drive.

>chkdsk /r C:

Locates sectors and repairs them on the C drive.

>defrag C:

Defrags C drive.  -a can be used to purely analyze and not immediately defrag.

========================================================================

Making a script:

>notepad yourScriptName.bat

Opens up a batch file with notepad.

Inside, you may create whatever command cycles you'd like, separated by newlines.

Example 1:

>notepad test_script1.bat

title Kirk_Tutorial &rem This is a comment thanks to Rem!

rem Rem is short for remark, and we should use &rem for the in-line variant.

color 0F &rem Thought we should make the text brighter :P

echo Hello_world!

@echo Silent Hello! &rem The @ silences command restatements.

@echo. &rem The echo. makes a newline

@echo The quieter you become...

>test_script1.bat

The above would create a file in Notepad called test_script1.bat,

which we would be able to directly edit.

We placed the lines (title down to the that last echo) denoted above in this file,

closed Notepad, and ran the batch file on the command line, which ran our commands.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Example 2:

>notepad adder.bat

@rem This test script was designed to give but an intro to the possibilities, and

@rem is therefore intentionally inefficient.

@rem But let's do some math! % denotes an argument, %var% denotes a variable.

@if %errorlevel% neq 0 ((echo %errMsg%) & (goto :EOF))

@set num1=%1

@set opr=%2

@set num2=%3

@set errMsg=Command Syntax is adder.bat num1 + num2

@set specMsg=Dat sum good shit.  Mmm...

@if not "%opr%"=="+" ((echo %errMsg%) & (exit /b 0)) else (set /a sum=%num1%+%num2%)

@rem The above lines say, if the statement is semantically invalid, or operator isn't a plus.

@rem tell the user our syntax and exit.

@rem Otherwise, the program proceeds to add the two numbers,

@rem where /a enables arithmetic setting.

@echo %sum%

@if %sum% equ 420 ((echo %specMsg%) & (timeout /t -1 /nobreak > nul))

@if %sum% equ 1337 (color 0A)

>adder.bat 3 + 8

------------------------------------------------------------------

And now, POWERSHELL:

Yes, the rumors are true -- some of our old Unix commands make

their triumphant return, but only very common ones.

Welcome back guys.  We missed you.

Some things have also been changed from CMD.

Take our relatively simple color example.  To do the same thing

in PowerShell, we'll use

>$host.UI.RawUI.ForegroundColor="darkgreen"

>$host.UI.RawUI.BackgroundColor="black"

>Clear-Host

Clear-Host is Powershell's way of clearing the screen.  But guess what?

We can also use CMD's cls and Unix's clear!

Here are some other neat PowerShell commands:

http://ss64.com/ps/
