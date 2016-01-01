#ifndef _SYSMODULE_H_
#define _SYSMODULE_H_

#include "ntifs.h"
#include "ntos.h"

//////////////////////////////////////////////////////////////////////////////////////

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

int ExAllocatePoolWithTagPatchCodeLength;
PVOID ExAllocatePoolWithTagRet;

extern PDRIVER_OBJECT PDriverObject;

extern BOOL DebugOn;


VOID __stdcall StealthCodeScanInExAllocatePoolWithTag(ULONG ulPage);

ULONG GetSystemRoutineAddress(int IntType,PVOID lpwzFunction);
BOOL IsAddressInSystem(ULONG ulDriverBase,ULONG *ulSysModuleBase,ULONG *ulSize,char *lpszSysModuleImage);
NTSTATUS SafeCopyMemory(PVOID SrcAddr, PVOID DstAddr, ULONG Size);
#endif