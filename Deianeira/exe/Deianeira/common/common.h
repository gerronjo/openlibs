
//防止重定义
#ifndef _COMMON_
#define _COMMON_

#include <Winsock2.h>
#include <Windows.h>
#include <atlconv.h>
#include <stdio.h>
#include <tchar.h>
#include <IPHlpApi.h>
#include <Mscat.h>
#include <PSAPI.H>
#include <Reason.h>
#include <ShellAPI.h> 
#include <SoftPub.h>
#include <TlHelp32.h>
#include <wincrypt.h>
#include <winsvc.h>
#include <Ws2spi.h>
#include <WinInet.h>
#include <Shobjidl.h>
#include <shlobj.h>
#include <Imagehlp.h>
#include <map>
#include <string>
#include <iostream>
#include "..\Deianeira\ListViewItemComparer.h"
#include "..\\..\\..\\common\ioctlcode.h"
#include "..\\..\\..\\common\\lib\\sqlite3\\sqlite3.h"
#include "..\\..\\..\\common\\lib\\QQwry\\QQwry.h"

#pragma comment(lib,"AdvApi32.lib")
#pragma comment(lib,"crypt32.lib")
#pragma comment(lib,"Iphlpapi.lib")
#pragma comment(lib,"psapi.lib")
#pragma comment(lib,"shell32.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"Wininet.lib")
#pragma comment(lib, "wintrust.lib")
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"Ole32.lib")
#pragma comment(lib,"Imagehlp.lib")
#pragma comment(lib,"Gdi32.lib")
#pragma comment(lib,"..\\..\\..\\common\\lib\\sqlite3\\sqlite3.lib")

using namespace std;

typedef LONG NTSTATUS;
#define NT_SUCCESS(status)          ((NTSTATUS)(status)>=0)
#define STATUS_SUCCESS              ((NTSTATUS)0x00000000L)
#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004L)
#define STATUS_ACCESS_DENIED        ((NTSTATUS)0xC0000022L)

#define check_hdriver(v) if(!v) return;
#define init_hdriver(bRet,dwRet) bRet = FALSE;dwRet = 0;

typedef struct _UNICODE_STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

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

//
typedef enum _FILE_INFORMATION_CLASS
{
	FileDirectoryInformation = 1,
	FileFullDirectoryInformation,
	FileBothDirectoryInformation,
	FileBasicInformation,
	FileStandardInformation,
	FileInternalInformation,
	FileEaInformation,
	FileAccessInformation,
	FileNameInformation,
	FileRenameInformation,
	FileLinkInformation,
	FileNamesInformation,
	FileDispositionInformation,
	FilePositionInformation,
	FileFullEaInformation,
	FileModeInformation,
	FileAlignmentInformation,
	FileAllInformation,
	FileAllocationInformation,
	FileEndOfFileInformation,
	FileAlternateNameInformation,
	FileStreamInformation,
	FilePipeInformation,
	FilePipeLocalInformation,
	FilePipeRemoteInformation,
	FileMailslotQueryInformation,
	FileMailslotSetInformation,
	FileCompressionInformation,
	FileObjectIdInformation,
	FileCompletionInformation,
	FileMoveClusterInformation,
	FileQuotaInformation,
	FileReparsePointInformation,
	FileNetworkOpenInformation,
	FileAttributeTagInformation,
	FileTrackingInformation,
	FileIdBothDirectoryInformation,
	FileIdFullDirectoryInformation,
	FileValidDataLengthInformation,
	FileShortNameInformation,
	FileMaximumInformation
} FILE_INFORMATION_CLASS, *PFILE_INFORMATION_CLASS;

//
typedef enum _OBJECT_INFORMATION_CLASS
{
	ObjectBasicInformation,
	ObjectNameInformation,
	ObjectTypeInformation,
	ObjectTypesInformation,
	ObjectHandleFlagInformation,
	ObjectSessionInformation,
	MaxObjectInfoClass
} OBJECT_INFORMATION_CLASS;

//
typedef struct _FILE_NAME_INFORMATION
{
	ULONG FileNameLength;
	WCHAR FileName[1];
} FILE_NAME_INFORMATION, *PFILE_NAME_INFORMATION;

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

//
typedef struct _IO_STATUS_BLOCK {
	union {
		NTSTATUS Status;
		PVOID Pointer;
	} DUMMYUNIONNAME;

	ULONG_PTR Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

//
typedef struct _OBJECT_NAME_INFORMATION
{
	UNICODE_STRING Name;
} OBJECT_NAME_INFORMATION, *POBJECT_NAME_INFORMATION;

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

typedef enum _THREAD_STATE{
	StateInitialized,
	StateReady,
	StateRunning,
	StateStandby,
	StateTerminated,
	StateWait,
	StateTransition,
	StateUnknown
} THREAD_STATE;

typedef enum _KWAIT_REASON {
	Executive,
	FreePage,
	PageIn,
	PoolAllocation,
	DelayExecution,
	Suspended,
	UserRequest,
	WrExecutive,
	WrFreePage,
	WrPageIn,
	WrPoolAllocation,
	WrDelayExecution,
	WrSuspended,
	WrUserRequest,
	WrEventPair,
	WrQueue,
	WrLpcReceive,
	WrLpcReply,
	WrVirtualMemory,
	WrPageOut,
	WrRendezvous,
	Spare2,
	Spare3,
	Spare4,
	Spare5,
	Spare6,
	WrKernel
} KWAIT_REASON;

typedef struct _VM_COUNTERS {
	ULONG PeakVirtualSize;
	ULONG VirtualSize;
	ULONG PageFaultCount;
	ULONG PeakWorkingSetSize;
	ULONG WorkingSetSize;
	ULONG QuotaPeakPagedPoolUsage;
	ULONG QuotaPagedPoolUsage;
	ULONG QuotaPeakNonPagedPoolUsage;
	ULONG QuotaNonPagedPoolUsage;
	ULONG PagefileUsage;
	ULONG PeakPagefileUsage;
} VM_COUNTERS, *PVM_COUNTERS;

typedef struct _CLIENT_ID
{
	ULONG ulPID;
	ULONG ulTID;
}CLIENT_ID,*PCLIENT_ID;

typedef struct _SYSTEM_THREADS {
	LARGE_INTEGER KernelTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER CreateTime;
	ULONG WaitTime;
	PVOID StartAddress;
	CLIENT_ID ClientId;
	//KPRIORITY Priority;
	LONG Priority;
	//KPRIORITY BasePriority;
	LONG BasePriority;
	ULONG ContextSwitchCount;
	THREAD_STATE dwState;
	//DWORD dwState;
	KWAIT_REASON dwWaitReason;
	//DWORD dwWaitReason;
} SYSTEM_THREADS, *PSYSTEM_THREADS;

typedef struct _SYSTEM_PROCESSES { // Information Class 5
	ULONG NextEntryDelta;
	ULONG ThreadCount;
	ULONG Reserved1[6];
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER KernelTime;
	UNICODE_STRING ProcessName;
	//KPRIORITY BasePriority;
	LONG BasePriority;
	ULONG ProcessId;
	ULONG InheritedFromProcessId;
	ULONG HandleCount;
	ULONG Reserved2[2];
	VM_COUNTERS VmCounters;
	IO_COUNTERS IoCounters; // Windows 2000 only
	SYSTEM_THREADS Threads[1];
} SYSTEM_PROCESSES, *PSYSTEM_PROCESSES;

typedef enum _PROCESSINFOCLASS {
	ProcessBasicInformation, // 0 Y N
	ProcessQuotaLimits, // 1 Y Y
	ProcessIoCounters, // 2 Y N
	ProcessVmCounters, // 3 Y N
	ProcessTimes, // 4 Y N
	ProcessBasePriority, // 5 N Y
	ProcessRaisePriority, // 6 N Y
	ProcessDebugPort, // 7 Y Y
	ProcessExceptionPort, // 8 N Y
	ProcessAccessToken, // 9 N Y
	ProcessLdtInformation, // 10 Y Y
	ProcessLdtSize, // 11 N Y
	ProcessDefaultHardErrorMode, // 12 Y Y
	ProcessIoPortHandlers, // 13 N Y
	ProcessPooledUsageAndLimits, // 14 Y N
	ProcessWorkingSetWatch, // 15 Y Y
	ProcessUserModeIOPL, // 16 N Y
	ProcessEnableAlignmentFaultFixup, // 17 N Y
	ProcessPriorityClass, // 18 N Y
	ProcessWx86Information, // 19 Y N
	ProcessHandleCount, // 20 Y N
	ProcessAffinityMask, // 21 N Y
	ProcessPriorityBoost, // 22 Y Y
	ProcessDeviceMap, // 23 Y Y
	ProcessSessionInformation, // 24 Y Y
	ProcessForegroundInformation, // 25 N Y
	ProcessWow64Information // 26 Y N
} PROCESSINFOCLASS;

typedef struct _PROCESS_BASIC_INFORMATION { // Information Class 0
	NTSTATUS ExitStatus;
	//PPEB PebBaseAddress;
	DWORD PebBaseAddress;
	//KAFFINITY AffinityMask;
	DWORD AffinityMask;
	//KPRIORITY BasePriority;
	DWORD BasePriority;
	ULONG UniqueProcessId;
	ULONG InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION, *PPROCESS_BASIC_INFORMATION;

//可能的TCP状态
static TCHAR TcpState[][12] = {
	TEXT("CLOSED"),
	TEXT("LISTENING"),
	TEXT("SYN_SENT"),
	TEXT("SYN_RCVD"),
	TEXT("ESTABLISHED"),
	TEXT("FIN_WAIT1"),
	TEXT("FIN_WAIT2"),
	TEXT("CLOSE_WAIT"),
	TEXT("CLOSING"),
	TEXT("LAST_ACK"),
	TEXT("TIME_WAIT"),
	TEXT("DELETE_TCB")
};

typedef enum _SHUTDOWN_ACTION
{
	ShutdownNoReboot,
	ShutdownReboot,
	ShutdownPowerOff
} SHUTDOWN_ACTION;

//typedef enum _POWER_ACTION {
//	PowerActionNone			= 0,
//	PowerActionReserved		= 1, 
//	PowerActionSleep		= 2, 
//	PowerActionHibernate	= 3, 
//	PowerActionShutdown		= 4, 
//	PowerActionShutdownReset= 5, 
//	PowerActionShutdownOff	= 6, 
//	PowerActionWarmEject	= 7  	
//} POWER_ACTION, *PPOWER_ACTION;
//
//typedef enum _SYSTEM_POWER_STATE {
//	PowerSystemUnspecified = 0, 
//	PowerSystemWorking     = 1,	
//	PowerSystemSleeping1   = 2,	
//	PowerSystemSleeping2   = 3,	
//	PowerSystemSleeping3   = 4,	
//	PowerSystemHibernate   = 5,	
//	PowerSystemShutdown    = 6,	
//	PowerSystemMaximum     = 7	
//} SYSTEM_POWER_STATE, *PSYSTEM_POWER_STATE;

//定义结构体，存储进程模块信息
typedef struct  _ProcessModuleInfo{
	WCHAR FileName[256];
	WCHAR FilePath[256];
	ULONG ulBaseAddress;
	ULONG ulSize;
	_ProcessModuleInfo *next;
}PROCESS_MODULE_INFO,*PPROCESS_MODULE_INFO;

//进程句柄信息
typedef struct _ProcessHandleInfo
{   
	//句柄类型
	WCHAR wstrHandleTypeName[25];
	//句柄名称
	WCHAR wstrHandleName[256];
	//句柄
	ULONG ulHandle;
	//句柄对象
	ULONG ulObject;
	//句柄类型索引
	ULONG ulTypeIndex;
	//引用计数
	ULONG ulReferenceCount;

	//单向列表指针
	_ProcessHandleInfo* next;
}PROCESS_HANDLE_INFO, *PPROCESS_HANDLE_INFO; 

//线程信息单向列表
typedef struct _FileHandleInfo
{   
	//
	ULONG ulPID;
	//
	ULONG ulHandle;
	//
	WCHAR wstrHandleName[256];
	//单向列表指针
	_FileHandleInfo* next;
}FILE_HANDLE_INFO, *PFILE_HANDLE_INFO; 

//定义NtQuerySystemInformation函数原型
typedef NTSTATUS (WINAPI *PNtQuerySystemInformation)(
	IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
	IN OUT PVOID SystemInformation,
	IN ULONG SystemInformationLength,
	OUT PULONG ReturnLength OPTIONAL
	);

//定义NtQueryInformationThread函数原型
typedef NTSTATUS (WINAPI *PNtQueryInformationThread)( 
	IN   HANDLE   ThreadHandle, 
	IN   ULONG   ThreadInformationClass, 
	OUT  PVOID   ThreadInformation, 
	IN   ULONG   ThreadInformationLength, 
	OUT  PULONG   ReturnLength   OPTIONAL 
	);

//
typedef NTSTATUS (WINAPI *PNtQueryInformationProcess)(
	IN   HANDLE  ProcessHandle, 
	IN   PROCESSINFOCLASS InformationClass, 
	OUT  PVOID   ProcessInformation, 
	IN   ULONG   ProcessInformationLength,
	OUT  PULONG  ReturnLength OPTIONAL
	);

//
typedef NTSTATUS (WINAPI* PNtSetSystemPowerState)(
	IN POWER_ACTION SystemAction,
	IN SYSTEM_POWER_STATE MinSystemState,
	IN ULONG Flags);

//
typedef NTSTATUS (WINAPI* PNtShutdownSystem)(
	SHUTDOWN_ACTION Action
	);

////
//typedef PVOID (WINAPI *PImageRvaToVa)(
//	IN PIMAGE_NT_HEADERS NtHeaders,
//	IN PVOID Base,
//	IN ULONG Rva,
//	IN OUT PIMAGE_SECTION_HEADER *LastRvaSection
//	);
//
//static PImageRvaToVa ImageRvaToVa;

static PPROCESS_MODULE_INFO pProcessModuleInfo,pProcessModuleInfoHead;

//
char* common_TCHARToCHAR(TCHAR* pwstr);

//
TCHAR* common_CHARToTCHAR(char* pstr_);

//UTF-8到GBK的转换
char* common_UTFToGBK(const char* pstr_);

//GBK到UTF-8的转换
char* common_GBKToUTF(const char* pstr_);

//关机重启
//bReboot:标志是关机还是重启
//返回:true:成功，false:失败
bool common_shutdown(bool bReboot);

//提升进程权限
//hToken:进程令牌
//lpszPrivilege:权限别名
//bEnablePrivilege:使能
bool common_setPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);

//提升进程权限
//lpszPrivilege:权限别名
//bEnablePrivilege:使能
bool common_setPrivilege(ULONG ulPID, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);


//加载驱动
//lpFileName:驱动文件路径
//lpLinkName:服务名称
//返回:成功则返回驱动句柄,失败则返回NULL
HANDLE common_loadDriver(LPCTSTR lpFileName,LPTSTR lpLinkName);

//卸载驱动
//hDriver:驱动模块句柄
//lpLinkName:服务名称
//返回:true:成功，false:失败
bool common_unloadDriver(HANDLE hDriver,LPTSTR lpLinkName);


//打开文件属性对话框
//lpFilePath:目标文件路径
//返回:无
void common_showFileProperty(LPTSTR lpFilePath);

//定位文件
//lpFilePath:目标文件路径
//返回:无
void common_explorerFile(LPSTR lpFilePath);

//定位注册表
//lpRegPath:注册表路径
//返回:true:成功，false:失败
bool common_explorerReg(LPTSTR lpRegPath);


//根据资源ID来生产文件 
//resId:资源ID 
//lpResType:资源类型
//lpFileName:目标文件路径 
//返回:true:成功，false:失败
bool common_createFileFromResource(int resId,LPTSTR lpResType,LPTSTR lpFileName);

//添加服务
//lpName:服务名称
//lpDisplayName:友好显示名称
//ulStartMode:启动类型
//lpPath:应用程序路径
//返回:true:成功，false:失败
bool common_addService(LPTSTR lpName,LPTSTR lpDisplayName,ULONG ulStartMode,LPTSTR lpPath);

//删除服务
//lpName:服务名称
//返回:true:成功，false:失败
bool common_delService(LPTSTR lpName);

//设置代理
//lpIP:IP地址
//lpPort:端口
//返回:true:成功，false:失败
bool common_setProxy(LPTSTR lpIP,LPTSTR lpPort);

//取消代理
//返回:true:成功，false:失败
bool common_delProxy();

//判断文件是否是PE文件
//lpFilePath:目标文件
//返回:true:是，false:不是
bool common_isPEFile(LPTSTR lpFilePath);

//获得快捷方式文件所指向的路径
//lpwsLinkName:lnk文件的路径
//lpwsLinkPath:用于存放所指程序路径的缓冲区
//返回:HRESULT
HRESULT common_getLnkPath(IN LPWSTR lpwsLinkName,OUT LPWSTR lpwsLinkPath);

//生成IP地址字符串
PCHAR GetIP(unsigned int ipaddr);

#endif //_COMMON_