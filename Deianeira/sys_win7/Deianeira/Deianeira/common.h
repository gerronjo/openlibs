
//防止重定义
#ifndef _COMMON_
#define _COMMON_

#include <ntifs.h>
#include "..\..\..\common\ioctlcode.h"
#include <WinDef.h>
#include "LDasm.h"
#include <ntstrsafe.h>

#define MUTEX_TYPE         KMUTEX
#define MUTEX_INIT(v)      KeInitializeMutex(&v,0)
#define MUTEX_P(v)         KeWaitForMutexObject(&v,Executive,KernelMode,FALSE,NULL)
#define MUTEX_V(v)         KeReleaseMutex(&v,FALSE)


//获得SSDT函数地址
#define GetSSDTFuncAddress(_func) KeServiceDescriptorTable->ServiceTableBase[ *(PULONG)((PUCHAR)_func+1)]

//全局的
PDRIVER_OBJECT global_pDriverObj;


//全局变量
//应用程序进程ID
ULONG ulProcessID;

//全局变量
//系统版本号
ULONG ulOSVersion;

//全局变量
//ring3应用程序的EPROCESS
PEPROCESS pEProcess;


//禁止创建进程
BOOL bGlobal_disableCreateProcess;

BOOL b_disableCreateProcess;

//禁止创建线程
BOOL bGlobal_disableCreateThread;

//禁止结束进程
BOOL bGlobal_disableTerminateProcess;

BOOL b_disableTerminateProcess;

//禁止结束线程
BOOL bGlobal_disableTerminateThread;


//
typedef struct _DisableCreateProcess{
	//进程ID
	ULONG ulPID;
	//下个结构体指针
	struct DISABLE_CREATE_PROCESS *next;
}DISABLE_CREATE_PROCESS,*PDISABLE_CREATE_PROCESS;

//
typedef struct _DisableTerminateProcess{
	//进程ID
	ULONG ulPID;
	//下个结构体指针
	struct DISABLE_TERMINATE_PROCESS *next;
}DISABLE_TERMINATE_PROCESS,*PDISABLE_TERMINATE_PROCESS;


PDISABLE_TERMINATE_PROCESS pDisableTerminateProcessInfo,pDisableTerminateProcessHead,pDisableTerminateProcessTmp;

PDISABLE_CREATE_PROCESS pDisableCreateProcessInfo,pDisableCreateProcessHead,pDisableCreateProcessTmp;


//禁止创建文件
BOOL bGlobal_disableCreateFile;

BOOL b_disableCreateFile;

//禁止文件删除
BOOL bGlobal_disableDeleteFile ;

BOOL b_disableDeleteFile ;

//禁止文件打开
BOOL bGlobal_disableOpenFile ;

BOOL b_disableOpenFile ;


//
typedef struct _DisableCreateFile{
	//进程ID
	ULONG ulPID;
	//下个结构体指针
	struct DISABLE_CREATE_FILE *next;
}DISABLE_CREATE_FILE,*PDISABLE_CREATE_FILE;

//
typedef struct _DisableDeleteFile{
	//进程ID
	ULONG ulPID;
	//下个结构体指针
	struct DISABLE_DELETE_FILE *next;
}DISABLE_DELETE_FILE,*PDISABLE_DELETE_FILE;

//
typedef struct _DisableOpenFile{
	//进程ID
	ULONG ulPID;
	//下个结构体指针
	struct DISABLE_OPEN_FILE *next;
}DISABLE_OPEN_FILE,*PDISABLE_OPEN_FILE;


PDISABLE_CREATE_FILE pDisableCreateFileInfo,pDisableCreateFileHead,pDisableCreateFileTmp;

PDISABLE_DELETE_FILE pDisableDeleteFileInfo,pDisableDeleteFileHead,pDisableDeleteFileTmp;

PDISABLE_OPEN_FILE pDisableOpenFileInfo,pDisableOpenFileHead,pDisableOpenFileTmp;


BOOL bGlobal_disableCreateReg ;

BOOL b_disableCreateReg ;

BOOL bGlobal_disableDeleteReg ;

BOOL b_disableDeleteReg ;

BOOL bGlobal_disableOpenReg ;

BOOL b_disableOpenReg ;

// 
typedef struct _DisableCreateReg{
	//进程ID
	ULONG ulPID;
	//下个结构体指针
	struct DISABLE_CREATE_REG *next;
}DISABLE_CREATE_REG,*PDISABLE_CREATE_REG;

//
typedef struct _DisableDeleteReg{
	//进程ID
	ULONG ulPID;
	//下个结构体指针
	struct DISABLE_DELETE_REG *next;
}DISABLE_DELETE_REG,*PDISABLE_DELETE_REG;

//
typedef struct _DisableOpenReg{
	//进程ID
	ULONG ulPID;
	//下个结构体指针
	struct DISABLE_OPEN_REG *next;
}DISABLE_OPEN_REG,*PDISABLE_OPEN_REG;


PDISABLE_CREATE_REG pDisableCreateRegInfo,pDisableCreateRegHead,pDisableCreateRegTmp;

PDISABLE_DELETE_REG pDisableDeleteRegInfo,pDisableDeleteRegHead,pDisableDeleteRegTmp;

PDISABLE_OPEN_REG pDisableOpenRegInfo,pDisableOpenRegHead,pDisableOpenRegTmp;




//开启ObReferenceObjectByHandle自我保护
BOOL b_startSelfProtectionByObReferenceObjectByHandle;


//SDT结构体
typedef struct ServiceDescriptorTable {
	unsigned int *ServiceTableBase;
	unsigned int *ServiceCounterTable;
	unsigned int NumberOfServices;
	unsigned int *ParamTableBase;
}ServiceDescriptorTable,*PServiceDescriptorTable;


typedef struct _PEB_LDR_DATA {
	ULONG                   Length;
	BOOLEAN                 Initialized;
	PVOID                   SsHandle;
	LIST_ENTRY              InLoadOrderModuleList;
	LIST_ENTRY              InMemoryOrderModuleList;
	LIST_ENTRY              InInitializationOrderModuleList;
	PVOID                   EntryInProgress;
} PEB_LDR_DATA, *PPEB_LDR_DATA;


typedef struct _LDR_DATA_TABLE_ENTRY {
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
	PVOID DllBase;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	ULONG Flags;
	USHORT LoadCount;
	USHORT TlsIndex;
	union {
		LIST_ENTRY HashLinks;
		struct {
			PVOID SectionPointer;
			ULONG CheckSum;
		};
	};
	union {
		struct {
			ULONG TimeDateStamp;
		};
		struct {
			PVOID LoadedImports;
		};
	};
	PVOID EntryPointActivationContext;
	PVOID PatchInformation;
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;


typedef enum _SYSTEM_INFORMATION_CLASS {
	SystemBasicInformation,                // 0 Y N
	SystemProcessorInformation,            // 1 Y N
	SystemPerformanceInformation,        // 2 Y N
	SystemTimeOfDayInformation,            // 3 Y N
	SystemNotImplemented1,                // 4 Y N
	SystemProcessesAndThreadsInformation, // 5 Y N
	SystemCallCounts,                    // 6 Y N
	SystemConfigurationInformation,        // 7 Y N
	SystemProcessorTimes,                // 8 Y N
	SystemGlobalFlag,                    // 9 Y Y
	SystemNotImplemented2,                // 10 Y N
	SystemModuleInformation,            // 11 Y N
	SystemLockInformation,                // 12 Y N
	SystemNotImplemented3,                // 13 Y N
	SystemNotImplemented4,                // 14 Y N
	SystemNotImplemented5,                // 15 Y N
	SystemHandleInformation,            // 16 Y N
	SystemObjectInformation,            // 17 Y N
	SystemPagefileInformation,            // 18 Y N
	SystemInstructionEmulationCounts,    // 19 Y N
	SystemInvalidInfoClass1,            // 20
	SystemCacheInformation,                // 21 Y Y
	SystemPoolTagInformation,            // 22 Y N
	SystemProcessorStatistics,            // 23 Y N
	SystemDpcInformation,                // 24 Y Y
	SystemNotImplemented6,                // 25 Y N
	SystemLoadImage,                    // 26 N Y
	SystemUnloadImage,                    // 27 N Y
	SystemTimeAdjustment,                // 28 Y Y
	SystemNotImplemented7,                // 29 Y N
	SystemNotImplemented8,                // 30 Y N
	SystemNotImplemented9,                // 31 Y N
	SystemCrashDumpInformation,            // 32 Y N
	SystemExceptionInformation,            // 33 Y N
	SystemCrashDumpStateInformation,    // 34 Y Y/N
	SystemKernelDebuggerInformation,    // 35 Y N
	SystemContextSwitchInformation,        // 36 Y N
	SystemRegistryQuotaInformation,        // 37 Y Y
	SystemLoadAndCallImage,                // 38 N Y
	SystemPrioritySeparation,            // 39 N Y
	SystemNotImplemented10,                // 40 Y N
	SystemNotImplemented11,                // 41 Y N
	SystemInvalidInfoClass2,            // 42
	SystemInvalidInfoClass3,            // 43
	SystemTimeZoneInformation,            // 44 Y N
	SystemLookasideInformation,            // 45 Y N
	SystemSetTimeSlipEvent,                // 46 N Y
	SystemCreateSession,                // 47 N Y
	SystemDeleteSession,                // 48 N Y
	SystemInvalidInfoClass4,            // 49
	SystemRangeStartInformation,        // 50 Y N
	SystemVerifierInformation,            // 51 Y Y
	SystemAddVerifier,                    // 52 N Y
	SystemSessionProcessesInformation    // 53 Y N
} SYSTEM_INFORMATION_CLASS;

//// Information Class 11
typedef struct _SYSTEM_MODULE_INFORMATION { 
	ULONG Reserved[2];
	PVOID Base;
	ULONG Size;
	ULONG Flags;
	USHORT Index;
	USHORT Unknown;
	USHORT LoadCount;
	USHORT ModuleNameOffset;
	CHAR ImageName[256];
} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;

//系统模块列表 
typedef struct _SYSTEM_MODULE_LIST{ 
	ULONG ulCount; 
	SYSTEM_MODULE_INFORMATION smi[1]; 
} SYSTEM_MODULE_LIST, *PSYSTEM_MODULE_LIST; 

typedef enum _SYSTEM_HANDLE_TYPE
{
	OB_TYPE_UNKNOWN=0,   //0 "Unknown"
	OB_TYPE_TYPE,    // 1,fixed "Type"
	OB_TYPE_DIRECTORY,   // 2,fixed "Directory"
	OB_TYPE_SYMBOLIC_LINK, // 3,fixed "SymbolicLink"
	OB_TYPE_TOKEN,    // 4,fixed "Token"
	OB_TYPE_PROCESS,   // 5,fixed "Process"
	OB_TYPE_THREAD,    // 6,fixed "Thread"
	OB_TYPE_JOB,    // 7,fixed "Job" "Job"
	OB_TYPE_DEBUG_OBJECT, // 8,fixed "Debug"
	OB_TYPE_EVENT,    // 9,fixed "Event"
	OB_TYPE_EVENT_PAIR,   //10,fixed "EventPair"
	OB_TYPE_MUTANT,    //11,fixed "Mutant"
	OB_TYPE_CALLBACK,   //12,fixed "Callback"
	OB_TYPE_SEMAPHORE,   //13,fixed "Semaphore"
	OB_TYPE_TIMER,    //14,fixed "Timer"
	OB_TYPE_PROFILE,   //15,fixed "Profile"
	OB_TYPE_KEYED_EVENT, //16,fixed "Keyed"
	OB_TYPE_WINDOWS_STATION,//17,fixed "WindowsStation"
	OB_TYPE_DESKTOP,   //18,fixed "Desktop"
	OB_TYPE_SECTION,   //19,fixed "Section"
	OB_TYPE_KEY,    //20,fixed "Key"
	OB_TYPE_PORT,    //21,fixed  "Port"
	OB_TYPE_WAITABLE_PORT, //22,fixed "WaitablePort"
	OB_TYPE_ADAPTER,   //23,fixed "Adapter"
	OB_TYPE_CONTROLLER,   //24,fixed "Controller"
	OB_TYPE_DEVICE,    //25,fixed "Device"
	OB_TYPE_DRIVER,    //26,fixed "Driver"
	OB_TYPE_IOCOMPLETION, //27,fixed "IoCompletion"
	OB_TYPE_FILE,    //28,fixed "File"
	OB_TYPE_WMIGUID    //29,fixed "WmiGuid"
}SYSTEM_HANDLE_TYPE;

//
typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO {
	USHORT UniqueProcessId;
	USHORT CreatorBackTraceIndex;
	UCHAR ObjectTypeIndex;
	UCHAR HandleAttributes;
	USHORT HandleValue;
	PVOID Object;
	ULONG GrantedAccess;
} SYSTEM_HANDLE_TABLE_ENTRY_INFO, *PSYSTEM_HANDLE_TABLE_ENTRY_INFO;

//
typedef struct _SYSTEM_HANDLE_INFORMATION {
	ULONG NumberOfHandles;
	SYSTEM_HANDLE_TABLE_ENTRY_INFO Information[ 1 ];
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;

////
//typedef enum _OBJECT_INFORMATION_CLASS { 
//	ObjectBasicInformation,
//	ObjectNameInformation,
//	ObjectTypeInformation,
//	ObjectAllInformation,
//	ObjectDataInformation
//} OBJECT_INFORMATION_CLASS, *POBJECT_INFORMATION_CLASS;

//
typedef struct _OBJECT_BASIC_INFORMATION {
	ULONG                   Attributes;
	ACCESS_MASK             DesiredAccess;
	ULONG                   HandleCount;
	ULONG                   ReferenceCount;
	ULONG                   PagedPoolUsage;
	ULONG                   NonPagedPoolUsage;
	ULONG                   Reserved[3];
	ULONG                   NameInformationLength;
	ULONG                   TypeInformationLength;
	ULONG                   SecurityDescriptorLength;
	LARGE_INTEGER           CreationTime;
} OBJECT_BASIC_INFORMATION, *POBJECT_BASIC_INFORMATION;

//
typedef struct _KOBJECT_NAME_INFORMATION { 
	UNICODE_STRING          Name;
	WCHAR                   NameBuffer[];
} KOBJECT_NAME_INFORMATION, *PKOBJECT_NAME_INFORMATION;

//
typedef struct _OBJECT_TYPE_INFORMATION { 
	UNICODE_STRING          TypeName;
	ULONG                   TotalNumberOfHandles;
	ULONG                   TotalNumberOfObjects;
	WCHAR                   Unused1[8];
	ULONG                   HighWaterNumberOfHandles;
	ULONG                   HighWaterNumberOfObjects;
	WCHAR                   Unused2[8];
	ACCESS_MASK             InvalidAttributes;
	GENERIC_MAPPING         GenericMapping;
	ACCESS_MASK             ValidAttributes;
	BOOLEAN                 SecurityRequired;
	BOOLEAN                 MaintainHandleCount;
	USHORT                  MaintainTypeList;
	POOL_TYPE               PoolType;
	ULONG                   DefaultPagedPoolCharge;
	ULONG                   DefaultNonPagedPoolCharge;
} OBJECT_TYPE_INFORMATION, *POBJECT_TYPE_INFORMATION;


NTSYSAPI
BOOLEAN
NTAPI
KeAddSystemServiceTable (
						 IN PULONG_PTR Base,
						 IN PULONG Count OPTIONAL,
						 IN ULONG Limit,
						 IN PUCHAR Number,
						 IN ULONG Index
						 );

NTSYSAPI
NTSTATUS
NTAPI 
PsLookupProcessByProcessId(
						   __in   HANDLE ProcessId,
						   __out  PEPROCESS *pEProcess
									);

NTSYSAPI
NTSTATUS
NTAPI 
PsLookupThreadByThreadId(
						 __in   HANDLE ThreadId,
						 __out  PETHREAD *pEThread
						 );

NTSYSAPI
NTSTATUS
NTAPI
ZwQuerySystemInformation(
	IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
	IN OUT PVOID SystemInformation,
	IN ULONG SystemInformationLength,
	OUT PULONG ReturnLength OPTIONAL
	);

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryInformationProcess(
						  IN HANDLE ProcessHandle,
						  IN PROCESSINFOCLASS ProcessInformationClass,
						  OUT PVOID ProcessInformation,
						  IN ULONG ProcessInformationLength,
						  OUT PULONG ReturnLength OPTIONAL
						  );

NTKERNELAPI
NTSTATUS
ObReferenceObjectByName(
						IN PUNICODE_STRING ObjectName,
						IN ULONG Attributes,
						IN PACCESS_STATE PassedAccessState OPTIONAL,
						IN ACCESS_MASK DesiredAccess OPTIONAL,
						IN POBJECT_TYPE ObjectType,
						IN KPROCESSOR_MODE AccessMode,
						IN OUT PVOID ParseContext OPTIONAL,
						OUT PVOID *Object
						);

NTKERNELAPI
PDEVICE_OBJECT
NTAPI
IoGetBaseFileSystemDeviceObject (
								 IN PFILE_OBJECT FileObject
								 );

//从nt中导出，所以只要声明
NTSYSAPI
NTSTATUS
NTAPI 
ObReferenceObjectByHandle(
						  IN HANDLE  Handle,
						  IN ACCESS_MASK  DesiredAccess,
						  IN POBJECT_TYPE  ObjectType  OPTIONAL,
						  IN KPROCESSOR_MODE  AccessMode,
						  OUT PVOID  *Object,
						  OUT POBJECT_HANDLE_INFORMATION  HandleInformation  OPTIONAL
						  );
 
NTSYSAPI 
NTSTATUS 
NTAPI 
ZwOpenThread(              
			 OUT PHANDLE ThreadHandle,              
			 IN ACCESS_MASK DesiredAccess,              
			 IN POBJECT_ATTRIBUTES ObjectAttributes,              
			 IN PCLIENT_ID ClientId              
			 );   

NTSYSAPI 
NTSTATUS 
NTAPI 
ZwQueryInformationThread(                          
						 IN HANDLE ThreadHandle,                          
						 IN THREADINFOCLASS ThreadInformationClass,                         
						 OUT PVOID ThreadInformation,                          
						 IN ULONG ThreadInformationLength,                          
						 OUT PULONG ReturnLength OPTIONAL                         
						 ); 

NTSYSAPI
NTSTATUS
NTAPI
ZwOpenSymbolicLinkObject(
						 OUT PHANDLE LinkHandle,
						 IN ACCESS_MASK DesiredAccess,
						 IN POBJECT_ATTRIBUTES ObjectAttributes
						 );

NTSYSAPI
NTSTATUS
NTAPI
ZwQuerySymbolicLinkObject(
						  IN HANDLE LinkHandle,
						  IN OUT PUNICODE_STRING LinkTarget,
						  OUT PULONG ReturnedLength OPTIONAL
						  );

NTSYSAPI
NTSTATUS
NTAPI
ZwDuplicateObject(
				  IN HANDLE SourceProcessHandle,
				  IN HANDLE SourceHandle,
				  IN HANDLE TargetProcessHandle OPTIONAL,
				  OUT PHANDLE TargetHandle OPTIONAL,
				  IN ACCESS_MASK DesiredAccess,
				  IN ULONG HandleAttributes,
				  IN ULONG Options
				  );

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryObject(
			  IN HANDLE Handle,
			  IN OBJECT_INFORMATION_CLASS ObjectInformationClass,
			  OUT PVOID ObjectInformation,
			  IN ULONG ObjectInformationLength,
			  OUT PULONG ReturnLength OPTIONAL
			  );

PUCHAR PsGetProcessImageFileName( 
								 __in PEPROCESS Process 
								 );


extern POBJECT_TYPE IoDeviceObjectType;
extern POBJECT_TYPE IoDriverObjectType;
extern POBJECT_TYPE *PsProcessType;
extern POBJECT_TYPE *PsThreadType;
extern POBJECT_TYPE *IoFileObjectType;
extern POBJECT_TYPE CmpKeyObjectType;

//获得SSDT在内核中的地址
extern PServiceDescriptorTable KeServiceDescriptorTable;

//Shadow SSDT
PServiceDescriptorTable KeServiceDescriptorTableShadow;


//获得Shadow SSDT 地址
PULONG getAddressOfShadowTable();


//获得导出但是未文档化函数的地址
PVOID GetExportedFuncAddress(LPTSTR lpFuncName);


//获得进程镜像名称
NTSTATUS GetProcessImageNameByProcessHandle(HANDLE ProcessHandle,LPTSTR lpFilePath);

//通过进程句柄来获得进程ID
ULONG GetProcessIDByProcessHandle(HANDLE ProcessHandle);

//获得父进程ID
LONG GetParentProcessIDByProcessHandle(HANDLE ProcessHandle);

//从StartAddr地址  开始找OldAddr  替换为NewAddr地址   长度是 SIZE 
BOOL common_callAddrHook(PVOID StartAddr,PVOID OldAddr,PVOID NewAddr,ULONG Size);

//去除内存保护
VOID UnProtected();

//恢复内存保护
VOID Protected();

//根据类型索引获得类型名称
VOID common_getHandleTypeName(ULONG ulObjectTypeIndex,LPTSTR lpstr);

#endif //_COMMON_