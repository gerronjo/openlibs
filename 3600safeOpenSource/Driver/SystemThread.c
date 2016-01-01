//**************************************************************************
//*
//*          A盾电脑防护 website:http://www.3600safe.com/
//*        
//*文件说明：
//*         系统线程/进程线程
//**************************************************************************
#include "SystemThread.h"

UCHAR GetThreadStatus(PETHREAD Thread)
{
	ULONG ulStatus;
	UCHAR ulRet = 0;
	WIN_VER_DETAIL WinVer;

	WinVer = GetWindowsVersion();
	switch (WinVer)
	{
	case WINDOWS_VERSION_XP:
		ulStatus = 0x02d;
		break;
	case WINDOWS_VERSION_2K3_SP1_SP2:
		ulStatus = 0x04c;
		break;
	case WINDOWS_VERSION_7:
		ulStatus = 0x068;
		break;
	}
	if (RMmIsAddressValid(((ULONG)Thread + ulStatus)))
		ulRet = *(UCHAR *)((ULONG)Thread + ulStatus);

	return ulRet;
}

//恢复线程运行
NTSTATUS ResumeThread(PETHREAD Thread)
{
	NTSTATUS status=STATUS_UNSUCCESSFUL;
	int SSDTIndex;
	NTRESUMETHREAD NtRealResumeThread;
	//ULONG ulResumeCount;
	PULONG ThreadFlags;

	if (!GetFunctionIndexByName("NtResumeThread",&SSDTIndex))
		return status;

	if (DebugOn)
		KdPrint(("Function Index:%d",SSDTIndex));

	if (SSDTIndex >= 0 &&
		SSDTIndex <= OriginalServiceDescriptorTable->TableSize)
	{
		NtRealResumeThread = OriginalServiceDescriptorTable->ServiceTable[SSDTIndex];
		if (RMmIsAddressValid(NtRealResumeThread) &&
			RMmIsAddressValid(Thread))
		{
			//if (GetThreadStatus(Thread) != StateWaiting &&
			//	GetThreadStatus(Thread) != StateRunning)
			//{

				if (DebugOn)
					KdPrint(("NtRealResumeThread:%08X",NtRealResumeThread));

				ThreadFlags = (ULONG *)((ULONG)Thread + ulCrossThreadFlagsOffset); 
				*ThreadFlags = (*ThreadFlags)|THREAD_SUSPEND_RESUME; 

				status = NtRealResumeThread(Thread,0);
			//}
		}
	}
	return status;
}
//暂停线程运行
NTSTATUS SuspendThread(PETHREAD Thread)
{
	NTSTATUS status=STATUS_UNSUCCESSFUL;
	int SSDTIndex;
	NTSUSPENDTHREAD NtRealSuspendThread;
	//ULONG ulSuspendCount;
	PULONG ThreadFlags;

	if (!GetFunctionIndexByName("NtSuspendThread",&SSDTIndex))
		return status;

	if (DebugOn)
		KdPrint(("Function Index:%d",SSDTIndex));

	if (SSDTIndex >= 0 &&
		SSDTIndex <= OriginalServiceDescriptorTable->TableSize)
	{
		NtRealSuspendThread = OriginalServiceDescriptorTable->ServiceTable[SSDTIndex];
		if (RMmIsAddressValid(NtRealSuspendThread) &&
			RMmIsAddressValid(Thread))
		{
			//if (GetThreadStatus(Thread) == StateWaiting ||
			//	GetThreadStatus(Thread) == StateRunning)
			//{
				if (DebugOn)
					KdPrint(("NtRealSuspendThread:%08X",NtRealSuspendThread));

				ThreadFlags = (ULONG *)((ULONG)Thread + ulCrossThreadFlagsOffset); 
				*ThreadFlags = (*ThreadFlags)|THREAD_SUSPEND_RESUME; 

				status = NtRealSuspendThread(Thread,0);
			//}
		}
	}
	return status;
}
VOID IniOffsetObject()
{
	WIN_VER_DETAIL WinVer;

	WinVer = GetWindowsVersion();
	switch (WinVer)
	{
	case WINDOWS_VERSION_XP:
		ThreadProc = 0x22C;
		ThreadListHead = 0x190; 
		ulCrossThreadFlagsOffset = 0x248;
		ulHideDebug = 0x248;
		ulThreadID = 0x1f0;
		ulTEB = 0x20;
		ulThreadStart = 0x224;
		ulWin32ThreadStart = 0x228;
		ulStatus = 0x02d;
		ulPriority = 0x033;
		ulContextSwitches = 0x04c;
		break;
	case WINDOWS_VERSION_2K3_SP1_SP2:
		ThreadProc = 0x224;
		ThreadListHead = 0x180;
		ulCrossThreadFlagsOffset = 0x240;
		ulHideDebug = 0x240;
		ulThreadID = 0x1e8;
		ulTEB = 0x74;
		ulThreadStart = 0x21c;
		ulWin32ThreadStart = 0x220;
		ulStatus = 0x04c;
		ulPriority = 0x05b;
		ulContextSwitches = 0x048;
		break;
	case WINDOWS_VERSION_7:
		ThreadProc = 0x268;
		ThreadListHead = 0x188;
		ulCrossThreadFlagsOffset = 0x280;
 		ulHideDebug = 0x280;
 		ulThreadID = 0x230;
 		ulTEB = 0x088;
 		ulThreadStart = 0x218;
		ulWin32ThreadStart = 0x260;
 		ulStatus = 0x068;
		ulPriority = 0x057;
		ulContextSwitches = 0x064;
		break;
	}
	return;
}
VOID QuerySystemThread(PSYSTEM_THREAD_INFO SystemThread,PEPROCESS EProcess)
{
	PETHREAD          Thread;
	PEPROCESS         Process;
	int i=0;
	ULONG ulSysModuleBase,ulSysModuleSize;

	if (!RMmIsAddressValid(EProcess) ||
		!IsExitProcess(EProcess))
	{
		if (DebugOn)
			KdPrint(("Eprocess failed"));
		return;
	}
	IniOffsetObject();

	Process = EProcess;
	__try
	{
		for (Thread = GetNextProcessThread(Process, NULL );
			Thread != NULL;
			Thread = GetNextProcessThread(Process, Thread ))
		{
			if (RMmIsAddressValid(Thread))
			{
				if (RMmIsAddressValid(((ULONG)Thread + ulStatus)))
					SystemThread->SystemThreadInfo[i].ulStatus = *(UCHAR *)((ULONG)Thread + ulStatus);

				if (DebugOn)
					KdPrint(("Thread status:%x:%x\r\n",(ULONG)Thread + ulStatus,SystemThread->SystemThreadInfo[i].ulStatus));

				if (RMmIsAddressValid(((ULONG)Thread + ulHideDebug)))
					SystemThread->SystemThreadInfo[i].HideDebug = *(ULONG *)((ULONG)Thread + ulHideDebug);

				SystemThread->SystemThreadInfo[i].KernelThread = (ULONG)Thread;

				if (RMmIsAddressValid(((ULONG)Thread + ulTEB)))
					SystemThread->SystemThreadInfo[i].TEB = *(ULONG *)((ULONG)Thread + ulTEB);

				if (RMmIsAddressValid(((ULONG)Thread + ulThreadID)))
					SystemThread->SystemThreadInfo[i].ThreadID = *(ULONG *)((ULONG)Thread + ulThreadID);

				if (EProcess == SystemEProcess)
				{
					if (RMmIsAddressValid(((ULONG)Thread + ulThreadStart)))
						SystemThread->SystemThreadInfo[i].ThreadStart = *(ULONG *)((ULONG)Thread + ulThreadStart);
				}else
				{
					if (RMmIsAddressValid(((ULONG)Thread + ulWin32ThreadStart)))
						SystemThread->SystemThreadInfo[i].ThreadStart = *(ULONG *)((ULONG)Thread + ulWin32ThreadStart);
				}
				
				if (RMmIsAddressValid(((ULONG)Thread + ulPriority)))
					SystemThread->SystemThreadInfo[i].ulPriority = *(ULONG *)((ULONG)Thread + ulPriority);

				if (RMmIsAddressValid(((ULONG)Thread + ulContextSwitches)))
					SystemThread->SystemThreadInfo[i].ulContextSwitches = *(ULONG *)((ULONG)Thread + ulContextSwitches);

				if (EProcess == SystemEProcess)
				{
					memset(SystemThread->SystemThreadInfo[i].lpszThreadModule,0,sizeof(SystemThread->SystemThreadInfo[i].lpszThreadModule));
					if (!IsAddressInSystem(
						SystemThread->SystemThreadInfo[i].ThreadStart,
						&ulSysModuleBase,
						&ulSysModuleSize,
						SystemThread->SystemThreadInfo[i].lpszThreadModule))
					{
						strcat(SystemThread->SystemThreadInfo[i].lpszThreadModule,"Unknown3(隐藏线程，危险！)");
						SystemThread->SystemThreadInfo[i].ulHideType = 1;   //隐藏线程
					}
				}
				if (DebugOn)
					KdPrint(("Thread:%08x-%08x-%s\r\n",Thread,SystemThread->SystemThreadInfo[i].ThreadStart,SystemThread->SystemThreadInfo[i].lpszThreadModule));
				i++;
				SystemThread->ulCount = i;
			}
		}
	}__except(EXCEPTION_EXECUTE_HANDLER){
		KdPrint(("QuerySystemThread:%08x",GetExceptionCode()));
	}
}
VOID KernelTerminateThreadRoutine( 
	IN PKAPC Apc, 
	IN OUT PKNORMAL_ROUTINE *NormalRoutine, 
	IN OUT PVOID *NormalContext, 
	IN OUT PVOID *SystemArgument1, 
	IN OUT PVOID *SystemArgument2 
	) 
{ 
	PULONG ThreadFlags;
	BOOL bInit = FALSE;

	if (RExFreePool)
		RExFreePool(Apc);
	else
		ExFreePool(Apc);

	if (ulCrossThreadFlagsOffset) 
	{ 
		ThreadFlags=(ULONG *)((ULONG)(PsGetCurrentThread())+ulCrossThreadFlagsOffset); 
		*ThreadFlags=(*ThreadFlags)|PS_CROSS_THREAD_FLAGS_SYSTEM; 

		if (RPsTerminateSystemThread)
			RPsTerminateSystemThread(STATUS_SUCCESS);
		else
			PsTerminateSystemThread(STATUS_SUCCESS); //o(∩_∩)o 
	} 
	return; //never be here 
} 
BOOL KillThread(PETHREAD Thread)
{
	PKAPC Apc=NULL; 
	BOOL blnSucceed=FALSE; 
	BOOL bInit = FALSE;

	RExAllocatePool = ReLoadNtosCALL(L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
	RExFreePool = ReLoadNtosCALL(L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
	RKeInitializeApc = ReLoadNtosCALL(L"KeInitializeApc",SystemKernelModuleBase,ImageModuleBase);
	RKeInsertQueueApc = ReLoadNtosCALL(L"KeInsertQueueApc",SystemKernelModuleBase,ImageModuleBase);
	RPsTerminateSystemThread = ReLoadNtosCALL(L"PsTerminateSystemThread",SystemKernelModuleBase,ImageModuleBase);
	if (RExAllocatePool &&
		RExFreePool &&
		RKeInitializeApc &&
		RKeInsertQueueApc)
	{
		bInit = TRUE;
	}
	if (!bInit)
		return blnSucceed;

	if (!RMmIsAddressValid(Thread)) return FALSE; //error. 

	Apc = RExAllocatePool(NonPagedPool,sizeof(KAPC)); 
	if (!Apc)
	{
		return blnSucceed; 
	}
	RKeInitializeApc(Apc, 
		Thread, 
		0, 
		KernelTerminateThreadRoutine, 
		NULL, 
		NULL, 
		KernelMode, 
		NULL); //special apc - whether alertable or not makes no difference.. 

	blnSucceed = RKeInsertQueueApc(Apc, 
		NULL, 
		NULL, 
		0); 
	//add some code works like KeForceResumeThread here. 
	return blnSucceed; 
}