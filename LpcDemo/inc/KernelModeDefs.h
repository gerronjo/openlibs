#pragma once

/*
this header file contains the context that cannot find at WDK
but we wanna use in ring0
*/

#include <ntddk.h>
#include <WinDef.h>

#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)




#define ACTIVE_PROCESS_LINK 0x88
#define PROCESS_NAME	    0x174
#define PROCESS_ID	        0x84

#define POINT_TO_PROCESS(lst) (PEPROCESS)((ULONG)lst - ACTIVE_PROCESS_LINK)
#define POINT_TO_PROCNAME(proc) (char*)((ULONG)proc + PROCESS_NAME)
#define POINT_TO_PROCID(proc) (ULONG*)((ULONG)proc + PROCESS_ID)

typedef struct _SYSTEM_SERVICE_TABLE 
{ 
	PVOID    ServiceTable;  // array of entry points 
	PULONG     CounterTable;  // array of usage counters 
	ULONG      ServiceLimit;   // number of table entries 
	PBYTE     ArgumentTable;   // array of byte counts 
} 
SYSTEM_SERVICE_TABLE, 
*PSYSTEM_SERVICE_TABLE, 
**PPSYSTEM_SERVICE_TABLE; 

//----------------------------------------------------------------------------------------------------------- 

typedef struct _SERVICE_DESCRIPTOR_TABLE 
{ 
	SYSTEM_SERVICE_TABLE ntoskrnl;      // ntoskrnl.exe ( native api ) 
	SYSTEM_SERVICE_TABLE win32k;          // win32k.sys (gdi/user support) 
	SYSTEM_SERVICE_TABLE Table3;         // not used 
	SYSTEM_SERVICE_TABLE Table4;         // not used 
} 
SYSTEM_DESCRIPTOR_TABLE, 
*PSYSTEM_DESCRIPTOR_TABLE, 
**PPSYSTEM_DESCRIPTOR_TABLE; 

typedef enum _SYSTEM_INFORMATION_CLASS { 
	SystemBasicInformation,                // 0        Y        N 
	SystemProcessorInformation,            // 1        Y        N 
	SystemPerformanceInformation,          // 2        Y        N 
	SystemTimeOfDayInformation,            // 3        Y        N 
	SystemNotImplemented1,                 // 4        Y        N 
	SystemProcessesAndThreadsInformation,  // 5        Y        N 
	SystemCallCounts,                      // 6        Y        N 
	SystemConfigurationInformation,        // 7        Y        N 
	SystemProcessorTimes,                  // 8        Y        N 
	SystemGlobalFlag,                      // 9        Y        Y 
	SystemNotImplemented2,                 // 10       Y        N 
	SystemModuleInformation,               // 11       Y        N 
	SystemLockInformation,               // 12       Y        N 
	SystemNotImplemented3,               // 13       Y        N 
	SystemNotImplemented4,               // 14       Y        N 
	SystemNotImplemented5,               // 15       Y        N 
	SystemHandleInformation,             // 16       Y        N 
	SystemObjectInformation,             // 17       Y        N 
	SystemPagefileInformation,           // 18       Y        N 
	SystemInstructionEmulationCounts,    // 19       Y        N 
	SystemInvalidInfoClass1,             // 20 
	SystemCacheInformation,              // 21       Y        Y 
	SystemPoolTagInformation,            // 22       Y        N 
	SystemProcessorStatistics,           // 23       Y        N 
	SystemDpcInformation,                // 24       Y        Y 
	SystemNotImplemented6,               // 25       Y        N 
	SystemLoadImage,                     // 26       N        Y 
	SystemUnloadImage,                   // 27       N        Y 
	SystemTimeAdjustment,                // 28       Y        Y 
	SystemNotImplemented7,               // 29       Y        N 
	SystemNotImplemented8,               // 30       Y        N 
	SystemNotImplemented9,               // 31       Y        N 
	SystemCrashDumpInformation,          // 32       Y        N 
	SystemExceptionInformation,          // 33       Y        N 
	SystemCrashDumpStateInformation,     // 34       Y        Y/N 
	SystemKernelDebuggerInformation,     // 35       Y        N 
	SystemContextSwitchInformation,      // 36       Y        N 
	SystemRegistryQuotaInformation,      // 37       Y        Y 
	SystemLoadAndCallImage,              // 38       N        Y 
	SystemPrioritySeparation,            // 39       N        Y 
	SystemNotImplemented10,              // 40       Y        N 
	SystemNotImplemented11,              // 41       Y        N 
	SystemInvalidInfoClass2,             // 42 
	SystemInvalidInfoClass3,             // 43 
	SystemTimeZoneInformation,           // 44       Y        N 
	SystemLookasideInformation,          // 45       Y        N 
	SystemSetTimeSlipEvent,              // 46       N        Y 
	SystemCreateSession,                 // 47       N        Y 
	SystemDeleteSession,                 // 48       N        Y 
	SystemInvalidInfoClass4,             // 49 
	SystemRangeStartInformation,         // 50       Y        N 
	SystemVerifierInformation,           // 51       Y        Y 
	SystemAddVerifier,                   // 52       N        Y 
	SystemSessionProcessesInformation    // 53       Y        N 
} SYSTEM_INFORMATION_CLASS; 

