#ifndef _SAFE_SYSTEM_H_
#define _SAFE_SYSTEM_H_

#include "ntifs.h"
#include "Protect.h"
#include "SSDT.h"
#include "tcpip.h"
#include "services.h"

//初始化的时候，就要收集好进程了
//#include "Process.h"
//PPROCESSINFO HideProcessInfo;
//测试
#include "Ntfs.h"
//#include "port.h"
//PTCPUDPINFO TCPUDPInfo;

//#include "ProcessModule.h"
//#include "KernelHookCheck.h"
//PINLINEHOOKINFO InlineHookInfo;
#include "ObjectHookCheck.h"

BOOL DebugOn = FALSE;
BOOL bKernelBooting = FALSE;

PDRIVER_OBJECT PDriverObject;
DWORD RetAddress;

PEPROCESS SystemEProcess;

ULONG ulMyDriverBase;
ULONG ulMyDriverSize;

NTSTATUS ReLoadNtos(
	PDRIVER_OBJECT   DriverObject,
	DWORD RetAddress
	);

NTSTATUS ReLoadNtfs(PDRIVER_OBJECT DriverObject,PNTFSDISPATCHBAKUP NtfsDispatchBakUp,int IniType);
NTSTATUS ReLoadNtfsFree(PNTFSDISPATCHBAKUP NtfsDispatchBakUp);

NTSTATUS ReLoadKbdclass(
	PDRIVER_OBJECT DriverObject
	);

BOOL InitControl();  //通信控制
//BOOL ProtectCode();  //自我保护

NTSTATUS LookupProcessByName(
	IN PCHAR pcProcessName,
	OUT PEPROCESS *pEprocess
	);

VOID WaitMicroSecond(
	LONG MicroSeconds
	);

BOOL Safe_CreateValueKey(
	PWCHAR SafeKey,
	ULONG Reg_Type,
	PWCHAR ValueName,
	PWCHAR Value
	);


// ULONG GetKeServiceDescriptorTable();
// BOOL PrintSSDT();
// void ShowHidedDriver();
// BOOL SetAllSSDTFunction();
//NTSTATUS ReLoadTcpip(PDRIVER_OBJECT DriverObject,PVOID TcpDispatchBakUp,int Type);
// NTSTATUS ReLoadTcpipFree(PDRIVER_OBJECT DriverObject,PTCPDISPATCHBAKUP TcpDispatchBakUp);
//NTSTATUS ReLoadNsiproxy(PDRIVER_OBJECT DriverObject,PVOID TcpDispatchBakUp,int Type);
// NTSTATUS ReLoadNsiproxyFree(PDRIVER_OBJECT DriverObject,PTCPDISPATCHBAKUP TcpDispatchBakUp);
//BOOL PrintTcpIpInWin7(PTCPUDPINFO TCPUDPInfo);
//BOOL GetNormalProcessList(PPROCESSINFO Info,PPROCESSINFO HideInfo);
//BOOL KernelHookCheck(PINLINEHOOKINFO InlineHookInfo);
//VOID EnumKernelModule(PDRIVER_OBJECT DriverObject,PSYSINFO SysModuleInfo);
//BOOLEAN QueryServicesRegistry(OUT PNTSTATUS StatusOut,PSERVICESREGISTRY ServicesRegistry);
//BOOL DeleteServicesKey(WCHAR *ServicesKey);
//BOOL ShadowSSDTHookCheck();
// VOID IoFileObjectTypeHookInfo(POBJECTHOOKINFO ObjectHookInfo);
// VOID IoDeviceObjectTypeHookInfo(POBJECTHOOKINFO ObjectHookInfo);
//VOID ResetMyControl();
//NTSTATUS kernel_filterDriverEnum();
//VOID EunmProcessModuleByVirtualMemory(ULONG ulPid,PDLLINFO PDll);
// NTSTATUS ResumeThread(PETHREAD Thread);
// NTSTATUS SuspendThread(PETHREAD Thread);
//NTSTATUS ReLoadAtapi(PDRIVER_OBJECT DriverObject,PVOID AtapiDispatchBakUp,int Type);
//ULONG QuerySystemNotify(PDRIVER_OBJECT DriverObj,PSYSTEM_NOTIFY SystemNotify);
#endif