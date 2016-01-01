@echo off
rem **********************************************************************
rem 				NT Driver Guy
rem		Made By 侯佩|hopy 2009-01-17 @ HFE_Airport
rem blog	: http://blog.csdn.net/mydo
rem email 	: k26s@hotmail.com
rem **********************************************************************

cls

echo NT DDK DRIVER BUILDER 2010-06-17 by 侯佩@hopy

rem 设置DDK的安装路径
set DDK_PATH=d:\code\WINDDK\3790.1830

if (%1) == (sc) goto SET_CHK
if (%1) == (sf) goto SET_FREE
if (%1) == (c) goto CHK
if (%1) == (f) goto FREE
if (%1) == (h) goto HELP

echo Unknow KONB , Try To : db h
goto EXIT

:HELP
echo "生成 DDK PART 使用方法  db [sc|sf|c|f|h]"
echo sc : 	设置DDK编译版本为check
echo sf : 	设置DDK编译版本为free
echo c  : 	以check版本生成项目
echo f  : 	以free版本生成项目
echo h  : 	打印帮助信息
echo 生成 NT GCC OBJECT 请参考帮助 b h
goto EXIT
rem color 0c

:SET_CHK
set CD_PATH=%cd%
call %DDK_PATH%\bin\setenv.bat %DDK_PATH% chk WXP
cd %CD_PATH%
echo Set NtDriverGuy Object to CHK Mode
goto EXIT

:CHK
set BUILD_KONB=/Od /Gz /c /W3 /D_X86_
echo Build With CHK
goto NEXT

:SET_FREE
set CD_PATH=%cd%
call %DDK_PATH%\bin\setenv.bat %DDK_PATH% fre WXP
cd %CD_PATH%
echo Set NtDriverGuy Object to FREE Mode
goto EXIT

:FREE
set BUILD_KONB=/Ox /Gz /c /W3 /D_X86_
echo Build With FREE
goto NEXT

:NEXT
cl %BUILD_KONB% /I %DDK_PATH%\inc\ddk\wxp /I %DDK_PATH%\inc\crt /TC /FoDDK_PART\DDK_C.obj DDK_PART\DDK_C.c

:EXIT