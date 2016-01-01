@echo off
rem **********************************************************************
rem 				NT Driver Guy
rem		Made By 侯佩|hopy 2009-01-17 @ HFE_Airport
rem blog	: http://blog.csdn.net/mydo
rem email 	: k26s@hotmail.com
rem **********************************************************************

cls

echo GCC NT Driver Guy 2009-02-12 by 侯佩@hopy

rem 设置 Masm 和 MinGW 的安装路径
set MASM_PATH=D:\work\masm32
set MINGW_PATH=C:\Program Files\MinGW5.1.6

rem 设置项目的名称
set Project_Name=NtInfoGuy

set Bin_Path=%MASM_PATH%\bin;%MINGW_PATH%\bin

if (%1) == (p) goto SETPATH
if (%1) == (c) goto CHECK
if (%1) == (f) goto FREE
if (%1) == (F) goto FINAL
if (%1) == (bu) goto BUILDER
if (%1) == (h) goto HELP

echo Unknow KONB , Try To : b h
goto EXIT

:HELP
echo "生成 NT GCC OBJECT 使用方法  b [p|c|f|F|bu|h]"
echo p : 	设置Masm和MinGW路径，由builder使用，一般不会再次调用
echo c : 	以check版本生成项目
echo f : 	以free版本生成项目
echo F : 	以final版本生成项目
echo bu: 	重新生成builder.exe
echo h : 	打印帮助信息
echo 生成 DDK PART 请参考帮助 db h
goto EXIT

:BUILDER
echo build for builder.exe...
gcc -std=c99 -Wall -O3 -g0 -s -o builder.exe builder.c
goto EXIT

:SETPATH
color 0a
path %path%;%Bin_Path%
echo Set Bin_Path = %Bin_Path% Successed!
goto EXIT

:CHECK
if not exist check mkdir check
cd check
del /Q *.*
cd ..
set BUILD_PATH=check
set BUILD_KONB=-std=c99 -Wall -O0 -g3 -D_GOTO_DEBUG_
set LINK_KONB=
set BUILD_SYM=CHK
goto NEXT

:FREE
if not exist free mkdir free
cd free
del /Q *.*
cd ..
set BUILD_PATH=free
set BUILD_KONB=-std=c99 -Wall -O3 -g0 -D_GOTO_DEBUG_
set LINK_KONB=-s
set BUILD_SYM=FRE
goto NEXT

:FINAL
if not exist final mkdir final
cd final
del /Q *.*
cd ..
set BUILD_PATH=final
set BUILD_KONB=-std=c99 -Wall -O3 -g0
set LINK_KONB=-s
set BUILD_SYM=FNL
goto NEXT

:NEXT

@echo on

gcc %BUILD_KONB% -DPRJNAME=%Project_Name% -c -o %BUILD_PATH%\%Project_Name%_sys.obj sys.c sysext.c

link /nologo /driver /subsystem:native /base:0x10000 /align:0x20 /entry:DriverEntry /nodefaultlib:libc.lib /nodefaultlib:oldnames.lib /defaultlib:%MASM_PATH%\lib\ddk\w2k\ntoskrnl.lib /defaultlib:%MASM_PATH%\lib\ddk\w2k\hal.lib /libpath:%MASM_PATH%\lib\ /libpath:%MASM_PATH%\lib\ddk\w2k /ignore:4078 /out:%BUILD_PATH%\sys.sys %BUILD_PATH%\%Project_Name%_sys.obj DDK_PART\DDK_C.obj

windres -D%BUILD_SYM% -i res.rc -O COFF -o %BUILD_PATH%\%Project_Name%.res

windres -D%BUILD_SYM% -i exe.rc -O COFF -o %BUILD_PATH%\%Project_Name%_exe.res

gcc %BUILD_KONB% -DPRJNAME=%Project_Name% -c -o %BUILD_PATH%\dll.o dll.c
gcc %BUILD_KONB% -DPRJNAME=%Project_Name% -c -o %BUILD_PATH%\dllext.o dllext.c

gcc -shared --subsystem,windows -Wl,--output-def,%BUILD_PATH%\%Project_Name%.def %LINK_KONB% -o %BUILD_PATH%\%Project_Name%.dll %BUILD_PATH%\dll.o %BUILD_PATH%\dllext.o %BUILD_PATH%\%Project_Name%.res Crypt32.Lib WinTrust.Lib

@echo off
echo Wscript.Sleep Wscript.Arguments(0) * 1000>delay.vbs
delay.vbs 0.5
del delay.vbs
echo wake up!
@echo on

gcc -shared --subsystem,windows -Wl,--kill-at %LINK_KONB% -o %BUILD_PATH%\%Project_Name%.dll %BUILD_PATH%\dll.o %BUILD_PATH%\dllext.o %BUILD_PATH%\%Project_Name%.res Crypt32.Lib WinTrust.Lib

dlltool -d %BUILD_PATH%\%Project_Name%.def --dllname %BUILD_PATH%\%Project_Name%.dll --output-lib %BUILD_PATH%\lib%Project_Name%X.a --kill-at

@echo off
rem gcc -std=c99 -Wall -g0 -O3 -D PRJNAME="\"%Project_Name%\"" -o dbg.exe dbg.c
@echo on

gcc %BUILD_KONB% -DPRJNAME=%Project_Name% %LINK_KONB% -L %BUILD_PATH%\ -o %BUILD_PATH%\dbg.exe dbg.c -l%Project_Name%X

gcc %BUILD_KONB% -DPRJNAME=%Project_Name% %LINK_KONB% -L %BUILD_PATH%\ -o %BUILD_PATH%\NtInfoGuy.exe NtInfoGuy.c HyColorText.c HyDbgHelp.c dbghelp.lib -l%Project_Name%X -lntdll %BUILD_PATH%\%Project_Name%_exe.res

:EXIT
 