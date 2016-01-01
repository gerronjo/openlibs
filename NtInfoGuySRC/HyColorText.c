#include "HyColorText.h"

//打印彩色文本,成功返回true，否则返回false
bool HyPrintColorText(const char *pText,USHORT color)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	//DWORD CharsWritten;
	if(!pText) return false;
	if(!SetConsoleTextAttribute(hOut,color)) return false;
	//if(!WriteConsole(hOut,pText,strlen(pText)+1,&CharsWritten,NULL)) return false;
	//不用WriteConsole这么麻烦，用C的打印函数就可以了！
	printf(pText);
	//恢复白色文字和黑色背景
	return SetConsoleTextAttribute(hOut,CLR_TEXT_WHITE);
}
