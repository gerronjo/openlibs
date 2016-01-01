//**************************************************************************
//*
//*          A盾电脑防护 website:http://www.3600safe.com/
//*        
//*文件说明：
//*         枚举进程
//**************************************************************************
#include "Process.h"

ULONG IsHideProcess(ULONG ulEProcess,PPROCESSINFO Info)
{
	SYSTEM_PROCESSES *pProcess;
	PVOID buf;
	ULONG n = 0;
	NTSTATUS status;
	BOOL bInit = FALSE;
	BOOL bRetOK = FALSE;
	int i=0;
	PEPROCESS EProcess;

	char *lpszProName = NULL;
	BOOL bIsIn = FALSE;
	ULONG ulPid;

	WIN_VER_DETAIL WinVer;
	PEPROCESS pCurrentEprocess;
	PLIST_ENTRY	pListActiveProcess;
	ULONG g_Offset_Eprocess_ProcessId;
	ULONG g_Offset_Eprocess_Flink;
	ULONG		ulCurrentProcessId = 0;
	ULONG		ulStartProcessId = 0; 
	ULONG		uCount = 0;

	//列举进程的时候，先暂停修改结构
	if (bPaused)
	{
		return bRetOK;
	}
	//如果ring3退出了
	if (!IsExitProcess(ProtectEProcess))
	{
		return bRetOK;
	}
	if (!WinVersion)
		WinVer = GetWindowsVersion();
	else
		WinVer = WinVersion;

	switch(WinVer)
	{
	case WINDOWS_VERSION_XP:
		g_Offset_Eprocess_ProcessId = 0x84;
		g_Offset_Eprocess_Flink = 0x88;
		break;
	case WINDOWS_VERSION_7:
		g_Offset_Eprocess_ProcessId = 0xb4;
		g_Offset_Eprocess_Flink = 0xb8;
		break;
	case WINDOWS_VERSION_VISTA_2008:
		g_Offset_Eprocess_ProcessId = 0x09c;
		g_Offset_Eprocess_Flink = 0x0a0;
		break;
	case WINDOWS_VERSION_2K3_SP1_SP2:
		g_Offset_Eprocess_ProcessId = 0x94;
		g_Offset_Eprocess_Flink = 0x98;
		break;
	case WINDOWS_VERSION_2K3:
		g_Offset_Eprocess_ProcessId = 0x084;
		g_Offset_Eprocess_Flink = 0x088;
		break;
	}
	if (!g_Offset_Eprocess_ProcessId ||
		!g_Offset_Eprocess_Flink)
	{
		return bRetOK;
	}
	bRetOK = 8;  //初始化，表示所有隐藏

	pCurrentEprocess = SystemEProcess;
	__try
	{
		while(pCurrentEprocess != 0)
		{
			if (ulEProcess == pCurrentEprocess)
			{
				//正常进程
				bRetOK = 0;
				break;
			}
			pListActiveProcess = (LIST_ENTRY *)((ULONG)pCurrentEprocess + g_Offset_Eprocess_Flink);
			(ULONG)pCurrentEprocess = (ULONG)pListActiveProcess->Flink - g_Offset_Eprocess_Flink;
			if (pCurrentEprocess == SystemEProcess)
			{
				break;
			}
		}

	}__except(EXCEPTION_EXECUTE_HANDLER){

	}
	
	//发现隐藏进程！！！
	if (bRetOK == 8)
	{
		__try
		{
			ulPid = *(ULONG *)(ulEProcess + g_Offset_Eprocess_ProcessId);

			for (i=0;i<100;i++)
			{
				if (Info->ProcessInfo[i].EProcess == ulEProcess)
				{
					break;
				}
				if (!IsExitProcess(Info->ProcessInfo[i].EProcess))
				{
					Info->ProcessInfo[i].IntHideType = 1;
					Info->ProcessInfo[i].ulPid = ulPid;
					Info->ProcessInfo[i].EProcess = ulEProcess;
					Info->ulCount++;
					break;
				}
			}
		}__except(EXCEPTION_EXECUTE_HANDLER){

		}
	}
	return bRetOK;
}
BOOL GetNormalProcessList(PPROCESSINFO Info,PPROCESSINFO HideInfo)
{
	SYSTEM_PROCESSES *pProcess;
	PVOID buf;
	ULONG n = 0;
	NTSTATUS status;
	PEPROCESS EProcess;
	BOOL bRetOK = FALSE;
	BOOL bInit = FALSE;
	BOOL bNoHided = FALSE;
	int i=0,x=0,y=0;
	WIN_VER_DETAIL WinVer;
	PEPROCESS pCurrentEprocess;
	PLIST_ENTRY	pListActiveProcess;
	ULONG g_Offset_Eprocess_ProcessId;
	ULONG g_Offset_Eprocess_Flink;
	ULONG		ulCurrentProcessId = 0;
	ULONG		ulStartProcessId = 0; 
	ULONG		uCount = 0;
	WCHAR *lpwzNtFullPath = NULL;

	if (!WinVersion)
		WinVer = GetWindowsVersion();
	else
		WinVer = WinVersion;

	switch(WinVer)
	{
	case WINDOWS_VERSION_XP:
		g_Offset_Eprocess_ProcessId = 0x84;
		g_Offset_Eprocess_Flink = 0x88;
		break;
	case WINDOWS_VERSION_7:
		g_Offset_Eprocess_ProcessId = 0xb4;
		g_Offset_Eprocess_Flink = 0xb8;
		break;
	case WINDOWS_VERSION_VISTA_2008:
		g_Offset_Eprocess_ProcessId = 0x09c;
		g_Offset_Eprocess_Flink = 0x0a0;
		break;
	case WINDOWS_VERSION_2K3_SP1_SP2:
		g_Offset_Eprocess_ProcessId = 0x94;
		g_Offset_Eprocess_Flink = 0x98;
		break;
	case WINDOWS_VERSION_2K3:
		g_Offset_Eprocess_ProcessId = 0x084;
		g_Offset_Eprocess_Flink = 0x088;
		break;
	}
	if (!g_Offset_Eprocess_ProcessId ||
		!g_Offset_Eprocess_Flink)
	{
		return bRetOK;
	}

	__try
	{
		pCurrentEprocess = SystemEProcess;
		while(pCurrentEprocess != 0)
		{
			if (DebugOn)
				KdPrint(("%08x\r\n",pCurrentEprocess));

			if (IsExitProcess(pCurrentEprocess))
			{
				Info->ProcessInfo[i].IntHideType = 0;
				Info->ProcessInfo[i].ulPid = *((PULONG)((ULONG)pCurrentEprocess + g_Offset_Eprocess_ProcessId));

				Info->ProcessInfo[i].EProcess = pCurrentEprocess;
				memset(Info->ProcessInfo[i].lpwzFullProcessPath,0,sizeof(Info->ProcessInfo[i].lpwzFullProcessPath));

				if (!GetProcessImagePath(Info->ProcessInfo[i].ulPid,Info->ProcessInfo[i].lpwzFullProcessPath))
					wcsncat(Info->ProcessInfo[i].lpwzFullProcessPath,L"Unknown",wcslen(L"Unknown"));

				if (DebugOn)
					KdPrint(("%ws",Info->ProcessInfo[i].lpwzFullProcessPath));

				Info->ProcessInfo[i].ulKernelOpen = KernelStatus(Info->ProcessInfo[i].ulPid);
				Info->ProcessInfo[i].ulInheritedFromProcessId = GetInheritedProcessPid(Info->ProcessInfo[i].ulPid);
				i++;
				Info->ulCount = i;
			}
			pListActiveProcess = (LIST_ENTRY *)((ULONG)pCurrentEprocess + g_Offset_Eprocess_Flink);
			(ULONG)pCurrentEprocess = (ULONG)pListActiveProcess->Flink - g_Offset_Eprocess_Flink;
			if (pCurrentEprocess == SystemEProcess)
			{
				break;
			}
		}

	}__except(EXCEPTION_EXECUTE_HANDLER){

	}

	for (x=0;x<100;x++)
	{
		//进程是否退出
		if (IsExitProcess(HideInfo->ProcessInfo[x].EProcess))
		{
			bNoHided = FALSE;
			//再来一次准确的判断，如果存在于正常链表，则表示非隐藏进程
			pCurrentEprocess = SystemEProcess;

			while(pCurrentEprocess != 0)
			{
				if (HideInfo->ProcessInfo[x].EProcess == pCurrentEprocess)
				{
					//正常进程
					bNoHided = TRUE;
					break;
				}
				pListActiveProcess = (LIST_ENTRY *)((ULONG)pCurrentEprocess + g_Offset_Eprocess_Flink);
				(ULONG)pCurrentEprocess = (ULONG)pListActiveProcess->Flink - g_Offset_Eprocess_Flink;
				if (pCurrentEprocess == SystemEProcess)
				{
					bNoHided = TRUE;
					break;
				}
			}
			//隐藏进程
			if (!bNoHided)
			{
				Info->ProcessInfo[i].IntHideType = HideInfo->ProcessInfo[x].IntHideType;
				Info->ProcessInfo[i].ulPid = HideInfo->ProcessInfo[x].ulPid;
				Info->ProcessInfo[i].EProcess = HideInfo->ProcessInfo[x].EProcess;

				memset(Info->ProcessInfo[i].lpwzFullProcessPath,0,sizeof(Info->ProcessInfo[i].lpwzFullProcessPath));

				if (!GetProcessImagePath(HideInfo->ProcessInfo[x].ulPid,Info->ProcessInfo[i].lpwzFullProcessPath))
					wcsncat(Info->ProcessInfo[i].lpwzFullProcessPath,L"Unknown",wcslen(L"Unknown"));

				Info->ProcessInfo[i].ulKernelOpen = KernelStatus(HideInfo->ProcessInfo[x].ulPid);
				Info->ProcessInfo[i].ulInheritedFromProcessId = GetInheritedProcessPid(HideInfo->ProcessInfo[x].ulPid);
				i++;
				Info->ulCount = i;
			}
		}
	}
	return bRetOK;
}