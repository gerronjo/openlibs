#include "protection.h"


//用于保存上一次NtUserGetForegroundWindow的结果
HWND hWndLastForeground;

PVOID pNtOpenProcess;
PVOID pNtOpenThread;


//NtOpenThread
NTSTATUS NewObOpenObjectByPointer_forNtOpenThread(IN PVOID  Object,IN ULONG  HandleAttributes,
												  IN PACCESS_STATE  PassedAccessState  OPTIONAL,
												  IN ACCESS_MASK  DesiredAccess  OPTIONAL,
												  IN POBJECT_TYPE  ObjectType  OPTIONAL,
												  IN KPROCESSOR_MODE  AccessMode,OUT PHANDLE  Handle)
{
	//如果不是自己操作自己的话
	if (PsGetCurrentProcessId() != ulProcessID)
	{
		//如果打开的目标线程所在的进程是r3应用程序的话
		if (IoThreadToProcess(Object) == pEProcess)
		{
			return STATUS_ACCESS_DENIED;
		}
	}

	return ObOpenObjectByPointer(Object,
		HandleAttributes,
		PassedAccessState,
		DesiredAccess,
		ObjectType,
		AccessMode,
		Handle
		);
}

//NtOpenProcess
NTSTATUS NewObOpenObjectByPointer_forNtOpenProcess(IN PVOID  Object,IN ULONG  HandleAttributes,
												   IN PACCESS_STATE  PassedAccessState  OPTIONAL,
												   IN ACCESS_MASK  DesiredAccess  OPTIONAL,
												   IN POBJECT_TYPE  ObjectType  OPTIONAL,
												   IN KPROCESSOR_MODE  AccessMode,OUT PHANDLE  Handle
												   )
{
	//如果不是自己操作自己的话
	if (PsGetCurrentProcessId() != ulProcessID)
	{
		//如果打开的目标进程是r3应用程序的话
		if (Object == pEProcess)
		{
			return STATUS_ACCESS_DENIED;
		}
	}

	return ObOpenObjectByPointer(Object,
		HandleAttributes,
		PassedAccessState,
		DesiredAccess,
		ObjectType,
		AccessMode,
		Handle
		);
}


//shadow ssdt hook
NTSTATUS NewNtUserBuildHwndList(
								IN HDESK hdesk,
								IN HWND hwndNext,
								IN BOOL fEnumChildren,
								IN DWORD idThread,
								IN UINT cHwndMax,
								OUT HWND *phwndFirst,
								OUT PUINT pcHwndNeeded)
{
	NTSTATUS status;

	if (PsGetCurrentProcessId()!= ulProcessID)
	{
		ULONG ProcessID;

		if (!fEnumChildren)
		{
			//得到进程ID
			ProcessID = OldNtUserQueryWindow(hwndNext, 0);

			//如果等于本地应用程序，返回失败
			if (ProcessID == ulProcessID)
			{
				return STATUS_UNSUCCESSFUL;
			}
		}

		status = OldNtUserBuildHwndList(hdesk,hwndNext,fEnumChildren,idThread,cHwndMax,phwndFirst,pcHwndNeeded);

		if (NT_SUCCESS(status))
		{
			ULONG i=0;
			ULONG j;

			while (i < *pcHwndNeeded)
			{
				//查找对应句柄的进程ID
				ProcessID=OldNtUserQueryWindow(phwndFirst[i],0);

				//如果是要保护的进程ID
				if (ProcessID == ulProcessID)
				{
					//从列表里面去掉
					for (j = i; j < (*pcHwndNeeded) - 1; j++)
					{
						phwndFirst[j] = phwndFirst[j+1]; 
					}

					//最后一个赋值为0，为了避免和倒数第二个重复
					phwndFirst[*pcHwndNeeded-1] = 0; 
					
					//相应的减1
					(*pcHwndNeeded)--;

					//继续查找其他窗体，因为一个程序可能不止一个窗体
					continue; 
				}

				i++;
			}
		}

		return status;
	}

	return OldNtUserBuildHwndList(hdesk,hwndNext,fEnumChildren,idThread,cHwndMax,phwndFirst,pcHwndNeeded);
}

BOOL NewNtUserDestroyWindow(
							IN HWND hwnd)
{
	//如果不是自己操作自己
	if (PsGetCurrentProcessId() != ulProcessID)
	{
		ULONG ulPID;
		//查询指定句柄的进程ID
		ulPID = (ULONG)OldNtUserQueryWindow(hwnd, 0);

		//如果指定句柄的进程ID是要保护的进程ID
		if (ulPID==ulProcessID)
		{
			return FALSE;
		}
	}

	//如果是自己操作自己的话，直接返回结果
	return OldNtUserDestroyWindow(hwnd);
}


HWND NewNtUserFindWindowEx(
							  IN HWND hwndParent,
							  IN HWND hwndChild,
							  IN PUNICODE_STRING pstrClassName OPTIONAL,
							  IN PUNICODE_STRING pstrWindowName OPTIONAL,
							  IN DWORD dwType)
{
	HWND hWnd;

	hWnd = OldNtUserFindWindowEx(hwndParent, hwndChild, pstrClassName, pstrWindowName, dwType);

	//如果不是自己操作自己
	if (PsGetCurrentProcessId() != ulProcessID)
	{
		ULONG ulPID;
        //查询指定句柄的进程ID
		ulPID = (ULONG)OldNtUserQueryWindow(hWnd, 0);

		//如果指定句柄的进程ID是要保护的进程ID
		if (ulPID==ulProcessID)
		{
			return NULL;
		}
	}

	//如果是自己操作自己的话，直接返回结果
	return hWnd;;
}

HWND NewNtUserGetForegroundWindow(VOID)
{
	HWND hWnd;

	hWnd = OldNtUserGetForegroundWindow();   

	//如果不是自己操作自己
	if (PsGetCurrentProcessId()!=ulProcessID)
	{
		ULONG ulPID;
		//查询指定句柄的进程ID
		ulPID = (ULONG)OldNtUserQueryWindow(hWnd, 0);

		if (ulPID == ulProcessID)
		{
			hWnd = hWndLastForeground;
		}
		else
		{
			hWndLastForeground = hWnd;
		}		
	}   

	//如果是自己操作自己的话，直接返回结果
	return hWnd;
}

LRESULT NewNtUserMessageCall(
							 IN HWND hwnd,
							 IN UINT msg,
							 IN WPARAM wParam,
							 IN LPARAM lParam,
							 IN ULONG_PTR xParam,
							 IN DWORD xpfnProc,
							 IN BOOL bAnsi)
{
	ULONG ulPID;

	//如果不是自己操作自己
	if (PsGetCurrentProcessId()!=ulProcessID)
	{
		//获得目标句柄的进程ID
		ulPID = (ULONG)OldNtUserQueryWindow(hwnd,0);

		//如果是要保护的进程ID
		if (ulPID == ulProcessID)
		{
			return FALSE;
		}
	}

	//不是的话
	return OldNtUserMessageCall(hwnd,msg,wParam,lParam,xParam,xpfnProc,bAnsi);
}

BOOL NewNtUserPostMessage(
						  IN HWND hwnd,
						  IN UINT msg,
						  IN WPARAM wParam,
						  IN LPARAM lParam
						  )
{
	ULONG ulPID;

	//如果不是自己操作自己
	if (PsGetCurrentProcessId()!=ulProcessID)
	{
		//获得目标句柄的进程ID
		ulPID = (ULONG)OldNtUserQueryWindow(hwnd,0);

		//如果是要保护的进程ID
		if (ulPID == ulProcessID)
		{
			return FALSE;
		}
	}

	//不是的话
	return OldNtUserPostMessage(hwnd,msg,wParam,lParam);
}

BOOL NewNtUserPostThreadMessage(
								IN DWORD id,
								IN UINT msg,
								IN WPARAM wParam,
								IN LPARAM lParam)
{
	PETHREAD pEThread;
	NTSTATUS status;

	//如果不是自己操作自己
	if (PsGetCurrentProcessId()!=ulProcessID)
	{
		//PETHREAD
		status = PsLookupThreadByThreadId((HANDLE)id,&pEThread);

		if (NT_SUCCESS(status))
		{
			if (IoThreadToProcess(pEThread) == pEProcess)
			{
				return FALSE;
			}
			else
			{
				return OldNtUserPostThreadMessage(id,msg,wParam,lParam);
			}
		}
	}
	
	return OldNtUserPostThreadMessage(id,msg,wParam,lParam);;
}

HANDLE NewNtUserQueryWindow(
							 IN HWND WindowHandle,
							 IN ULONG TypeInformation)
{
	ULONG ulPID;

	//如果不是自己操作自己
	if (PsGetCurrentProcessId()!=ulProcessID)
	{
		//获得目标句柄的进程ID
		ulPID = (ULONG)OldNtUserQueryWindow(WindowHandle,0);

		//如果是要保护的进程ID
		if (ulPID == ulProcessID)
		{
			return 0;
		}
	}

	//不是的话
	return OldNtUserQueryWindow(WindowHandle,TypeInformation);
}

LONG NewNtUserSetWindowLong(
							IN HWND hwnd,
							IN int nIndex,
							IN LONG dwNewLong,
							IN BOOL bAnsi)
{
	//如果不是自己操作自己
	if (PsGetCurrentProcessId() != ulProcessID)
	{
		ULONG ulPID;
		//查询指定句柄的进程ID
		ulPID = (ULONG)OldNtUserQueryWindow(hwnd, 0);

		//如果指定句柄的进程ID是要保护的进程ID
		if (ulPID==ulProcessID)
		{
			return NULL;
		}
	}

	//如果是自己操作自己的话，直接返回结果
	return OldNtUserSetWindowLong(hwnd,nIndex,dwNewLong,bAnsi);
}

BOOL NewNtUserShowWindow(
						 IN HWND hwnd,
						 IN int nCmdShow)
{
	//如果不是自己操作自己
	if (PsGetCurrentProcessId() != ulProcessID)
	{
		ULONG ulPID;
		//查询指定句柄的进程ID
		ulPID = (ULONG)OldNtUserQueryWindow(hwnd, 0);

		//如果指定句柄的进程ID是要保护的进程ID
		if (ulPID==ulProcessID)
		{
			return NULL;
		}
	}

	//如果是自己操作自己的话，直接返回结果
	return OldNtUserShowWindow(hwnd,nCmdShow);
}

HWND NewNtUserWindowFromPoint(LONG x, LONG y)
{
	return 0;
}


//
VOID protection_startInlineHook()
{

	//保存原有地址
	pNtOpenProcess = GetExportedFuncAddress(L"NtOpenProcess");

	//如果获得地址失败
	if (pNtOpenProcess==NULL)
	{
		DbgPrint("protection_startInlineHook pNtOpenProcess error!");
		return;
	}

	pNtOpenThread = GetExportedFuncAddress(L"NtOpenThread");

	//如果获得地址失败
	if (pNtOpenThread==NULL)
	{
		DbgPrint("protection_startInlineHook pNtOpenThread error!");
		return;
	}

	//inline hook
	common_callAddrHook(pNtOpenProcess,ObOpenObjectByPointer,NewObOpenObjectByPointer_forNtOpenProcess,PAGE_SIZE);
	common_callAddrHook(pNtOpenThread,ObOpenObjectByPointer,NewObOpenObjectByPointer_forNtOpenThread,PAGE_SIZE);
}

//
VOID protection_stopInlineHook()
{
	//把地址替换回来 
	common_callAddrHook(pNtOpenProcess,NewObOpenObjectByPointer_forNtOpenProcess,ObOpenObjectByPointer,PAGE_SIZE);
	common_callAddrHook(pNtOpenThread,NewObOpenObjectByPointer_forNtOpenThread,ObOpenObjectByPointer,PAGE_SIZE);
}


//开启shadow ssdt hook
VOID protection_startShadowSSDTHook()
{
	//附加到GUI进程地址空间
	KeAttachProcess(pEProcess);

	_try
	{
		if (ulOSVersion == OS_WINDOWS_XP)
		{
			//首先保存原来的地址
			OldNtUserBuildHwndList = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserBuildHwndList_XP];
			OldNtUserDestroyWindow = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserDestroyWindow_XP];
			OldNtUserFindWindowEx = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserFindWindowEx_XP];						;
			OldNtUserGetForegroundWindow = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserGetForegroundWindow_XP];
            OldNtUserMessageCall = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserMessageCall_XP];
			OldNtUserPostMessage = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserPostMessage_XP];
			OldNtUserPostThreadMessage = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserPostThreadMessage_XP];
			OldNtUserQueryWindow = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserQueryWindow_XP];
			OldNtUserSetWindowLong = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserSetWindowLong_XP];
			OldNtUserShowWindow = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserShowWindow_XP];
			OldNtUserWindowFromPoint = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserWindowFromPoint_XP];		
		}
		else if (ulOSVersion == OS_WINDOWS_7_2008R2)
		{
			//首先保存原来的地址
			OldNtUserBuildHwndList = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserBuildHwndList_WIN7];
			OldNtUserDestroyWindow = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserDestroyWindow_WIN7];
			OldNtUserFindWindowEx = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserFindWindowEx_WIN7];						;
			OldNtUserGetForegroundWindow = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserGetForegroundWindow_WIN7];
			OldNtUserMessageCall = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserMessageCall_WIN7];
			OldNtUserPostMessage = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserPostMessage_WIN7];
			OldNtUserPostThreadMessage = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserPostThreadMessage_WIN7];
			OldNtUserQueryWindow = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserQueryWindow_WIN7];
			OldNtUserSetWindowLong = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserSetWindowLong_WIN7];
			OldNtUserShowWindow = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserShowWindow_WIN7];
			OldNtUserWindowFromPoint = KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserWindowFromPoint_WIN7];
		}



		//去除内存保护
		UnProtected();

		if (ulOSVersion == OS_WINDOWS_XP)
		{
			//重新定位shadow SSDT地址
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserBuildHwndList_XP] = NewNtUserBuildHwndList;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserDestroyWindow_XP] = NewNtUserDestroyWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserFindWindowEx_XP] = NewNtUserFindWindowEx;			
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserGetForegroundWindow_XP] = NewNtUserGetForegroundWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserMessageCall_XP] = NewNtUserMessageCall;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserPostMessage_XP] = NewNtUserPostMessage;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserPostThreadMessage_XP] = NewNtUserPostThreadMessage;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserQueryWindow_XP] = NewNtUserQueryWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserSetWindowLong_XP] = NewNtUserSetWindowLong;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserShowWindow_XP] = NewNtUserShowWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserWindowFromPoint_XP] = NewNtUserWindowFromPoint;
		}
		else if (ulOSVersion == OS_WINDOWS_7_2008R2)
		{
			//重新定位shadow SSDT地址
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserBuildHwndList_WIN7] = NewNtUserBuildHwndList;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserDestroyWindow_WIN7] = NewNtUserDestroyWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserFindWindowEx_WIN7] = NewNtUserFindWindowEx;			
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserGetForegroundWindow_WIN7] = NewNtUserGetForegroundWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserMessageCall_WIN7] = NewNtUserMessageCall;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserPostMessage_WIN7] = NewNtUserPostMessage;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserPostThreadMessage_WIN7] = NewNtUserPostThreadMessage;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserQueryWindow_WIN7] = NewNtUserQueryWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserSetWindowLong_WIN7] = NewNtUserSetWindowLong;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserShowWindow_WIN7] = NewNtUserShowWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserWindowFromPoint_WIN7] = NewNtUserWindowFromPoint;
		}
		

		//恢复内存保护
		Protected();
	}
	__finally
	{
		KeDetachProcess();
	}
}

//停止shadow ssdt hook
VOID protection_stopShadowSSDTHook()
{
	//附加到GUI进程地址空间
	KeAttachProcess(pEProcess);

	_try
	{
		//去除内存保护
		UnProtected();

		if (ulOSVersion == OS_WINDOWS_XP)
		{
			//重新定位shadow SSDT地址
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserBuildHwndList_XP] = OldNtUserBuildHwndList;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserDestroyWindow_XP] = OldNtUserDestroyWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserFindWindowEx_XP] = OldNtUserFindWindowEx;			
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserGetForegroundWindow_XP] = OldNtUserGetForegroundWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserMessageCall_XP] = OldNtUserMessageCall;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserPostMessage_XP] = OldNtUserPostMessage;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserPostThreadMessage_XP] = OldNtUserPostThreadMessage;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserQueryWindow_XP] = OldNtUserQueryWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserSetWindowLong_XP] = OldNtUserSetWindowLong;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserShowWindow_XP] = OldNtUserShowWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserWindowFromPoint_XP] = OldNtUserWindowFromPoint;
		}
		else if (ulOSVersion == OS_WINDOWS_7_2008R2)
		{
			//重新定位shadow SSDT地址
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserBuildHwndList_WIN7] = OldNtUserBuildHwndList;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserDestroyWindow_WIN7] = OldNtUserDestroyWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserFindWindowEx_WIN7] = OldNtUserFindWindowEx;			
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserGetForegroundWindow_WIN7] = OldNtUserGetForegroundWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserMessageCall_WIN7] = OldNtUserMessageCall;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserPostMessage_WIN7] = OldNtUserPostMessage;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserPostThreadMessage_WIN7] = OldNtUserPostThreadMessage;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserQueryWindow_WIN7] = OldNtUserQueryWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserSetWindowLong_WIN7] = OldNtUserSetWindowLong;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserShowWindow_WIN7] = OldNtUserShowWindow;
			KeServiceDescriptorTableShadow[1].ServiceTableBase[NtUserWindowFromPoint_WIN7] = OldNtUserWindowFromPoint;
		}

		//恢复内存保护
		Protected();
	}
	__finally
	{
		KeDetachProcess();
	}
}