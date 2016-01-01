#include "StdAfx.h"

typedef struct _INLINEHOOKINFO_INFORMATION {          //INLINEHOOKINFO_INFORMATION
	ULONG ulHookType;
	ULONG ulMemoryFunctionBase;    //原始地址
	ULONG ulMemoryHookBase;        //HOOK 地址
	CHAR lpszFunction[256];
	CHAR lpszHookModuleImage[256];
	ULONG ulHookModuleBase;
	ULONG ulHookModuleSize;

} INLINEHOOKINFO_INFORMATION, *PINLINEHOOKINFO_INFORMATION;

typedef struct _INLINEHOOKINFO {          //InlineHook
	ULONG ulCount;
	INLINEHOOKINFO_INFORMATION InlineHook[1];
} INLINEHOOKINFO, *PINLINEHOOKINFO;

PINLINEHOOKINFO InlineHookInfo;

CHAR* setClipboardText(CHAR* str);