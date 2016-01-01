

#include <ntifs.h>
//#include "ntddk.h"
#include "dbghelp.h"
#include "KsBinSword.h"

#include <stdio.h>
#include <stdarg.h>
#include <ntimage.h>
#include <ntiologc.h>
#include <srb.h>

//-----------宏定义-------------------------------
#define FindPspTerminateThreadByPointer 0
#define FindPspTerminateProcess 1
#define EPROCESS_SIZE 0
#define PEB_OFFSET 1
#define FILE_NAME_OFFSET 2

#define PROCESS_LINK_OFFSET 3
#define PROCESS_ID_OFFSET 4
#define EXIT_TIME_OFFSET 5


#define MAXPATHLEN 256
#define SEC_IMAGE    0x01000000

// Length of process name (rounded up to next DWORD)
#define PROCNAMELEN     20
// Maximum length of NT process name
#define NT_PROCNAMELEN  16
ULONG gProcessNameOffset;

// Length of process name (rounded up to next DWORD)
#define PROCNAMELEN     20
// Maximum length of NT process name
#define NT_PROCNAMELEN  16

#define MaxMonNum 50
#define MaxLenNum 200
//----函数指针类型定义--------------------------------------

typedef NTSTATUS (*ZWCREATEPROCESS)(
									OUT PHANDLE ProcessHandle,
									IN ACCESS_MASK DesiredAccess,
									IN POBJECT_ATTRIBUTES ObjectAttributes,
									IN HANDLE InheritFromProcessHandle,
									IN BOOLEAN InheritHandles,
									IN HANDLE SectionHandle OPTIONAL,
									IN HANDLE DebugPort OPTIONAL,
									IN HANDLE ExceptionPort OPTIONAL,
									IN HANDLE Unknown 
									);
typedef NTSTATUS (*ZWSETVALUEKEY)
(
 IN HANDLE  KeyHandle,
 IN PUNICODE_STRING  ValueName,
 IN ULONG  TitleIndex  OPTIONAL,
 IN ULONG  Type,
 IN PVOID  Data,
 IN ULONG  DataSize
 );
typedef NTSTATUS (*ZWLOADDRIVER)
(
 IN PUNICODE_STRING  DriverServiceName
 );
typedef NTSTATUS NTKERNELAPI (*PSPTERMINATETPROCESS)(
PEPROCESS Process,
NTSTATUS ExitStatus
);
typedef NTSTATUS (*PSPTERMINATETHREADBYPOINTER)( PETHREAD, NTSTATUS);//线程
PSPTERMINATETPROCESS LPspTerminateProcess ; 

typedef NTSTATUS (*NTQUERYSYSTEMINFORMATION)(
											 
											 IN ULONG                        SystemInformationClass,
											 OUT PVOID                        SystemInformation,
											 IN ULONG                        SystemInformationLength,
											 OUT PULONG                        ReturnLength OPTIONAL  );


//---------------------Native API-----------------------------
NTSYSAPI
NTSTATUS
NTAPI
ZwQuerySystemInformation(
						 IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
						 IN OUT PVOID SystemInformation,
						 IN ULONG SystemInformationLength,
						 OUT PULONG ReturnLength OPTIONAL
						 );
NTSYSAPI NTSTATUS NTAPI ObReferenceObjectByName(IN PUNICODE_STRING ObjectName,
												IN ULONG Attributes, IN PACCESS_STATE AccessState OPTIONAL, IN ACCESS_MASK
												DesiredAccess OPTIONAL, IN POBJECT_TYPE ObjectType, IN KPROCESSOR_MODE
  AccessMode, IN OUT PVOID ParseContext OPTIONAL, OUT PVOID *Object);


NTKERNELAPI
NTSTATUS
PsLookupProcessByProcessId (
							IN ULONG        ProcessId,
							OUT PEPROCESS   *Process
							);

NTKERNELAPI
VOID
KeStackAttachProcess (
					  IN PRKPROCESS PROCESS,
					  OUT PRKAPC_STATE ApcState
					  );

NTKERNELAPI
VOID
KeUnstackDetachProcess (
						IN PRKAPC_STATE ApcState
						);

NTSTATUS
NTAPI
ZwTerminateProcess(
				   IN HANDLE  ProcessHandle  OPTIONAL,
				   IN NTSTATUS  ExitStatus);


NTSTATUS
PsSetCreateProcessNotifyRoutine(
								IN PCREATE_PROCESS_NOTIFY_ROUTINE  NotifyRoutine,
								IN BOOLEAN  Remove
								);



NTSTATUS
PsSetCreateThreadNotifyRoutine(
							   IN PCREATE_THREAD_NOTIFY_ROUTINE  NotifyRoutine
							   );
NTSTATUS
PsSetLoadImageNotifyRoutine(
							IN PLOAD_IMAGE_NOTIFY_ROUTINE  NotifyRoutine
							);


NTSTATUS
 PsRemoveCreateThreadNotifyRoutine(
 IN PCREATE_THREAD_NOTIFY_ROUTINE  NotifyRoutine 
 );
NTSTATUS 
 PsRemoveLoadImageNotifyRoutine( 
 IN PLOAD_IMAGE_NOTIFY_ROUTINE  NotifyRoutine  
 );



//----------自定义函数申明------------------------------


NTSTATUS KsBinSwordDispatchCreate(
						 IN PDEVICE_OBJECT		DeviceObject,
                         IN PIRP					Irp
	);
NTSTATUS KsBinSwordDispatchClose(
						IN PDEVICE_OBJECT		DeviceObject,
						IN PIRP					Irp
	);
NTSTATUS KsBinSwordDispatchDeviceControl(
								IN PDEVICE_OBJECT		DeviceObject,
								IN PIRP					Irp
	);
VOID
KsBinSwordUnload(
				 IN PDRIVER_OBJECT		DriverObject
	);
//------------监控部分-------------------------------------
DWORD GetPlantformDependentInfo ( DWORD dwFlag ) ;



NTSTATUS FakedZwCreateProcess(
							  OUT PHANDLE ProcessHandle,
							  IN ACCESS_MASK DesiredAccess,
							  IN POBJECT_ATTRIBUTES ObjectAttributes,
							  IN HANDLE InheritFromProcessHandle,
							  IN BOOLEAN InheritHandles,
							  IN HANDLE SectionHandle OPTIONAL,
							  IN HANDLE DebugPort OPTIONAL,
							  IN HANDLE ExceptionPort OPTIONAL,
							  IN HANDLE Unknown 
							  );

// 
// NTSTATUS PsLookupProcessByProcessId(
// 									IN ULONG ulProcId, 
// 									OUT PEPROCESS * pEProcess
// 									);



NTSTATUS FakedZwSetValueKey
(
 IN HANDLE  KeyHandle,
 IN PUNICODE_STRING  ValueName,
 IN ULONG  TitleIndex  OPTIONAL,
 IN ULONG  Type,
 IN PVOID  Data,
 IN ULONG  DataSize
 );


NTSTATUS FakedZwLoadDriver
(						
 IN PUNICODE_STRING DriverServiceName
 );



VOID RegMoniterOn();
VOID RegMoniterOff();
VOID ProcMoniterOn();
VOID ProcMoniterOff();
VOID ModMonitorOn();
VOID ModMonitorOff();
//-------------------------进程部分--------------------
PVOID GetUndocumentFunctionAdress(int FindWhat);
void KillThread(int Pid,int Tid);
VOID KillPro(int pid);
PVOID GetDriverBaseAdress(void);
ULONG GetCidAddr();
ULONG GetProcessType();
VOID RecordInfo(ULONG i);
VOID IsValidProcess();
int GetProNum();
VOID DisplayProcess(ProcessInfo *Pro);
VOID FreeProcess();
VOID FreeThread();
VOID DisplayThread(PTHREAD_INFO  OutBuf);
DWORD GetPlantformDependentInfo ( DWORD dwFlag );
PTHREAD_INFO GetThread(int Pid);
//-------------------------磁盘部分--------------------
NTSTATUS IrpCompletionRoutine_0(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN
								PVOID Context);
PDEVICE_OBJECT GetLastDiskDeviceObject(PDRIVER_OBJECT drv_object);
NTSTATUS MyIoCallDriver(PDEVICE_OBJECT DeviceObject, PIRP Irp);
PDEVICE_OBJECT GetAtaDr0DevObject();
ULONG AtapiReadWriteDisk(PDEVICE_OBJECT dev_object, 
						 ULONG MajorFunction, 
						 PVOID buffer, 
						 ULONG DiskPos,//扇区号
						 int BlockCount//读写多少个扇区
						 );
//------------------全局变量-----------------
BOOLEAN bRegMon = FALSE;
BOOLEAN bProcMon= FALSE;
BOOLEAN bModMon = FALSE;

int    position;
int    pos;
int	   po;
KEVENT event ; 
char *output;
extern POBJECT_TYPE *IoDriverObjectType;
ProcessInfo *head,*pCid;//头指针

PTHREAD_INFO ThreadInfoHead={0};//线程信息头
extern PServiceDescriptorTableEntry KeServiceDescriptorTable; 
LARGE_INTEGER realdiskpos;
ULONG sectorspercluster;
ULONG gProcessNameOffset;
ZWSETVALUEKEY RealZwSetValueKey;
ZWCREATEPROCESS RealZwCreateProcess;
ZWLOADDRIVER RealZwLoadDriver;
NTQUERYSYSTEMINFORMATION NtQuerySystemInformation;
// #define MaxMonNum 50
// #define MaxLenNum 200
CHAR  g_MonInfoStrArr[MaxMonNum][MaxLenNum];
ULONG  g_MonInfoInIndex=0;
ULONG  g_MonInfoOutIndex=0;
void PrintMonInfosMon(CHAR wcStr[])
{
	if (strlen(wcStr)>MaxLenNum-1)
	{
		return;
	}
	g_MonInfoInIndex=++g_MonInfoInIndex==MaxMonNum?0:g_MonInfoInIndex;
	
	strcpy(g_MonInfoStrArr[g_MonInfoInIndex],wcStr);
}


#include "KBSProcess.c"
#include "KBSscsi.c"
#include "KBSmon.c"
//---------函数定义---------------------------------


VOID ProcessCreateMon ( IN HANDLE hParentId, IN HANDLE PId,IN BOOLEAN bCreate )
{
	
    PEPROCESS  EProcess;
    ULONG      ulCurrentProcessId;
    LPTSTR       lpCurProc;
    NTSTATUS   status;
	CHAR PrintArr[MaxLenNum]={'\0'};
    status = PsLookupProcessByProcessId( (ULONG)PId, &EProcess);
    if (!NT_SUCCESS( status ))
    {
        DbgPrint("PsLookupProcessByProcessId()\n");
        return ;
    }
    
	
    if ( bCreate )
    {
		lpCurProc = (LPTSTR)EProcess;
		
        lpCurProc = lpCurProc + IMAGEFILENAME;
		//DbgBreakPoint();
        DbgPrint( "CREATE PROCESS = PROCESS NAME: %s , PROCESS PARENTID: %d, PROCESS ID: %d, PROCESS ADDRESS %x:\n", 
			lpCurProc,
			hParentId,
			PId,
			EProcess );
		sprintf(PrintArr,"创建进程！; 进程名: %s , 父进程ID: %d, 本进程 ID: %d, 进程基址 %x:\r\n", 
			lpCurProc,
			hParentId,
			PId,
			EProcess );
        PrintMonInfosMon(PrintArr);
    }
	
    else
    {
		
        DbgPrint( "TERMINATED == PROCESS ID: %d\n", PId);
		sprintf(PrintArr,"结束进程！ ID: %d\n", PId);
        PrintMonInfosMon(PrintArr);
    }
	
}

VOID ThreadCreateMon (IN HANDLE PId, IN HANDLE TId, IN BOOLEAN  bCreate)
{
	
    PEPROCESS   EProcess;
    ULONG        ulCurrentProcessId;
    LPTSTR        lpCurProc;
    NTSTATUS    status;
	CHAR PrintArr[MaxLenNum]={'\0'};
    status = PsLookupProcessByProcessId( (ULONG)PId, &EProcess);
    if (!NT_SUCCESS( status ))
    {
        DbgPrint("PsLookupProcessByProcessId()\n");
        return ;
    }    
	
    if ( bCreate )
    {
		lpCurProc    = (LPTSTR)EProcess;
		//DbgBreakPoint();
        lpCurProc    = lpCurProc + IMAGEFILENAME;
	
        DbgPrint( "CREATE THREAD = PROCESS NAME: %s PROCESS ID: %d, THREAD ID: %d\n", lpCurProc, PId, TId );
		sprintf(PrintArr,"创建线程！ 所属进程名: %s 进程 ID: %d, 线程 ID: %d\n", lpCurProc, PId, TId);
        PrintMonInfosMon(PrintArr);
	}
	
    else
    {
		
        DbgPrint( "TERMINATED == THREAD ID: %d\n", TId);
		sprintf(PrintArr,"线程结束！ID: %d\n", TId);
        PrintMonInfosMon(PrintArr);
    }
	
}

VOID ImageCreateMon (IN PUNICODE_STRING  FullImageName, IN HANDLE  ProcessId, IN PIMAGE_INFO  ImageInfo )
{
	CHAR PrintArr[MaxLenNum]={'\0'};
    DbgPrint("FullImageName: %S,Process ID: %d\n",FullImageName->Buffer,ProcessId);
    DbgPrint("ImageBase: %x,ImageSize: %d\n",ImageInfo->ImageBase,ImageInfo->ImageSize);

	sprintf(PrintArr,"模块加载！路径: %S,进程ID: %d\n",FullImageName->Buffer,ProcessId);
    PrintMonInfosMon(PrintArr);
	sprintf(PrintArr,"模块基址: %x,模块大小: %d\n",ImageInfo->ImageBase,ImageInfo->ImageSize);
    PrintMonInfosMon(PrintArr);
	
}


NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{    

	PTHREAD_INFO ProInfoHead;


	//--
	NTSTATUS			status = STATUS_SUCCESS;    
    UNICODE_STRING		ntDeviceName;
	UNICODE_STRING		dosDeviceName;
    PDEVICE_EXTENSION	deviceExtension;
	PDEVICE_OBJECT		deviceObject = NULL;
	BOOLEAN				fSymbolicLink = FALSE;
	int i;
    UNICODE_STRING dllName;
    DWORD functionAddress;
	UNICODE_STRING devname;
	UNICODE_STRING devlink;
	PDEVICE_OBJECT devob ;
	//KdBreakPoint();
    //KillPro(464);
	//KillThread(1012,1076);
    RtlInitUnicodeString(&ntDeviceName, KSBINSWORD_DEVICE_NAME_W);
	
	//IsValidProcess();
	//Display();
	//GetDriverBaseAdress();
    status = IoCreateDevice(
		DriverObject,
		sizeof(DEVICE_EXTENSION), // DeviceExtensionSize
		&ntDeviceName,            // DeviceName
		FILE_DEVICE_KSBINSWORD,   // DeviceType
		0,                        // DeviceCharacteristics
		TRUE,                     // Exclusive
		&deviceObject             // [OUT]
		);
	
    if (!NT_SUCCESS(status))
	{
		dprintf("[KsBinSword] IoCreateDevice = 0x%x\n", status);
		goto __failed;
	}
	
	deviceExtension = (PDEVICE_EXTENSION)deviceObject->DeviceExtension;
	
    RtlInitUnicodeString(&dosDeviceName, KSBINSWORD_DOS_DEVICE_NAME_W);
	
    status = IoCreateSymbolicLink(&dosDeviceName, &ntDeviceName);
	
    if (!NT_SUCCESS(status))
    {
        dprintf("[KsBinSword] IoCreateSymbolicLink = 0x%x\n", status);
		goto __failed;
    }
	
	fSymbolicLink = TRUE;
	
    DriverObject->MajorFunction[IRP_MJ_CREATE]         = KsBinSwordDispatchCreate;
    DriverObject->MajorFunction[IRP_MJ_CLOSE]          = KsBinSwordDispatchClose;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = KsBinSwordDispatchDeviceControl;
    DriverObject->DriverUnload                         = KsBinSwordUnload;
	
	
//--
	status = PsSetLoadImageNotifyRoutine(ImageCreateMon);
    if (!NT_SUCCESS( status ))
    {
        DbgPrint("PsSetLoadImageNotifyRoutine()\n");
        return status;
    }    
	
    status = PsSetCreateThreadNotifyRoutine(ThreadCreateMon);
    if (!NT_SUCCESS( status ))
    {
        DbgPrint("PsSetCreateThreadNotifyRoutine()\n");
        return status;
    }    
	
//     status = PsSetCreateProcessNotifyRoutine(ProcessCreateMon, FALSE);
//     if (!NT_SUCCESS( status ))
//     {
//         DbgPrint("PsSetCreateProcessNotifyRoutine()\n");
//         return status;
//     }    
//     

	//---


	KeInitializeEvent(&event,SynchronizationEvent,1);
	
	
	
    GetProcessNameOffset();
    
    
    RtlInitUnicodeString(&dllName, L"\\Device\\HarddiskVolume1\\Windows\\System32\\ntdll.dll");
    functionAddress = GetDllFunctionAddress("ZwCreateProcessEx", &dllName);
    position = *((WORD*)(functionAddress+1));
	
    //DbgPrint("ZwCreateProcessEx's Id:%d\n", position);
	
	RealZwCreateProcess = (ZWCREATEPROCESS)(*(((PServiceDescriptorTableEntry)KeServiceDescriptorTable)->ServiceTableBase + position));
	functionAddress = GetDllFunctionAddress("ZwLoadDriver",&dllName);
	pos =  *((WORD*)((DWORD)ZwSetValueKey+1));
	po  =  *((WORD *)(functionAddress+1));
	
	//DbgPrint("ZwSetValueKey's Id:%d\n", pos);
	//DbgPrint("ZwLoadDriver's Id:%d\n",po);
	//DbgPrint("ZwLoadDriver's address is %d\n",ZwLoadDriver);

	RealZwSetValueKey =   (ZWSETVALUEKEY)(*(((PServiceDescriptorTableEntry)KeServiceDescriptorTable)->ServiceTableBase + pos));
	RealZwLoadDriver  =	  (ZWLOADDRIVER)(*(((PServiceDescriptorTableEntry)KeServiceDescriptorTable)->ServiceTableBase + po));
	DbgPrint("RealZwCreateProcess's address is %x\n",RealZwCreateProcess);
	return status;
__failed:
	
	if (fSymbolicLink)
		IoDeleteSymbolicLink(&dosDeviceName);
	
	if (deviceObject)
		IoDeleteDevice(deviceObject);
	
	return status;

}

NTSTATUS
KsBinSwordDispatchCreate(
	IN PDEVICE_OBJECT		DeviceObject,
	IN PIRP					Irp
	)
{
	NTSTATUS status = STATUS_SUCCESS;

    Irp->IoStatus.Information = 0;

	//dprintf("[KsBinSword] IRP_MJ_CREATE\n");

    Irp->IoStatus.Status = status;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return status;
}

NTSTATUS
KsBinSwordDispatchClose(
	IN PDEVICE_OBJECT		DeviceObject,
	IN PIRP					Irp
	)
{
	NTSTATUS status = STATUS_SUCCESS;
    //DbgBreakPoint();
    Irp->IoStatus.Information = 0;

	//dprintf("[KsBinSword] IRP_MJ_CLOSE\n");

    Irp->IoStatus.Status = status;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return status;
}


NTSTATUS
KsBinSwordDispatchDeviceControl(
	IN PDEVICE_OBJECT		DeviceObject,
	IN PIRP					Irp
	)
{
	NTSTATUS			status = STATUS_SUCCESS;
    PIO_STACK_LOCATION	irpStack;
    PDEVICE_EXTENSION	deviceExtension;
    PVOID				ioBuf;
	PVOID InputBuffer;
	PVOID OutputBuffer;
	ULONG InputBufferLength;
	ULONG OutputBufferLength;

    ULONG				inBufLength, outBufLength;
	ULONG				ioControlCode;
    PDEVICE_OBJECT AtaDr0DevObject;
	ULONG Sect=0;//读写的扇区号
	BYTE  DiskBuf[0x200]={'\0'};//读写的扇区缓冲
	PTHREAD_INFO InfoTem={0};
	int ThreaNum=0;
	UCHAR *buff =0;
	PKillThreadStruct pKillThread;
	ULONG a,i=0,j=0;
	WCHAR **wcOutMonInfoArr;//输出xx启动信息的数组
	NTSTATUS s = Irp->IoStatus.Status = STATUS_SUCCESS;
    irpStack = IoGetCurrentIrpStackLocation(Irp);
    deviceExtension = (PDEVICE_EXTENSION)DeviceObject->DeviceExtension;

    Irp->IoStatus.Information = 0;
	
    //
    // Get the pointer to the input/output buffer and it's length
    //

    InputBuffer=OutputBuffer= ioBuf = Irp->AssociatedIrp.SystemBuffer;
    inBufLength = irpStack->Parameters.DeviceIoControl.InputBufferLength;
    outBufLength = irpStack->Parameters.DeviceIoControl.OutputBufferLength;
    ioControlCode = irpStack->Parameters.DeviceIoControl.IoControlCode;
    //DbgBreakPoint();
    //dprintf("ioControlCode:%x\n",ioControlCode);
    switch (ioControlCode)
    {
	case IOCTL_KSBINSWORD_GETPRONUM:
		{

			status=STATUS_SUCCESS;
			IsValidProcess();
            *(int *)ioBuf=GetProNum();
			DbgPrint("GetProNum :%d,%x",*(int *)ioBuf,pCid);
			outBufLength=sizeof(int);
			Irp->IoStatus.Information = sizeof(int);
            break;
		}
	case IOCTL_KSBINSWORD_GETPRO:
		{
			DbgPrint("GetPro:%x",pCid);
			status=STATUS_SUCCESS;
			DisplayProcess((ProcessInfo *)ioBuf);
			outBufLength=inBufLength;
			Irp->IoStatus.Information = inBufLength;
            break;
		}
	case IOCTL_KSBINSWORD_GETTHRNUM:
		{
			status=STATUS_SUCCESS;
            //DbgBreakPoint();
			ThreadInfoHead=GetThread(*(int*)ioBuf);
			if (ThreadInfoHead==NULL)
			{
				ThreaNum=0;
			}
			else
			{
              InfoTem= ThreadInfoHead;
			  while (InfoTem)
			  {
                ThreaNum++;
				InfoTem=InfoTem->Next;
			  }
			}
			DbgPrint("ThreaNum :%d",ThreaNum);
			*(int *)ioBuf=ThreaNum;
			outBufLength=sizeof(int);
			Irp->IoStatus.Information = sizeof(int);
			break;
		}
	case IOCTL_KSBINSWORD_CLEARPROCESS://清除查询进程痕迹
		{
			
			status=STATUS_SUCCESS;
			
			//DbgBreakPoint();
			FreeProcess();
			
			outBufLength=inBufLength;
			Irp->IoStatus.Information = inBufLength;
            break;
		}
	case IOCTL_KSBINSWORD_CLEARTHREAD://清除查询线程痕迹
		{
			
			status=STATUS_SUCCESS;
			
			//DbgBreakPoint();
			FreeThread();
			
			outBufLength=inBufLength;
			Irp->IoStatus.Information = inBufLength;
            break;
		}
	case IOCTL_KSBINSWORD_GETTHR://得到线程
		{
			
			status=STATUS_SUCCESS;

			//DbgBreakPoint();
			DisplayThread((PTHREAD_INFO)ioBuf);

			outBufLength=inBufLength;
			Irp->IoStatus.Information = inBufLength;
            break;
		}
	case IOCTL_KSBINSWORD_KILLTHREAD://强制kill 线程
		{
			
			status=STATUS_SUCCESS;
			DbgPrint("强制kill 线程");
			//DbgBreakPoint();
			
			pKillThread=(PKillThreadStruct)ioBuf;
			//KillPro(464);
         	KillThread((int)(pKillThread->dwProcessId),(int)(pKillThread->dwCid));
			outBufLength=inBufLength;
			Irp->IoStatus.Information = inBufLength;
            break;
		}
	case IOCTL_KSBINSWORD_KILLPROCESS://强制杀掉 进程
		{
			
			status=STATUS_SUCCESS;
			DbgPrint("IOCTL_KSBINSWORD_KILLPROCESS");
			//DbgBreakPoint();
			
			KillPro(*(int*)ioBuf);
			outBufLength=inBufLength;
			Irp->IoStatus.Information = inBufLength;
            break;
		}
	case IOCTL_KSBINSWORD_PMON:
		DbgPrint("IoControlCode 1000\n");
		if(!bProcMon)
		{
			buff = (UCHAR *)Irp->AssociatedIrp.SystemBuffer ;
			
			ProcMoniterOn();
			
			memmove(&a,&buff[4],4);
			output=(char*)MmMapIoSpace(MmGetPhysicalAddress((void*)a),256,0);
			
			
			
		}
		break;
		
	case IOCTL_KSBINSWORD_PMOFF:
		DbgPrint("IoControlCode 1001\n");
		if (bProcMon)
		{
			ProcMoniterOff();
			
		}
		break;
		
	case IOCTL_KSBINSWORD_RMON:
		DbgPrint("IoControlCode 1002\n");
		if (!bRegMon)
		{
			buff = (UCHAR *)Irp->AssociatedIrp.SystemBuffer ;
			
			RegMoniterOn();
			
			memmove(&a,&buff[4],4);
			output=(char*)MmMapIoSpace(MmGetPhysicalAddress((void*)a),256,0);
			
		}
		break;
		
	case IOCTL_KSBINSWORD_RMOFF:
		DbgPrint("IoControlCode 1003\n");
		if (bRegMon)
		{
			RegMoniterOff();
			
		}
		break;
		
	case IOCTL_KSBINSWORD_MMON:
		DbgPrint("IoControlCode 1004\n");
		if (!bModMon)
		{
			buff = (UCHAR *)Irp->AssociatedIrp.SystemBuffer ;
			
			ModMonitorOn();
			
			memmove(&a,&buff[4],4);
			output=(char*)MmMapIoSpace(MmGetPhysicalAddress((void*)a),256,0);
		}
		break;
		
	case IOCTL_KSBINSWORD_MMOFF:
		DbgPrint("IoControlCode 1005\n");
		if (bModMon)
		{
			ModMonitorOff();
		}
			break;
	//*************************************************
	case IOCTL_KSBINSWORD_GETSSDT:	//得到SSDT
		
		//__try
		{
			//ProbeForWrite( OutputBuffer, sizeof( SSDT ), sizeof( ULONG ) );
			//DbgBreakPoint();
			//DbgPrint("!!:%x",*(int*)OutputBuffer);

			RtlCopyMemory( OutputBuffer, KeServiceDescriptorTable, sizeof( SSDT ) );
		}
// 		__except( EXCEPTION_EXECUTE_HANDLER )
// 		{
// 			s = GetExceptionCode();
// 			DbgPrint( "GetExceptionCode:%x !",s );
// 			break;
// 		}
		outBufLength=sizeof(SSDT);
		Irp->IoStatus.Information = sizeof(SSDT);
		DbgPrint( "SSDT: GetSSDT Completeled!" );
		break;
	//*************************************************
	case IOCTL_KSBINSWORD_SETSSDT: //设置 SSDT
		status=STATUS_SUCCESS;
		__try
		{
			//ProbeForRead( InputBuffer, sizeof( SSDT ), sizeof( ULONG ) );
			//去掉内存保护
			__asm
			{
				cli		;//关中断
				mov eax, cr0
				and eax, ~0x10000
				mov cr0, eax
			}
			RtlCopyMemory( KeServiceDescriptorTable, InputBuffer, sizeof( SSDT ) );
			//开中断,把内存保护加上
			 __asm
			 {
				mov eax, cr0
				or eax, 0x10000
				mov cr0, eax
				sti		;//开中断
			 }
		}
		__except( EXCEPTION_EXECUTE_HANDLER )
		{
			status = GetExceptionCode();
			break;
		}
		DbgPrint( "SSDT: SetSSDT Completeled!" );
		break;
	//*************************************************
	case IOCTL_KSBINSWORD_GETHOOK:	//查询SSDT指定地址
// 		__try
// 		{
// 			ProbeForRead( InputBuffer, sizeof( ULONG ), sizeof( ULONG ) );
// 			ProbeForWrite( OutputBuffer, sizeof( ULONG ), sizeof( ULONG ) );
// 		}
// 		__except( EXCEPTION_EXECUTE_HANDLER )
// 		{
// 			s = GetExceptionCode();
// 			break;
// 		}
		//测试传入的参数是否正确
		status=STATUS_SUCCESS;
		if( KeServiceDescriptorTable->NumberOfServices <= *(PULONG)InputBuffer )
		{
			status = STATUS_INVALID_PARAMETER;
			break;
		}
		//将结果传到用户输出位置
		*((PULONG)OutputBuffer) = *( (PULONG)(KeServiceDescriptorTable->ServiceTableBase) + *(PULONG)InputBuffer );
		outBufLength=sizeof( ULONG );
		Irp->IoStatus.Information = sizeof( ULONG );
		//DbgPrint( "SSDT: GetHookedAddress Completeled!" );
		break;
	//*************************************************
	case IOCTL_KSBINSWORD_SETHOOK:	//设置SSDT指定地址

		//测试传入的参数是否正确
		status=STATUS_SUCCESS;
		if( KeServiceDescriptorTable->NumberOfServices <= *(PULONG)InputBuffer )
		{
			status = STATUS_INVALID_PARAMETER;
			break;
		}
		//
		//在此将输出缓冲区当作输入缓冲区来用,输入指定SSDT HOOK的地址值
		//去掉内存保护
		//DbgBreakPoint();
		OutputBuffer = Irp->UserBuffer;
// 		__try
// 		{
// 			ProbeForRead( InputBuffer, sizeof( ULONG ), sizeof( ULONG ) );
// 			ProbeForRead( OutputBuffer, sizeof( ULONG ), sizeof( ULONG ) );
// 		}
// 		__except( EXCEPTION_EXECUTE_HANDLER )
// 		{
// 			status = GetExceptionCode();
// 			break;
// 		}
		__asm
		{
			cli		;//关中断
			mov eax, cr0
			and eax, ~0x10000
			mov cr0, eax
		}
		 *( (PULONG)(KeServiceDescriptorTable->ServiceTableBase) + *(PULONG)InputBuffer ) = *((PULONG)OutputBuffer);
		 
		 //开中断,把内存保护加上
		 __asm
		 {
			mov eax, cr0
			or eax, 0x10000
			mov cr0, eax
			sti		;//开中断
		 }
		 
		 DbgPrint("SetHookedAddress:%x,%x",*(PULONG)InputBuffer,*((PULONG)OutputBuffer) );
		 break;
	case IOCTL_KSBINSWORD_SCSI_READ:
		{
			
			
			
			DbgPrint("ioBuf :%x,%x,%x",*(ULONG *)ioBuf,inBufLength,outBufLength);
			Irp->IoStatus.Information = 0x200*sizeof(BYTE);
			AtaDr0DevObject = GetAtaDr0DevObject();
			Sect=	*(ULONG *)ioBuf ;	
			
            AtapiReadWriteDisk(AtaDr0DevObject, IRP_MJ_READ, &DiskBuf, Sect, 1);
			//DbgBreakPoint();
            RtlCopyMemory(ioBuf,DiskBuf,0x200);
			
			status=STATUS_SUCCESS;
            break;
		}
	case IOCTL_KSBINSWORD_SCSI_WRITE:
		{
			DbgPrint("ioBuf :%x,%x,%x",*(ULONG *)ioBuf,inBufLength,outBufLength);
			Irp->IoStatus.Information = sizeof(ULONG);
			AtaDr0DevObject = GetAtaDr0DevObject();
			Sect=*(ULONG*)((BYTE*)ioBuf+0x200*sizeof(BYTE));
			RtlCopyMemory(DiskBuf,ioBuf,0x200);
            AtapiReadWriteDisk(AtaDr0DevObject, IRP_MJ_WRITE, &DiskBuf, Sect, 1);
			//DbgBreakPoint();
            
			
			status=STATUS_SUCCESS;
            break;
		}
	case IOCTL_KSBINSWORD_GETMONINFO:
		{
// #define MaxMonNum 50
// #define MaxLenNum 70
// WCHAR  g_MonInfoStrArr[MaxMonNum][MaxLenNum];
// ULONG  g_MonInfoInIndex=0;
// ULONG  g_MonInfoOutIndex=0;

			wcOutMonInfoArr=(WCHAR**)ioBuf;
			dprintf("i:%d,o:%d\n",g_MonInfoOutIndex,g_MonInfoInIndex);
			if (g_MonInfoOutIndex==g_MonInfoInIndex)
			{
				break;
			}
			for (i=(g_MonInfoOutIndex+1)%MaxMonNum,j=0;i!=g_MonInfoInIndex;i=(++i)%MaxMonNum,j++)
			{
				
				strcpy((UCHAR *)((UCHAR*)wcOutMonInfoArr+j*sizeof(UCHAR)*MaxLenNum),g_MonInfoStrArr[i]);
				
			}
			//DbgBreakPoint();
			strcpy((UCHAR *)((UCHAR*)wcOutMonInfoArr+j*sizeof(UCHAR)*MaxLenNum),g_MonInfoStrArr[i]);	
			
			
			g_MonInfoOutIndex	=i;	
			Irp->IoStatus.Information = sizeof(UCHAR)*MaxMonNum*MaxLenNum;
				break;
		}
		
	//*************************************************
    default:
        status = STATUS_INVALID_PARAMETER;

        dprintf("[KsBinSword] unknown IRP_MJ_DEVICE_CONTROL = 0x%x (%04x,%04x)\n",
			ioControlCode, DEVICE_TYPE_FROM_CTL_CODE(ioControlCode),
			IoGetFunctionCodeFromCtlCode(ioControlCode));

        break;
	}


    Irp->IoStatus.Status = status;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return status;
}

VOID
KsBinSwordUnload(
	IN PDRIVER_OBJECT		DriverObject
	)
{
    UNICODE_STRING dosDeviceName;
    //DbgBreakPoint();
	if (bRegMon)
	{
		RegMoniterOff();
	}
	if(bProcMon)
	{
		ProcMoniterOff();
	}
	if (bModMon)
	{
		ModMonitorOff();
	}
    RtlInitUnicodeString(&dosDeviceName, KSBINSWORD_DOS_DEVICE_NAME_W);

    IoDeleteSymbolicLink(&dosDeviceName);

    //
    // Delete the device object
    //
    //PsSetCreateProcessNotifyRoutine(ProcessCreateMon, TRUE);
	PsRemoveCreateThreadNotifyRoutine(ThreadCreateMon);
	PsRemoveLoadImageNotifyRoutine(ImageCreateMon); 
	
    IoDeleteDevice(DriverObject->DeviceObject);

    dprintf("[KsBinSword] unloaded\n");
}