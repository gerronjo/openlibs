#ifndef _PROCESS_FULL_PATH_H
#define _PROCESS_FULL_PATH_H

#include "ntifs.h"
#include "InitWindowsVersion.h"
//#include "KernelReload.h"
#include "ntos.h"

#define SEC_IMAGE         0x1000000
#define MEM_IMAGE SEC_IMAGE 

extern BOOL DebugOn;

typedef enum _MEMORY_INFORMATION_CLASS
{
	MemoryBasicInformation,
	MemoryWorkingSetList,
	MemorySectionName
}MEMORY_INFORMATION_CLASS;

// typedef struct _MEMORY_SECTION_NAME{
// 	UNICODE_STRING SectionFileName;
// 	WCHAR       NameBuffer[ANYSIZE_ARRAY];
// } MEMORY_SECTION_NAME,*PMEMORY_SECTION_NAME;

typedef struct _MEMORY_BASIC_INFORMATION { 
	PVOID BaseAddress; 
	PVOID AllocationBase; 
	DWORD AllocationProtect; 
	DWORD RegionSize; 
	DWORD State; 
	DWORD Protect; 
	DWORD Type; 
} MEMORY_BASIC_INFORMATION,*PMEMORY_BASIC_INFORMATION; 

typedef struct _PEB_LDR_DATA
{
	ULONG Length;
	BOOLEAN Initialized;
	PVOID SsHandle;
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList;
	LIST_ENTRY InInitializationOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;
// 
// typedef struct _EX_FAST_REF {
// 	union {
// 		PVOID Object;
// 		ULONG RefCnt : 3;
// 		ULONG Value;
// 	};
// } EX_FAST_REF, *PEX_FAST_REF;


typedef struct _CONTROL_AREA {
	//CONTROL_AREA Strutct for winxp
	PVOID Segment; //PSEGMENT
	LIST_ENTRY DereferenceList;
	ULONG NumberOfSectionReferences;    // All section refs & image flushes
	ULONG NumberOfPfnReferences;        // valid + transition prototype PTEs
	ULONG NumberOfMappedViews;          // total # mapped views, including
	// system cache & system space views
	USHORT NumberOfSubsections;     // system cache views only
	USHORT FlushInProgressCount;
	ULONG NumberOfUserReferences;       // user section & view references
	ULONG LongFlags;
	PFILE_OBJECT FilePointer;
	PVOID WaitingForDeletion; //PEVENT_COUNTER
	USHORT ModifiedWriteCount;
	USHORT NumberOfSystemCacheViews;
} CONTROL_AREA, *PCONTROL_AREA;


typedef struct _MMVAD {
	//MMVAD Struct for winxp
	ULONG_PTR StartingVpn;
	ULONG_PTR EndingVpn;
	struct _MMVAD *Parent;
	struct _MMVAD *LeftChild;
	struct _MMVAD *RightChild;
	ULONG_PTR LongFlags;
	PCONTROL_AREA ControlArea;
	PVOID FirstPrototypePte; //PMMPTE
	PVOID LastContiguousPte;//PMMPTE
	ULONG LongFlags2;
} MMVAD, *PMMVAD;

//---------------------------------------------------------------------------------------
//dll
//---------------------------------------------------------------------------------------
typedef struct _DLL_INFORMATION {          //DLL_INFORMATION
	ULONG ulBase;
	WCHAR lpwzDllModule[256];                    //
} DLL_INFORMATION, *PDLL_INFORMATION;

typedef struct _DLLINFO {          //DLL
	ULONG ulCount;
	DLL_INFORMATION DllInfo[1];
} DLLINFO, *PDLLINFO;

//---------------------------------------------------------------------------------------
PDLLINFO PDll;
int ModuleCount;

VOID EunmProcessModule(
	ULONG Eprocess,
	PDLLINFO PDll
	);
NTSTATUS SafeCopyMemory(
	PVOID SrcAddr, 
	PVOID DstAddr, 
	ULONG Size
	);
BOOLEAN ValidateUnicodeString(
	PUNICODE_STRING usStr
	);
NTSTATUS LookupProcessByName(
	IN PCHAR pcProcessName,
	OUT PEPROCESS *pEprocess
	);

NTSTATUS ZwQueryVirtualMemory(
	IN HANDLE               ProcessHandle,
	IN PVOID                BaseAddress,
	IN MEMORY_INFORMATION_CLASS MemoryInformationClass,
	OUT PVOID               Buffer,
	IN ULONG                Length,
	OUT PULONG              ResultLength OPTIONAL
	);
NTSTATUS ZwOpenProcess(
	__out     PHANDLE ProcessHandle,
	__in      ACCESS_MASK DesiredAccess,
	__in      POBJECT_ATTRIBUTES ObjectAttributes,
	__in_opt  PCLIENT_ID ClientId
	);
DWORD CsGetFileSize(HANDLE FileHandle,PDWORD HightLength);

#endif