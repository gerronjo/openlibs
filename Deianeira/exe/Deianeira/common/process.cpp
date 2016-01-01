#include "process.h"


//获取线程的状态
//ulPID:目标进程ID
//ulTID:目标线程ID
//返回:0:表示发生异常,1:表示进程处于挂起状态,2:表示进程没有被挂起
ULONG process_getThreadState(ULONG ulPID,ULONG ulTID)
{
	PNtQuerySystemInformation NtQuerySystemInformation;
	HMODULE hModule=LoadLibrary(L"ntdll.dll");
	if (hModule==NULL)
	{
		return 0;
	}

	NtQuerySystemInformation=(PNtQuerySystemInformation)GetProcAddress(hModule, "NtQuerySystemInformation");
	if (NtQuerySystemInformation==NULL)
	{
		FreeLibrary(hModule);
		return 0;
	}

	//枚举得到所有进程
	ULONG n = 0x100;
	PSYSTEM_PROCESSES sp = new SYSTEM_PROCESSES[n];

	while (NtQuerySystemInformation(
		SystemProcessesAndThreadsInformation,sp, n*sizeof(SYSTEM_PROCESSES), 0)
		== STATUS_INFO_LENGTH_MISMATCH)
	{
		delete[] sp;
		sp = new SYSTEM_PROCESSES[n = n * 2];
	}

	bool done = false;

	//遍历进程列表
	for (PSYSTEM_PROCESSES p = sp; !done;
		p = PSYSTEM_PROCESSES(PCHAR(p) + p->NextEntryDelta))
	{
		if (p->ProcessId==ulPID)
		{
			for(UINT32 i=0;i<p->ThreadCount;i++)
			{
				SYSTEM_THREADS systemThread=p->Threads[i];
				if(systemThread.ClientId.ulTID == ulTID) //找到线程              
				{
					if(systemThread.dwState == StateWait && systemThread.dwWaitReason == Suspended) //线程被挂起
					{
						delete[] sp; 
						FreeLibrary(hModule);
						return 1;
					}
					else
					{
						delete[] sp; 
						FreeLibrary(hModule);
						return 2;
					}
				}
			}
		}
		done = p->NextEntryDelta == 0;
	}

	delete[] sp; 
	FreeLibrary(hModule);
	return 0;
}

//内核挂起线程
//hDriver:驱动句柄
//ulTID:目标线程ID
//返回:true:表示挂起成功,false:表示挂起失败
bool process_suspendThread(HANDLE hDriver,ULONG ulTID){
	BOOL bRet = FALSE;
	DWORD dwRet = 0;

	bRet = DeviceIoControl(hDriver,IOCTL_PROCESS_SUSPEND,&ulTID,sizeof(ULONG),NULL,0,&dwRet,NULL);

	if (bRet)
	{
		return true;
	}

	return false;
}

//内核恢复线程
//hDriver:驱动句柄
//ulTID:目标线程ID
//返回:true:表示恢复成功,false:表示恢复失败
bool process_resumeThread(HANDLE hDriver,ULONG ulTID){
	BOOL bRet = FALSE;
	DWORD dwRet = 0;

	bRet = DeviceIoControl(hDriver,IOCTL_PROCESS_RESUME,&ulTID,sizeof(ULONG),NULL,0,&dwRet,NULL);

	if (bRet)
	{
		return true;
	}

	return false;
}

//内核结束线程
//hDriver:驱动句柄
//ulTID:目标线程ID
//返回:true:表示删除成功,false:表示删除失败
bool process_killThread(HANDLE hDriver,ULONG ulTID){
	BOOL bRet = FALSE;
	DWORD dwRet = 0;

	bRet = DeviceIoControl(hDriver,IOCTL_PROCESS_KILLTHREAD,&ulTID,sizeof(ULONG),NULL,0,&dwRet,NULL);

	if (bRet)
	{
		return true;
	}

	return false;
}

//获取进程的父进程ID
//ulTID:目标线程ID
//返回:-1:失败，否则成功
ULONG process_getProcessParentPID(ULONG ulPID)
{
	ULONG ulPPID = -1;
	PPROCESS_BASIC_INFORMATION pbi = new PROCESS_BASIC_INFORMATION();
	
	PNtQueryInformationProcess NtQueryInformationProcess;
	HMODULE hModule=LoadLibrary(L"ntdll.dll");
	if (hModule==NULL)
	{
		return -1;
	}

	NtQueryInformationProcess=(PNtQueryInformationProcess)GetProcAddress(hModule, "NtQueryInformationProcess");
	if (NtQueryInformationProcess==NULL)
	{
		FreeLibrary(hModule);
		return -1;
	}

	//打开进程获得进程句柄
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION,FALSE,ulPID);

	//如果打开进程失败
	if (hProcess==NULL)
	{
		return -1;
	}

	//查询进程信息
	NTSTATUS status = NtQueryInformationProcess(hProcess,ProcessBasicInformation,
		pbi,sizeof(PROCESS_BASIC_INFORMATION),NULL);

	//如果获得进程信息成功
	if (NT_SUCCESS(status))
	{
		//父进程ID
		ulPPID = pbi->InheritedFromUniqueProcessId;
	}

	//释放进程句柄
	CloseHandle (hProcess);

	delete[] pbi;

	//返回父进程ID
	return ulPPID;
}

//获取进程的状态
//ulPID:目标进程ID
//返回:0:表示发生异常,1:表示进程处于挂起状态,2:表示进程没有被挂起
ULONG process_getProcessState(ULONG ulPID)
{
	PNtQuerySystemInformation NtQuerySystemInformation;
	HMODULE hModule=LoadLibrary(L"ntdll.dll");
	if (hModule==NULL)
	{
		return 0;
	}

	NtQuerySystemInformation=(PNtQuerySystemInformation)GetProcAddress(hModule, "NtQuerySystemInformation");
	if (NtQuerySystemInformation==NULL)
	{
		FreeLibrary(hModule);
		return 0;
	}

	//枚举得到所有进程
	ULONG n = 0x100;
	PSYSTEM_PROCESSES sp = new SYSTEM_PROCESSES[n];

	while (NtQuerySystemInformation(
		SystemProcessesAndThreadsInformation,sp, n*sizeof(SYSTEM_PROCESSES), 0)
		== STATUS_INFO_LENGTH_MISMATCH)
	{
		delete[] sp;
		sp = new SYSTEM_PROCESSES[n = n * 2];
	}

	bool done = false;

	//遍历进程列表
	for (PSYSTEM_PROCESSES p = sp; !done;
		p = PSYSTEM_PROCESSES(PCHAR(p) + p->NextEntryDelta))
	{
		if (p->ProcessId==ulPID)
		{
			SYSTEM_THREADS systemThread=p->Threads[0];
			if (systemThread.dwState==5 && systemThread.dwWaitReason==5)
			{
				delete[] sp; 
				FreeLibrary(hModule);
				//进程处于挂起状态
				return 1;
			}
			else
			{
				delete[] sp; 
				FreeLibrary(hModule);
				//进程没有被挂起
				return 2;
			}
		}
		done = p->NextEntryDelta == 0;
	}

	delete[] sp; 
	FreeLibrary(hModule);
	return 0;
}

//锁掉进程
//ulPID:目标进程ID
//返回:true:表示挂起成功,false:表示挂起失败
bool process_suspendProcess(ULONG ulPID)
{
	bool bRet = false;

	PNtQuerySystemInformation NtQuerySystemInformation;
	HMODULE hModule=LoadLibrary(L"ntdll.dll");
	if (hModule==NULL)
	{
		return false;
	}

	NtQuerySystemInformation=(PNtQuerySystemInformation)GetProcAddress(hModule, "NtQuerySystemInformation");
	if (NtQuerySystemInformation==NULL)
	{
		FreeLibrary(hModule);
		return false;
	}

	//枚举得到所有进程
	ULONG n = 0x100;
	PSYSTEM_PROCESSES sp = new SYSTEM_PROCESSES[n];

	while (NtQuerySystemInformation(
		SystemProcessesAndThreadsInformation,sp, n*sizeof(SYSTEM_PROCESSES), 0)
		== STATUS_INFO_LENGTH_MISMATCH)
	{
		delete[] sp;
		sp = new SYSTEM_PROCESSES[n = n * 2];
	}

	bool done = false;

	//遍历进程列表
	for (PSYSTEM_PROCESSES p = sp; !done;
		p = PSYSTEM_PROCESSES(PCHAR(p) + p->NextEntryDelta))
	{
		if (p->ProcessId==ulPID)
		{
			SYSTEM_THREADS systemThread=p->Threads[0];
			if (systemThread.dwState==5 && systemThread.dwWaitReason!=5)
			{
				HANDLE hThread=OpenThread(THREAD_ALL_ACCESS,FALSE,systemThread.ClientId.ulTID); 
				if(!SuspendThread(hThread))
				{
					bRet = true;
				}
				else
				{
					bRet = false;
				}
				CloseHandle(hThread); 
			}
		}
		done = p->NextEntryDelta == 0;
	}

	delete[] sp; 
	FreeLibrary(hModule);
	return bRet;
}

//内核锁掉进程
//hDriver:驱动句柄
//ulPID:目标进程ID
//返回:true:表示挂起成功,false:表示挂起失败
bool process_suspendProcess(HANDLE hDriver,ULONG ulPID){
	BOOL bRet = FALSE;
	DWORD dwRet = 0;

	bRet = DeviceIoControl(hDriver,IOCTL_PROCESS_SUSPEND,&ulPID,sizeof(ULONG),NULL,0,&dwRet,NULL);

	if (bRet)
	{
		return true;
	}

	return false;
}

//恢复进程
//ulPID:目标进程ID
//返回:true:表示恢复成功,false:表示恢复失败
bool process_resumeProcess(ULONG ulPID)
{
	bool bRet = false;

	PNtQuerySystemInformation NtQuerySystemInformation;
	HMODULE hModule=LoadLibrary(L"ntdll.dll");
	if (hModule==NULL)
	{
		return false;
	}

	NtQuerySystemInformation=(PNtQuerySystemInformation)GetProcAddress(hModule, "NtQuerySystemInformation");
	if (NtQuerySystemInformation==NULL)
	{
		FreeLibrary(hModule);
		return false;
	}

	//枚举得到所有进程
	ULONG n = 0x100;
	PSYSTEM_PROCESSES sp = new SYSTEM_PROCESSES[n];

	while (NtQuerySystemInformation(
		SystemProcessesAndThreadsInformation,sp, n*sizeof(SYSTEM_PROCESSES), 0)
		== STATUS_INFO_LENGTH_MISMATCH)
	{
		delete[] sp;
		sp = new SYSTEM_PROCESSES[n = n * 2];
	}

	bool done = false;

	//遍历进程列表
	for (PSYSTEM_PROCESSES p = sp; !done;
		p = PSYSTEM_PROCESSES(PCHAR(p) + p->NextEntryDelta))
	{
		if (p->ProcessId==ulPID)
		{
			SYSTEM_THREADS systemThread=p->Threads[0];
			if (systemThread.dwState==5 && systemThread.dwWaitReason==5)
			{
				//在这里进行进程解锁的处理
				HANDLE hThread=OpenThread(THREAD_ALL_ACCESS,FALSE,systemThread.ClientId.ulTID); 
				if(ResumeThread(hThread))
				{
					//恢复成功
					bRet = true;
				}
				else
				{
					//恢复失败
					bRet = false;
				}
				CloseHandle(hThread);
			}
		}
		done = p->NextEntryDelta == 0;
	}

	delete[] sp; 
	FreeLibrary(hModule);
	return bRet;
}

//内核恢复进程
//hDriver:驱动句柄
//ulPID:目标进程ID
//返回:true:表示恢复成功,false:表示恢复失败
bool process_resumeProcess(HANDLE hDriver,ULONG ulPID){
	BOOL bRet = FALSE;
	DWORD dwRet = 0;

	bRet = DeviceIoControl(hDriver,IOCTL_PROCESS_RESUME,&ulPID,sizeof(ULONG),NULL,0,&dwRet,NULL);

	if (bRet)
	{
		return true;
	}

	return false;
}

//内核结束进程
//hDriver:驱动句柄
//ulPID:目标进程ID
//返回:true:表示恢复成功,false:表示恢复失败
bool process_killProcess(HANDLE hDriver,ULONG ulPID){
	BOOL bRet = FALSE;
	DWORD dwRet = 0;

	bRet = DeviceIoControl(hDriver,IOCTL_PROCESS_KILL2,&ulPID,sizeof(ULONG),NULL,0,&dwRet,NULL);

	if (bRet)
	{
		return true;
	}

	return false;
}

//内核结束进程,内存清零
//hDriver:驱动句柄
//ulPID:目标进程ID
//返回:true:表示恢复成功,false:表示恢复失败
bool process_killProcess_memclr(HANDLE hDriver,ULONG ulPID){
	BOOL bRet = FALSE;
	DWORD dwRet = 0;

	bRet = DeviceIoControl(hDriver,IOCTL_PROCESS_KILL,&ulPID,sizeof(ULONG),NULL,0,&dwRet,NULL);

	if (bRet)
	{
		return true;
	}

	return false;
}

//内核进程模块卸载
//hDriver:驱动句柄
//ulPID:目标进程ID
//ulDllBase:DLL模块基址
//返回:true:表示卸载成功,false:表示卸载失败
bool process_unloadModule(HANDLE hDriver,ULONG ulPID,ULONG ulDllBase){
	BOOL bRet = FALSE;
	DWORD dwRet = 0;

	UNLOADDLL_CLIENT_ID clientID;
	clientID.ulPID = ulPID;
	clientID.ulBaseAddress = ulDllBase;

	bRet = DeviceIoControl(hDriver,IOCTL_PROCESS_MODULE_UNLOAD,&clientID,sizeof(UNLOADDLL_CLIENT_ID),NULL,0,&dwRet,NULL);

	if (bRet)
	{
		return true;
	}

	return false;
}

//注入DLL
//lpFilePath:DLL文件路径
//ulPID:进程ID
//返回:true:注入成功,false:注入失败
bool process_injectDll(LPTSTR lpFilePath,ULONG ulPID)
{
	//获得进程权限
	HANDLE  hProcess = OpenProcess(PROCESS_VM_WRITE|PROCESS_CREATE_THREAD|PROCESS_VM_OPERATION, FALSE, ulPID); 

	//如果打开进程成功
	if(hProcess)
	{
		DWORD dwSize,dwWritten;
		//进程路径大小
		dwSize = sizeof(WCHAR)*lstrlenW(lpFilePath)+1;

		//在目标进程空间内，分配空间用于存放DLL文件路径
		LPVOID lpBuffer=VirtualAllocEx(hProcess,NULL,dwSize,MEM_COMMIT,PAGE_READWRITE);

		//如果分配空间失败
		if(lpBuffer==NULL)
		{
			CloseHandle(hProcess);
			return false;
		}

		//往刚分配的空间写入DLL文件路径
		if(WriteProcessMemory(hProcess,lpBuffer,lpFilePath,dwSize,&dwWritten))
		{
			//如果实际写入的数据不等于DLL路径大小
			if(dwWritten!=dwSize)
			{
				//释放刚才分配的空间
				VirtualFreeEx(hProcess,lpBuffer,dwSize,MEM_DECOMMIT);
				CloseHandle(hProcess);
				return false;
			} 

			//获得kernel动态链接库句柄
			HMODULE hModule=GetModuleHandle(L"Kernel32.dll");

			//如果获得失败
			if(hModule==NULL)
			{
				//释放刚才分配的空间
				VirtualFreeEx(hProcess,lpBuffer,dwSize,MEM_DECOMMIT);
				CloseHandle(hProcess);
				return false;
			}

			//获得模块中函数的地址
			LPTHREAD_START_ROUTINE pFunc=(LPTHREAD_START_ROUTINE)GetProcAddress(hModule,"LoadLibraryW");

			//获取函数地址失败
			if(pFunc==NULL)
			{
				//释放刚才分配的空间
				VirtualFreeEx(hProcess,lpBuffer,dwSize,MEM_DECOMMIT);
				CloseHandle(hProcess);
				return false;           
			}

			//在目标进程空间内创建线程
			HANDLE hThread=CreateRemoteThread(hProcess,NULL,0,pFunc,lpBuffer,0,NULL);
			//无限等待目标线程的结束
			WaitForSingleObject(hThread,INFINITE);
			//释放线程句柄
			CloseHandle(hThread);

			//释放刚才分配的空间
			VirtualFreeEx(hProcess,lpBuffer,dwSize,MEM_DECOMMIT);
			CloseHandle(hProcess);

			return true;
		}

		return false;
	}

	return false;
}

//获得进程的模块信息列表
//返回:true:成功,false:失败
bool process_getModuleInfoTable()
{
	PPROCESS_MODULE_INFO p;
	MODULEENTRY32	me32;
	me32.dwSize	= sizeof(MODULEENTRY32);

	//初始化
	pProcessModuleInfoHead = pProcessModuleInfo = new PROCESS_MODULE_INFO;

	//
	HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);

	if (!hModuleSnap)
	{
		return false;
	}

	//
	BOOL bRet = Module32First(hModuleSnap, &me32);

	while(bRet)
	{
		p = new PROCESS_MODULE_INFO;
		wcscpy_s(p->FileName,me32.szModule);
		wcscpy_s(p->FilePath,me32.szExePath);

		p->next = NULL;

		p->ulBaseAddress = (DWORD)me32.modBaseAddr;
		p->ulSize = (DWORD)(me32.modBaseSize);

		pProcessModuleInfo->next = p;
		pProcessModuleInfo = p;	

		pProcessModuleInfo = pProcessModuleInfoHead->next;

		//
		bRet = Module32Next(hModuleSnap, &me32);
	}

	CloseHandle(hModuleSnap);
	return true;
}

//获得进程线程列表
PSYSTEM_PROCESSES GetThreadList(ULONG ulPID) 
{ 
	PNtQuerySystemInformation NtQuerySystemInformation;
	HMODULE hModule=LoadLibrary(L"ntdll.dll");
	if (hModule==NULL)
	{
		return NULL;
	}

	NtQuerySystemInformation=(PNtQuerySystemInformation)GetProcAddress(hModule, "NtQuerySystemInformation");
	if (NtQuerySystemInformation==NULL)
	{
		FreeLibrary(hModule);
		return NULL;
	}

	//枚举得到所有进程
	ULONG n = 0x100;
	PSYSTEM_PROCESSES sp = new SYSTEM_PROCESSES[n];

	while (NtQuerySystemInformation(
		SystemProcessesAndThreadsInformation,sp, n*sizeof(SYSTEM_PROCESSES), 0)
		== STATUS_INFO_LENGTH_MISMATCH)
	{
		delete[] sp;
		sp = new SYSTEM_PROCESSES[n = n * 2];
	}

	bool done = false;

	//遍历进程列表
	for (PSYSTEM_PROCESSES p = sp; !done;
		p = PSYSTEM_PROCESSES(PCHAR(p) + p->NextEntryDelta))
	{
		if (p->ProcessId==ulPID) 
		{            
			FreeLibrary(hModule);
			//返回列表
			return p;
		} 
		done = p->NextEntryDelta == 0; 
	}

	return NULL;
}