#include "stdafx.h"
#include "Md5.h"

typedef struct _SYSINFO_INFORMATION {          //SYSINFO_INFORMATION
	ULONG ulSysBase;
	ULONG SizeOfImage;
	WCHAR lpwzFullSysName[256];
	WCHAR lpwzBaseSysName[256];
	ULONG LoadCount;
	int  IntHideType;     //ÊÇ·ñÒþ²Ø

} SYSINFO_INFORMATION, *PSYSINFO_INFORMATION;

typedef struct _SYSINFO {          //sysmodule
	ULONG ulCount;
	SYSINFO_INFORMATION SysInfo[1];
} SYSINFO, *PSYSINFO;

PSYSINFO SysModuleInfo;


extern unsigned int conv(unsigned int a);


BOOL FileVerify(char *lpszFullPath,WCHAR *lpwzFileMd5,WCHAR *lpwzTrue);
CHAR* setClipboardText(CHAR* str);