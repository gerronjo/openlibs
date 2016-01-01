#ifndef _SYSTEM_THREAD_H_
#define _SYSTEM_THREAD_H_

#include "ntifs.h"
#include "ntos.h"
#include "KillProcess.h"

#define THREAD_SUSPEND_RESUME          (0x0002)  // winnt
#define PS_CROSS_THREAD_FLAGS_SYSTEM 0x00000010UL 

extern PSERVICE_DESCRIPTOR_TABLE OriginalServiceDescriptorTable;

typedef enum _KAPC_ENVIRONMENT {
	OriginalApcEnvironment,
	AttachedApcEnvironment,
	CurrentApcEnvironment,
	InsertApcEnvironment
} KAPC_ENVIRONMENT;

typedef enum _THREAD_STATE
{
	StateInitialized,
	StateReady,
	StateRunning,
	StateStandby,
	StateTerminated,
	StateWaiting,
	StateTransition,
	StateUnknown

}THREAD_STATE;

typedef struct _THREAD_INFO{
	ULONG ulHideType;           //1 找不到线程所在模块，既认为隐藏线程，显示粉红色，0 正常
	ULONG ThreadID;
	ULONG KernelThread;
	ULONG TEB;
	ULONG ThreadStart;
	ULONG HideDebug;            //如果是0，则显示褐色
	CHAR lpszThreadModule[256];
	UCHAR ulStatus;
	UCHAR ulPriority;    //优先级
	ULONG ulContextSwitches;

}THREAD_INFO,*PTHREAD_INFO;

typedef struct _SYSTEM_THREAD_INFO {          //InlineHook
	ULONG ulCount;
	THREAD_INFO SystemThreadInfo[1];
} SYSTEM_THREAD_INFO, *PSYSTEM_THREAD_INFO;

PSYSTEM_THREAD_INFO SystemThread;

// ULONG ulThreadProc;
// ULONG ulThreadListHead;
ULONG ulCrossThreadFlagsOffset;
ULONG ulHideDebug;
ULONG ulThreadID;
ULONG ulTEB;
ULONG ulThreadStart;
ULONG ulWin32ThreadStart;
ULONG ulStatus;
ULONG ulPriority;
ULONG ulContextSwitches;   //切换次数

extern PEPROCESS SystemEProcess;

typedef NTSTATUS (__stdcall *NTRESUMETHREAD)(
	IN HANDLE ThreadHandle,
	OUT PULONG PreviousSuspendCount OPTIONAL
	);

typedef NTSTATUS (__stdcall *NTSUSPENDTHREAD)(
	IN HANDLE               ThreadHandle,
	OUT PULONG              PreviousSuspendCount OPTIONAL
	);

PETHREAD GetNextProcessThread(IN PEPROCESS Process,
	IN PETHREAD Thread OPTIONAL);

BOOL IsAddressInSystem(
	ULONG ulDriverBase,
	ULONG *ulSysModuleBase,
	ULONG *ulSize,
	char *lpszSysModuleImage
	);

BOOL IniKillObject();

BOOL GetFunctionIndexByName(CHAR *lpszFunctionName,int *Index);

#endif