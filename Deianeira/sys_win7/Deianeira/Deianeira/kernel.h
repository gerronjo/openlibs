#include "common.h"
#include "sysmodule.h"


typedef struct _EX_FAST_REF
{
	union
	{
		PVOID Object;
		ULONG_PTR RefCnt:3;
		ULONG_PTR Value;
	};
} EX_FAST_REF, *PEX_FAST_REF;


typedef struct _EX_CALLBACK_ROUTINE_BLOCK
{
	EX_RUNDOWN_REF RundownProtect;
	PEX_CALLBACK_FUNCTION Function;
	PVOID Context;
} EX_CALLBACK_ROUTINE_BLOCK, *PEX_CALLBACK_ROUTINE_BLOCK;


typedef struct _DIRECTORY_BASIC_INFORMATION {
	UNICODE_STRING ObjectName;
	UNICODE_STRING ObjectTypeName;
} DIRECTORY_BASIC_INFORMATION, *PDIRECTORY_BASIC_INFORMATION;

NTSYSAPI     
NTSTATUS    
NTAPI
ZwOpenDirectoryObject( 
					  __out PHANDLE DirectoryHandle, 
					  __in ACCESS_MASK DesiredAccess,
					  __in POBJECT_ATTRIBUTES ObjectAttributes 
					  );

NTSYSAPI     
NTSTATUS    
NTAPI 
ZwQueryDirectoryObject(
					   IN HANDLE DirectoryHandle,
					   OUT PVOID Buffer,
					   IN ULONG BufferLength,
					   IN BOOLEAN ReturnSingleEntry,
					   IN BOOLEAN RestartScan,
					   IN OUT PULONG Context,
					   OUT PULONG ReturnLength OPTIONAL
					   );


//检测到的系统回调函数中的信息
typedef struct _KernelSystemCallbackInfo{
	//回调类型
	ULONG ulType;
	//驱动路径
	WCHAR PathName[256];
	//
	ULONG ulEntryAddress;
	//回调函数地址
	ULONG ulCallBackAddress;
	//附加信息
	ULONG ulAttatch;

	//指向下一个
    struct KERNEL_SYSTEM_CALLBACK_INFO* next;
}KERNEL_SYSTEM_CALLBACK_INFO,*PKERNEL_SYSTEM_CALLBACK_INFO;

//检测到的过滤驱动的信息
typedef struct _KernelFilterDriverInfo{
	ULONG ulObjType;
	WCHAR FileName[64];
	WCHAR FilePath[256];
	ULONG ulAttachDevice;
	//宿主
	WCHAR HostFileName[64];
	//指向下一个
	struct KERNEL_FILTERDRIVER_INFO* next;
}KERNEL_FILTERDRIVER_INFO,*PKERNEL_FILTERDRIVER_INFO;

//全局
PKERNEL_SYSTEM_CALLBACK_INFO pKernelSystemCallbackInfo,pKernelSystemCallbackInfoHead;

//全局
PKERNEL_FILTERDRIVER_INFO pKernelFilterDriverInfo,pKernelFilterDriverInfoHead;


//检测系统的回调函数
NTSTATUS kernel_systemCallbackEnum();

//获得大小
ULONG kernel_systemCallbackGetCount();

//发送数据到ring3
NTSTATUS kernel_systemCallbackGetInfo(PKERNEL_SYSTEM_CALLBACK_INFO pBuffer);

//卸载
NTSTATUS kernel_systemCallbackKill(ULONG ulEntryAddress,ULONG ulFuncAddr);

//枚举过滤驱动
NTSTATUS kernel_filterDriverEnum();

//获得大小
ULONG kernel_filterDriverGetCount();

//发送数据到ring3
NTSTATUS kernel_filterDriverGetInfo(PKERNEL_FILTERDRIVER_INFO pBuffer);

//摘除过滤驱动
NTSTATUS kernel_filterDriverKill(PDEVICE_OBJECT pAttachObj);