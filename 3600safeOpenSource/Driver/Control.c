//**************************************************************************
//*
//*          A盾电脑防护 website:http://www.3600safe.com/
//*        
//*文件说明：
//*         负责跟ring3通信
//**************************************************************************
#include "Control.h"

NTSTATUS __stdcall NewNtReadFile(
	__in      HANDLE FileHandle,
	__in_opt  HANDLE Event,
	__in_opt  PIO_APC_ROUTINE ApcRoutine,
	__in_opt  PVOID ApcContext,
	__out     PIO_STATUS_BLOCK IoStatusBlock,
	__out     PVOID Buffer,
	__in      ULONG Length,
	__in_opt  PLARGE_INTEGER ByteOffset,
	__in_opt  PULONG Key
	)
{
	NTSTATUS status;
	ULONG ulSize;
	ULONG ulKeServiceDescriptorTable;
	int i=0,x=0;
	BOOL bInit = FALSE;
	WIN_VER_DETAIL WinVer;
	HANDLE HFileHandle;
	WCHAR lpwzKey[256];
	char *lpszProName = NULL;
	BOOL bIsNormalServices = FALSE;
	ULONG g_Offset_Eprocess_ProcessId;
	PVOID KernelBuffer;
	ULONG ulCsrssTemp;
	ULONG ulRealDispatch;
	CHAR lpszModule[256];
	ZWREADFILE OldZwReadFile;

	if (RMmIsAddressValid(OriginalServiceDescriptorTable->ServiceTable[ZwReadFileIndex]))
	{
		OldZwReadFile = OriginalServiceDescriptorTable->ServiceTable[ZwReadFileIndex];
	}else
		OldZwReadFile = KeServiceDescriptorTable->ServiceTable[ZwReadFileIndex];

	if (!IsExitProcess(CsrssEProcess))
	{
		lpszProName = (char *)PsGetProcessImageFileName(PsGetCurrentProcess());
		if (_stricmp(lpszProName,"csrss.exe") == 0)
		{
			CsrssEProcess = PsGetCurrentProcess();

			//保护自己
			if (!bProtect)
			{
				ProtectCode();
				bProtect = TRUE;
			}
		}
	}
	//判断如果调用的命令不是自己操作的，就直接返回吧
	if (bIsInitSuccess == TRUE)
	{
		if (IsExitProcess(ProtectEProcess))
		{
			if (RPsGetCurrentProcess)
			{
				if (RPsGetCurrentProcess() != ProtectEProcess)
				{
					goto _FunctionRet;
				}
			}
		}
	}

	//等待的时候暂停
	//SuspendThread(0x81c52020);

	//准备的时候恢复
	//ResumeThread(0x81c52020);

	//暂停
	if (FileHandle == SUSPEND_THREAD)
	{
		if (Length > 0x123456)
		{
			if (SuspendThread((PETHREAD)Length) == STATUS_SUCCESS)
			{
				KdPrint(("Suspend Thread:%08x",Length));
			}
		}
		return STATUS_UNSUCCESSFUL;
	}
	//恢复
	if (FileHandle == RESUME_THREAD)
	{
		if (Length > 0x123456)
		{
			if (ResumeThread((PETHREAD)Length) == STATUS_SUCCESS)
			{
				KdPrint(("ResumeThread Thread:%08x",Length));
			}
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == KERNEL_THREAD)
	{
		bDisKernelThread = TRUE;
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == DIS_KERNEL_THREAD)
	{
		bDisKernelThread = FALSE;
		return STATUS_UNSUCCESSFUL;
	}

	if (FileHandle == RESET_SRV)
	{
		bDisResetSrv = TRUE;
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == DIS_RESET_SRV)
	{
		bDisResetSrv = FALSE;
		return STATUS_UNSUCCESSFUL;
	}

	if (FileHandle == PROTECT_PROCESS)
	{
		bProtectProcess = TRUE;
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == UNPROTECT_PROCESS)
	{
		bProtectProcess = FALSE;
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == INIT_KILL_SYSTEM_NOTIFY)
	{
		IntNotify = Length;
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == KILL_SYSTEM_NOTIFY)
	{
		if (DebugOn)
			KdPrint(("Length:0x%08X IntNotify:%d\r\n",Length,IntNotify));
		RemoveNotifyRoutine(Length,IntNotify);
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == LIST_SYSTEM_NOTIFY)
	{
		RExAllocatePool = ReLoadNtosCALL(L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
		RExFreePool = ReLoadNtosCALL(L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
		Rmemcpy = ReLoadNtosCALL(L"memcpy",SystemKernelModuleBase,ImageModuleBase);
		if (RExAllocatePool &&
			RExFreePool &&
			Rmemcpy)
		{
			bInit = TRUE;
		}
		if (!bInit)
			return STATUS_UNSUCCESSFUL;

		SystemNotify = RExAllocatePool(PagedPool,(sizeof(SYSTEM_NOTIFY)+sizeof(SYSTEM_NOTIFY_INFORMATION))*1024);    //分配足够大的缓冲
		if (SystemNotify == NULL) 
		{
			return STATUS_UNSUCCESSFUL;
		}
		memset(SystemNotify,0,(sizeof(SYSTEM_NOTIFY)+sizeof(SYSTEM_NOTIFY_INFORMATION))*1024);
		QuerySystemNotify(PDriverObject,SystemNotify);

		if (DebugOn)
			KdPrint(("Length:%08x :%08x\r\n",Length,(sizeof(SYSTEM_NOTIFY)+sizeof(SYSTEM_NOTIFY_INFORMATION))*1024));

		if (Length >  (sizeof(SYSTEM_NOTIFY)+sizeof(SYSTEM_NOTIFY_INFORMATION))*1024)
		{
			for (i=0;i<SystemNotify->ulCount;i++)
			{
				if (DebugOn)
					KdPrint(("[%d]回调类型:%ws\r\n"
					"回调入口:%08X\r\n"
					"所在模块:%s\r\n"
					"对象:%ws\r\n\r\n",
					i,
					SystemNotify->NotifyInfo[i].lpwzType,
					SystemNotify->NotifyInfo[i].ulNotifyBase,
					SystemNotify->NotifyInfo[i].lpszModule,
					SystemNotify->NotifyInfo[i].lpwzObject));
			}
			status = OldZwReadFile(
				FileHandle,
				Event,
				ApcRoutine,
				ApcContext,
				IoStatusBlock,
				Buffer,
				Length,
				ByteOffset,
				Key
				);
			Rmemcpy(Buffer,SystemNotify,(sizeof(SYSTEM_NOTIFY)+sizeof(SYSTEM_NOTIFY_INFORMATION))*1024);
			Length =  (sizeof(SYSTEM_NOTIFY)+sizeof(SYSTEM_NOTIFY_INFORMATION))*1024;
		}
		RExFreePool(SystemNotify);
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == KILL_DPC_TIMER)
	{
		if (Length > 0x123456)
		{
			if (DebugOn)
				KdPrint(("Timer:0x%08X",Length));

			KillDcpTimer((PKTIMER)Length);
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == LIST_DPC_TIMER)
	{
		RExAllocatePool = ReLoadNtosCALL(L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
		RExFreePool = ReLoadNtosCALL(L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
		Rmemcpy = ReLoadNtosCALL(L"memcpy",SystemKernelModuleBase,ImageModuleBase);
		if (RExAllocatePool &&
			RExFreePool &&
			Rmemcpy)
		{
			bInit = TRUE;
		}
		if (!bInit)
			return STATUS_UNSUCCESSFUL;

		DpcTimer = RExAllocatePool(PagedPool,(sizeof(MyDpcTimer)+sizeof(MyDpcTimerInfo))*MAX_DPCTIMER_COUNT);    //分配足够大的缓冲
		if (DpcTimer == NULL) 
		{
			return STATUS_UNSUCCESSFUL;
		}
		memset(DpcTimer,0,(sizeof(MyDpcTimer)+sizeof(MyDpcTimerInfo))*MAX_DPCTIMER_COUNT);
		WinVer = GetWindowsVersion();
		switch (WinVer)
		{
		case WINDOWS_VERSION_2K3_SP1_SP2:
		case WINDOWS_VERSION_XP:
			GetDpcTimerInformation_XP_2K3(DpcTimer);
			break;
		case WINDOWS_VERSION_7:
			GetDpcTimerInformation_WIN7(DpcTimer);
			break;
		}
		
		if (Length >  (sizeof(MyDpcTimer)+sizeof(MyDpcTimerInfo))*MAX_DPCTIMER_COUNT)
		{
			for (i=0;i<DpcTimer->ulCount;i++)
			{
				if (DebugOn)
					KdPrint(("[%d]定时器对象:%08x\r\n"
					"触发周期:%d\r\n"
					"函数入口:%08x\r\n"
					"函数入口所在模块:%s\r\n"
					"DPC结构地址:%08x\r\n",
					i,
					DpcTimer->MyTimer[i].TimerAddress,
					DpcTimer->MyTimer[i].Period,
					DpcTimer->MyTimer[i].DpcRoutineAddress,
					DpcTimer->MyTimer[i].lpszModule,
					DpcTimer->MyTimer[i].DpcAddress));
			}
			status = OldZwReadFile(
				FileHandle,
				Event,
				ApcRoutine,
				ApcContext,
				IoStatusBlock,
				Buffer,
				Length,
				ByteOffset,
				Key
				);
			Rmemcpy(Buffer,DpcTimer,(sizeof(MyDpcTimer)+sizeof(MyDpcTimerInfo))*MAX_DPCTIMER_COUNT);
			Length =  (sizeof(MyDpcTimer)+sizeof(MyDpcTimerInfo))*MAX_DPCTIMER_COUNT;
		}
		RExFreePool(DpcTimer);
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == INIT_KERNEL_FILTER_DRIVER)
	{
		if (Length > 0 &&
			Buffer != 0)
		{
			memset(lpwzFilter,0,sizeof(lpwzFilter));
			wcsncat(lpwzFilter,Buffer,Length);
			if (DebugOn)
				KdPrint(("lpwzFilter:%ws",lpwzFilter));
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == DELETE_KERNEL_FILTER_DRIVER)
	{
		if (Length > 0x123456)
		{
			ulDeviceObject = Length;
			if (DebugOn)
				KdPrint(("ulDeviceObject:%08X",ulDeviceObject));

			ClearFilters(lpwzFilter,ulDeviceObject);
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == CLEAR_KERNEL_THREAD)
	{
		RExAllocatePool = ReLoadNtosCALL(L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
		RExFreePool = ReLoadNtosCALL(L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
		if (RExAllocatePool &&
			RExFreePool)
		{
			bInit = TRUE;
		}
		if (!bInit)
			return STATUS_UNSUCCESSFUL;

		bIsInitSuccess = FALSE;   //暂停所有

		if (KernelThread)
			RExFreePool(KernelThread);

		ThreadCount = 0;
		KernelThread = (PKERNEL_THREAD_INFO)RExAllocatePool(NonPagedPool,(sizeof(KERNEL_THREAD_INFO)+sizeof(KERNEL_INFO))*1024);
		if (!KernelThread)
		{
			if (DebugOn)
				KdPrint(("KernelThread failed"));
			return STATUS_UNSUCCESSFUL;
		}
		memset(KernelThread,0,(sizeof(KERNEL_THREAD_INFO)+sizeof(KERNEL_INFO))*1024);

		bIsInitSuccess = TRUE;   //恢复

		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == LIST_KERNEL_THREAD)
	{
		if (Length >  (sizeof(KERNEL_THREAD_INFO)+sizeof(KERNEL_INFO))*1024)
		{
			if (DebugOn)
				KdPrint(("Length:%08x-%08x",Length,(sizeof(KERNEL_THREAD_INFO)+sizeof(KERNEL_INFO))*1024));
			for (i=0;i<ThreadCount;i++)
			{
				if (RMmIsAddressValid(KernelThread->KernelThreadInfo[i].ThreadStart))
				{
					if (DebugOn)
						KdPrint(("ThreadStart:%08x",KernelThread->KernelThreadInfo[i].ThreadStart));

					memset(lpszModule,0,sizeof(lpszModule));
					if (!IsAddressInSystem(
						KernelThread->KernelThreadInfo[i].ThreadStart,
						&ulThreadModuleBase,
						&ulThreadModuleSize,
						lpszModule))
					{
						KernelThread->KernelThreadInfo[i].ulHideType = 1;  //隐藏线程
					}
					if (DebugOn)
						KdPrint(("Hided:%08x:%s",KernelThread->KernelThreadInfo[i].ThreadStart,lpszModule));
				}else
				{
					KernelThread->KernelThreadInfo[i].ulStatus = 1;   //线程退出
				}
			}
			KernelThread->ulCount = ThreadCount;
			if (DebugOn)
				KdPrint(("ThreadCount:%d",KernelThread->ulCount));

			status = OldZwReadFile(
				FileHandle,
				Event,
				ApcRoutine,
				ApcContext,
				IoStatusBlock,
				Buffer,
				Length,
				ByteOffset,
				Key
				);
			Rmemcpy(Buffer,KernelThread,(sizeof(KERNEL_THREAD_INFO)+sizeof(KERNEL_INFO))*1024);
			Length =  (sizeof(KERNEL_THREAD_INFO)+sizeof(KERNEL_INFO))*1024;
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == INIT_PROCESS_THREAD)
	{
		if (RMmIsAddressValid(Length))
		{
			TempEProcess = (PEPROCESS)Length;
			KdPrint(("TempEProcess:%08x success",TempEProcess));
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == KILL_SYSTEM_THREAD)
	{
		if (Length > 0x123456)
		{
			if (KillThread((PETHREAD)Length))
			{
				if (DebugOn)
					KdPrint(("Kill ETHREAD:%08x success",Length));
			}
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == LIST_SYSTEM_THREAD ||
		FileHandle == LIST_PROCESS_THREAD)
	{
		RExAllocatePool = ReLoadNtosCALL(L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
		RExFreePool = ReLoadNtosCALL(L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
		Rmemcpy = ReLoadNtosCALL(L"memcpy",SystemKernelModuleBase,ImageModuleBase);
		if (RExAllocatePool &&
			RExFreePool &&
			Rmemcpy)
		{
			bInit = TRUE;
		}
		if (!bInit)
			return STATUS_UNSUCCESSFUL;

		SystemThread = (PSYSTEM_THREAD_INFO)RExAllocatePool(NonPagedPool,(sizeof(SYSTEM_THREAD_INFO)+sizeof(THREAD_INFO))*256);
		if (!SystemThread)
		{
			if (DebugOn)
				KdPrint(("SystemThread failed"));
			return STATUS_UNSUCCESSFUL;
		}
		memset(SystemThread,0,(sizeof(SYSTEM_THREAD_INFO)+sizeof(THREAD_INFO))*256);
		if (FileHandle == LIST_PROCESS_THREAD)
		{
			QuerySystemThread(SystemThread,TempEProcess);
		}else
			QuerySystemThread(SystemThread,SystemEProcess);

		if (Length >  (sizeof(SYSTEM_THREAD_INFO)+sizeof(THREAD_INFO))*256)
		{
			status = OldZwReadFile(
				FileHandle,
				Event,
				ApcRoutine,
				ApcContext,
				IoStatusBlock,
				Buffer,
				Length,
				ByteOffset,
				Key
				);
			Rmemcpy(Buffer,SystemThread,(sizeof(SYSTEM_THREAD_INFO)+sizeof(THREAD_INFO))*256);
			Length =  (sizeof(SYSTEM_THREAD_INFO)+sizeof(THREAD_INFO))*256;
		}
		RExFreePool(SystemThread);
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == LIST_KERNEL_FILTER_DRIVER)
	{
		RExAllocatePool = ReLoadNtosCALL(L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
		RExFreePool = ReLoadNtosCALL(L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
		Rmemcpy = ReLoadNtosCALL(L"memcpy",SystemKernelModuleBase,ImageModuleBase);
		if (RExAllocatePool &&
			RExFreePool &&
			Rmemcpy)
		{
			bInit = TRUE;
		}
		if (!bInit)
			return STATUS_UNSUCCESSFUL;

		KernelFilterDriver = (PKERNEL_FILTERDRIVER)RExAllocatePool(NonPagedPool,(sizeof(KERNEL_FILTERDRIVER)+sizeof(KERNEL_FILTERDRIVER_INFO))*1024);
		if (!KernelFilterDriver)
		{
			if (DebugOn)
				KdPrint(("KernelFilterDriver failed"));
			return STATUS_UNSUCCESSFUL;
		}
		memset(KernelFilterDriver,0,(sizeof(KERNEL_FILTERDRIVER)+sizeof(KERNEL_FILTERDRIVER_INFO))*1024);
		if (KernelFilterDriverEnum(KernelFilterDriver) == STATUS_SUCCESS)
		{
			if (DebugOn)
				KdPrint(("KernelFilterDriverEnum STATUS_SUCCESS"));
			if (Length >  (sizeof(KERNEL_FILTERDRIVER)+sizeof(KERNEL_FILTERDRIVER_INFO))*1024)
			{
				for (i=0;i<KernelFilterDriver->ulCount;i++)
				{
					if (DebugOn)
						KdPrint(("[%d]过滤驱动\r\n"
						"类型:%08X\r\n" 
						"驱动对象名:%ws\r\n"
						"驱动路径:%ws\r\n"
						"设备地址:%08X\r\n"
						"宿主驱动对象名:%ws\r\n\r\n",
						i,
						KernelFilterDriver->KernelFilterDriverInfo[i].ulObjType,
						KernelFilterDriver->KernelFilterDriverInfo[i].FileName,
						KernelFilterDriver->KernelFilterDriverInfo[i].FilePath,
						KernelFilterDriver->KernelFilterDriverInfo[i].ulAttachDevice,
						KernelFilterDriver->KernelFilterDriverInfo[i].HostFileName));
				}
				status = OldZwReadFile(
					FileHandle,
					Event,
					ApcRoutine,
					ApcContext,
					IoStatusBlock,
					Buffer,
					Length,
					ByteOffset,
					Key
					);
				Rmemcpy(Buffer,KernelFilterDriver, (sizeof(KERNEL_FILTERDRIVER)+sizeof(KERNEL_FILTERDRIVER_INFO))*1024);
				Length =  (sizeof(KERNEL_FILTERDRIVER)+sizeof(KERNEL_FILTERDRIVER_INFO))*1024;
			}
		}
		RExFreePool(KernelFilterDriver);
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == PROTECT_360SAFE)
	{
		bIsProtect360 = TRUE;
		//Fix360Hook(bIsProtect360);
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == UNPROTECT_360SAFE)
	{
		bIsProtect360 = FALSE;
		//Fix360Hook(bIsProtect360);
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == LIST_OBJECT_HOOK)
	{
		RExAllocatePool = ReLoadNtosCALL(L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
		RExFreePool = ReLoadNtosCALL(L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
		Rmemcpy = ReLoadNtosCALL(L"memcpy",SystemKernelModuleBase,ImageModuleBase);
		if (RExAllocatePool &&
			RExFreePool &&
			Rmemcpy)
		{
			bInit = TRUE;
		}
		if (!bInit)
			return STATUS_UNSUCCESSFUL;

		ObjectHookInfo = (POBJECTHOOKINFO)RExAllocatePool(NonPagedPool, (sizeof(OBJECTHOOKINFO)+sizeof(OBJECTHOOKINFO_INFORMATION))*1024);
		if (!ObjectHookInfo)
		{
			//KdPrint(("ObjectHookInfo failed"));
			return STATUS_UNSUCCESSFUL;
		}
		memset(ObjectHookInfo,0, (sizeof(OBJECTHOOKINFO)+sizeof(OBJECTHOOKINFO_INFORMATION))*1024);
		IoFileObjectTypeHookInfo(ObjectHookInfo);
		IoDeviceObjectTypeHookInfo(ObjectHookInfo);
		IoDriverObjectTypeHookInfo(ObjectHookInfo);
		CmpKeyObjectTypeHookInfo(ObjectHookInfo);

		if (DebugOn)
			KdPrint(("Length:%08x-ObjectHookInfo:%08x",Length, (sizeof(OBJECTHOOKINFO)+sizeof(OBJECTHOOKINFO_INFORMATION))*1024));

		if (Length >  (sizeof(OBJECTHOOKINFO)+sizeof(OBJECTHOOKINFO_INFORMATION))*1024)
		{
			for (i=0;i<ObjectHookInfo->ulCount;i++)
			{
				if (DebugOn)
				   KdPrint(("[%d]ObjectHook\r\n"
					"当前函数地址:%08X\r\n"
					"原始函数地址:%08X\r\n"
					"函数名:%s\r\n"
					"所在模块:%s\r\n"
					"ObjectType地址:%08X\r\n"
					"hook类型:%d\r\n"
					"objectType类型:%s\r\n",
					i,
					ObjectHookInfo->ObjectHook[i].ulMemoryHookBase,
					ObjectHookInfo->ObjectHook[i].ulMemoryFunctionBase,
					ObjectHookInfo->ObjectHook[i].lpszFunction,
					ObjectHookInfo->ObjectHook[i].lpszHookModuleImage,
					ObjectHookInfo->ObjectHook[i].ulObjectTypeBase,
					ObjectHookInfo->ObjectHook[i].ulHookType,
					ObjectHookInfo->ObjectHook[i].lpszObjectTypeName
					));
			}
			status = OldZwReadFile(
				FileHandle,
				Event,
				ApcRoutine,
				ApcContext,
				IoStatusBlock,
				Buffer,
				Length,
				ByteOffset,
				Key
				);
			Rmemcpy(Buffer,ObjectHookInfo, (sizeof(OBJECTHOOKINFO)+sizeof(OBJECTHOOKINFO_INFORMATION))*1024);
			Length =  (sizeof(OBJECTHOOKINFO)+sizeof(OBJECTHOOKINFO_INFORMATION))*1024;
		}
		RExFreePool(ObjectHookInfo);
	}
	if (FileHandle == SET_SHADOWSSDT_INLINE_HOOK)
	{
		//传入服务号
		if (Length > 0 ||
			Length == 0)
		{
			if (IsExitProcess(ProtectEProcess))
			{
				RKeAttachProcess = ReLoadNtosCALL(L"KeAttachProcess",SystemKernelModuleBase,ImageModuleBase);
				RKeDetachProcess = ReLoadNtosCALL(L"KeDetachProcess",SystemKernelModuleBase,ImageModuleBase);
				if (RKeAttachProcess &&
					RKeDetachProcess)
				{
					RKeAttachProcess(ProtectEProcess);
					RestoreShadowInlineHook(Length);
					RKeDetachProcess();
				}
			}
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == SET_ONE_SHADOWSSDT)
	{
		//传入服务号
		if (Length > 0 ||
			Length == 0)
		{
			if (IsExitProcess(ProtectEProcess))
			{
				RKeAttachProcess = ReLoadNtosCALL(L"KeAttachProcess",SystemKernelModuleBase,ImageModuleBase);
				RKeDetachProcess = ReLoadNtosCALL(L"KeDetachProcess",SystemKernelModuleBase,ImageModuleBase);
				if (RKeAttachProcess &&
					RKeDetachProcess)
				{
					RKeAttachProcess(ProtectEProcess);
					RestoreAllShadowSSDTFunction(Length);
					RKeDetachProcess();
				}
			}
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == SET_ALL_SHADOWSSDT)
	{
		if (IsExitProcess(ProtectEProcess))
		{
			RKeAttachProcess = ReLoadNtosCALL(L"KeAttachProcess",SystemKernelModuleBase,ImageModuleBase);
			RKeDetachProcess = ReLoadNtosCALL(L"KeDetachProcess",SystemKernelModuleBase,ImageModuleBase);
			if (RKeAttachProcess &&
				RKeDetachProcess)
			{
				RKeAttachProcess(ProtectEProcess);
				RestoreAllShadowSSDTFunction(8888);
				RKeDetachProcess();
			}
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == LIST_SHADOWSSDT ||
		FileHandle == LIST_SHADOWSSDT_ALL)
	{
		if (FileHandle == LIST_SHADOWSSDT_ALL)
		{
			//KdPrint(("Print SSDT All"));
			bShadowSSDTAll = TRUE;
		}
		if (FileHandle == LIST_SHADOWSSDT)
		{
			//KdPrint(("Print SSDT"));
		}
		RExAllocatePool = ReLoadNtosCALL(L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
		RExFreePool = ReLoadNtosCALL(L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
		Rmemcpy = ReLoadNtosCALL(L"memcpy",SystemKernelModuleBase,ImageModuleBase);
		if (RExAllocatePool &&
			RExFreePool &&
			Rmemcpy)
		{
			bInit = TRUE;
		}
		if (!bInit)
			return STATUS_UNSUCCESSFUL;

		ShadowSSDTInfo = (PSHADOWSSDTINFO)RExAllocatePool(NonPagedPool,(sizeof(SHADOWSSDTINFO)+sizeof(SHADOWSSDTINFO_INFORMATION))*900);
		if (!ShadowSSDTInfo)
		{
			if (DebugOn)
				KdPrint(("ShadowSSDTInfo failed:%08x\r\n",(sizeof(SHADOWSSDTINFO)+sizeof(SHADOWSSDTINFO_INFORMATION))*900));
			return STATUS_UNSUCCESSFUL;
		}
		memset(ShadowSSDTInfo,0,(sizeof(SHADOWSSDTINFO)+sizeof(SHADOWSSDTINFO_INFORMATION))*900);
		if (IsExitProcess(ProtectEProcess))
		{
			RKeAttachProcess = ReLoadNtosCALL(L"KeAttachProcess",SystemKernelModuleBase,ImageModuleBase);
			RKeDetachProcess = ReLoadNtosCALL(L"KeDetachProcess",SystemKernelModuleBase,ImageModuleBase);
			if (RKeAttachProcess &&
				RKeDetachProcess)
			{
				RKeAttachProcess(ProtectEProcess);
				ShadowSSDTHookCheck(ShadowSSDTInfo);
				RKeDetachProcess();
				if (DebugOn)
					KdPrint(("Length:%08x-ShadowSSDTInfo:%08x\r\n",Length,(sizeof(SHADOWSSDTINFO)+sizeof(SHADOWSSDTINFO_INFORMATION))*900));

				if (Length > (sizeof(SHADOWSSDTINFO)+sizeof(SHADOWSSDTINFO_INFORMATION))*900)
				{
					for (i=0;i<ShadowSSDTInfo->ulCount;i++)
					{
						if (DebugOn)
							KdPrint(("[%d]发现ShadowSSDT hook\r\n"
							"服务号:%d\r\n"
							"当前地址:%08x\r\n"
							"函数名字:%s\r\n"
							"当前hook模块:%s\r\n"
							"当前模块地址:%08x\r\n"
							"当前模块大小:%d KB\r\n"
							"Hook类型:%d\r\n\r\n",
							i,
							ShadowSSDTInfo->SSDT[i].ulNumber,
							ShadowSSDTInfo->SSDT[i].ulMemoryFunctionBase,
							ShadowSSDTInfo->SSDT[i].lpszFunction,
							ShadowSSDTInfo->SSDT[i].lpszHookModuleImage,
							ShadowSSDTInfo->SSDT[i].ulHookModuleBase,
							ShadowSSDTInfo->SSDT[i].ulHookModuleSize/1024,
							ShadowSSDTInfo->SSDT[i].IntHookType));
					}
					status = OldZwReadFile(
						FileHandle,
						Event,
						ApcRoutine,
						ApcContext,
						IoStatusBlock,
						Buffer,
						Length,
						ByteOffset,
						Key
						);
					Rmemcpy(Buffer,ShadowSSDTInfo,(sizeof(SHADOWSSDTINFO)+sizeof(SHADOWSSDTINFO_INFORMATION))*900);
					Length = (sizeof(SHADOWSSDTINFO)+sizeof(SHADOWSSDTINFO_INFORMATION))*900;
				}
			}
		}
		bShadowSSDTAll = FALSE;
		RExFreePool(ShadowSSDTInfo);
		return STATUS_UNSUCCESSFUL;
	}
	//强制重启
	if (FileHandle == SHUT_DOWN_SYSTEM)
	{
		KeBugCheck(POWER_FAILURE_SIMULATE);
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == LOAD_DRIVER)
	{
		if (bDisLoadDriver == FALSE)
		{
			bDisLoadDriver = TRUE;
			EnableDriverLoading();   //允许加载驱动
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == DIS_LOAD_DRIVER)
	{
		if (bDisLoadDriver == TRUE)
		{
			bDisLoadDriver = FALSE;
			DisEnableDriverLoading();    //禁止加载驱动
		}
		return STATUS_UNSUCCESSFUL;
	}
	//---------------------------------------------
	if (FileHandle == WRITE_FILE)
	{
		bDisWriteFile = TRUE;
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == DIS_WRITE_FILE)
	{
		bDisWriteFile = FALSE;
		return STATUS_UNSUCCESSFUL;
	}
	//----------------------------------------------------
	if (FileHandle == CREATE_PROCESS)
	{
		bDisCreateProcess = TRUE;
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == DIS_CREATE_PROCESS)
	{
		bDisCreateProcess = FALSE;
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == DUMP_KERNEL_MODULE_MEMORY)
	{
		RExAllocatePool = ReLoadNtosCALL(L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
		RExFreePool = ReLoadNtosCALL(L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
		Rmemcpy = ReLoadNtosCALL(L"memcpy",SystemKernelModuleBase,ImageModuleBase);
		if (RExAllocatePool &&
			RExFreePool &&
			Rmemcpy)
		{
			bInit = TRUE;
		}
		if (!bInit)
			return STATUS_UNSUCCESSFUL;

		if (Buffer != NULL &&
			Length > 0)
		{
			//KdPrint(("savefile:%ws",Buffer));

			KernelBuffer = RExAllocatePool(NonPagedPool,ulDumpKernelSize+0x100); //申请一样的内存
			if (KernelBuffer)
			{
				memset(KernelBuffer,0,ulDumpKernelSize);
				if (RMmIsAddressValid(ulDumpKernelBase))
				{
					if (DumpMemory((PVOID)ulDumpKernelBase,KernelBuffer,ulDumpKernelSize) == STATUS_SUCCESS)
					{
						DebugWriteToFile(Buffer,KernelBuffer,ulDumpKernelSize);

						if (DebugOn)
							KdPrint(("DumpKernel success"));
					}
				}
				RExFreePool(KernelBuffer);
			}
		}
	}
	if (FileHandle == INIT_DUMP_KERNEL_MODULE_MEMORY)
	{
		if (Buffer != NULL &&
			Length > 0x123456)
		{
			ulDumpKernelBase = Length;  //初始化
			ulDumpKernelSize = atoi(Buffer);
			if (DebugOn)
				KdPrint(("ulDumpKernelBase:%08x\nulDumpKernelSize:%08x",ulDumpKernelBase,ulDumpKernelSize));
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == CLEAR_LIST_LOG)
	{
		RExAllocatePool = ReLoadNtosCALL(L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
		RExFreePool = ReLoadNtosCALL(L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
		if (RExAllocatePool &&
			RExFreePool)
		{
			bInit = TRUE;
		}
		if (!bInit)
			return STATUS_UNSUCCESSFUL;

		bIsInitSuccess = FALSE;   // 暂停所有

		if (LogDefenseInfo)
			RExFreePool(LogDefenseInfo);

		ulLogCount = 0;
		LogDefenseInfo = (PLOGDEFENSE)RExAllocatePool(NonPagedPool,(sizeof(LOGDEFENSE)+sizeof(LOGDEFENSE_INFORMATION))*1024);
		if (!LogDefenseInfo)
		{
			return STATUS_UNSUCCESSFUL;
		}
		memset(LogDefenseInfo,0,(sizeof(LOGDEFENSE)+sizeof(LOGDEFENSE_INFORMATION))*1024);

		bIsInitSuccess = TRUE;   //恢复

		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == LIST_LOG)
	{
		WinVer = GetWindowsVersion();
		switch(WinVer)
		{
		case WINDOWS_VERSION_XP:
			g_Offset_Eprocess_ProcessId = 0x84;
			break;
		case WINDOWS_VERSION_7:
			g_Offset_Eprocess_ProcessId = 0xb4;
			break;
		case WINDOWS_VERSION_2K3_SP1_SP2:
			g_Offset_Eprocess_ProcessId = 0x94;
			break;
		}
		
		if (DebugOn)
			KdPrint(("Length:%x %x",Length,(sizeof(LOGDEFENSE)+sizeof(LOGDEFENSE_INFORMATION))*1024));

		if (Length > (sizeof(LOGDEFENSE)+sizeof(LOGDEFENSE_INFORMATION))*1024)
		{
			__try
			{
				for (i=0;i<ulLogCount;i++)
				{
					if (LogDefenseInfo->LogDefense[i].ulPID)
					{
						if (!IsExitProcess(LogDefenseInfo->LogDefense[i].EProcess))
						{
							memset(LogDefenseInfo->LogDefense[i].lpszProName,0,sizeof(LogDefenseInfo->LogDefense[i].lpszProName));
							strcat(LogDefenseInfo->LogDefense[i].lpszProName,"Unknown");

							LogDefenseInfo->LogDefense[i].ulPID = 0;

						}else
						{
							memset(LogDefenseInfo->LogDefense[i].lpszProName,0,sizeof(LogDefenseInfo->LogDefense[i].lpszProName));

							lpszProName = (CHAR *)PsGetProcessImageFileName((PEPROCESS)LogDefenseInfo->LogDefense[i].EProcess);
							if (lpszProName)
							{
								strcat(LogDefenseInfo->LogDefense[i].lpszProName,lpszProName);
							}
							lpszProName = NULL;
							LogDefenseInfo->LogDefense[i].ulInheritedFromProcessId = GetInheritedProcessPid((PEPROCESS)LogDefenseInfo->LogDefense[i].ulPID);
						}
					    LogDefenseInfo->ulCount = ulLogCount;
					}
				}
				if (DebugOn)
					KdPrint(("ulLogCount:%d",LogDefenseInfo->ulCount));

			}__except(EXCEPTION_EXECUTE_HANDLER){
				if (DebugOn)
					KdPrint(("[EXCEPTION_EXECUTE_HANDLER]ulLogCount:%d,%d:%ws",LogDefenseInfo->ulCount,ulLogCount,LogDefenseInfo->LogDefense[i].lpwzCreateProcess));

				status = OldZwReadFile(
					FileHandle,
					Event,
					ApcRoutine,
					ApcContext,
					IoStatusBlock,
					Buffer,
					Length,
					ByteOffset,
					Key
					);
				Rmemcpy(Buffer,LogDefenseInfo,(sizeof(LOGDEFENSE)+sizeof(LOGDEFENSE_INFORMATION))*1024);
				Length = (sizeof(LOGDEFENSE)+sizeof(LOGDEFENSE_INFORMATION))*1024;
				return STATUS_UNSUCCESSFUL;
			}
			status = OldZwReadFile(
				FileHandle,
				Event,
				ApcRoutine,
				ApcContext,
				IoStatusBlock,
				Buffer,
				Length,
				ByteOffset,
				Key
				);
			Rmemcpy(Buffer,LogDefenseInfo,(sizeof(LOGDEFENSE)+sizeof(LOGDEFENSE_INFORMATION))*1024);
			Length = (sizeof(LOGDEFENSE)+sizeof(LOGDEFENSE_INFORMATION))*1024;
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == CHANG_SERVICES_TYPE_1 ||
		FileHandle == CHANG_SERVICES_TYPE_2 ||
		FileHandle == CHANG_SERVICES_TYPE_3)
	{
		if (Buffer != 0 &&
			Length > 0)
		{
			memset(lpwzKey,0,sizeof(lpwzKey));
			wcscat(lpwzKey,L"\\Registry\\Machine\\SYSTEM\\CurrentControlSet\\Services\\");
			wcscat(lpwzKey,Buffer);

			switch ((ULONG)FileHandle)
			{
			case CHANG_SERVICES_TYPE_1:  //手动
				Safe_CreateValueKey(lpwzKey,REG_DWORD,L"Start",0x3);
				break;
			case CHANG_SERVICES_TYPE_2:  //自动
				Safe_CreateValueKey(lpwzKey,REG_DWORD,L"Start",0x2);
				break;
			case CHANG_SERVICES_TYPE_3:  //禁用
				Safe_CreateValueKey(lpwzKey,REG_DWORD,L"Start",0x4);
				break;
			}
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == LIST_DEPTH_SERVICES)
	{
		if (DepthServicesRegistry == NULL)
		{
			if (DebugOn)
				KdPrint(("DepthServicesRegistry is NULL"));
			return STATUS_UNSUCCESSFUL;
		}
		if (!RMmIsAddressValid(DepthServicesRegistry))
		{
			if (DebugOn)
				KdPrint(("RMmIsAddressValid!!!"));
			return STATUS_UNSUCCESSFUL;
		}
		
		if (DebugOn)
			KdPrint(("Length:%08x--DepthServicesRegistry:%08x",Length,(sizeof(SERVICESREGISTRY)+sizeof(SERVICESREGISTRY_INFORMATION))*1024));
		if (Length > (sizeof(SERVICESREGISTRY)+sizeof(SERVICESREGISTRY_INFORMATION))*1024)
		{
			if (DepthServicesRegistry->ulCount)
			{
				for (i=0;i<DepthServicesRegistry->ulCount;i++)
				{
					if (DebugOn)
						KdPrint(("[%d]深度服务查看\r\n"
						"服务名:%ws\r\n"
						"映像路径:%ws\r\n"
						"动态链接库:%ws\r\n\r\n",
						i,
						DepthServicesRegistry->SrvReg[i].lpwzSrvName,
						DepthServicesRegistry->SrvReg[i].lpwzImageName,
						DepthServicesRegistry->SrvReg[i].lpwzDLLPath
						));
				}
				status = OldZwReadFile(
					FileHandle,
					Event,
					ApcRoutine,
					ApcContext,
					IoStatusBlock,
					Buffer,
					Length,
					ByteOffset,
					Key
					);
				Rmemcpy(Buffer,DepthServicesRegistry,(sizeof(SERVICESREGISTRY)+sizeof(SERVICESREGISTRY_INFORMATION))*1024);
				Length = (sizeof(SERVICESREGISTRY)+sizeof(SERVICESREGISTRY_INFORMATION))*1024;
			}
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == LIST_SERVICES)
	{
		Rmemcpy = ReLoadNtosCALL(L"memcpy",SystemKernelModuleBase,ImageModuleBase);
		RExAllocatePool = ReLoadNtosCALL(L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
		RExFreePool = ReLoadNtosCALL(L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
		if (RExAllocatePool &&
			RExFreePool &&
			Rmemcpy)
		{
			bInit = TRUE;
		}
		if (!bInit)
			return STATUS_UNSUCCESSFUL;

		ServicesRegistry = (PSERVICESREGISTRY)RExAllocatePool(NonPagedPool,(sizeof(SERVICESREGISTRY)+sizeof(SERVICESREGISTRY_INFORMATION))*1024);
		if (!ServicesRegistry)
		{
			if (DebugOn)
				KdPrint(("RExAllocatePool !!"));
			return STATUS_UNSUCCESSFUL;
		}
		if (DebugOn)
			KdPrint(("search !!"));
		memset(ServicesRegistry,0,(sizeof(SERVICESREGISTRY)+sizeof(SERVICESREGISTRY_INFORMATION))*1024);
		if (QueryServicesRegistry(ServicesRegistry) == STATUS_SUCCESS)
		{
			if (DebugOn)
				KdPrint(("Length:%08x-ServicesRegistry:%08x",Length,(sizeof(SERVICESREGISTRY)+sizeof(SERVICESREGISTRY_INFORMATION))*1024));
			if (Length > (sizeof(SERVICESREGISTRY)+sizeof(SERVICESREGISTRY_INFORMATION))*1024)
			{
				if (DebugOn)
					KdPrint(("Length !!"));

				for (i=0;i<ServicesRegistry->ulCount;i++)
				{
					if (DepthServicesRegistry)
					{
						for (x=0;x<DepthServicesRegistry->ulCount;x++)
						{
							bIsNormalServices = FALSE;

							if (_wcsnicmp(ServicesRegistry->SrvReg[i].lpwzSrvName,DepthServicesRegistry->SrvReg[x].lpwzSrvName,wcslen(DepthServicesRegistry->SrvReg[x].lpwzSrvName)) == 0)
							{
								bIsNormalServices = TRUE;
								break;
							}
						}
						//服务不正常
						if (!bIsNormalServices)
						{
							wcscat(ServicesRegistry->SrvReg[i].lpwzSrvName,L"(最新创建)");
						}
					}
					if (DebugOn)
						KdPrint(("[%d]服务查看\r\n"
						"服务名:%ws\r\n"
						"映像路径:%ws\r\n"
						"动态链接库:%ws\r\n\r\n",
						i,
						ServicesRegistry->SrvReg[i].lpwzSrvName,
						ServicesRegistry->SrvReg[i].lpwzImageName,
						ServicesRegistry->SrvReg[i].lpwzDLLPath
						));
				}
				status = OldZwReadFile(
					FileHandle,
					Event,
					ApcRoutine,
					ApcContext,
					IoStatusBlock,
					Buffer,
					Length,
					ByteOffset,
					Key
					);
				Rmemcpy(Buffer,ServicesRegistry,(sizeof(SERVICESREGISTRY)+sizeof(SERVICESREGISTRY_INFORMATION))*1024);
				Length = (sizeof(SERVICESREGISTRY)+sizeof(SERVICESREGISTRY_INFORMATION))*1024;
			}
		}
		RExFreePool(ServicesRegistry);
		return STATUS_UNSUCCESSFUL;
	}
	//随便赋值，让进程顺利exit~！
	if (FileHandle == EXIT_PROCESS)
	{
		bIsInitSuccess = FALSE;   //恢复标记
		ProtectEProcess = 0x12345678;

		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == LIST_SYS_MODULE)
	{
		Rmemcpy = ReLoadNtosCALL(L"memcpy",SystemKernelModuleBase,ImageModuleBase);
		RExAllocatePool = ReLoadNtosCALL(L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
		RExFreePool = ReLoadNtosCALL(L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
		if (RExAllocatePool &&
			RExFreePool &&
			Rmemcpy)
		{
			bInit = TRUE;
		}
		if (!bInit)
			return STATUS_UNSUCCESSFUL;

		SysModuleInfo = (PSYSINFO)RExAllocatePool(NonPagedPool,(sizeof(SYSINFO)+sizeof(SYSINFO_INFORMATION))*800);
		if (!SysModuleInfo)
		{
			return STATUS_UNSUCCESSFUL;
		}
		memset(SysModuleInfo,0,(sizeof(SYSINFO)+sizeof(SYSINFO_INFORMATION))*800);
		EnumKernelModule(PDriverObject,SysModuleInfo);
		if (Length > (sizeof(SYSINFO)+sizeof(SYSINFO_INFORMATION))*800)
		{
			for (i=0;i<SysModuleInfo->ulCount;i++)
			{
				if (DebugOn)
					KdPrint(("[%d]SysModule\r\n"
					"基址:%08x\r\n"
					"大小:%x\r\n"
					"驱动名:%ws\r\n"
					"驱动路径:%ws\r\n"
					"加载顺序:%08x\r\n"
					"隐藏类型:%x\r\n",
					i,
					SysModuleInfo->SysInfo[i].ulSysBase,
					SysModuleInfo->SysInfo[i].SizeOfImage,
					SysModuleInfo->SysInfo[i].lpwzBaseSysName,
					SysModuleInfo->SysInfo[i].lpwzFullSysName,
					SysModuleInfo->SysInfo[i].LoadCount,
					SysModuleInfo->SysInfo[i].IntHideType
					));
			}
			status = OldZwReadFile(
				FileHandle,
				Event,
				ApcRoutine,
				ApcContext,
				IoStatusBlock,
				Buffer,
				Length,
				ByteOffset,
				Key
				);
			Rmemcpy(Buffer,SysModuleInfo,(sizeof(SYSINFO)+sizeof(SYSINFO_INFORMATION))*800);
			Length = (sizeof(SYSINFO)+sizeof(SYSINFO_INFORMATION))*800;
		}
		RExFreePool(SysModuleInfo);
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == NO_KERNEL_SAFE_MODULE)
	{
		bKernelSafeModule = FALSE;  //关闭内核安全模式
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == KERNEL_SAFE_MODULE)
	{
		bKernelSafeModule = TRUE;  //开启内核安全模式，让系统不存在任何的hook
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == SET_EAT_HOOK)
	{
		if (Buffer != 0 &&
			Length > 0)
		{
			ReSetEatHook(Buffer,ImageModuleBase,SystemKernelModuleBase);
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == ANTI_INLINEHOOK)
	{
		if (Buffer != 0 &&
			Length > 0)
		{
			AntiInlineHook(Buffer);
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == LIST_INLINEHOOK)
	{
		Rmemcpy = ReLoadNtosCALL(L"memcpy",SystemKernelModuleBase,ImageModuleBase);
		RExAllocatePool = ReLoadNtosCALL(L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
		RExFreePool = ReLoadNtosCALL(L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
		RMmIsAddressValid = ReLoadNtosCALL(L"MmIsAddressValid",SystemKernelModuleBase,ImageModuleBase);
		if (RExAllocatePool &&
			RExFreePool &&
			RMmIsAddressValid &&
			Rmemcpy)
		{
			bInit = TRUE;
		}
		if (!bInit)
			return STATUS_UNSUCCESSFUL;

		InlineHookInfo = (PINLINEHOOKINFO)RExAllocatePool(NonPagedPool,(sizeof(INLINEHOOKINFO)+sizeof(INLINEHOOKINFO_INFORMATION))*1024);
		if (!InlineHookInfo)
		{
			if (DebugOn)
				KdPrint(("InlineHookInfo failed\r\n"));
			return STATUS_UNSUCCESSFUL;
		}
		memset(InlineHookInfo,0,(sizeof(INLINEHOOKINFO)+sizeof(INLINEHOOKINFO_INFORMATION))*1024);
		KernelHookCheck(InlineHookInfo);
		//KdPrint(("%08x---%08x\r\n",Length,(sizeof(INLINEHOOKINFO)+sizeof(INLINEHOOKINFO_INFORMATION))*1024));
		if (Length > (sizeof(INLINEHOOKINFO)+sizeof(INLINEHOOKINFO_INFORMATION))*1024)
		{
			for (i=0;i<InlineHookInfo->ulCount;i++)
			{
				if (DebugOn)
					KdPrint(("[%d]KernelHook\r\n"
					"被挂钩地址:%08x\r\n"
					"挂钩函数:%s\r\n"
					"hook跳转地址:%08x\r\n"
					"所在模块:%s\r\n"
					"模块基址:%08x\r\n"
					"模块大小:%x\r\n",
					i,
					InlineHookInfo->InlineHook[i].ulMemoryFunctionBase,
					InlineHookInfo->InlineHook[i].lpszFunction,
					InlineHookInfo->InlineHook[i].ulMemoryHookBase,
					InlineHookInfo->InlineHook[i].lpszHookModuleImage,
					InlineHookInfo->InlineHook[i].ulHookModuleBase,
					InlineHookInfo->InlineHook[i].ulHookModuleSize
					));
			}
			status = OldZwReadFile(
				FileHandle,
				Event,
				ApcRoutine,
				ApcContext,
				IoStatusBlock,
				Buffer,
				Length,
				ByteOffset,
				Key
				);
			Rmemcpy(Buffer,InlineHookInfo,(sizeof(INLINEHOOKINFO)+sizeof(INLINEHOOKINFO_INFORMATION))*1024);
			Length = (sizeof(INLINEHOOKINFO)+sizeof(INLINEHOOKINFO_INFORMATION))*1024;
		}
		RExFreePool(InlineHookInfo);
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == INIT_PROCESS_LIST_PROCESS_MODULE)
	{
		if (Length > 0x123456)
		{
			ulInitEProcess = Length;  //初始化
			if (DebugOn)
				KdPrint(("InitEprocess:%08x\n",ulInitEProcess));
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == LIST_PROCESS_MODULE)
	{
		Rmemcpy = ReLoadNtosCALL(L"memcpy",SystemKernelModuleBase,ImageModuleBase);
		RExAllocatePool = ReLoadNtosCALL(L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
		RExFreePool = ReLoadNtosCALL(L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
		RMmIsAddressValid = ReLoadNtosCALL(L"MmIsAddressValid",SystemKernelModuleBase,ImageModuleBase);
		if (RExAllocatePool &&
			RExFreePool &&
			RMmIsAddressValid &&
			Rmemcpy)
		{
			bInit = TRUE;
		}
		if (!bInit)
			return STATUS_UNSUCCESSFUL;

		if (!RMmIsAddressValid(ulInitEProcess))
		{
			return STATUS_UNSUCCESSFUL;
		}
		PDll = (PDLLINFO)RExAllocatePool(NonPagedPool,(sizeof(DLLINFO)+sizeof(DLL_INFORMATION))*800);
		if (!PDll)
		{
			return STATUS_UNSUCCESSFUL;
		}
		memset(PDll,0,(sizeof(DLLINFO)+sizeof(DLL_INFORMATION))*800);
		
		EunmProcessModule(ulInitEProcess,PDll);

		ulInitEProcess = NULL;  //恢复为NULL
		if (Length > (sizeof(DLLINFO)+sizeof(DLL_INFORMATION))*800)
		{
			for (i=0;i<PDll->ulCount;i++)
			{
				if (DebugOn)
					KdPrint(("[%d]Dll模块\r\n"
					"Path:%ws\r\n"
					"Base:%08X\r\n\r\n",
					i,
					PDll->DllInfo[i].lpwzDllModule,
					PDll->DllInfo[i].ulBase
					));
			}
			status = OldZwReadFile(
				FileHandle,
				Event,
				ApcRoutine,
				ApcContext,
				IoStatusBlock,
				Buffer,
				Length,
				ByteOffset,
				Key
				);
			Rmemcpy(Buffer,PDll,(sizeof(DLLINFO)+sizeof(DLL_INFORMATION))*800);
			Length = (sizeof(DLLINFO)+sizeof(DLL_INFORMATION))*800;
		}
		RExFreePool(PDll);
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == LIST_PROCESS)
	{
		Rmemcpy = ReLoadNtosCALL(L"memcpy",SystemKernelModuleBase,ImageModuleBase);
		RExAllocatePool = ReLoadNtosCALL(L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
		RExFreePool = ReLoadNtosCALL(L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
		if (RExAllocatePool &&
			RExFreePool &&
			Rmemcpy)
		{
			bInit = TRUE;
		}
		if (!bInit)
			return STATUS_UNSUCCESSFUL;

		NormalProcessInfo = (PPROCESSINFO)RExAllocatePool(NonPagedPool,(sizeof(PROCESSINFO)+sizeof(SAFESYSTEM_PROCESS_INFORMATION))*800);
		if (!NormalProcessInfo)
		{
			return STATUS_UNSUCCESSFUL;
		}
		memset(NormalProcessInfo,0,(sizeof(PROCESSINFO)+sizeof(SAFESYSTEM_PROCESS_INFORMATION))*800);

		bPaused = TRUE;  //暂停下读取内存，免得造成同步问题

		GetNormalProcessList(NormalProcessInfo,HideProcessInfo);
		if (DebugOn)
			KdPrint(("Length:%08x-NormalProcessInfo:%08x",Length,(sizeof(PROCESSINFO)+sizeof(SAFESYSTEM_PROCESS_INFORMATION))*800));
		if (Length > (sizeof(PROCESSINFO)+sizeof(SAFESYSTEM_PROCESS_INFORMATION))*800)
		{
			for (i=0;i<NormalProcessInfo->ulCount;i++)
			{
				if (DebugOn)
					KdPrint(("[%d]进程查看\r\n"
						"隐藏状态:%d\r\n"
						"pid:%d\r\n"
						"父进程:%d\r\n"
						"内核访问状态:%d\r\n"
						"PEPROCESS:%08x\r\n"
						"进程全路径:%ws\r\n\r\n",
						i,
						NormalProcessInfo->ProcessInfo[i].IntHideType,
						NormalProcessInfo->ProcessInfo[i].ulPid,
						NormalProcessInfo->ProcessInfo[i].ulInheritedFromProcessId,
						NormalProcessInfo->ProcessInfo[i].ulKernelOpen,
						NormalProcessInfo->ProcessInfo[i].EProcess,
						NormalProcessInfo->ProcessInfo[i].lpwzFullProcessPath
						));
			}
			status = OldZwReadFile(
					FileHandle,
					Event,
					ApcRoutine,
					ApcContext,
					IoStatusBlock,
					Buffer,
					Length,
					ByteOffset,
					Key
					);
				Rmemcpy(Buffer,NormalProcessInfo,(sizeof(PROCESSINFO)+sizeof(SAFESYSTEM_PROCESS_INFORMATION))*800);
				Length = (sizeof(PROCESSINFO)+sizeof(SAFESYSTEM_PROCESS_INFORMATION))*800;
		}
		RExFreePool(NormalProcessInfo);

		//查看过后，就要清零一次
		//因为bPaused控制，所以就不担心同步问题

		memset(HideProcessInfo,0,(sizeof(PROCESSINFO)+sizeof(SAFESYSTEM_PROCESS_INFORMATION))*120);

		bPaused = FALSE;   //恢复隐藏进程的读取

		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == KILL_PROCESS_BY_PID)
	{
		//传入服务号
		if (Length > 0)
		{
			bKernelSafeModule = TRUE;

			KillPro(Length);

			bKernelSafeModule = FALSE;
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == INIT_SET_ATAPI_HOOK)
	{
		if (Length >= NULL && Length <= 0x1c)
		{
			ulNumber = Length;  //初始化
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == SET_ATAPI_HOOK)
	{
		if (Length > 0x123456)
		{
			ulRealDispatch = Length;

			if (DebugOn)
				KdPrint(("Init ulRealDispatch:[%d]%X\n",ulNumber,ulRealDispatch));

			if (ulNumber >= NULL && ulNumber <= 0x1c)
			{
				SetAtapiHook(ulNumber,ulRealDispatch);
			}
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == LIST_ATAPI_HOOK)
	{
		//开始填充
		Rmemcpy = ReLoadNtosCALL(L"memcpy",SystemKernelModuleBase,ImageModuleBase);
		RExAllocatePool = ReLoadNtosCALL(L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
		RExFreePool = ReLoadNtosCALL(L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
		if (RExAllocatePool &&
			RExFreePool &&
			Rmemcpy)
		{
			bInit = TRUE;
		}
		if (!bInit)
			return STATUS_UNSUCCESSFUL;

		AtapiDispatchBakUp = (PATAPIDISPATCHBAKUP)RExAllocatePool(NonPagedPool,(sizeof(ATAPIDISPATCHBAKUP)+sizeof(ATAPIDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION);
		if (!AtapiDispatchBakUp)
		{
			return STATUS_UNSUCCESSFUL;
		}
		memset(AtapiDispatchBakUp,0,(sizeof(ATAPIDISPATCHBAKUP)+sizeof(ATAPIDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION);

		ReLoadAtapi(PDriverObject,AtapiDispatchBakUp,1);  //kbdclass hook

		if (Length > (sizeof(ATAPIDISPATCHBAKUP)+sizeof(ATAPIDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION)
		{
			status = OldZwReadFile(
				FileHandle,
				Event,
				ApcRoutine,
				ApcContext,
				IoStatusBlock,
				Buffer,
				Length,
				ByteOffset,
				Key
				);
			Rmemcpy(Buffer,AtapiDispatchBakUp,(sizeof(ATAPIDISPATCHBAKUP)+sizeof(ATAPIDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION);
			Length = (sizeof(ATAPIDISPATCHBAKUP)+sizeof(ATAPIDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION;
		}
		RExFreePool(AtapiDispatchBakUp);

		return STATUS_UNSUCCESSFUL;
	}
	/////////////////
	if (FileHandle == INIT_SET_MOUCLASS_HOOK)
	{
		if (Length >= NULL && Length <= IRP_MJ_MAXIMUM_FUNCTION)
		{
			ulNumber = Length;  //初始化
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == SET_MOUCLASS_HOOK)
	{
		if (Length > 0x123456)
		{
			ulRealDispatch = Length;

			if (DebugOn)
				KdPrint(("Init ulRealDispatch:[%d]%X\n",ulNumber,ulRealDispatch));

			if (ulNumber >= NULL && ulNumber <= IRP_MJ_MAXIMUM_FUNCTION)
			{
				SetMouclassHook(ulNumber,ulRealDispatch);
			}
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == LIST_MOUCLASS_HOOK)
	{
		//开始填充
		Rmemcpy = ReLoadNtosCALL(L"memcpy",SystemKernelModuleBase,ImageModuleBase);
		RExAllocatePool = ReLoadNtosCALL(L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
		RExFreePool = ReLoadNtosCALL(L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
		if (RExAllocatePool &&
			RExFreePool &&
			Rmemcpy)
		{
			bInit = TRUE;
		}
		if (!bInit)
			return STATUS_UNSUCCESSFUL;

		MouclassDispatchBakUp = (PMOUCLASSDISPATCHBAKUP)RExAllocatePool(NonPagedPool,(sizeof(MOUCLASSDISPATCHBAKUP)+sizeof(MOUCLASSDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION);
		if (!MouclassDispatchBakUp)
		{
			return STATUS_UNSUCCESSFUL;
		}
		memset(MouclassDispatchBakUp,0,(sizeof(MOUCLASSDISPATCHBAKUP)+sizeof(MOUCLASSDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION);

		ReLoadMouclass(PDriverObject,MouclassDispatchBakUp,1);  //kbdclass hook

		if (Length > (sizeof(MOUCLASSDISPATCHBAKUP)+sizeof(MOUCLASSDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION)
		{
			status = OldZwReadFile(
				FileHandle,
				Event,
				ApcRoutine,
				ApcContext,
				IoStatusBlock,
				Buffer,
				Length,
				ByteOffset,
				Key
				);
			Rmemcpy(Buffer,MouclassDispatchBakUp,(sizeof(MOUCLASSDISPATCHBAKUP)+sizeof(MOUCLASSDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION);
			Length = (sizeof(MOUCLASSDISPATCHBAKUP)+sizeof(MOUCLASSDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION;
		}
		RExFreePool(MouclassDispatchBakUp);

		return STATUS_UNSUCCESSFUL;
	}
	///////
	if (FileHandle == INIT_SET_KBDCLASS_HOOK)
	{
		if (Length >= NULL && Length <= IRP_MJ_MAXIMUM_FUNCTION)
		{
			ulNumber = Length;  //初始化
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == SET_KBDCLASS_HOOK)
	{
		if (Length > 0x123456)
		{
			ulRealDispatch = Length;

			if (DebugOn)
				KdPrint(("Init ulRealDispatch:[%d]%X\n",ulNumber,ulRealDispatch));

			if (ulNumber >= NULL && ulNumber <= IRP_MJ_MAXIMUM_FUNCTION)
			{
				SetKbdclassHook(ulNumber,ulRealDispatch);
			}
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == LIST_KBDCLASS_HOOK)
	{
		//开始填充
		Rmemcpy = ReLoadNtosCALL(L"memcpy",SystemKernelModuleBase,ImageModuleBase);
		RExAllocatePool = ReLoadNtosCALL(L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
		RExFreePool = ReLoadNtosCALL(L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
		if (RExAllocatePool &&
			RExFreePool &&
			Rmemcpy)
		{
			bInit = TRUE;
		}
		if (!bInit)
			return STATUS_UNSUCCESSFUL;

		KbdclassDispatchBakUp = (PKBDCLASSDISPATCHBAKUP)RExAllocatePool(NonPagedPool,(sizeof(KBDCLASSDISPATCHBAKUP)+sizeof(KBDCLASSDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION);
		if (!KbdclassDispatchBakUp)
		{
			return STATUS_UNSUCCESSFUL;
		}
		memset(KbdclassDispatchBakUp,0,(sizeof(KBDCLASSDISPATCHBAKUP)+sizeof(KBDCLASSDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION);

		ReLoadKbdclass(PDriverObject,KbdclassDispatchBakUp,1);  //kbdclass hook

		if (Length > (sizeof(KBDCLASSDISPATCHBAKUP)+sizeof(KBDCLASSDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION)
		{
			status = OldZwReadFile(
				FileHandle,
				Event,
				ApcRoutine,
				ApcContext,
				IoStatusBlock,
				Buffer,
				Length,
				ByteOffset,
				Key
				);
			Rmemcpy(Buffer,KbdclassDispatchBakUp,(sizeof(KBDCLASSDISPATCHBAKUP)+sizeof(KBDCLASSDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION);
			Length = (sizeof(KBDCLASSDISPATCHBAKUP)+sizeof(KBDCLASSDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION;
		}
		RExFreePool(KbdclassDispatchBakUp);

		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == INIT_SET_FSD_HOOK)
	{
		if (Length >= NULL && Length <= IRP_MJ_MAXIMUM_FUNCTION)
		{
			ulNumber = Length;  //初始化
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == SET_FSD_HOOK)
	{
		if (Length > 0x123456)
		{
			ulRealDispatch = Length;

			if (DebugOn)
				KdPrint(("Init ulRealDispatch:[%d]%X\n",ulNumber,ulRealDispatch));

			if (ulNumber >= NULL && ulNumber <= IRP_MJ_MAXIMUM_FUNCTION)
			{
				SetFsdHook(ulNumber,ulRealDispatch);
			}
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == LIST_FSD_HOOK)
	{
		Rmemcpy = ReLoadNtosCALL(L"memcpy",SystemKernelModuleBase,ImageModuleBase);
		RExAllocatePool = ReLoadNtosCALL(L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
		RExFreePool = ReLoadNtosCALL(L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
		if (RExAllocatePool &&
			RExFreePool &&
			Rmemcpy)
		{
			bInit = TRUE;
		}
		if (!bInit)
			return STATUS_UNSUCCESSFUL;

		NtfsDispatchBakUp = (PNTFSDISPATCHBAKUP)RExAllocatePool(NonPagedPool,(sizeof(NTFSDISPATCHBAKU)+sizeof(NTFSDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION);
		if (!NtfsDispatchBakUp)
		{
			return STATUS_UNSUCCESSFUL;
		}
		memset(NtfsDispatchBakUp,0,(sizeof(NTFSDISPATCHBAKU)+sizeof(NTFSDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION);

		ReLoadNtfs(PDriverObject,NtfsDispatchBakUp,1);  //fsd hook

		if (Length > (sizeof(NTFSDISPATCHBAKU)+sizeof(NTFSDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION)
		{
			status = OldZwReadFile(
				FileHandle,
				Event,
				ApcRoutine,
				ApcContext,
				IoStatusBlock,
				Buffer,
				Length,
				ByteOffset,
				Key
				);
			Rmemcpy(Buffer,NtfsDispatchBakUp,(sizeof(NTFSDISPATCHBAKU)+sizeof(NTFSDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION);
			Length = (sizeof(NTFSDISPATCHBAKU)+sizeof(NTFSDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION;
		}
		RExFreePool(NtfsDispatchBakUp);

		return STATUS_UNSUCCESSFUL;
	}
	//ONLY_DELETE_FILE是删除360文件，因此不需要reload
	if (FileHandle == DELETE_FILE ||
		FileHandle == ONLY_DELETE_FILE)
	{
		if (Buffer != 0 &&
			Length > 0)
		{
			bKernelSafeModule = TRUE;

			if (FileHandle == DELETE_FILE)
			{
				ReLoadNtfs(PDriverObject,0,0);  //reload ntfs 恢复真实地址
				ReLoadAtapi(PDriverObject,0,0); //reload atapi 
			}

			HFileHandle = SkillIoOpenFile(
				Buffer,   //删除dll文件
				FILE_READ_ATTRIBUTES,
				FILE_SHARE_DELETE);
			if (HFileHandle!=NULL)
			{
				SKillDeleteFile(HFileHandle);
				ZwClose(HFileHandle);
			}
			if (FileHandle == DELETE_FILE)
			{
				ReLoadNtfsFree();  //恢复
				ReLoadAtapiFree();
			}
			bKernelSafeModule = FALSE;
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == INIT_SET_TCPIP_HOOK)
	{
		if (Length >= NULL && Length <= IRP_MJ_MAXIMUM_FUNCTION)
		{
			ulNumber = Length;  //初始化
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == SET_TCPIP_HOOK)
	{
		if (Length > 0x123456)
		{
			ulRealDispatch = Length;

			if (DebugOn)
				KdPrint(("Init ulRealDispatch:[%d]%X\n",ulNumber,ulRealDispatch));

			if (ulNumber >= NULL && ulNumber <= IRP_MJ_MAXIMUM_FUNCTION)
			{
				SetTcpHook(ulNumber,ulRealDispatch);
			}
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == LIST_TCPIP_HOOK)
	{
		Rmemcpy = ReLoadNtosCALL(L"memcpy",SystemKernelModuleBase,ImageModuleBase);
		RExAllocatePool = ReLoadNtosCALL(L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
		RExFreePool = ReLoadNtosCALL(L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
		if (RExAllocatePool &&
			RExFreePool &&
			Rmemcpy)
		{
			bInit = TRUE;
		}
		if (!bInit)
			return STATUS_UNSUCCESSFUL;

		TcpDispatchBakUp = (PTCPDISPATCHBAKUP)RExAllocatePool(NonPagedPool,(sizeof(TCPDISPATCHBAKUP)+sizeof(TCPDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION);
		if (!TcpDispatchBakUp)
		{
			return STATUS_UNSUCCESSFUL;
		}
		memset(TcpDispatchBakUp,0,(sizeof(TCPDISPATCHBAKUP)+sizeof(TCPDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION);

		ReLoadTcpip(PDriverObject,TcpDispatchBakUp,1);
		if (Length > (sizeof(TCPDISPATCHBAKUP)+sizeof(TCPDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION)
		{
			status = OldZwReadFile(
				FileHandle,
				Event,
				ApcRoutine,
				ApcContext,
				IoStatusBlock,
				Buffer,
				Length,
				ByteOffset,
				Key
				);
			Rmemcpy(Buffer,TcpDispatchBakUp,(sizeof(TCPDISPATCHBAKUP)+sizeof(TCPDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION);
			Length = (sizeof(TCPDISPATCHBAKUP)+sizeof(TCPDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION;
		}
		ReLoadTcpipFree(); //释放

		RExFreePool(TcpDispatchBakUp);

		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == INIT_SET_NSIPROXY_HOOK)
	{
		if (Length >= NULL && Length <= IRP_MJ_MAXIMUM_FUNCTION)
		{
			ulNumber = Length;  //初始化
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == SET_NSIPROXY_HOOK)
	{
		if (Length > 0x123456)
		{
			ulRealDispatch = Length;

			if (DebugOn)
				KdPrint(("Init ulRealDispatch:[%d]%X\n",ulNumber,ulRealDispatch));

			if (ulNumber >= NULL && ulNumber <= IRP_MJ_MAXIMUM_FUNCTION)
			{
				SetNsiproxyHook(ulNumber,ulRealDispatch);
			}
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == LIST_NSIPROXY_HOOK)
	{
		Rmemcpy = ReLoadNtosCALL(L"memcpy",SystemKernelModuleBase,ImageModuleBase);
		RExAllocatePool = ReLoadNtosCALL(L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
		RExFreePool = ReLoadNtosCALL(L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
		if (RExAllocatePool &&
			RExFreePool &&
			Rmemcpy)
		{
			bInit = TRUE;
		}
		if (!bInit)
			return STATUS_UNSUCCESSFUL;

		NsiproxyDispatchBakUp = (PNSIPROXYDISPATCHBAKUP)RExAllocatePool(NonPagedPool,(sizeof(NSIPROXYDISPATCHBAKUP)+sizeof(NSIPROXYDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION);
		if (!NsiproxyDispatchBakUp)
		{
			return STATUS_UNSUCCESSFUL;
		}
		memset(NsiproxyDispatchBakUp,0,(sizeof(NSIPROXYDISPATCHBAKUP)+sizeof(NSIPROXYDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION);
		ReLoadNsiproxy(PDriverObject,NsiproxyDispatchBakUp,1);

		if (Length > (sizeof(NSIPROXYDISPATCHBAKUP)+sizeof(NSIPROXYDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION)
		{
			status = OldZwReadFile(
				FileHandle,
				Event,
				ApcRoutine,
				ApcContext,
				IoStatusBlock,
				Buffer,
				Length,
				ByteOffset,
				Key
				);
			Rmemcpy(Buffer,NsiproxyDispatchBakUp,(sizeof(NSIPROXYDISPATCHBAKUP)+sizeof(NSIPROXYDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION);
			Length = (sizeof(NSIPROXYDISPATCHBAKUP)+sizeof(NSIPROXYDISPATCH))*IRP_MJ_MAXIMUM_FUNCTION;
		}
		RExFreePool(NsiproxyDispatchBakUp);
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == LIST_TCPUDP)
	{
		Rmemcpy = ReLoadNtosCALL(L"memcpy",SystemKernelModuleBase,ImageModuleBase);
		RExAllocatePool = ReLoadNtosCALL(L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
		RExFreePool = ReLoadNtosCALL(L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
		if (RExAllocatePool &&
			RExFreePool &&
			Rmemcpy)
		{
			bInit = TRUE;
		}
		if (!bInit)
			return STATUS_UNSUCCESSFUL;

		ReLoadTcpip(PDriverObject,0,0);  //先reload
		ReLoadNsiproxy(PDriverObject,0,0);  //先reload

		TCPUDPInfo = (PTCPUDPINFO)RExAllocatePool(NonPagedPool,(sizeof(TCPUDPINFO)+sizeof(TCPUDPINFO_INFORMATION))*1024);
		if (TCPUDPInfo)
		{
			memset(TCPUDPInfo,0,(sizeof(TCPUDPINFO)+sizeof(TCPUDPINFO_INFORMATION))*1024);

			WinVer = GetWindowsVersion();
			if (WinVer == WINDOWS_VERSION_XP ||
				WinVer == WINDOWS_VERSION_2K3_SP1_SP2)
			{
				PrintTcpIp(TCPUDPInfo);
			}
			else if (WinVer == WINDOWS_VERSION_7)
			{
				PrintTcpIpInWin7(TCPUDPInfo);
			}
			if (Length > (sizeof(TCPUDPINFO)+sizeof(TCPUDPINFO_INFORMATION))*1024)
			{
				for (i = 0; i<TCPUDPInfo->ulCount ;i++)
				{
					if (DebugOn)
						KdPrint(("[%d]网络连接\r\n"
						"协议:%d\r\n"
						"连接状态:%d\r\n"
						"本地地址:%08x\r\n"
						"本地端口:%d\r\n"
						"进程pid:%d\r\n"
						"进程路径:%ws\r\n"
						"远程地址:%08x\r\n"
						"远程端口:%d\r\n\r\n",
						i,
						TCPUDPInfo->TCPUDP[i].ulType,
						TCPUDPInfo->TCPUDP[i].ulConnectType,
						TCPUDPInfo->TCPUDP[i].ulLocalAddress,
						TCPUDPInfo->TCPUDP[i].ulLocalPort,
						TCPUDPInfo->TCPUDP[i].ulPid,
						TCPUDPInfo->TCPUDP[i].lpwzFullPath,
						TCPUDPInfo->TCPUDP[i].ulRemoteAddress,
						TCPUDPInfo->TCPUDP[i].ulRemotePort));
				}
				status = OldZwReadFile(
					FileHandle,
					Event,
					ApcRoutine,
					ApcContext,
					IoStatusBlock,
					Buffer,
					Length,
					ByteOffset,
					Key
					);
				Rmemcpy(Buffer,TCPUDPInfo,(sizeof(TCPUDPINFO)+sizeof(TCPUDPINFO_INFORMATION))*1024);
				Length = (sizeof(TCPUDPINFO)+sizeof(TCPUDPINFO_INFORMATION))*1024;
			}
			RExFreePool(TCPUDPInfo);
		}
		ReLoadTcpipFree(); //释放
		ReLoadNsiproxyFree(); //释放

		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == SET_INLINE_HOOK)
	{
		if (Buffer != NULL &&
			Length > 0)
		{
			if (DebugOn)
				KdPrint(("Set Inline hook:%ws",Buffer));
			RestoreKiFastCallEntryInlineHook(Buffer);
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == SET_ONE_SSDT)
	{
		//传入服务号
		if (Length > 0 ||
			Length == 0)
		{
			RestoreAllSSDTFunction(Length);
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == SET_ALL_SSDT)
	{
		RestoreAllSSDTFunction(8888);
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == UNPROTECT_DRIVER_FILE)
	{
		bProtectDriverFile = FALSE;   //取消保护
		KdPrint(("UnProtect Driver File\r\n"));
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == PROTECT_DRIVER_FILE)
	{
		bProtectDriverFile = TRUE;     //保护
		KdPrint(("Protect Driver File\r\n"));
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == SAFE_SYSTEM)
	{
		if (DebugOn)
			KdPrint(("ProtectCode:%08x\r\n",SAFE_SYSTEM));

		status = OldZwReadFile(
			FileHandle,
			Event,
			ApcRoutine,
			ApcContext,
			IoStatusBlock,
			Buffer,
			Length,
			ByteOffset,
			Key
			);
		if (Length >= 4)
		{
			ProtectEProcess = PsGetCurrentProcess();   //自己的进程啊

		    if (DebugOn)
				KdPrint(("ProtectCode:%08x\r\n",ProtectEProcess));

			strcat(Buffer,"safe");
			Length = strlen("safe");

			bIsInitSuccess = TRUE;
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (FileHandle == LIST_SSDT ||
		FileHandle == LIST_SSDT_ALL)
	{
		if (FileHandle == LIST_SSDT_ALL)
		{
			//KdPrint(("Print SSDT All"));
			bSSDTAll = TRUE;
		}
		if (FileHandle == LIST_SSDT)
		{
			//KdPrint(("Print SSDT"));
		}

		Rmemcpy = ReLoadNtosCALL(L"memcpy",SystemKernelModuleBase,ImageModuleBase);
		RExAllocatePool = ReLoadNtosCALL(L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
		RExFreePool = ReLoadNtosCALL(L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
		RMmIsAddressValid = ReLoadNtosCALL(L"MmIsAddressValid",SystemKernelModuleBase,ImageModuleBase);
		if (RExAllocatePool &&
			RExFreePool &&
			RMmIsAddressValid &&
			Rmemcpy)
		{
			bInit = TRUE;
		}
		if (!bInit)
			return STATUS_UNSUCCESSFUL;

// 		ulKeServiceDescriptorTable = GetSystemRoutineAddress(1,L"KeServiceDescriptorTable");
// 		ulSize = ((PSERVICE_DESCRIPTOR_TABLE)ulKeServiceDescriptorTable)->TableSize;
		SSDTInfo = (PSSDTINFO)RExAllocatePool(NonPagedPool,(sizeof(SSDTINFO)+sizeof(SSDT_INFORMATION))*800);
		if (SSDTInfo)
		{
			memset(SSDTInfo,0,(sizeof(SSDTINFO)+sizeof(SSDT_INFORMATION))*800);
			PrintSSDT(SSDTInfo);
			if (Length > (sizeof(SSDTINFO)+sizeof(SSDT_INFORMATION))*800)
			{
				for (i = 0; i< SSDTInfo->ulCount ;i++)
				{
					if (DebugOn)
						KdPrint(("[%d]发现SSDT hook\r\n"
						"服务号:%d\r\n"
						"当前地址:%08x\r\n"
						"函数名字:%s\r\n"
						"当前hook模块:%s\r\n"
						"当前模块地址:%08x\r\n"
						"当前模块大小:%d KB\r\n"
						"Hook类型:%d\r\n\r\n",
						i,
						SSDTInfo->SSDT[i].ulNumber,
						SSDTInfo->SSDT[i].ulMemoryFunctionBase,
						SSDTInfo->SSDT[i].lpszFunction,
						SSDTInfo->SSDT[i].lpszHookModuleImage,
						SSDTInfo->SSDT[i].ulHookModuleBase,
						SSDTInfo->SSDT[i].ulHookModuleSize/1024,
						SSDTInfo->SSDT[i].IntHookType));
				}
				status = OldZwReadFile(
					FileHandle,
					Event,
					ApcRoutine,
					ApcContext,
					IoStatusBlock,
					Buffer,
					Length,
					ByteOffset,
					Key
					);
				Rmemcpy(Buffer,SSDTInfo,(sizeof(SSDTINFO)+sizeof(SSDT_INFORMATION))*800);
				Length = (sizeof(SSDTINFO)+sizeof(SSDT_INFORMATION))*800;
			}
			RExFreePool(SSDTInfo);
		}
		bSSDTAll = FALSE;
		return status;
	}
_FunctionRet:
	return OldZwReadFile(
		FileHandle,
		Event,
		ApcRoutine,
		ApcContext,
		IoStatusBlock,
		Buffer,
		Length,
		ByteOffset,
		Key
		);
}
__declspec(naked) NTSTATUS NtReadFileHookZone(,...)
{
	_asm
	{
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		jmp [NtReadFileRet];
	}
}
VOID WaitMicroSecond(LONG MicroSeconds)
{
	KEVENT KEnentTemp;
	LARGE_INTEGER waitTime;

	KeInitializeEvent(
		&KEnentTemp, 
		SynchronizationEvent, 
		FALSE
		);
	waitTime = RtlConvertLongToLargeInteger(-10 * MicroSeconds);

	KeWaitForSingleObject(
		&KEnentTemp,
		Executive,
		KernelMode,
		FALSE, 
		&waitTime
		);
}
VOID ResetMyControl()
{
	BOOL bRet = FALSE;
	KIRQL oldIrql = NULL; 

	while (1)
	{
		if (!bRet)
		{
			bRet = HookFunctionHeader((DWORD)NewNtReadFile,L"ZwReadFile",TRUE,0,(PVOID)NtReadFileHookZone,&NtReadFilePatchCodeLen,&NtReadFileRet);
			if (DebugOn)
				KdPrint(("inline hook ZwReadFile success"));
		}
		//取到csrsspid之后
		if (IsExitProcess(CsrssEProcess))
		{
			//挂载，然后reload win32K
			if (ReloadWin32K() == STATUS_SUCCESS)
			{
				KdPrint(("Init Win32K module success\r\n"));
				bInitWin32K = TRUE; //success
			}

			//线程里的IRQL太高了，要降低下
			if (KeGetCurrentIrql() <= DISPATCH_LEVEL &&
				KeGetCurrentIrql() > PASSIVE_LEVEL)
			{
				if (!oldIrql)
					oldIrql = KeRaiseIrqlToDpcLevel(); //注意自旋锁 
			}
			UnHookFunctionHeader(L"ZwReadFile",TRUE,0,(PVOID)NtReadFileHookZone,NtReadFilePatchCodeLen);  //恢复一下

			if (oldIrql)
				KeLowerIrql(oldIrql);

			KdPrint(("Init Protect Thread success\r\n"));

			PsTerminateSystemThread(STATUS_SUCCESS);
		}
		WaitMicroSecond(2000);
	}
}
NTSTATUS __stdcall NewZwTerminateProcess(
	IN HANDLE  ProcessHandle,
	IN NTSTATUS  ExitStatus
	)
{
	PEPROCESS EProcess;
	NTSTATUS status;
	ZWTERMINATEPROCESS OldZwTerminateProcess;
	KPROCESSOR_MODE PreviousMode;

	//KdPrint(("bIsProtect360:%d",bIsProtect360));
	if (KeGetCurrentIrql() > PASSIVE_LEVEL)
	{
		goto _FunctionRet;
	}
	//如果退出了
	if (!bIsInitSuccess)
		goto _FunctionRet;

	//默认第一层简单保护下
// 	if (!bProtectProcess)
// 		goto _FunctionRet;

	if (ProcessHandle &&
		ARGUMENT_PRESENT(ProcessHandle))
	{
		status = ObReferenceObjectByHandle(
			ProcessHandle,
			PROCESS_ALL_ACCESS,
			0,
			KernelMode,
			(PVOID*)&EProcess,
			NULL
			);
		if (NT_SUCCESS(status))
		{
			ObDereferenceObject(EProcess);

			//双层保护3600safe自身
			if (EProcess == ProtectEProcess)
			{
				return STATUS_ACCESS_DENIED;
			}
		}
	}
_FunctionRet:
	if (MmIsAddressValid(OriginalServiceDescriptorTable->ServiceTable[ZwTerminateProcessIndex]))
	{
		OldZwTerminateProcess = OriginalServiceDescriptorTable->ServiceTable[ZwTerminateProcessIndex];
	}else
		OldZwTerminateProcess = KeServiceDescriptorTable->ServiceTable[ZwTerminateProcessIndex];

	return OldZwTerminateProcess(
		ProcessHandle,
		ExitStatus
		);
}
BOOL InitControl()
{
	UNICODE_STRING UnicdeFunction;
	HANDLE ThreadHandle;
	PEPROCESS EProcess;

 	if (SystemCallEntryTableHook(
		"ZwReadFile",
		&ZwReadFileIndex,
		NewNtReadFile) == TRUE)
	{
		if (DebugOn)
			KdPrint(("Init Control Thread success 1\r\n"));
	}
	if (bKernelBooting)
	{
		//重启就要初始化
		bIsInitSuccess = TRUE;
		KdPrint(("kernel booting\r\n"));
	}
	if (SystemCallEntryTableHook(
		"ZwTerminateProcess",
		&ZwTerminateProcessIndex,
		NewZwTerminateProcess) == TRUE)
	{
		if (DebugOn)
			KdPrint(("Create Control Thread success 2\r\n"));
	}
	InitZwSetValueKey();  //注册表
 	InitNetworkDefence();
 	InitWriteFile();

	//去掉object hook保护文件，暂时不需要吧
	//InstallFileObejctHook();
	InitKernelThreadData();   //kernel thread hook

	//备用的通信线程
	//创建系统线程反复回写吧！
	if (PsCreateSystemThread(
		&ThreadHandle,
		0,
		NULL,
		NULL,
		NULL,
		ResetMyControl,
		NULL) == STATUS_SUCCESS)
	{
		ZwClose(ThreadHandle);
		if (DebugOn)
			KdPrint(("Create Control Thread success 2\r\n"));
	}
}