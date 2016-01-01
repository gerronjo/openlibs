#ifndef _SHADOWSSDT_H_
#define _SHADOWSSDT_H_

#include "ntifs.h"
#include "ntos.h"
#include "libdasm.h"

extern PSERVICE_DESCRIPTOR_TABLE OriginalShadowServiceDescriptorTable;
extern PSERVICE_DESCRIPTOR_TABLE ShadowSSDTTable;

extern BYTE *Win32kImageModuleBase;
extern ULONG ulWin32kBase;
extern ULONG ulWin32kSize;

//---------------------------------------------------------------------------------------
//ShadowSSDT
//---------------------------------------------------------------------------------------
typedef struct _SHADOWSSDTINFO_INFORMATION {          //SHADOWSSDTINFO_INFORMATION
	ULONG ulNumber;
	ULONG ulMemoryFunctionBase;
	CHAR lpszFunction[256];
	CHAR lpszHookModuleImage[256];
	ULONG ulHookModuleBase;
	ULONG ulHookModuleSize;
	int  IntHookType;                     //
} SHADOWSSDTINFO_INFORMATION, *PSHADOWSSDTINFO_INFORMATION;

typedef struct _SHADOWSSDTINFO {          //SSDT
	ULONG ulCount;
	SHADOWSSDTINFO_INFORMATION SSDT[1];
} SHADOWSSDTINFO, *PSHADOWSSDTINFO;

//---------------------------------------------------------------------------------------
PSHADOWSSDTINFO ShadowSSDTInfo;

BOOL bShadowHooked;
extern BOOL bShadowSSDTAll;

ULONG GetSystemRoutineAddress(
	int IntType,
	PVOID lpwzFunction
	);

unsigned long __fastcall GetFunctionCodeSize(
	void *Proc
	);

BOOL IsAddressInSystem(
	ULONG ulDriverBase,
	ULONG *ulSysModuleBase,
	ULONG *ulSize,
	char *lpszSysModuleImage
	);

BOOL RestoreShadowInlineHook(ULONG ulNumber);

#endif