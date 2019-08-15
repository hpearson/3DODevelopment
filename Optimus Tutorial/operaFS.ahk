#SingleInstance force
IfWinExist OperaFS (De)Compiler 0.1 [altmer.arts-union.ru]
{
	WinActivate
	Send !{f4} ; Simulates the keypress alt+f4
	Run C:\Projects\3DOproj\OperaFS\OperaFS.exe
	WinWait, OperaFS[De]Compiler,
	Sleep, 250 ; wait for 200ms
	SEND {Tab}{Space}
	Sleep, 250 ; wait for 200ms
	WinWait, Select Directory,
	SEND {Tab}C:\Projects\3DOproj\ISOTemplate{ENTER}
	Sleep, 250 ; wait for 200ms
	SEND C:\Projects\3DOproj\demo.iso{ENTER}
	Sleep, 250 ; wait for 200ms
	Send !{f4} ; Simulates the keypress alt+f4
}
else
{
	Run C:\Projects\3DOproj\OperaFS\OperaFS.exe
	WinWait, OperaFS (De)Compiler 0.1 [altmer.arts-union.ru],
	Sleep, 250 ; wait for 200ms
	SEND {Tab}{Space}
	Sleep, 250 ; wait for 200ms
	WinWait, Select Directory,
	SEND {Tab}C:\Projects\3DOproj\ISOTemplate{ENTER}
	Sleep, 250 ; wait for 200ms
	SEND C:\Projects\3DOproj\demo.iso{ENTER}
	Sleep, 250 ; wait for 200ms
	Send !{f4} ; Simulates the keypress alt+f4
}

return
