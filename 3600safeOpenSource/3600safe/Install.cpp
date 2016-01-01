#include "stdafx.h"
#include "Install.h"

//QueryUserAgent(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings","User Agent",OutBuffer);
BOOL QueryUserAgent(HKEY HKey,char *lpSubKey,char *lpValueName,char *OutBuffer)
{
	HKEY hKey1;
	DWORD dwBufSize = 256;
	DWORD dwDataType = REG_EXPAND_SZ;
	unsigned char szQueryKey[256];

	if (RegOpenKeyExA(HKey,lpSubKey,NULL,KEY_QUERY_VALUE,&hKey1) != ERROR_SUCCESS)
	{
		return FALSE;
	}
	RegQueryValueExA(hKey1,lpValueName,NULL,&dwDataType,szQueryKey,&dwBufSize);
	RegCloseKey(hKey1);
	wsprintfA(OutBuffer,"%s",(char *)szQueryKey);

	return TRUE;
}
BOOL PrintfDosPath(__in LPCTSTR lpwzNtFullPath,__out LPCTSTR lpwzDosFullPath)
{
	char cDrive = 'A';

	for (int i=0;i<26;i++)  
	{
		memset((WCHAR *)lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
		swprintf(
			(WCHAR *)lpwzDosFullPath,
			L"%c:%s",
			cDrive+i,
			lpwzNtFullPath
			);
		if (GetFileAttributesW((WCHAR *)lpwzDosFullPath) != INVALID_FILE_ATTRIBUTES)
		{
			return TRUE;
		}
	}
	memset((WCHAR *)lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
	wcsncat((WCHAR *)lpwzDosFullPath,lpwzNtFullPath,wcslen(lpwzNtFullPath));
	return FALSE;
}
BOOL NtFilePathToDosFilePath(__in LPCTSTR lpwzNtFilePath,__out LPCTSTR lpwzDosFilePath)
{
	WCHAR lpwzDisk[10] = {0};
	WCHAR lpwzTemp[260] = {0};
	WCHAR lpwzNTDevice[260] = {0};
	char cDrive = 'A';

	//L"\\Device\\Harddisk\\WINDOWS\\system32\\drivers\\3600safe.sys"

	wcsncat(lpwzNTDevice,lpwzNtFilePath+wcslen(L"\\Device\\"),wcslen(lpwzNtFilePath)-wcslen(L"\\Device\\"));
	WCHAR *p = wcsstr(lpwzNTDevice,L"\\");
	if (!p)
	{
		wcsncat((WCHAR *)lpwzDosFilePath,lpwzNtFilePath,wcslen(lpwzNtFilePath));
		return FALSE;
	}
	swprintf(lpwzTemp,L"%ws",p);
	return PrintfDosPath(lpwzTemp,lpwzDosFilePath);
}
void SaveToFile(CHAR *lpszBuffer,WCHAR *lpwzFilePath)
{
	HANDLE	hFile = CreateFileW(lpwzFilePath, GENERIC_WRITE, FILE_SHARE_WRITE,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwBytesWrite = 0;
	SetFilePointer(hFile, 0, 0, FILE_END);
	WriteFile(hFile,lpszBuffer,strlen(lpszBuffer), &dwBytesWrite, NULL);
	CloseHandle(hFile);
}
BOOL IsWindows7()   
{   
	BOOL   bOsVersionInfoEx;   
	OSVERSIONINFOEX   osvi;   

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));   
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);   
	if(!(bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*)&osvi)))   
	{   
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);   
		if(!GetVersionEx((OSVERSIONINFO*)&osvi))     
			return   FALSE;   
	}
	if (osvi.dwBuildNumber >= 7600)
	{
		return TRUE;
	}
	return FALSE;
}

CHAR* setClipboardText(CHAR* str)
{
	if(OpenClipboard(NULL)==FALSE)
		return NULL;
	if(EmptyClipboard()==FALSE)
	{
		CloseClipboard();
		return NULL;
	}
	int sz=strlen(str)+1;
	HANDLE hMem=GlobalAlloc(GMEM_MOVEABLE, sz);
	if(hMem==NULL)
	{
		CloseClipboard();
		return NULL;
	}
	CHAR *pMem=(CHAR*)GlobalLock(hMem);
	if(pMem==NULL)
	{
		GlobalFree(hMem);
		CloseClipboard();
		return NULL;
	}
	memcpy(pMem,str,sz);
	GlobalUnlock(hMem);
	if(SetClipboardData(CF_TEXT,hMem)==FALSE)
	{
		GlobalFree(hMem);
		CloseClipboard();
		return NULL;
	}
	CloseClipboard();
	return str;
}
BOOL WINAPI EnableDebugPriv(LPCTSTR name)//提权函数
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tp;
	LUID luid;
	//打开进程令牌环
	if(!OpenProcessToken(
		GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,
		&hToken
		))
	{
		//  OpenProcessToken Error
		return FALSE;
	}
	//获得进程本地唯一ID
	if(!LookupPrivilegeValue(NULL, name, &luid))
	{
		//  LookupPrivivlegeValue Error;
		return FALSE;
	}

	tp.PrivilegeCount=1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	tp.Privileges[0].Luid=luid;

	//调整权限  
	if(!AdjustTokenPrivileges(
		hToken, 
		0,
		&tp, 
		sizeof(TOKEN_PRIVILEGES),
		NULL,
		NULL
		))
	{
		// AdjustTokenPrivileges Error
		return FALSE;
	}

	return TRUE;
}
ULONG GetDLLFileSize(char *lpszDLLPath)
{
	ULONG ulFileSize = NULL;

	HANDLE hFile = CreateFileA(lpszDLLPath,     //创建文件的名称。
		GENERIC_READ,          // 写和读文件。
		0,                      // 不共享读写。
		NULL,                   // 缺省安全属性。
		OPEN_EXISTING,          // 
		FILE_ATTRIBUTE_NORMAL, // 一般的文件。       
		NULL);                 // 模板文件为空。

	if (hFile != INVALID_HANDLE_VALUE) 
	{
		ulFileSize = GetFileSize(hFile,NULL);
		ulFileSize = ulFileSize/1024;
		CloseHandle(hFile);
	}
	return ulFileSize;
}
void ShutdownWindows( DWORD dwReason )
{
	EnableDebugPriv(SE_SHUTDOWN_NAME);
	ExitWindowsEx(dwReason, 0);
	EnableDebugPriv(SE_SHUTDOWN_NAME);	
}
DWORD InstallByZwLoadDriver(LPSTR Path,LPSTR lpszServiceName)
{
	DWORD Status, Ret=1, Value=SERVICE_KERNEL_DRIVER;
	UNICODE_STRING usKey;
	HKEY hk;
	char lpszSrvForMat[MAX_PATH] = {0};
	WCHAR lpwzServiceName[MAX_PATH] = {0};
	WCHAR lpwzLoadDriverForMat[MAX_PATH] = {0};

	sprintf(lpszSrvForMat,
		"SYSTEM\\CurrentControlSet\\Services\\%s",
		lpszServiceName);

	if(RegCreateKeyExA(HKEY_LOCAL_MACHINE, 
		lpszSrvForMat, 
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,
		NULL,
		&hk,
		NULL)!=ERROR_SUCCESS)
	{
		printf("Error with RegCreateKeyEx : %d\n", GetLastError());
		Ret=0;
		goto cleanup;
	}

	if(RegSetValueExA(
		hk, 
		"Type", 
		0, 
		REG_DWORD, 
		(LPBYTE)&Value,
		sizeof(DWORD))!=ERROR_SUCCESS)
	{
		printf("Error with RegSetValueEx : %d\n", GetLastError());
		Ret=0;
		goto cleanup;
	}		

	/*
	If dwType is the REG_SZ, REG_MULTI_SZ, or REG_EXPAND_SZ type and the ANSI version of this function is used 
	(either by explicitly calling RegSetValueExA or by not defining UNICODE before including the Windows.h file),
	the data pointed to by the lpData parameter must be an ANSI character string. 
	The string is converted to Unicode before it is stored in the registry
	*/

	if(RegSetValueExA(hk, "ImagePath", 0, REG_EXPAND_SZ, (const PBYTE)Path, lstrlenA(Path))!=ERROR_SUCCESS)
	{
		printf("Error with RegSetValueEx : %d\n", GetLastError());
		Ret=0;
		goto cleanup;
	}

	Value=SERVICE_DEMAND_START;

	if(RegSetValueExA(hk, "Start", 0, REG_DWORD, (LPBYTE)&Value, sizeof(DWORD))!=ERROR_SUCCESS)
	{
		printf("Error with RegSetValueEx : %d\n", GetLastError());
		Ret=0;
		goto cleanup;
	}
	MultiByteToWideChar (CP_ACP,
		0, 
		lpszServiceName, 
		-1, 
		lpwzServiceName, 
		strlen(lpszServiceName)
		);
	wcscat(lpwzLoadDriverForMat,L"\\Registry\\Machine\\System\\CurrentControlSet\\Services\\");
	wcscat(lpwzLoadDriverForMat,lpwzServiceName);

	RtlInitUnicodeString(&usKey,lpwzLoadDriverForMat);

	Status=NtLoadDriver(&usKey);
	if(Status!=STATUS_SUCCESS)
	{	
		//printf("Error with NtLoadDriver : 0x%x : %d \n", Status, RtlNtStatusToDosError(Status));
		Ret=0;
	}

cleanup:

	RegCloseKey(hk); 

	return Ret; 
}
BOOL BFS_WriteFile(char *targetPath,unsigned char *lpszCode,ULONG ulSize)
{
	DWORD dwBytesWrite;

	HANDLE	hFile = CreateFileA(targetPath, GENERIC_WRITE, FILE_SHARE_WRITE,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	SetFilePointer(hFile, 0, 0, FILE_END);
	WriteFile(hFile,lpszCode,ulSize, &dwBytesWrite, NULL);  //写入尾
	CloseHandle(hFile);
	return TRUE;
}
BOOL Install(HWND hwndDlg)
{

	DWORD dwReadByte;
	char lpszInit[4] = {0};
	char lpszWindowsPath[256] = {0};
	char lpszNumber[256] = {0};
	char lpszLoadDriverPath[256] = {0};
	int i=0;

	memset(lpszInit,0,sizeof(lpszInit));
	ReadFile((HANDLE)SAFE_SYSTEM,lpszInit,4,&dwReadByte,0);
	if (strcmpi("safe",lpszInit) == NULL)
	{
		goto InitSuccess;
	}
	GetWindowsDirectoryA(
		lpszWindowsPath,
		sizeof(lpszWindowsPath)
		);
	//sprintf(lpszNumber,"%d",GetTickCount());
	sprintf(lpszNumber,"%s","A-Protect");

	char lpszSrvices[256] = {0};
	sprintf(lpszSrvices,"SYSTEM\\CurrentControlSet\\Services\\%s",lpszNumber);
	SHDeleteKeyA(HKEY_LOCAL_MACHINE,lpszSrvices);

	strcat(lpszWindowsPath,"\\");
	strcat(lpszWindowsPath,lpszNumber);
	strcat(lpszWindowsPath,".sys");

	BFS_WriteFile(
		lpszWindowsPath,
		lpszKernelModule,
		sizeof(lpszKernelModule)
		);
	if (GetFileAttributesA(lpszWindowsPath) == INVALID_FILE_ATTRIBUTES)
	{
		if (IsWindows7())
			MessageBoxA(hwndDlg,"释放驱动文件失败，win7系统下右键“以管理员身份运行”","“A盾电脑防护”",MB_ICONERROR);
		else
			MessageBoxA(hwndDlg,"释放驱动文件失败","“A盾电脑防护”",MB_ICONERROR);

		ExitProcess(0);
	}
	wsprintfA(
		lpszLoadDriverPath,
		"\\??\\%s",
		lpszWindowsPath
		);

	if(!EnableDebugPriv(SE_LOAD_DRIVER_NAME))
	{
		DeleteFileA(lpszWindowsPath);
		MessageBoxA(hwndDlg,"没有足够的权限加载驱动！","“A盾电脑防护”",MB_ICONERROR);
		ExitProcess(0);
	}
	if (!InstallByZwLoadDriver(lpszLoadDriverPath,lpszNumber))
	{
		DeleteFileA(lpszWindowsPath);
		MessageBoxA(hwndDlg,"加载驱动失败！","“A盾电脑防护”",MB_ICONERROR);
		SHDeleteKeyA(HKEY_LOCAL_MACHINE,lpszSrvices);
		ExitProcess(0);
	}
	DeleteFileA(lpszWindowsPath);
	SHDeleteKeyA(HKEY_LOCAL_MACHINE,lpszSrvices);

	i = 0;
Last:
	Sleep(3000);
	memset(lpszInit,0,sizeof(lpszInit));
	ReadFile((HANDLE)SAFE_SYSTEM,lpszInit,4,&dwReadByte,0);
	if (strcmpi("safe",lpszInit) != NULL)
	{
		i++;
		if (i>5)
		{
			MessageBoxA(hwndDlg,"“A盾电脑防护”初始化失败，有可能如下原因导致：\r\n\r\n1:病毒阻止了“A盾电脑防护”的启动\r\n2:某些安全软件恢复、阻止“A盾电脑防护”的钩子\r\n3:在装有微点的机器上，“A盾电脑防护”的初始化失败","“A盾电脑防护”",MB_ICONERROR);
			SHDeleteKeyA(HKEY_LOCAL_MACHINE,lpszSrvices);
			DeleteFileA(lpszWindowsPath);
			ExitProcess(0);
		}
		goto Last;
	}

InitSuccess:

	return TRUE;
}
/////////////////////////////////////////////

BOOL InstallDepthServicesScan(CHAR * serviceName)
{
	BOOL	ret = FALSE;
	LONG	result;
	CHAR	sysDirPath[MAX_PATH];
	CHAR	targetPath[MAX_PATH];
	DWORD   dwBytesWrite;

	GetSystemDirectoryA(sysDirPath, sizeof(sysDirPath));
	wsprintfA(targetPath, "%s\\Drivers\\%s.sys", sysDirPath, serviceName);
	if (GetFileAttributesA(targetPath) != INVALID_FILE_ATTRIBUTES)
	{
		return TRUE;
	}
	if (BFS_WriteFile(targetPath,lpszKernelModule,sizeof(lpszKernelModule)) == FALSE)
	{
		printf("create file failed\r\n");
		return FALSE;
	}

	HKEY regKey;
	result =RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		"SYSTEM\\CurrentControlSet\\Services",
		0,
		KEY_ALL_ACCESS,
		&regKey);

	if (SUCCEEDED(result))
	{
		HKEY	subKey;
		if (SUCCEEDED(RegCreateKeyA(regKey, serviceName, &subKey)))
		{
			DWORD	data = 0x1;
#define SET_DWORD(_key, _valueName, _data) {data = _data; RegSetValueExA(_key, _valueName, NULL, REG_DWORD, (LPBYTE)&data, sizeof(DWORD));}
			data = 0x1;

			SET_DWORD(subKey, "ErrorControl", SERVICE_ERROR_NORMAL);
			SET_DWORD(subKey, "Start", SERVICE_BOOT_START);
			SET_DWORD(subKey, "Type", SERVICE_KERNEL_DRIVER);
			SET_DWORD(subKey, "Tag", 10);
			RegFlushKey(subKey);
			RegCloseKey(subKey);
		}
		RegCloseKey(regKey);
	}

	result =RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		"SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E967-E325-11CE-BFC1-08002BE10318}",
		0,
		KEY_READ|KEY_WRITE,
		&regKey);

	if (SUCCEEDED(result))
	{
		CHAR buff[1024];
		DWORD retLen = sizeof(buff);
		ULONG type = REG_MULTI_SZ;

		memset( buff, 0, sizeof(buff));

		RegQueryValueExA( regKey,
			"UpperFilters",
			0,
			&type,
			(LPBYTE)buff, 
			&retLen);

		BOOL	alreadyExists = FALSE;
		CHAR * ptr = NULL;
		for (ptr = buff; *ptr; ptr += lstrlenA(ptr) + 1)
		{
			if(lstrcmpiA(ptr, serviceName) == 0)
			{
				alreadyExists = TRUE;
				break;
			}
		}
		if (!alreadyExists)
		{
			DWORD	added = lstrlenA(serviceName);
			memcpy(ptr, serviceName, added * sizeof(CHAR));

			ptr += added;

			*ptr = '\0';
			*(ptr+1) = '\0';

			result = RegSetValueExA(regKey, "UpperFilters", 0, REG_MULTI_SZ, (LPBYTE)buff, retLen + ((added + 1) * sizeof(CHAR)));
		}

		ret = TRUE;

		RegCloseKey(regKey);
	}

	return ret;
}
BOOL UninstallDepthServicesScan(CHAR * serviceName)
{
	BOOL	ret = FALSE;

	CHAR	sysDirPath[MAX_PATH];
	CHAR	targetPath[MAX_PATH];

	GetSystemDirectoryA(sysDirPath, sizeof(sysDirPath));
	wsprintfA(targetPath, "%s\\Drivers\\%s.sys", sysDirPath, serviceName);

	DeleteFileA(targetPath);

	HKEY regKey;
	LONG result;
	result =RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		"SYSTEM\\CurrentControlSet\\Services",
		0,
		KEY_READ|KEY_WRITE,
		&regKey);
	if( ERROR_SUCCESS == result )
	{
		SHDeleteKeyA(regKey, serviceName);
		// 一定要flush,否则不保存
		RegFlushKey(regKey);
	}

	result =RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		"SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E967-E325-11CE-BFC1-08002BE10318}",
		0,
		KEY_READ|KEY_WRITE,
		&regKey);

	if( ERROR_SUCCESS == result )
	{
		CHAR buff[1024];
		DWORD retLen = sizeof(buff);
		ULONG type = REG_MULTI_SZ;

		memset( buff, 0, sizeof(buff));

		RegQueryValueExA( regKey,
			"UpperFilters",
			0,
			&type,
			(LPBYTE)buff, 
			&retLen);

		BOOL	alreadyExists = FALSE;
		for (CHAR * ptr = buff; *ptr; ptr += lstrlenA(ptr) + 1)
		{
			if(lstrcmpiA(ptr, serviceName) == 0)
			{
				DWORD	removeLength = (lstrlenA(ptr) + 1) * sizeof(CHAR);
				memmove(ptr, (char *)ptr + removeLength, ((char *)ptr + removeLength - (char *)buff) *  sizeof(CHAR));

				result = RegSetValueExA(regKey,"UpperFilters", 0, REG_MULTI_SZ, (LPBYTE)buff, retLen - removeLength);
				break;
			}
		}

		ret = TRUE;
		RegFlushKey(regKey);
		RegCloseKey(regKey);
	}
	return ret;
}