#include "StdAfx.h"
#include "MySystem.h"

HANDLE  CMySystem:: hMyHandle=(HANDLE)NULL;
BOOL  CMySystem:: bdrv=true;
DWORD  CMySystem:: controlbuff[64]={0};
char CMySystem:: outputbuff[256]={0}; 
DWORD CMySystem:: dw=0;
DWORD CMySystem::  stringcount=0;
char * CMySystem::strings[256]={0}; 

ULONG CMySystem::NTBase = 0;			//NT 基址
ULONG CMySystem::TotalSSDTCount = -1;		//SSDT服务个数
pSSDTSaveTable CMySystem::pSSDTST = NULL;	//保存的SSDT缓冲表
PSYSMODULELIST CMySystem::pList = NULL;	//模块信息链表
ULONG CMySystem::RealCount = 0;		//枚举所找到的服务个数
pNtQuerySystemInformationProto CMySystem::pNtQuerySystemInformation = NULL;
CMySystem::CMySystem(void)
{
}

CMySystem::~CMySystem(void)
{
}

int CMySystem::HexToInt(CString Hex)
{
	int ret=0;
	int Tem=1;

	for (int i=0;i<Hex.GetLength();i++)
	{

		ret+=(Hex.GetAt( Hex.GetLength()-i-1)-0x30)*Tem;
		Tem*=16;
	}

	return ret;
}
__int64 CMySystem::HexToBigInt(CString Hex)
{
	TCHAR *value=Hex.GetBuffer();
	struct CHexMap
	{
		TCHAR chr;
		int value;
	};
	const int HexMapL = 16;
	CHexMap HexMap[HexMapL] =
	{
		{'0', 0}, {'1', 1},
		{'2', 2}, {'3', 3},
		{'4', 4}, {'5', 5},
		{'6', 6}, {'7', 7},
		{'8', 8}, {'9', 9},
		{'A', 10}, {'B', 11},
		{'C', 12}, {'D', 13},
		{'E', 14}, {'F', 15}
	};
	TCHAR *mstr = _tcsupr(_tcsdup(value));
	TCHAR *s = mstr;
	__int64 result = 0;
	if (*s == '0' && *(s + 1) == 'X') s += 2;
	bool firsttime = true;
	while (*s != '\0')
	{
		bool found = false;
		for (int i = 0; i < HexMapL; i++)
		{
			if (*s == HexMap[i].chr)
			{
				if (!firsttime) result <<= 4;
				result |= HexMap[i].value;
				found = true;
				break;
			}
		}
		if (!found) break;
		s++;
		firsttime = false;
	}
	free(mstr);
	return result;
}
void CMySystem::CharToWCHAR(WCHAR* wctar,char*cSource)
{
	int   nLen  =   0;   
	int   nwLen =   0;  
	nLen   =   strlen(cSource)   +   1;    
	nwLen  =   MultiByteToWideChar(CP_ACP,   0,  cSource,   nLen,   NULL,   0);  
	MultiByteToWideChar(CP_ACP,   0,   cSource,   nLen,   wctar,   nwLen);   

}
void CMySystem::WCHARToChar(char*cSource,WCHAR* wctar)
{

   int   nLen   =   wcslen(wctar)+1;     
   WideCharToMultiByte(CP_ACP,   0,   wctar,   nLen,   cSource,   2*nLen,   NULL,   NULL);  
}
//BOOL CKsBinSwordApp::EnablePrivilege(HANDLE hToken,LPCTSTR szPrivName,BOOL fEnable)
BOOL CMySystem::EnablePrivilege(LPCTSTR szPrivName)
{
	/*
	TOKEN_PRIVILEGES tp;
	tp.PrivilegeCount = 1;

	LookupPrivilegeValue (NULL, szPrivName, &tp.Privileges[0]. Luid);

	tp.Privileges[0].Attributes = fEnable ? SE_PRIVILEGE_ENABLED:0;

	AdjustTokenPrivileges (hToken, FALSE, &tp, sizeof(tp), NULL, NULL);

	return((GetLastError() == ERROR_SUCCESS));
	*/


	HANDLE hToken;
	TOKEN_PRIVILEGES tp;
	LUID luid;
	//打开进程令牌环
	if(!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,
		&hToken) )
	{
		printf("OpenProcessToken error.\n");
		return true;
	}
	//获得进程本地唯一ID
	if(!LookupPrivilegeValue(NULL,szPrivName,&luid) )
	{
		printf("LookupPrivilege error!\n");
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	tp.Privileges[0].Luid = luid;
	//调整权限
	if(!AdjustTokenPrivileges(hToken,0,&tp,sizeof(TOKEN_PRIVILEGES),NULL,NULL) )
	{
		printf("AdjustTokenPrivileges error!\n");
		return true;
	}

	return false;
}

DWORD CMySystem::GetPidByName(LPCWSTR szName) 
{
	HANDLE hProcessSnap = INVALID_HANDLE_VALUE;
	PROCESSENTRY32 pe32 = {0};
	DWORD dwRet=0;

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
		return 0;

	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First (hProcessSnap, &pe32)) {
		do {
			if (lstrcmpiW(szName, pe32.szExeFile) == 0) {
				dwRet=pe32.th32ProcessID;
				break;
			}
		}while (Process32Next(hProcessSnap,&pe32));
	}
	else {
		return 0;
	}

	if(hProcessSnap !=INVALID_HANDLE_VALUE) {
		CloseHandle(hProcessSnap);
	}

	return dwRet;
}

int CMySystem::GetProNum(HANDLE hMyHandle)
{
	int ProNum=0;
	unsigned long  DeviceRet=0;
	DeviceIoControl(
		hMyHandle  ,       // handle to volume
		IOCTL_KSBINSWORD_GETPRONUM,               // dwIoControlCode
		&ProNum,           // MOVE_FILE_DATA structure
		2*sizeof(int),         // size of input buffer
		&ProNum,                          // lpOutBuffer
		2*sizeof(int),                             // nOutBufferSize
		&DeviceRet,                // number of bytes returned
		0); // OVERLAPPED structure

	return  ProNum;
}
DWORD CMySystem::GetPro(HANDLE hMyHandle,void*ProcessNum,int size)
{
	unsigned long  DeviceRet=0;
	DWORD ErrCode=0;
	int ret=DeviceIoControl(
		hMyHandle  ,       // handle to volume
		IOCTL_KSBINSWORD_GETPRO,               // dwIoControlCode
		ProcessNum,           // MOVE_FILE_DATA structure
		size,         // size of input buffer
		ProcessNum,                          // lpOutBuffer
		size,                             // nOutBufferSize
		&DeviceRet,                // number of bytes returned
		0); // OVERLAPPED structure
	//wsprintf(ErrCode,L":%x,%d\n",size,GetLastError());
	ErrCode=GetLastError();
    TRACE(":%x,%d\n",size,ErrCode);
	//AfxMessageBox(ErrCode);
	return ErrCode;
}
bool CMySystem::ClearPro(HANDLE hMyHandle)
{
	int ProNum=0;
	unsigned long  DeviceRet=0;

	int ret=DeviceIoControl(
		hMyHandle  ,       // handle to volume
		IOCTL_KSBINSWORD_CLEARPROCESS,               // dwIoControlCode
		&ProNum,           // MOVE_FILE_DATA structure
		4,         // size of input buffer
		&ProNum,                          // lpOutBuffer
		4,                             // nOutBufferSize
		&DeviceRet,                // number of bytes returned
		0); // OVERLAPPED structure
	return true;
}
bool CMySystem::ClearThr(HANDLE hMyHandle)
{
	int ProNum=0;
	unsigned long  DeviceRet=0;

	int ret=DeviceIoControl(
		hMyHandle  ,       // handle to volume
		IOCTL_KSBINSWORD_CLEARTHREAD,               // dwIoControlCode
		&ProNum,           // MOVE_FILE_DATA structure
		4,         // size of input buffer
		&ProNum,                          // lpOutBuffer
		4,                             // nOutBufferSize
		&DeviceRet,                // number of bytes returned
		0); // OVERLAPPED structure
	return true;
}
int CMySystem::GetThrNum(HANDLE hMyHandle,int Tid)
{
	int ThreadNum=0,tTid=Tid;
	unsigned long  DeviceRet=0;
	DeviceIoControl(
		hMyHandle  ,       // handle to volume
		IOCTL_KSBINSWORD_GETTHRNUM,               // dwIoControlCode
		&tTid,           // MOVE_FILE_DATA structure
		sizeof(int),         // size of input buffer
		&ThreadNum,                          // lpOutBuffer
		sizeof(int),                             // nOutBufferSize
		&DeviceRet,                // number of bytes returned
		0); // OVERLAPPED structure

	return  ThreadNum;
}
int CMySystem::GetThr(HANDLE hMyHandle,void*ThreadNum,int size)
{
	int ProNum=0;
	unsigned long  DeviceRet=0;

	int ret=DeviceIoControl(
		hMyHandle  ,       // handle to volume
		IOCTL_KSBINSWORD_GETTHR,               // dwIoControlCode
		ThreadNum,           // MOVE_FILE_DATA structure
		size,         // size of input buffer
		ThreadNum,                          // lpOutBuffer
		size,                             // nOutBufferSize
		&DeviceRet,                // number of bytes returned
		0); // OVERLAPPED structure
	printf(":%x,%d\n",ret,GetLastError());
	return true;
}
BOOL CMySystem::ForceKillProcess(HANDLE hMyHandle,ULONG dwProcessId)
{
	int ProNum=0;
	unsigned long  DeviceRet=0;

	int ret=DeviceIoControl(
		hMyHandle  ,       // handle to volume
	    IOCTL_KSBINSWORD_KILLPROCESS    ,          // dwIoControlCode
		&dwProcessId,           // MOVE_FILE_DATA structure
		sizeof(int),         // size of input buffer
		0,                          // lpOutBuffer
		sizeof(int),                             // nOutBufferSize
		&DeviceRet,                // number of bytes returned
		0); // OVERLAPPED structure
	TRACE(":%d\n",GetLastError());
	return TRUE;
}

BOOL CMySystem::ForceKillThread(HANDLE hMyHandle,ULONG dwProcessId,ULONG dwCid)
{
	KillThreadStruct KillTread={dwProcessId, dwCid};
	int ProNum=0;
	unsigned long  DeviceRet=0;

	int ret=DeviceIoControl(
		hMyHandle  ,       // handle to volume
		IOCTL_KSBINSWORD_KILLTHREAD,               // dwIoControlCode
		&KillTread,           // MOVE_FILE_DATA structure
		sizeof(KillThreadStruct),         // size of input buffer
		0,                          // lpOutBuffer
		sizeof(KillThreadStruct),                             // nOutBufferSize
		&DeviceRet,                // number of bytes returned
		0); // OVERLAPPED structure
    return TRUE;
}
int  CMySystem::KillProcess(ULONG dwProcessId)
{ 

	HMODULE hNTDLL   =   GetModuleHandle(L"ntdll.dll");
	HANDLE     ph, h_dup; 

	PSYSTEM_HANDLE_INFORMATION     h_info={0}; 
	PROCESS_BASIC_INFORMATION      pbi={0};

	// 得到 csrss.exe 进程的PID
	HANDLE     csrss_id    = (HANDLE) GetPidByName (L"csrss.exe");
	CLIENT_ID client_id;
	client_id.UniqueProcess   = csrss_id;
	client_id.UniqueThread    = 0;

	// 初始化对象结构体
	OBJECT_ATTRIBUTES               attr;
	attr.Length                     = sizeof(OBJECT_ATTRIBUTES);
	attr.RootDirectory     = 0;
	attr.ObjectName      = 0;
	attr.Attributes      = 0;
	attr.SecurityDescriptor    = 0;
	attr.SecurityQualityOfService   = 0;


	////////////////////////////////////////////////////////////////////////////////////
	// 获得这些函数的实际地址

	pFnZwQuerySystemInformation ZwQuerySystemInformation = 
		(pFnZwQuerySystemInformation) GetProcAddress (hNTDLL, "ZwQuerySystemInformation");

	pFnNtOpenProcess ZwOpenProcess = 
		(pFnNtOpenProcess) GetProcAddress (hNTDLL, "ZwOpenProcess");

	pFnZwDuplicateObject ZwDuplicateObject =
		(pFnZwDuplicateObject) GetProcAddress (hNTDLL, "ZwDuplicateObject");

	pFnZwQueryInformationProcess ZwQueryInformationProcess = 
		(pFnZwQueryInformationProcess) GetProcAddress (hNTDLL, "ZwQueryInformationProcess");

	pFnZwAllocateVirtualMemory   ZwAllocateVirtualMemory =
		(pFnZwAllocateVirtualMemory) GetProcAddress (hNTDLL, "ZwAllocateVirtualMemory");

	pFnZwProtectVirtualMemory ZwProtectVirtualMemory = 
		(pFnZwProtectVirtualMemory) GetProcAddress (hNTDLL, "ZwProtectVirtualMemory");

	pFnZwWriteVirtualMemory ZwWriteVirtualMemory = 
		(pFnZwWriteVirtualMemory) GetProcAddress (hNTDLL, "ZwWriteVirtualMemory");

	pFnZwFreeVirtualMemory   ZwFreeVirtualMemory = 
		(pFnZwFreeVirtualMemory) GetProcAddress (GetModuleHandle(L"ntdll.dll"),   "ZwFreeVirtualMemory");

	pFnZwClose   ZwClose = 
		(pFnZwClose) GetProcAddress (hNTDLL, "ZwClose");

	////////////////////////////////////////////////////////////////////////////////////

	EnablePrivilege(SE_DEBUG_NAME);//提升权限
	// 打开CSRSS.EXE，获得其句柄
	ZwOpenProcess (&ph, PROCESS_ALL_ACCESS, &attr, &client_id);

	ULONG   bytesIO = 0x400000;
	PVOID buf     = 0;

	ZwAllocateVirtualMemory (GetCurrentProcess(), &buf, 0, &bytesIO, MEM_COMMIT, PAGE_READWRITE);

	// 为 ZwQuerySystemInformation 函数传递16号参数.获得系统句柄信息保存在buff中
	// buff的开始出保存的是系统句柄的数量.偏移4才是句柄信息
	ZwQuerySystemInformation (SystemHandleInformation, buf, 0x400000, &bytesIO);
	ULONG NumOfHandle = (ULONG) buf;
	h_info = (PSYSTEM_HANDLE_INFORMATION)((ULONG)buf+4);

	for (ULONG i= 0 ; i<NumOfHandle; i++, h_info++)
	{ 
		if ((h_info->ProcessId == (ULONG)csrss_id) && (h_info->ObjectTypeNumber == 5))
		{
			// 复制句柄
			if (ZwDuplicateObject(
				ph,
				(PHANDLE)h_info->Handle,
				(HANDLE)-1,
				&h_dup,
				0,
				0, 
				DUPLICATE_SAME_ACCESS) == STATUS_SUCCESS) 
			{

				ZwQueryInformationProcess(h_dup, 0, &pbi, sizeof(pbi), &bytesIO);
			}


			if (pbi.UniqueProcessId == dwProcessId)
			{
				MessageBox(0, L"目标已确定!", L"!", MB_OK);

				for (i = 0x1000; i < 0x8000000; i = i + 0x1000)//0x80000000
				{
					PVOID pAddress = (PVOID) i;
					ULONG sz = 0x1000;
					ULONG oldp;

					if (ZwProtectVirtualMemory (h_dup, &pAddress, &sz, PAGE_EXECUTE_READWRITE, &oldp) == STATUS_SUCCESS) {              
						ZwWriteVirtualMemory(h_dup, pAddress, buf, 0x1000, &oldp);
					}          
				}

				MessageBox(0, L"任务已完成！",L"!", 0);
				//   ZwClose(h_dup);     
				break;
			}
		}
	}

	bytesIO = 0;
	ZwFreeVirtualMemory(GetCurrentProcess(), &buf, &bytesIO, MEM_RELEASE);
	return 0;
}


//	远程线程函数

void CMySystem::RemoteThreadFun(PThreadParam PParam)
{
	DWORD dwFreeLibrary;
	DWORD dwGetModuleHandle;
	DWORD dwModuleName;

	dwFreeLibrary = PParam->dwFreeLibrary;
	dwGetModuleHandle = PParam->dwGetModuleHandle;
	dwModuleName = (DWORD)PParam->szModuleName;

	// 释放的最高次数
	DWORD dwCount = 100;

	// 循环寻找指定模块的句柄,如果找到,那么调用FreeLibrary释放,
	// 直到该模块被释放
	__asm
	{        
START:
		push dwModuleName;            // 模块名称压栈
		call dwGetModuleHandle;        // 调用GetModuleHandle
		test eax,eax;                // 
		jz OVER;                    // 没有找到模块返回
		dec dwCount;
		jz OVER;                    // 达到最高次数
		push eax;                    // 模块句柄压栈
		call dwFreeLibrary;            // 调用FreeLibrary
		test eax,eax;
		jnz START;
OVER:        
	}
	return;
}
void CMySystem::KillThreadFun(PKillThreadParam PParam)
{


	DWORD dwOpenThread;                    // OpenThread 地址
	DWORD dwTerminateThread;               // TerminateThread 地址
	DWORD dwCid;                           // 需要卸载的线程Cid
	HANDLE hThreadHandle=0;
	dwOpenThread = PParam->dwOpenThread;
	dwTerminateThread = PParam->dwTerminateThread;
	dwCid = (DWORD)PParam->dwCid;
	__asm{
		push dwCid
        push 0
		push THREAD_ALL_ACCESS
		mov eax,dwOpenThread
		call eax
		mov hThreadHandle,eax
        push 0
		push hThreadHandle
		mov eax,dwTerminateThread
		call eax
	}
    
     
	
	return;
}
void CMySystem::KillThreadByTerminateThread(DWORD dwProcessId,DWORD dwCid)
{
	// 参数构造
	// 局部变量
	HINSTANCE Kernel32lib = LoadLibrary(L"kernel32.dll");
	typedef HANDLE (* PNOpenThread)(DWORD,BOOL,DWORD);//OpenThread
	typedef BOOL   (* PNTerminateThread)(
		  HANDLE hThread,
		  DWORD dwExitCode
		);
    PNTerminateThread MyTerminateThread;
	PNOpenThread MyOpenThread;

	// 函数动作
	if(NULL == Kernel32lib)
	{
       return;
	}
	HANDLE my_thread = NULL;
	MyOpenThread = (PNOpenThread) GetProcAddress(Kernel32lib, "OpenThread");
    MyTerminateThread=(PNTerminateThread) GetProcAddress(Kernel32lib, "TerminateThread");
	KillThreadParam Param = {0};

	Param.dwOpenThread = (DWORD)MyOpenThread;
	Param.dwTerminateThread = (DWORD)MyTerminateThread;
	Param.dwCid = dwCid;
    AdjustPurview();
	// 打开指定的进程
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwProcessId);
	if(hProcess==NULL)
	{ 
		OutputDebugString(_T("OpenProcess failed!!"));
		return ;
	}

	// 参数写入
	LPVOID lpParam = NULL;
	lpParam = VirtualAllocEx(hProcess,NULL,sizeof(Param),MEM_COMMIT,PAGE_READWRITE);
	if (lpParam == NULL)
	{
		return ;
	}

	if (!WriteProcessMemory(hProcess,lpParam,&Param,sizeof(Param),0))
	{
		VirtualFreeEx(hProcess,lpParam,0,MEM_RELEASE);
		return ;
	}

	// 函数写入
	LPVOID lpThread = NULL;
	lpThread = VirtualAllocEx(hProcess,NULL,0x100,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	if (lpThread == NULL)
	{
		return ;
	}

	if (!WriteProcessMemory(hProcess,lpThread,    (LPCVOID)*CMySystem::KillThreadFun,0x100,0))
	{
		VirtualFreeEx(hProcess,lpThread,0,MEM_RELEASE);
		return ;
	}

	// 创建线程
	HANDLE hThread = NULL;
	hThread = CreateRemoteThread(hProcess,0,0,(LPTHREAD_START_ROUTINE)lpThread,lpParam,0,NULL);
	if(hThread == NULL)
	{
		return ;
	}

	// 等待线程结束
	//WaitForSingleObject(hThread,INFINITE);

	// 清理工作
	VirtualFreeEx(hProcess,lpThread,0,MEM_RELEASE);
	VirtualFreeEx(hProcess,lpParam,0,MEM_RELEASE);
	CloseHandle(hProcess);

	return ;
}
//	调整权限

bool CMySystem::AdjustPurview()
{
	TOKEN_PRIVILEGES TokenPrivileges;
	bool bRet;
	HANDLE hToken;

	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &TokenPrivileges.Privileges[0].Luid);   
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken);

	TokenPrivileges.PrivilegeCount = 1;   
	TokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	bRet = !!AdjustTokenPrivileges(hToken, FALSE, &TokenPrivileges, 0, NULL, NULL);

	CloseHandle(hToken);
	return bRet ;
}

BOOL CMySystem::FreeModuleByPid(TCHAR * szModuleName,DWORD dwPid)
{
	// 参数构造
	ThreadParam Param = {0};
	Param.dwFreeLibrary = (DWORD)FreeLibrary;
	Param.dwGetModuleHandle = (DWORD)GetModuleHandleA;
	MoveMemory(Param.szModuleName,szModuleName,MAX_MODULE_NAME_LEN);


	// 打开指定的进程
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwPid);
	if(hProcess==NULL)
	{ 
		OutputDebugString(_T("OpenProcess failed!!"));
		return FALSE;
	}

	// 参数写入
	LPVOID lpParam = NULL;
	lpParam = VirtualAllocEx(hProcess,NULL,sizeof(Param),MEM_COMMIT,PAGE_READWRITE);
	if (lpParam == NULL)
	{
		return FALSE;
	}

	if (!WriteProcessMemory(hProcess,lpParam,&Param,sizeof(Param),0))
	{
		VirtualFreeEx(hProcess,lpParam,0,MEM_RELEASE);
		return FALSE;
	}

	// 函数写入
	LPVOID lpThread = NULL;
	lpThread = VirtualAllocEx(hProcess,NULL,0x100,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	if (lpThread == NULL)
	{
		return FALSE;
	}

	if (!WriteProcessMemory(hProcess,lpThread,    (LPCVOID)*CMySystem::RemoteThreadFun,0x100,0))
	{
		VirtualFreeEx(hProcess,lpThread,0,MEM_RELEASE);
		return FALSE;
	}

	// 创建线程
	HANDLE hThread = NULL;
	hThread = CreateRemoteThread(hProcess,0,0,(LPTHREAD_START_ROUTINE)lpThread,lpParam,0,NULL);
	if(hThread == NULL)
	{
		return FALSE;
	}

	// 等待线程结束
	//WaitForSingleObject(hThread,INFINITE);

	// 清理工作
	VirtualFreeEx(hProcess,lpThread,0,MEM_RELEASE);
	VirtualFreeEx(hProcess,lpParam,0,MEM_RELEASE);
	CloseHandle(hProcess);

	return TRUE;
}
HANDLE
CMySystem::My_OpenProcess (
				 DWORD dwDesiredAccess,
				 BOOL   bInhert,
				 DWORD ProcessId,
				 BOOL   bOpenIt,
				 LPDWORD aryPids
				 )
				 /*++

				 参数 :
				 dwDesiredAccess - 希望以怎样的方式打开进程           
				 bInhert - 是否有继承权限           
				 ProcessId - PID
				 bOpenIt - 是要打开进程，还是要保存所有进程ID
				 paryPids - 保存PID的数组

				 返回 :
				 成功 - 指定的进程句柄

				 失败 - 0

				 功能 : 

				 1. 复制来复制去,总之是要获得指定进程的句柄
				 2. 用普通方法得到所有进程ID
				 --*/                   
{
//	CHAR                szBuffer[10];
	ULONG               uReturn;
	ULONG               NumOfHandle = 0;
	PCHAR               pBuffer = NULL;
	PVOID               pOneEprocess = 0;
	CLIENT_ID           cid;
	NTSTATUS           st;
	OBJECT_ATTRIBUTES   oa={sizeof(OBJECT_ATTRIBUTES), 0,NULL,NULL};
	PROCESS_BASIC_INFORMATION         pbi;
	PSYSTEM_HANDLE_TABLE_ENTRY_INFO   h_info;
	HANDLE hProcessToDup, hProcessCur, hProcessToRet;
	ULONG cbBuffer = 0x4000;
	HMODULE hModulentdll = LoadLibrary (L"ntdll.dll" ) ;
    AdjustPurview();
	pFnZwQueryInformationProcess ZwQueryInformationProcess = (pFnZwQueryInformationProcess)GetProcAddress ( hModulentdll, "ZwQueryInformationProcess" ) ;
	pFnNtOpenProcess NtOpenProcess=(pFnNtOpenProcess)GetProcAddress ( hModulentdll, "NtOpenProcess" ) ;
	pFnZwQuerySystemInformation ZwQuerySystemInformation =(pFnZwQuerySystemInformation)GetProcAddress ( hModulentdll, "ZwQuerySystemInformation" ) ;
	pFnZwDuplicateObject ZwDuplicateObject=(pFnZwDuplicateObject)GetProcAddress ( hModulentdll, "ZwDuplicateObject" ) ;
	pFnZwClose ZwClose=(pFnZwClose)GetProcAddress ( hModulentdll, "ZwClose" ) ;

	//oa.Length = sizeof (OBJECT_ATTRIBUTES);

	if (bInhert) {
		oa.Attributes |= OBJ_INHERIT;
	}

	if (bOpenIt) {
		// 看能否直接得到句柄
		cid.UniqueProcess = (HANDLE) (ProcessId );//+ 1
		cid.UniqueThread=(HANDLE) 0;
		st = NtOpenProcess (&hProcessToRet, dwDesiredAccess, &oa, &cid);

		if (NT_SUCCESS(st)) {
			return hProcessToRet;
		}
	}

	// 传递16号获得所有句柄,可能被hook过;不管了,走过场~
	do
	{
		pBuffer = (PCHAR)malloc(cbBuffer);
		if(pBuffer == NULL)
			return 0;
		memset(pBuffer,0,cbBuffer);
		st = ZwQuerySystemInformation(SystemHandleInformation,pBuffer,cbBuffer,&uReturn);
		if(st == STATUS_INFO_LENGTH_MISMATCH)
		{
			free(pBuffer);
			cbBuffer = cbBuffer * 2;
		}
	}while(st == STATUS_INFO_LENGTH_MISMATCH);
	NumOfHandle = *(ULONG*) pBuffer;
	h_info = (PSYSTEM_HANDLE_TABLE_ENTRY_INFO) ( (ULONG)pBuffer + 4 );

	for (ULONG i = 0; i < NumOfHandle; i++) {
		if (!bOpenIt)
		{aryPids[i] = h_info[i].UniqueProcessId;}

		if (bOpenIt) {
			if ( 5 == h_info[i].ObjectTypeIndex) { // 是进程的句柄，打开它

				cid.UniqueProcess = (HANDLE)h_info[i].UniqueProcessId;
			
				st = NtOpenProcess (&hProcessToDup, PROCESS_DUP_HANDLE, &oa, &cid);

				if (NT_SUCCESS (st)) { // 复制该句柄后赋予全部权限，以便调用ZwQuery*时顺利进行
					st = ZwDuplicateObject (hProcessToDup, (PHANDLE)h_info[i].HandleValue, (HANDLE)-1,

						&hProcessCur, PROCESS_ALL_ACCESS, 0, DUPLICATE_SAME_ATTRIBUTES);

					if (NT_SUCCESS (st)) { // 查看复制的句柄是否为我们想要的
						st = ZwQueryInformationProcess (hProcessCur, ProcessBasicInformation,\
							&pbi, sizeof(pbi), 0);

						if (NT_SUCCESS (st)) {
							if (ProcessId == pbi.UniqueProcessId ) { 
								// 若是想要的PID,就把该句柄按照希望的访问权限复制，然后返回
								st = ZwDuplicateObject (hProcessToDup, (PHANDLE)h_info[i].HandleValue,\
									(HANDLE)-1, &hProcessToRet, dwDesiredAccess,\
									OBJ_INHERIT, DUPLICATE_SAME_ATTRIBUTES);

								ZwClose (hProcessCur);
								ZwClose (hProcessToDup);
								//free(pBuffer);
								return hProcessToRet;
							}
						}     
					}

					ZwClose (hProcessCur);
				}

				ZwClose (hProcessToDup);
			}
		}
	}
    //free(pBuffer);
	return 0;
} 
HANDLE CMySystem:: OpenDevice(LPCTSTR lpszDevicePath)
{
	HANDLE hDevice;

	// 打开设备
	hDevice = ::CreateFile(lpszDevicePath,    // 设备路径
		GENERIC_READ | GENERIC_WRITE,        // 读写方式
		FILE_SHARE_READ | FILE_SHARE_WRITE,  // 共享方式
		NULL,                    // 默认的安全描述符
		OPEN_EXISTING,           // 创建方式
		0,                       // 不需设置文件属性
		NULL);                   // 不需参照模板文件

	return hDevice;
}

void CMySystem::BeginMon()
{

	Sleep(100);
	//create processing thread
	CreateThread(0,0,(LPTHREAD_START_ROUTINE)ThreadMon,0,0,&dw);

	//open device
	hMyHandle=CreateFile(L"\\\\.\\PRMONITOR",
		GENERIC_READ|GENERIC_WRITE,
		0,
		0,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_SYSTEM,
		0);

	if (hMyHandle == INVALID_HANDLE_VALUE)
	{
		bdrv = FALSE;

	}
	else
	{
		bdrv = TRUE;
	}

	DWORD * addr=(DWORD *)(1+(DWORD)GetProcAddress(GetModuleHandle(L"ntdll.dll"),"NtCreateProcess"));
	ZeroMemory(outputbuff,256);
	controlbuff[0]=addr[0];
	controlbuff[1]=(DWORD)&outputbuff[0];


}

void CMySystem::ThreadMon()
{
	DWORD a,x;
	char msgbuff[512];
	char *pdest;
	int  result;

	while(1)
	{
		memmove(&a,&outputbuff[0],4);


		if(!a){Sleep(10);continue;}



		char*name=(char*)&outputbuff[8];
		for(x=0;x<stringcount;x++)
		{
			if(!stricmp(name,strings[x])){a=1;goto skip;}
		}



		pdest = strstr(name,"##");
		if (pdest != NULL)
		{
			result = pdest-name;
			strcpy(msgbuff, "是否允许");
			strncat(msgbuff,&outputbuff[8],result);
			strcat(msgbuff,"运行");
			strcat(msgbuff,&outputbuff[result+10]);
		}
		else if((pdest=strstr(name,"$$")) != NULL)
		{			
			result = pdest-name;
			strcpy(msgbuff, "是否允许");
			strncat(msgbuff,&outputbuff[8],result);
			strcat(msgbuff,"设置注册表");
			strcat(msgbuff,&outputbuff[result+10]);
		}
		else
		{
			pdest = strstr(name,"&&");
			result = pdest-name;
			strcpy(msgbuff,"是否允许");
			strncat(msgbuff,&outputbuff[8],result);
			strcat(msgbuff,"加载驱动");
			strcat(msgbuff,&outputbuff[result]+10);
		}



		if(IDYES==MessageBoxA(0, msgbuff,"WARNING",MB_YESNO|MB_ICONQUESTION|0x00200000L))
		{
			a=1; //睡觉
			strings[stringcount]=_strdup(name);
			stringcount++;
		}
		else a=0;


skip:memmove(&outputbuff[4],&a,4);


		a=0;
		memmove(&outputbuff[0],&a,4);
	}

}




///////////////////////////////////////////////////
//加载驱动
void CMySystem::LoadDriver( IN WCHAR* lpszDriverName )
{

	WCHAR szDriverImagePath[256]={NULL};
	//得到完整的驱动路径
    int i=0;
	GetModuleFileName(NULL,szDriverImagePath,MAX_PATH);
	for (i=wcslen(szDriverImagePath);i>0;i--)
	{
		if ('\\'==szDriverImagePath[i])
		{
			szDriverImagePath[i+1]='\0';break;
		}
	}
	//GetSystemDirectory(szDriverImagePath,sizeof(szDriverImagePath));
	wcscat(szDriverImagePath,lpszDriverName);
	wcscat(szDriverImagePath,L".sys");
	BOOL bRet = FALSE;

	SC_HANDLE hServiceMgr=NULL;//SCM管理器的句柄
	SC_HANDLE hServiceDDK=NULL;//NT驱动程序的服务句柄

	//打开服务控制管理器
	hServiceMgr = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );

	if( hServiceMgr == NULL )  
	{
		//OpenSCManager失败
		printf( "OpenSCManager() Faild %d ! \n", GetLastError() );
		bRet = FALSE;
		goto BeforeLeave;
	}
	else
	{
		////OpenSCManager成功
		printf( "OpenSCManager() ok ! \n" );  
	}

	//创建驱动所对应的服务
	hServiceDDK = CreateService( hServiceMgr,
		lpszDriverName, //驱动程序的在注册表中的名字  
		lpszDriverName, // 注册表驱动程序的 DisplayName 值  
		SERVICE_ALL_ACCESS, // 加载驱动程序的访问权限  
		SERVICE_KERNEL_DRIVER,// 表示加载的服务是驱动程序  
		SERVICE_DEMAND_START, // 注册表驱动程序的 Start 值  
		//SERVICE_BOOT_START, //系统启动时启动该驱动
		SERVICE_ERROR_IGNORE, // 注册表驱动程序的 ErrorControl 值  
		szDriverImagePath, // 注册表驱动程序的 ImagePath 值  
		NULL,  
		NULL,  
		NULL,  
		NULL,  
		NULL);  

	DWORD dwRtn;
	//判断服务是否失败
	if( hServiceDDK == NULL )  
	{  
		dwRtn = GetLastError();
		if( dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_EXISTS )  
		{  
			//由于其他原因创建服务失败
			//printf( "CreateService() Faild %d ! \n", dwRtn );  
			bRet = FALSE;
			//goto BeforeLeave;
		}  
		else  
		{
			//服务创建失败，是由于服务已经创立过
			//printf( "CreateService() Faild Service is ERROR_IO_PENDING or ERROR_SERVICE_EXISTS! \n" );  
		}

		// 驱动程序已经加载，只需要打开  
		hServiceDDK = OpenService( hServiceMgr, lpszDriverName, SERVICE_ALL_ACCESS );  
		if( hServiceDDK == NULL )  
		{
			//如果打开服务也失败，则意味错误
			dwRtn = GetLastError();  
			//printf( "OpenService() Faild %d ! \n", dwRtn );  
			bRet = FALSE;
			goto BeforeLeave;
		}  
		else 
		{
			//printf( "OpenService() ok ! \n" );
		}
	}  
	else  
	{
		//printf( "CreateService() ok ! \n" );
	}

	//开启此项服务
	bRet= StartService(hServiceDDK, NULL, NULL );  
	if( !bRet )  
	{  
		DWORD dwRtn = GetLastError();  
		if( dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_ALREADY_RUNNING )  
		{  
			//printf( "StartService() Faild %d ! \n", dwRtn );  
			bRet = FALSE;
			goto BeforeLeave;
		}  
		else  
		{  
			if( dwRtn == ERROR_IO_PENDING )  
			{  
				//设备被挂住
				//printf( "StartService() Faild ERROR_IO_PENDING ! \n");
				bRet = FALSE;
				goto BeforeLeave;
			}  
			else  
			{  
				//服务已经开启
				//printf( "StartService() Faild ERROR_SERVICE_ALREADY_RUNNING ! \n");
				bRet = TRUE;
				goto BeforeLeave;
			}  
		}  
	}
	bRet = TRUE;
	//离开前关闭句柄
BeforeLeave:
	if(hServiceDDK)
	{
		CloseServiceHandle(hServiceDDK);
	}
	if(hServiceMgr)
	{
		CloseServiceHandle(hServiceMgr);
	}
	return ;


 	

}
//卸载驱动

void CMySystem::UnloadDriver( WCHAR * szSvrName ) 
{ 
	SC_HANDLE hServiceMgr, hServiceTwdm; 
	SERVICE_STATUS SvrSta; 
	hServiceMgr = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS ); 
	if( hServiceMgr == NULL ) 
	{ 
		//printf( "DelSvr::OpenSCManager() Faild %d ! \n", GetLastError() ); 
		return; 
	} 
	else 
	{ 
		//printf( "DelSvr::OpenSCManager() ok ! \n" ); 
	} 
	hServiceTwdm = OpenService( hServiceMgr, szSvrName, SERVICE_ALL_ACCESS ); 

	if( hServiceTwdm == NULL ) 
	{ 
		CloseServiceHandle( hServiceMgr ); 
		//printf( "DelSvr::OpenService() Faild %d ! \n", GetLastError() ); 
		return; 
	} 
	else 
	{ 
		//printf( "DelSvr::OpenService() ok ! \n" ); 
	} 
	//停止驱动程序，如果停止失败，只有重新启动才能，再动态加载。 
	if( !ControlService( hServiceTwdm, SERVICE_CONTROL_STOP , &SvrSta ) ) 
	{ 
		//printf( "DelSvr::ControlService() Faild %d !\n", GetLastError() ); 
	} 
	else 
	{ 
		//printf( "DelSvr::ControlService() ok !\n" ); 
	} 
	//动态卸载驱动程序。 
	if( !DeleteService( hServiceTwdm ) ) 
	{ 
		//printf( "DelSvr:eleteSrevice() Faild %d !\n", GetLastError() ); 
	} 
	else 
	{ 
		//printf( "DelSvr:eleteSrevice() ok !\n" ); 
	} 
	CloseServiceHandle( hServiceTwdm ); 
	CloseServiceHandle( hServiceMgr ); 
	return; 
} 
//得到SSDT
BOOL CMySystem::GetSSDT( IN HANDLE hDriver, OUT PSSDT ssdt )
{
	if( ssdt == NULL )
	{
		return FALSE;
	}
	DWORD dwRet;
	BOOL bRet = DeviceIoControl( hDriver, IOCTL_KSBINSWORD_GETSSDT, ssdt,  sizeof( SSDT ), ssdt, sizeof( SSDT ), \
		&dwRet, NULL );
	if( bRet )
	{
		TotalSSDTCount = ssdt->ulNumberOfServices;		//获取服务个数
		return TRUE;
	}
	return FALSE;
}
//设置SSDT
BOOL CMySystem::SetSSDT( IN HANDLE hDriver, IN PSSDT ssdt )
{
	if( ssdt == NULL )
	{
		return FALSE;
	}
	DWORD dwRet;
	BOOL bRet = DeviceIoControl( hDriver, IOCTL_KSBINSWORD_SETSSDT, ssdt, sizeof( SSDT ), \
		NULL, 0, &dwRet, NULL );
	if( bRet )
	{
		TotalSSDTCount = ssdt->ulNumberOfServices;		//获取服务个数
		return TRUE;
	}
	return FALSE;
}
//得到SSDT表中HOOK的地址
BOOL CMySystem::GetHook( IN HANDLE hDriver, IN ULONG ulIndex, OUT PULONG ulAddr )
{
	if( ulAddr == NULL )
	{
		return FALSE;
	}
	DWORD dwRet;
	BOOL bRet = DeviceIoControl( hDriver, IOCTL_KSBINSWORD_GETHOOK, &ulIndex, sizeof( ULONG ), \
		ulAddr, sizeof( ULONG ), &dwRet, NULL );

	return bRet;
}
//设置SSDT表中HOOK的地址
BOOL CMySystem::SetHook( IN HANDLE hDriver, IN ULONG ulIndex, IN OUT PULONG ulAddr )
{
	if( ulAddr == NULL )
	{
		return FALSE;
	}
	DWORD dwRet;
	BOOL bRet = DeviceIoControl( hDriver, IOCTL_KSBINSWORD_SETHOOK, &ulIndex, sizeof( ULONG ), \
		ulAddr, sizeof( ULONG ), &dwRet, NULL );
	return bRet;
}
//创建模块列表
PSYSMODULELIST CMySystem::CreateModList( OUT PULONG ulNtBase )
{
	HINSTANCE hNTDll;
	ULONG nRet;
	ULONG nQuerySize;
	ULONG Success;
	PSYSMODULELIST pModInfo = NULL;

	do
	{
		if( !( hNTDll = LoadLibrary( L"ntdll" ) ) )
		{
			break;
		}
		if( pNtQuerySystemInformation == NULL )
		{
			pNtQuerySystemInformation = (pNtQuerySystemInformationProto)GetProcAddress( hNTDll, "NtQuerySystemInformation" );
			if( !pNtQuerySystemInformation )
			{
				break;
			}
		}
		Success = pNtQuerySystemInformation( \
			SystemModuleInfo, 
			NULL, 
			0, 
			&nQuerySize
			);
		pModInfo = (PSYSMODULELIST)malloc( nQuerySize );
		if( !pModInfo )
		{
			break;
		}
		Success = pNtQuerySystemInformation( \
			SystemModuleInfo, 
			pModInfo, 
			nQuerySize, 
			&nRet
			);
		if( Success < 0 )
		{
			free( pModInfo );
			pModInfo = NULL;
			break;
		}

		*ulNtBase = (ULONG)(pModInfo->smi->Base);	//顺便得到NT基址(ntoskrnl.exe在内核中的加载基址) ^_^

	} while( FALSE );

	::FreeLibrary( hNTDll );

	return pModInfo;
}
//消毁列表
void CMySystem::DestroyModList( IN PSYSMODULELIST pList )
{
	free( pList );
}
//通过地址得到模块名
BOOL CMySystem::GetModuleNameByAddr( IN ULONG ulAddr, IN PSYSMODULELIST pList, OUT LPSTR buf, IN DWORD dwSize )
{
	ULONG i;

	for ( i = 0; i < pList->ulCount; i++ )
	{
		ULONG ulBase = (ULONG)pList->smi[i].Base;
		ULONG ulMax  = ulBase + pList->smi[i].Size;
		if ( ulBase <= ulAddr && ulAddr < ulMax )
		{

			lstrcpynA( buf, pList->smi[i].ImageName, dwSize );

			return TRUE;
		}
	}
	return FALSE;
}
//根据服务号将SSDT缓冲表排序
BOOL CMySystem::SSDTSTOrderByServiceNum( pSSDTSaveTable pSsdtST )
{
	ULONG ulMaxServiceNumber = 0;		//服务号中的最大值
	ULONG i, j;
	//找出最大的
	for( i = 0; i < RealCount; i ++ )
	{
		ULONG ulCurServiceNum;
		ulCurServiceNum = ((pSSDTSaveTable)((ULONG)pSSDTST + i * sizeof(SSDTSaveTable)))->ulServiceNumber;
		ulMaxServiceNumber = ulCurServiceNum > ulMaxServiceNumber? ulCurServiceNum : ulMaxServiceNumber;
	}
	//根据最大服务号设置后面没有服务号的
	for( i = RealCount; i < TotalSSDTCount; i ++ )
	{
		ulMaxServiceNumber ++;		//最大号依次加,来设置后面的服务号
		((pSSDTSaveTable)((ULONG)pSSDTST + i * sizeof(SSDTSaveTable)))->ulServiceNumber = \
			ulMaxServiceNumber;
	}
	//按服务号排序
	//冒泡排序法
	for( i = TotalSSDTCount - 1; i > 0; i -- )
	{
		for( j = 0; j < i; j ++ )
		{
			if( \
				((pSSDTSaveTable)((ULONG)pSSDTST + j * sizeof(SSDTSaveTable)))->ulServiceNumber \
		> ((pSSDTSaveTable)((ULONG)pSSDTST + (j+1) * sizeof(SSDTSaveTable)))->ulServiceNumber \
		)	//交换
			{
				SSDTSaveTable SSDTSTTemp;
				memcpy( \
					(void*)&SSDTSTTemp, \
					(void*)((ULONG)pSSDTST + j * sizeof(SSDTSaveTable)), \
					sizeof( SSDTSaveTable ) \
					);
				memcpy( \
					(void*)((ULONG)pSSDTST + j * sizeof(SSDTSaveTable)), \
					(void*)((ULONG)pSSDTST + (j+1) * sizeof(SSDTSaveTable)), \
					sizeof( SSDTSaveTable ) \
					);
				memcpy( \
					(void*)((ULONG)pSSDTST + (j+1) * sizeof(SSDTSaveTable)), \
					(void*)&SSDTSTTemp, \
					sizeof( SSDTSaveTable ) \
					);
			}
		}
	}

	return TRUE;
}


//枚举SSDT
BOOL CMySystem::EnumSSDT( IN HANDLE hDriver )
{
	HINSTANCE hNtDllInst = NULL;
	ULONG ulNtDllOffset;
	ULONG ulFuncNameCount = 0;
	PIMAGE_EXPORT_DIRECTORY pImgExpDir = NULL;
	PULONG pFuncNameArray = NULL;
	ULONG i;
	BOOL bOK = TRUE;
   
	do
	{
		RealCount = 0;			//个数清0
		if( pList )		//还有存没有释放
		{
			DestroyModList( pList );	//释放它
			pList = NULL;
		}
		pList = CreateModList( &NTBase );	//创建模块信息链表,顺便得到NT基址
		if( pList == NULL )		//创建失败
		{
			bOK = FALSE;
			break;
		}

		if( !( hNtDllInst = LoadLibrary( L"ntdll.dll" ) ) )
		{
			bOK = FALSE;
			break;
		}
		/////////////////////////////////////////////////////////
		//分配SSDT保存缓冲表
		//得到SSDT个数
		SSDT ssdt;
		if( !GetSSDT( hDriver, &ssdt ) )
		{
			bOK = FALSE;
			break;
		}
		if( TotalSSDTCount == -1 )		//得到SSDT个数失败
		{
			bOK = FALSE;
			break;
		}
		if( pSSDTST )		//pSSDTST已有值,先释放它
		{
			free( pSSDTST );
			pSSDTST = NULL;
		}
		pSSDTST = (pSSDTSaveTable)malloc( TotalSSDTCount * sizeof( SSDTSaveTable ) );
		if( pSSDTST == NULL )
		{
			bOK = FALSE;
			break;
		}
		for( i = 0; i < TotalSSDTCount; i ++ )	//初始化它
		{
			((pSSDTSaveTable)((ULONG)pSSDTST + i * sizeof(SSDTSaveTable)))->ulServiceNumber = -1;
			((pSSDTSaveTable)((ULONG)pSSDTST + i * sizeof(SSDTSaveTable)))->ulCurrentFunctionAddress = 0L;
			((pSSDTSaveTable)((ULONG)pSSDTST + i * sizeof(SSDTSaveTable)))->ulOriginalFunctionAddress = 0L;
			memset( ((pSSDTSaveTable)((ULONG)pSSDTST + i * sizeof(SSDTSaveTable)))->ServiceFunctionName, \
				0, \
				sizeof(((pSSDTSaveTable)((ULONG)pSSDTST + i * sizeof(SSDTSaveTable)))->ServiceFunctionName));
			memset( ((pSSDTSaveTable)((ULONG)pSSDTST + i * sizeof(SSDTSaveTable)))->ModuleName, \
				0, \
				sizeof(((pSSDTSaveTable)((ULONG)pSSDTST + i * sizeof(SSDTSaveTable)))->ModuleName));
		}
		/////////////////////////////////////////////////////////
		//枚举
		ulNtDllOffset = (ULONG)hNtDllInst;
		//PE头部
		ulNtDllOffset += ((PIMAGE_DOS_HEADER)hNtDllInst)->e_lfanew + sizeof( DWORD );
		//数据目录
		ulNtDllOffset += sizeof( IMAGE_FILE_HEADER ) + sizeof( IMAGE_OPTIONAL_HEADER )
			- IMAGE_NUMBEROF_DIRECTORY_ENTRIES * sizeof( IMAGE_DATA_DIRECTORY );
		//导出表
		ulNtDllOffset = (DWORD)hNtDllInst + ((PIMAGE_DATA_DIRECTORY)ulNtDllOffset)->VirtualAddress;
		//导出目录表
		pImgExpDir = (PIMAGE_EXPORT_DIRECTORY)ulNtDllOffset;
		//得到函数个数
		ulFuncNameCount = pImgExpDir->NumberOfNames;
		//函数名数组指针
		pFuncNameArray = (PULONG)( (ULONG)hNtDllInst + pImgExpDir->AddressOfNames );
		/////////////////////
		//循环找函数名
		for( i = 0; i < ulFuncNameCount; i ++ )
		{
			//函数名
			PCSTR pszName = (PCSTR)( pFuncNameArray[i] + (ULONG)hNtDllInst );
			if( pszName[0] == 'N' && pszName[1] == 't' )	//Nt 开头的函数
			{
				//查找表
				LPWORD pOrdNameArray = (LPWORD)( (ULONG)hNtDllInst + pImgExpDir->AddressOfNameOrdinals );
				//函数地址
				LPDWORD pFuncArray = (LPDWORD)( (ULONG)hNtDllInst + pImgExpDir->AddressOfFunctions );
				//函数代码
				LPCVOID pFuncCode = (LPCVOID)( (ULONG)hNtDllInst + pFuncArray[pOrdNameArray[i]] );
				//获取服务号
				SSDTEntry EntryCode;
				memcpy( &EntryCode, pFuncCode, sizeof( SSDTEntry ) );
				if( EntryCode.byMov == 0xB8 )	// MOV EAX, XXXX
				{
					ULONG ulAddr = 0;
					if( !GetHook( hDriver, EntryCode.ulIndex, &ulAddr ) )
					{
						bOK = FALSE;
						break;
					}
					////////////////////////
					//通过地址得到模块名
					char ModNameBuf[MAX_PATH+1];
					memset( ModNameBuf, 0, sizeof( ModNameBuf ) );

					if( GetModuleNameByAddr( ulAddr, pList, ModNameBuf, sizeof( ModNameBuf )-1 ) )
					{
						memcpy( \
							((pSSDTSaveTable)((ULONG)pSSDTST + RealCount * sizeof(SSDTSaveTable)))->ModuleName, \
							ModNameBuf, \
							sizeof( ModNameBuf ) \
							);
					}
					////////////////////////////////////////////////////
					//保存SSDT信息到缓冲表中
					((pSSDTSaveTable)((ULONG)pSSDTST + RealCount * sizeof(SSDTSaveTable)))->ulServiceNumber = \
						EntryCode.ulIndex;	//服务号
					((pSSDTSaveTable)((ULONG)pSSDTST + RealCount * sizeof(SSDTSaveTable)))->ulCurrentFunctionAddress = \
						ulAddr;		//当前函数地址
					memcpy( \
						((pSSDTSaveTable)((ULONG)pSSDTST + RealCount * sizeof(SSDTSaveTable)))->ServiceFunctionName, \
						pszName, \
						sizeof( ((pSSDTSaveTable)((ULONG)pSSDTST + RealCount * sizeof(SSDTSaveTable)))->ServiceFunctionName )
						);

					RealCount ++;
				}
			}
		}
	} while( FALSE );

	::FreeLibrary( hNtDllInst );

	if( bOK )	//成功
	{
		//获取剩下的服务号
		for( i = RealCount; i < TotalSSDTCount; i ++ )
		{
			if( !GetHook( hDriver, i, &((pSSDTSaveTable)((ULONG)pSSDTST + i * sizeof(SSDTSaveTable)))->ulCurrentFunctionAddress ) )
			{
				bOK = FALSE;
				break;
			}
			////////////////////////
			//通过地址得到模块名
			char ModNameBuf[MAX_PATH+1];
			memset( ModNameBuf, 0, sizeof( ModNameBuf ) );

			if( GetModuleNameByAddr( \
				((pSSDTSaveTable)((ULONG)pSSDTST + i * sizeof(SSDTSaveTable)))->ulCurrentFunctionAddress, \
				pList, ModNameBuf, sizeof( ModNameBuf )-1 ) )
			{
				memcpy( \
					((pSSDTSaveTable)((ULONG)pSSDTST + i * sizeof(SSDTSaveTable)))->ModuleName, \
					ModNameBuf, \
					sizeof( ModNameBuf ) \
					);
			}
		} 
		//按服务号进行排序
		SSDTSTOrderByServiceNum( pSSDTST );

		//获取原始函数地址
		GetOldSSDTAddress();
	}

	if( pList )
	{
		DestroyModList( pList );	//释放模块链表
		pList = NULL;
	}

	return bOK;
}
//恢复SSDT
BOOL CMySystem::ReSSDT( IN HANDLE hDriver )
{
	ULONG i;
	if( RealCount == 0 )
	{
		return FALSE;
	}
	for( i = 0; i < RealCount; i ++ )
	{
		if( \
			((pSSDTSaveTable)((ULONG)pSSDTST + i * sizeof(SSDTSaveTable)))->ulCurrentFunctionAddress \
			!= ((pSSDTSaveTable)((ULONG)pSSDTST + i * sizeof(SSDTSaveTable)))->ulOriginalFunctionAddress \
			)	//当前地址和原始地址不同,恢复原始地址值
		{
			if( !SetHook( hDriver, \
				((pSSDTSaveTable)((ULONG)pSSDTST + i * sizeof(SSDTSaveTable)))->ulServiceNumber, \
				&(((pSSDTSaveTable)((ULONG)pSSDTST + i * sizeof(SSDTSaveTable)))->ulOriginalFunctionAddress) \
				) )
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}
//恢复SSDT并去掉非系统表
BOOL CMySystem::ReSSDTAndThrowSpilth( IN HANDLE hDriver )
{
	if( !ReSSDT( hDriver ) )
	{
		return FALSE;
	}
	//读取SSDT
	SSDT ssdt;
	if( !GetSSDT( hDriver, &ssdt ) )
	{
		return FALSE;
	}
	if( RealCount == 0 )
	{
		return FALSE;
	}
	ssdt.ulNumberOfServices = RealCount;
	if( !SetSSDT( hDriver, &ssdt ) )
	{
		return FALSE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////
//获取原始函数地址,以便恢复SSDT
//    说明: 此功能的函数(下面的)来源于网络
//    PS: 我实在想不到能得到全部原始函数地址的方法了
//        我的方法只可以得到部分函数的原始地址(但所用的方法不同)
/////////////////////////////////////////////////////////
#define ibaseDD *(PDWORD)&ibase
#define RVATOVA(base,offset) ((PVOID)((DWORD)(base)+(DWORD)(offset)))

typedef struct { 
	WORD    offset:12;
	WORD    type:4;
} IMAGE_FIXUP_ENTRY, *PIMAGE_FIXUP_ENTRY;


DWORD CMySystem::GetHeaders(PCHAR ibase,
				 PIMAGE_FILE_HEADER *pfh,
				 PIMAGE_OPTIONAL_HEADER *poh,
				 PIMAGE_SECTION_HEADER *psh)

{ 
	PIMAGE_DOS_HEADER mzhead=(PIMAGE_DOS_HEADER)ibase;

	if    ((mzhead->e_magic!=IMAGE_DOS_SIGNATURE) ||
		(ibaseDD[mzhead->e_lfanew]!=IMAGE_NT_SIGNATURE))
		return FALSE;

	*pfh=(PIMAGE_FILE_HEADER)&ibase[mzhead->e_lfanew];
	if (((PIMAGE_NT_HEADERS)*pfh)->Signature!=IMAGE_NT_SIGNATURE) 
		return FALSE;
	*pfh=(PIMAGE_FILE_HEADER)((PBYTE)*pfh+sizeof(IMAGE_NT_SIGNATURE));

	*poh=(PIMAGE_OPTIONAL_HEADER)((PBYTE)*pfh+sizeof(IMAGE_FILE_HEADER));
	if ((*poh)->Magic!=IMAGE_NT_OPTIONAL_HDR32_MAGIC)
		return FALSE;

	*psh=(PIMAGE_SECTION_HEADER)((PBYTE)*poh+sizeof(IMAGE_OPTIONAL_HEADER));
	return TRUE;
}

DWORD CMySystem::FindKiServiceTable(HMODULE hModule,DWORD dwKSDT)
{ 
	PIMAGE_FILE_HEADER    pfh;
	PIMAGE_OPTIONAL_HEADER    poh;
	PIMAGE_SECTION_HEADER    psh;
	PIMAGE_BASE_RELOCATION    pbr;
	PIMAGE_FIXUP_ENTRY    pfe;

	DWORD    dwFixups=0,i,dwPointerRva,dwPointsToRva,dwKiServiceTable;
	BOOL    bFirstChunk;

	GetHeaders((PCHAR)hModule,&pfh,&poh,&psh);
	TRACE("hModule:%X",hModule);
	if ((poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress) &&
		(!((pfh->Characteristics)&IMAGE_FILE_RELOCS_STRIPPED))) {

			pbr=(PIMAGE_BASE_RELOCATION)RVATOVA(poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress,hModule);

			bFirstChunk=TRUE;

			while (bFirstChunk || pbr->VirtualAddress) { 
				bFirstChunk=FALSE;

				pfe=(PIMAGE_FIXUP_ENTRY)((DWORD)pbr+sizeof(IMAGE_BASE_RELOCATION));

				for (i=0;i<(pbr->SizeOfBlock-sizeof(IMAGE_BASE_RELOCATION))>>1;i++,pfe++) { 
					if (pfe->type==IMAGE_REL_BASED_HIGHLOW) { 
						dwFixups++;
						dwPointerRva=pbr->VirtualAddress+pfe->offset;

						dwPointsToRva=*(PDWORD)((DWORD)hModule+dwPointerRva)-(DWORD)poh->ImageBase;

						if (dwPointsToRva==dwKSDT) { 

							if (*(PWORD)((DWORD)hModule+dwPointerRva-2)==0x05c7) {

								dwKiServiceTable=*(PDWORD)((DWORD)hModule+dwPointerRva+4)-poh->ImageBase;
								return dwKiServiceTable;
							}
						}

					} 

				}
				*(PDWORD)&pbr+=pbr->SizeOfBlock;
			}
	}    

	return 0;
}
///////////////////////////
//获取原始函数地址声明
//void GetOldSSDTAddress();
void CMySystem::GetOldSSDTAddress()
{     
	HMODULE    hKernel;
	PCHAR    pKernelName;
	PDWORD    pService;
	PIMAGE_FILE_HEADER    pfh;
	PIMAGE_OPTIONAL_HEADER    poh;
	PIMAGE_SECTION_HEADER    psh;
	DWORD    dwKernelBase,dwServices=0;
	DWORD    dwKSDT;
	DWORD    dwKiServiceTable;

	ULONG n;

	pNtQuerySystemInformation(SystemModuleInfo,&n,0,&n);
	//	PULONG p=new ULONG[n];
	//这里用malloc代替new,要不然编译时,必须忽略一些库,比较麻烦
	PULONG p = (PULONG)malloc( n*sizeof(ULONG) );
	pNtQuerySystemInformation(SystemModuleInfo,p,n*sizeof(*p),0);
	PSYSTEM_MODULE_INFORMATION module=PSYSTEM_MODULE_INFORMATION(p+1);

	dwKernelBase=(DWORD)module->Base;

	pKernelName=module->ModuleNameOffset+module->ImageName;
    //pKernelName="G:\\源码仓库2\\MyProjects\\KsBinSword\\debug\\ntoskrnl.exe";
	hKernel=LoadLibraryExA(pKernelName,0,DONT_RESOLVE_DLL_REFERENCES);//加载NTOSKREL 的基址
	if (!hKernel) {
		return;
	}
	TRACE("hKernel:%X\n",hKernel);
	if (!(dwKSDT=(DWORD)GetProcAddress(hKernel,"KeServiceDescriptorTable"))) {
		return;
	}

	dwKSDT-=(DWORD)hKernel;

	if (!(dwKiServiceTable=FindKiServiceTable(hKernel,dwKSDT))) { 
		return;
	}
    TRACE("dwKiServiceTable:%X\n",dwKiServiceTable);
	GetHeaders((PCHAR)hKernel,&pfh,&poh,&psh);
	dwServices=0;

	for (pService=(PDWORD)((DWORD)hKernel+dwKiServiceTable);
		*pService-poh->ImageBase<poh->SizeOfImage;
		pService++,dwServices++)
	{
		 //TRACE("pService:%X\n",pService);
		((pSSDTSaveTable)((ULONG)pSSDTST + dwServices * sizeof(SSDTSaveTable)))->ulOriginalFunctionAddress = \
			*pService-poh->ImageBase+dwKernelBase;
	}
	FreeLibrary(hKernel);

	//	delete [] p;
	//同上new,为了使编译时不产生过多的麻烦,这里改delete为free函数
	free( p );
}
__int64 CMySystem::MyFileSeek (HANDLE hf, __int64 distance, DWORD MoveMethod)



{
	LARGE_INTEGER li;
	li.QuadPart = distance;
	li.LowPart = SetFilePointer (hf, 
		li.LowPart, 
		&li.HighPart, 
		MoveMethod);
	if (li.LowPart == INVALID_SET_FILE_POINTER && GetLastError() 

		!= NO_ERROR)
	{
		
		li.QuadPart = -1;
	}
	return li.QuadPart;
}
BOOL CMySystem::ReadSector(__int64 Sect,BYTE* OutBuf)//读一个扇区 \\\\.\\PhysicalDrive0
{
	HANDLE hFile=CreateFile(L"\\\\.\\PhysicalDrive0",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
  	MyFileSeek (hFile,(Sect)*0x200,FILE_BEGIN);
	//SetFilePointer(hFile,(Sect)*0x200,0,FILE_BEGIN);
	DWORD dwReadSize = 0;
    BOOL Ret= ReadFile(hFile,OutBuf,0x200,&dwReadSize,NULL);
	CloseHandle(hFile);
	return Ret;
}
BOOL CMySystem::WriteSector(__int64 Sect,BYTE* InBuf)//写一个扇区
{ 
	HANDLE hFile=CreateFile(L"\\\\.\\PhysicalDrive0",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	MyFileSeek (hFile,(Sect)*0x200,FILE_BEGIN);
	DWORD dwWritenSize = 0;
	WriteFile(hFile,InBuf,4,&dwWritenSize,NULL);
	CloseHandle(hFile);

    return true;
}
int CMySystem::ReadSectorByDrive(HANDLE hMyHandle,ULONG Sect,BYTE* InBuf)//读一个扇区,只支持小于4G的寻址
{

	int result=0;
	unsigned long  DeviceRet=0;

	*(ULONG*)InBuf=Sect;

	result=
		DeviceIoControl(
		hMyHandle  ,       // handle to volume
		IOCTL_KSBINSWORD_SCSI_READ,               // dwIoControlCode
		InBuf,           // MOVE_FILE_DATA structure
		0x200*sizeof(BYTE),         // size of input buffer
		InBuf,                          // lpOutBuffer
		0x200*sizeof(BYTE),      // nOutBufferSize
		&DeviceRet,                // number of bytes returned
		0); // OVERLAPPED structure
	
	return result;
}
int CMySystem::WriteSectorByDrive(HANDLE hMyHandle,ULONG Sect,BYTE* InBuf)//写一个扇区，只支持小于4G的寻址
{

	int result=0;
	unsigned long  DeviceRet=0;
    BYTE WriteBuf[0x204];
	memcpy(WriteBuf,InBuf,0x200);
	*(ULONG*)(WriteBuf+0x200*sizeof(BYTE))=Sect;

	result=
		DeviceIoControl(
		hMyHandle  ,       // handle to volume
		IOCTL_KSBINSWORD_SCSI_WRITE,               // dwIoControlCode
		WriteBuf,           // MOVE_FILE_DATA structure
		0x200*sizeof(BYTE)+sizeof(ULONG),         // size of input buffer
		WriteBuf,                          // lpOutBuffer
		sizeof(ULONG),      // nOutBufferSize
		&DeviceRet,                // number of bytes returned
		0); // OVERLAPPED structure
	
	return result;

}

void CMySystem::TrimPathW(WCHAR szModPath[])
{
	//\Device\HarddiskVolume1
	//int Len=strlen(szModPath);
	int i;
	if (szModPath[0]=='\\')
	{
		for ( i=0;szModPath[22+i];i++)
		{
			szModPath[i]=	szModPath[21+i];
		}
		szModPath[i]=	szModPath[21+i];
		szModPath[0]='c';
		szModPath[1]=':';
		szModPath[i+1]='\0';
	}
}
void CMySystem::TrimPath(char szModPath[])
{
	//\Device\HarddiskVolume1
	//int Len=strlen(szModPath);
	int i;
	if (szModPath[0]=='\\')
	{
		for ( i=0;szModPath[22+i];i++)
		{
			szModPath[i]=	szModPath[21+i];
		}
		szModPath[i]=	szModPath[21+i];
		szModPath[0]='c';
		szModPath[1]=':';
		szModPath[i+1]='\0';
	}
}
