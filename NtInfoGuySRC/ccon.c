#include <stdio.h>
#include <stdbool.h>
#include <windows.h>

#define NAME ZZWANG
#define STR1(x) #x
#define STR2(x) L###x

#define STRA(x) STR1(x)
#define STRW(x) STR2(x)

#define MSGA "Hello " STRA(NAME)
#define MSGW L"Hello " STRW(NAME)


int main(int argc,char *argv[],char *env[])
{
	printf("%S\n",MSGW);
	
	getchar();
	return 0;
}
