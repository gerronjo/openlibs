#ifndef _KERNEL_HOOK_CHECK_H
#define _KERNEL_HOOK_CHECK_H

#include "ntifs.h"
#include <ntimage.h>
#include "ntos.h"
#include "ldasm.h"
#include "libdasm.h"
//#include "xde.h"

extern PSERVICE_DESCRIPTOR_TABLE OriginalServiceDescriptorTable;

#define DEEP_LEN 0x03

extern WCHAR *SystemKernelFilePath;
extern BYTE *ImageModuleBase;
extern ULONG SystemKernelModuleBase;
extern ULONG SystemKernelModuleSize;

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

extern BOOL DebugOn;

ULONG ulNtDllModuleBase;
int IntHookCount;


HANDLE MapFileAsSection(
	PUNICODE_STRING FileName,
	PVOID *ModuleBase
	);

ULONG GetSystemRoutineAddress(
	int IntType,
	PVOID lpwzFunction
	);

BOOL IsAddressInSystem(
	ULONG ulDriverBase,
	ULONG *ulSysModuleBase,
	ULONG *ulSize,
	char *lpszSysModuleImage
	);

PIMAGE_NT_HEADERS RtlImageNtHeader(PVOID ImageBase);

ULONG GetEatHook(ULONG ulOldAddress,int x,ULONG ulSystemKernelModuleBase,ULONG ulSystemKernelModuleSize);

#endif