#include "monitor.h"


LPWSTR GetLocalTime()
{
	LARGE_INTEGER systemTime;
	LARGE_INTEGER localTime;
	TIME_FIELDS   timeFields;
	PUNICODE_STRING pUstrTime = NULL;
	WCHAR Time[256];

	__try
	{
		KeQuerySystemTime(&systemTime);
		ExSystemTimeToLocalTime(&systemTime,&localTime);
		RtlTimeToTimeFields(&localTime,&timeFields);

		swprintf(&Time,L"%d:%d:%d.%d",timeFields.Hour,timeFields.Minute,timeFields.Second,timeFields.Milliseconds);

		return &Time;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("GetLocalTime EXCEPTION_EXECUTE_HANDLER error!");
		return NULL;
	}
}

NTSTATUS monitor_updateProcessMonitorFilerInfo(PMONITOR_PROCESS_FILTER_INFO p)
{
	__try
	{
		pMonitorProcessFilterInfo->next = p;
		pMonitorProcessFilterInfo = p;

		return STATUS_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("monitor_updateProcessMonitorFilerInfo EXCEPTION_EXECUTE_HANDLER error!");
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS monitor_updatePathMonitorFilerInfo(PMONITOR_PATH_FILTER_INFO p)
{
	__try
	{
		pMonitorPathFilterInfo->next = p;
		pMonitorPathFilterInfo = p;

		return STATUS_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("monitor_updatePathMonitorFilerInfo EXCEPTION_EXECUTE_HANDLER error!");
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS monitor_updateMonitorInfo(LPTSTR lpProcessName,LPTSTR lpProcessPath,ULONG ulPID,LPTSTR lpAction,LPTSTR lpPathName,LPTSTR lpResult,LPTSTR lpOther)
{
	LPWSTR pTime = NULL;
	PMONITOR_INFO p = NULL;
    PMONITOR_PROCESS_FILTER_INFO p2,p4 = NULL;
	PMONITOR_PATH_FILTER_INFO p3,p5 = NULL;

	p = (PMONITOR_INFO)ExAllocatePool(NonPagedPool,sizeof(MONITOR_INFO));

	if (!p)
	{
		DbgPrint("monitor_updateMonitorInfo ExAllocatePool error!");
		return STATUS_UNSUCCESSFUL;
	}

	for(p2 = pMonitorProcessFilterInfoHead->next;p2;p2 = p2->next)
	{
		if (p2->bProcessInclude)
		{
			//包含进程
			if(_wcsnicmp(lpProcessName,p2->ProcessName,wcslen(p2->ProcessName)) != 0)
			{
				return;
			}
			else
			{
				//不包含进程
				for(p4 = pMonitorProcessFilterInfoHead->next;p4;p4 = p4->next)
				{
					if (!p4->bProcessInclude)
					{
						if(_wcsnicmp(lpProcessName,p4->ProcessName,wcslen(p4->ProcessName)) == 0)
						{
							return;
						}
					}
				}
			}
		}
		else
		{
			//不包含进程
			if(_wcsnicmp(lpProcessName,p2->ProcessName,wcslen(p2->ProcessName)) == 0)
			{
				return;
			}
			else
			{
				//包含进程
				for(p4 = pMonitorProcessFilterInfoHead->next;p4;p4 = p4->next)
				{
					if (p4->bProcessInclude)
					{
						if(_wcsnicmp(lpProcessName,p4->ProcessName,wcslen(p4->ProcessName)) != 0)
						{
							return;
						}
					}
				}
			}
		}
	}

	for(p3 = pMonitorPathFilterInfoHead->next;p3;p3 = p3->next)
	{
		if (p3->bPathInclude)
		{
			//包含路径
			if(_wcsnicmp(lpPathName,p3->PathName,wcslen(p3->PathName)) != 0)
			{
				return;
			}
			else
			{
				//不包含路径
				for(p5 = pMonitorPathFilterInfoHead->next;p5;p5 = p5->next)
				{
					if (!p5->bPathInclude)
					{
						if(_wcsnicmp(lpPathName,p5->PathName,wcslen(p5->PathName)) == 0)
						{
							return;
						}
					}
				}
			}
		}
		else
		{
			//不包含路径
			if(_wcsnicmp(lpPathName,p3->PathName,wcslen(p3->PathName)) == 0)
			{
				return;
			}
			else
			{
				//包含路径
				for(p5 = pMonitorPathFilterInfoHead->next;p5;p5 = p5->next)
				{
					if (p5->bPathInclude)
					{
						if(_wcsnicmp(lpPathName,p5->PathName,wcslen(p5->PathName)) != 0)
						{
							return;
						}
					}
				}
			}
		}
	}

	pTime = GetLocalTime();
	//时间
	wcscpy(p->Time,pTime);
	//
    wcscpy(p->ProcessName,lpProcessName);
	//
	wcscpy(p->ProcessPath,lpProcessPath);

	//进程ID
	p->ulPID = ulPID;
	//复制数据
	wcscpy(p->Action,lpAction);
	//进程路径
	wcscpy(p->PathName,lpPathName);
	//
	wcscpy(p->Result,lpResult);
	//
	wcscpy(p->Other,lpOther);
	p->next = NULL;
	pMonitorInfo->next = p;

	//记录下最后节点的位置
	pMonitorInfo = p;
}

void monitor_freeMonitorFilerInfo()
{
	PMONITOR_PROCESS_FILTER_INFO p,p2= NULL;
	PMONITOR_PATH_FILTER_INFO p3,p4 = NULL;

	p = pMonitorProcessFilterInfoHead->next;

	while(p != NULL)
	{
		p2 = p->next;
		ExFreePool(p);
		p  = p2;
	}

	pMonitorProcessFilterInfoHead->next = NULL;
    pMonitorProcessFilterInfo = pMonitorProcessFilterInfoHead;

	p3 = pMonitorPathFilterInfoHead->next;

	while(p3 != NULL)
	{
		p4 = p3->next;
		ExFreePool(p3);
		p3  = p4;
	}

	pMonitorPathFilterInfoHead->next = NULL;
	pMonitorPathFilterInfo = pMonitorPathFilterInfoHead;
}

// 进程回调函数
VOID ProcessCallbackFunc(IN HANDLE  hParentId, IN HANDLE  hProcessId, IN BOOLEAN bCreate)
{
	NTSTATUS status;
	PEPROCESS pEpr;
	ANSI_STRING astrProcessName;
	UNICODE_STRING ustrProcessName;
	WCHAR Path[256];
	UNICODE_STRING utsrAction;
	UNICODE_STRING ustrResult;
	WCHAR Other[256];

	try
	{
		status = PsLookupProcessByProcessId(hProcessId,&pEpr);
		if (NT_SUCCESS(status))
		{
			RtlInitAnsiString(&astrProcessName,PsGetProcessImageFileName(pEpr));
			RtlAnsiStringToUnicodeString(&ustrProcessName,&astrProcessName,TRUE);
		}

		status = GetProcessImageNameByProcessHandle(hProcessId,&Path);

		if (!NT_SUCCESS(status))
		{
			wcscpy(Path,L"");
		}

		//操作
		//如果是被创建
        if (bCreate)
        {	
			RtlInitUnicodeString(&utsrAction,L"Process Create");
        }
		else
		{
			RtlInitUnicodeString(&utsrAction,L"Process Exit");
		}
		//结果
		RtlInitUnicodeString(&ustrResult,L"SUCCESS");
		//其他
		swprintf(&Other,L"hProcessId: %d",hProcessId);

		MUTEX_P(MKMutex);
		monitor_updateMonitorInfo(ustrProcessName.Buffer,&Path,(ULONG)hParentId,utsrAction.Buffer,&Path,ustrResult.Buffer,&Other);
		MUTEX_V(MKMutex);
		
	}
	_except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("ProcessCallbackFunc EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//线程回调函数
VOID ThreadCallbackFunc(IN HANDLE  hProcessId, IN HANDLE  hThreadId, IN BOOLEAN bCreate)
{
	NTSTATUS status;
	PEPROCESS pEpr;
	ANSI_STRING astrProcessName;
	UNICODE_STRING ustrProcessName;
	PUNICODE_STRING pUstrPath;
	UNICODE_STRING utsrAction;
	UNICODE_STRING ustrResult;
	WCHAR Path[256];
	WCHAR Other[256];

	try
	{

		status = PsLookupProcessByProcessId(hProcessId,&pEpr);
		if (NT_SUCCESS(status))
		{
			RtlInitAnsiString(&astrProcessName,PsGetProcessImageFileName(pEpr));
            RtlAnsiStringToUnicodeString(&ustrProcessName,&astrProcessName,TRUE);
		}

		status = GetProcessImageNameByProcessHandle(hProcessId,&Path);

		if (!NT_SUCCESS(status))
		{
			wcscpy(Path,L"");
		}

		//没有路径信息，因为是线程创建
		//
		//如果是被创建
		if (bCreate)
		{	
			RtlInitUnicodeString(&utsrAction,L"Thread Create");
		}
		else
		{
			RtlInitUnicodeString(&utsrAction,L"Thread Exit");
		}
		//结果
		RtlInitUnicodeString(&ustrResult,L"SUCCESS");
		//其他
		swprintf(&Other,L"hThreadId: %d",hThreadId);

		MUTEX_P(MKMutex);
		monitor_updateMonitorInfo(ustrProcessName.Buffer,&Path,(ULONG)hProcessId,utsrAction.Buffer,L"",ustrResult.Buffer,&Other);
		MUTEX_V(MKMutex);
	}
	_except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("ThreadCallbackFunc EXCEPTION_EXECUTE_HANDLER error!");
	}
}

// LoadImage回调函数
VOID LoadImageCallbackFunc(IN PUNICODE_STRING  pUniImageName, IN HANDLE  hProcessId, IN PIMAGE_INFO pImageInfo)
{
	NTSTATUS status;
	PEPROCESS pEpr;
	ANSI_STRING astrProcessName;
	UNICODE_STRING ustrProcessName;
	PUNICODE_STRING pUstrPath;
	UNICODE_STRING utsrAction;
	UNICODE_STRING ustrResult;
	WCHAR Path[256];
	WCHAR Other[256];

	try
	{
		status = PsLookupProcessByProcessId(hProcessId,&pEpr);
		if (NT_SUCCESS(status))
		{
			RtlInitAnsiString(&astrProcessName,PsGetProcessImageFileName(pEpr));
			RtlAnsiStringToUnicodeString(&ustrProcessName,&astrProcessName,TRUE);
		}

		status = GetProcessImageNameByProcessHandle(hProcessId,&Path);

		if (!NT_SUCCESS(status))
		{
			wcscpy(Path,L"");
		}

		//操作
		RtlInitUnicodeString(&utsrAction,L"Load Image");		
		//结果
		RtlInitUnicodeString(&ustrResult,L"SUCCESS");
		//其他
		swprintf(&Other,L"Image Base: 0x%08X, Image Size: 0x%08X",pImageInfo->ImageBase,pImageInfo->ImageSize);

		MUTEX_P(MKMutex);
		monitor_updateMonitorInfo(ustrProcessName.Buffer,&Path,(ULONG)hProcessId,utsrAction.Buffer,pUniImageName->Buffer,ustrResult.Buffer,&Other);
		MUTEX_V(MKMutex);
	}
	_except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("LoadImageCallbackFunc EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//开启监控
NTSTATUS monitor_start()
{
	MUTEX_INIT(MKMutex);
	pMonitorInfo = pMonitorInfoHead = (PDISABLE_CREATE_REG)ExAllocatePool(NonPagedPool,sizeof(DISABLE_CREATE_REG));
    pMonitorInfo->next = pMonitorInfoHead->next = NULL;

	PsSetCreateProcessNotifyRoutine(ProcessCallbackFunc,FALSE);
	PsSetCreateThreadNotifyRoutine(ThreadCallbackFunc);
	PsSetLoadImageNotifyRoutine(LoadImageCallbackFunc);
}

//停止监控
NTSTATUS monitor_stop()
{
	PsSetCreateProcessNotifyRoutine(ProcessCallbackFunc,TRUE);
	PsRemoveCreateThreadNotifyRoutine(ThreadCallbackFunc);
	PsRemoveLoadImageNotifyRoutine(LoadImageCallbackFunc);
}

//释放内存
VOID monitor_free()
{
	PMONITOR_INFO p = NULL;

	__try
	{
		p = pMonitorInfoHead;

		while(p!=NULL)
		{
			p = pMonitorInfoHead->next;
			ExFreePool(pMonitorInfoHead);		
			pMonitorInfoHead = p;
		}

		pMonitorInfoHead = (PMONITOR_INFO)ExAllocatePool(NonPagedPool,sizeof(MONITOR_INFO));
		pMonitorInfoHead->next = NULL;
		pMonitorInfo = pMonitorInfoHead;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("monitor_free EXCEPTION_EXECUTE_HANDLER error!");
	}
}


//获得大小
ULONG monitor_getCount()
{
	__try
	{
		PMONITOR_INFO p=NULL;
		ULONG ulCount=0;

		//获得大小
		for (p=pMonitorInfoHead->next;p;p=p->next)
		{
			ulCount++;
		}

		return ulCount;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("monitor_getCount EXCEPTION_EXECUTE_HANDLER error!");
		return 0;
	}	
}

//发送数据到ring3
NTSTATUS monitor_getInfo(PMONITOR_INFO pBuffer)
{
	PMONITOR_INFO p=NULL;
	ULONG ulIndex = 0;

	__try
	{
		//遍历
		for (p=pMonitorInfoHead->next;p;p=p->next)
		{
			RtlCopyMemory(pBuffer + ulIndex,p,sizeof(MONITOR_INFO));

			ulIndex++;
		}

		MUTEX_P(MKMutex);
		monitor_free();
        MUTEX_V(MKMutex);

		return STATUS_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("monitor_getInfo EXCEPTION_EXECUTE_HANDLER error!");
		return STATUS_UNSUCCESSFUL;
	}
}