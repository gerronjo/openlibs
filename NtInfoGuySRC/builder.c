/*
builder.exe的源代码，用于创建一个控制台，并且初始化GCC以及DDK的环境。
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <windows.h>

int main(void)
{
	WinExec("cmd /k \"b.bat\" p",SW_SHOWNORMAL);
	WinExec("start cmd /k \"db.bat\"",SW_SHOWNORMAL);
	return 0;
}
