#SingleInstance force
Run, C:\Emuls\3DO\freedo_1_8\FreeDo1.9.exe
WinWait, FreeDO (Phoenix Sources - 35.1`%), 
IfWinNotActive, FreeDO (Phoenix Sources - 35.1`%), , WinActivate, FreeDO (Phoenix Sources - 35.1`%), 
WinWaitActive, FreeDO (Phoenix Sources - 35.1`%), 
Send, {ALTDOWN}{ALTUP}f{ENTER}C:\Projects\3DOproj\demo.iso{ENTER}
Send, {ALTDOWN}c{ALTUP}{ENTER}{ENTER}{F4}
return