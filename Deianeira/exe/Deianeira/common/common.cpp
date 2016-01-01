#include "common.h"

//
char* common_TCHARToCHAR(TCHAR* pwstr)
{
	//
	int len = 2 * wcslen(pwstr);
	char* pstr_ = new char[len + 1];
	//
	wcstombs(pstr_,pwstr,len + 1);
	return pstr_;
}

//
TCHAR* common_CHARToTCHAR(char* pstr_)
{
	//
	int len = strlen(pstr_);
	wchar_t* pwstr_ = new wchar_t[len + 1];
	//
	mbstowcs(pwstr_,pstr_,len + 1);
	return pwstr_;
}

//UTF-8到GBK的转换
char* common_UTFToGBK(const char* pstr_)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, pstr_, -1, NULL, 0);
	wchar_t* pwstr = new wchar_t[len + 1];
	ZeroMemory(pwstr,len + 1);

	MultiByteToWideChar(CP_UTF8, 0, pstr_, -1, pwstr, len);

	len = WideCharToMultiByte(CP_ACP, 0, pwstr, -1, NULL, 0, NULL, NULL);
	char* pstr = new char[len + 1];
	ZeroMemory(pstr,len + 1);

	WideCharToMultiByte(CP_ACP, 0, pwstr, -1, pstr, len, NULL, NULL);

	if(pwstr) delete[] pwstr;

	return pstr;
}

//GBK到UTF-8的转换
char* common_GBKToUTF(const char* pstr_)
{
	int len = MultiByteToWideChar(CP_ACP, 0, pstr_, -1, NULL, 0);
	wchar_t* pwstr = new wchar_t[len + 1];
	ZeroMemory(pwstr,len + 1);

	MultiByteToWideChar(CP_ACP, 0, pstr_, -1, pwstr, len);

	len = WideCharToMultiByte(CP_UTF8, 0, pwstr, -1, NULL, 0, NULL, NULL);
	char* pstr = new char[len + 1];
	ZeroMemory(pstr,len + 1);

	WideCharToMultiByte(CP_UTF8, 0, pwstr, -1, pstr, len, NULL, NULL);

	if(pwstr) delete[] pwstr;

	return pstr;
}

//关机重启
//bReboot:标志是关机还是重启
//返回:true:成功，false:失败
bool common_shutdown(bool bReboot)
{
	HMODULE hNtdll;
	NTSTATUS status;

	PNtSetSystemPowerState NtSetSystemPowerState = NULL;
	PNtShutdownSystem NtShutdownSystem = NULL;

	hNtdll = LoadLibrary(L"ntdll.dll");

	//如果加载失败
	if (!hNtdll)
	{
		return false;
	}

	//如果是重启的命令
	if (bReboot)
	{
		NtShutdownSystem = (PNtShutdownSystem)GetProcAddress(hNtdll, "NtShutdownSystem");

		//如果地址获得失败
		if (!NtShutdownSystem) 
		{
			return false;
		}

		status = NtShutdownSystem(ShutdownReboot);

		if (!NT_SUCCESS(status))
		{
			return false;
		}

		return true;
	}
	else
	{
		//获取地址
		NtSetSystemPowerState = (PNtSetSystemPowerState)GetProcAddress(hNtdll, "NtSetSystemPowerState");

		//如果地址获得失败
		if (NtSetSystemPowerState==NULL) 
		{
			return false;
		}

		//设置电源状态
		status = NtSetSystemPowerState((POWER_ACTION)PowerSystemShutdown,(SYSTEM_POWER_STATE)PowerActionShutdown,SHTDN_REASON_MAJOR_OTHER|SHTDN_REASON_MINOR_OTHER|SHTDN_REASON_FLAG_PLANNED);

		if (!NT_SUCCESS(status))
		{
			return false;
		}

		return true;
	}
}

//提升进程权限
//hToken:进程令牌
//lpszPrivilege:权限别名
//bEnablePrivilege:使能
//返回:true:成功，false:失败
bool common_setPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege){
	TOKEN_PRIVILEGES tp;

	tp.PrivilegeCount = 1;

	if (!LookupPrivilegeValue( 
		NULL,            // lookup privilege on local system
		lpszPrivilege,   // privilege to lookup 
		&tp.Privileges[0].Luid ))        // receives LUID of privilege
	{
		return false; 
	}

	if (bEnablePrivilege)
	{
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	}
	else
	{
		tp.Privileges[0].Attributes = 0;
	}

	// Enable the privilege or disable all privileges.
	if (!AdjustTokenPrivileges(
		hToken, 
		FALSE, 
		&tp, 
		sizeof(TOKEN_PRIVILEGES), 
		NULL, 
		NULL))
	{ 
		return false; 
	} 

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
	{
		return false;
	} 

	return true;
}

//提升进程权限
//lpszPrivilege:权限别名
//bEnablePrivilege:使能
//返回:true:成功，false:失败
bool common_setPrivilege(ULONG ulPID, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege){
	HANDLE hProcess;
	HANDLE hToken;

	hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,ulPID);

	//如果打开进程失败
	if (!hProcess)
	{
		return false;
	}

	//提升进程权限
	if(!OpenProcessToken(hProcess,TOKEN_ADJUST_PRIVILEGES,&hToken))
	{
		return false;
	}

	if (!common_setPrivilege(hToken,lpszPrivilege,bEnablePrivilege))
	{
		return false;
	}		

	return true;
}

//加载驱动
//lpFileName:驱动文件路径
//lpLinkName:服务名称
//返回:成功则返回驱动句柄,失败则返回NULL
HANDLE common_loadDriver(LPCTSTR lpFileName,LPTSTR lpLinkName){
	HANDLE hDriver;

	// 打开SCM管理器
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if(hSCM == NULL)
	{
		return NULL;
	}

	// 创建或打开服务
	SC_HANDLE hService = CreateService(hSCM, lpLinkName, lpLinkName, SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, lpFileName, NULL, 0, NULL, NULL, NULL);

	if(hService == NULL)
	{
		int nError = GetLastError();
		if(nError == ERROR_SERVICE_EXISTS || nError == ERROR_SERVICE_MARKED_FOR_DELETE)
		{
			hService = OpenService(hSCM, lpLinkName, SERVICE_ALL_ACCESS);
		}
	}

	if(hService == NULL)
	{
		return NULL;
	}

	// 启动服务
	// 这里调用DriverEntry例程
	if(!StartService(hService, 0, NULL))  
	{
		int nError = GetLastError();
		if(nError != ERROR_SERVICE_ALREADY_RUNNING)
		{	
			return NULL;
		}
	}

	// 打开到驱动程序所控制设备的句柄
	TCHAR sz[256];
	wsprintf(sz, L"\\\\.\\%s", lpLinkName);

	//打开设备
	hDriver = CreateFile(sz, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(!hDriver)
	{	
		return NULL;
	}

	return hDriver;
}

//卸载驱动
//hDriver:驱动模块句柄
//lpLinkName:服务名称
//返回:true:成功，false:失败
bool common_unloadDriver(HANDLE hDriver,LPTSTR lpLinkName){
	SERVICE_STATUS ss;

	//首先释放驱动句柄
	CloseHandle(hDriver);

	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);

	if (!hSCManager)
	{
		return false;
	}

	SC_HANDLE hService = OpenService(hSCManager, lpLinkName, DELETE | SERVICE_STOP);

	if (!hService)
	{
		CloseServiceHandle(hSCManager);
		return false;
	}

	ControlService(hService, SERVICE_CONTROL_STOP, &ss);
	DeleteService(hService );

	CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);

	return true;
}

//打开文件属性对话框
//lpFilePath:目标文件路径
//返回:无
void common_showFileProperty(LPTSTR lpFilePath)
{
	SHELLEXECUTEINFO sei;
	sei.fMask=SEE_MASK_INVOKEIDLIST|SEE_MASK_NOCLOSEPROCESS|SEE_MASK_FLAG_NO_UI; 
	sei.hwnd=NULL; 
	sei.hInstApp=0; 
	sei.lpVerb=L"properties"; 
	sei.lpFile=lpFilePath; 
	sei.lpParameters=NULL; 
	sei.lpDirectory=NULL; 
	sei.nShow=1; 
	sei.lpIDList=0; 
	sei.cbSize=sizeof(sei); 
	ShellExecuteEx(&sei);
} 

//定位文件
//lpFilePath:目标文件路径
//返回:无
void common_explorerFile(LPSTR lpFilePath)
{
	//防止文件路径过长，而无法保存
	CHAR buffer[512]={"explorer.exe   /select,"};
	strcat_s(buffer,lpFilePath);
	WinExec(buffer,SW_SHOW); 
} 

//定位注册表
//lpRegPath:注册表路径
//返回:true:成功，false:失败
bool common_explorerReg(LPTSTR lpRegPath)
{
	HKEY hKey;

	//打开相关注册表
	if (RegOpenKey(HKEY_CURRENT_USER,L"Software\\Microsoft\\Windows\\CurrentVersion\\Applets\\Regedit",&hKey))
	{
		return false;
	}

	//写入最后一次打开的位置
	if (RegSetValueEx(hKey,L"lastkey",0,REG_SZ,(LPBYTE)lpRegPath,MAX_PATH))
	{
		return false;
	}

	//如果写入成功，则关闭
	RegCloseKey(hKey);

    return true;
} 

//根据资源ID来生产文件 
//resId:资源ID 
//lpResType:资源类型
//lpFileName:目标文件路径 
//返回:true:成功，false:失败
bool common_createFileFromResource(int resId,LPTSTR lpResType,LPTSTR lpFileName) 
{ 
	//查找指定ID类型的资源
	HRSRC hRsrc = FindResource(NULL, MAKEINTRESOURCE(resId),lpResType); 

    //如果没有查找到相关资源
	if (!hRsrc) 
	{ 
		return false; 
	} 

	//得到资源的大小
	DWORD dwSize = SizeofResource(NULL, hRsrc);  
	//如果获取资源大小失败
	if (dwSize == 0) 
	{ 
		return false; 
	} 

	//装载资源
	HGLOBAL hGlobal = LoadResource(NULL, hRsrc);
    //如果装载资源失败
	if (!hGlobal) 
	{ 
		return false; 
	} 

    //锁定资源
	LPVOID lpBuffer = LockResource(hGlobal); 
	//如果锁定资源失败
	if (!lpBuffer) 
	{ 
		//释放加载的资源
		FreeResource(hGlobal); 
		return false; 
	} 

	//创建文件
	FILE* lpFile;
	_wfopen_s(&lpFile,lpFileName,L"wb"); 

	if (!lpFile)
	{
		//释放加载的资源
		FreeResource(hGlobal); 
		return false;
	}

    //如果创建文件成功
	//把资源写入文件
	if (dwSize == fwrite(lpBuffer, sizeof(byte), dwSize, lpFile)) 
	{
		fclose(lpFile); //释放文件句柄 
	}

	//释放加载的资源
	FreeResource(hGlobal); 
	return true;
}


//添加服务
//lpName:服务名称
//lpDisplayName:友好显示名称
//ulStartMode:启动类型
//lpPath:应用程序路径
//返回:true:成功，false:失败
bool common_addService(LPTSTR lpName,LPTSTR lpDisplayName,ULONG ulStartMode,LPTSTR lpPath)
{
	SC_HANDLE hManager;
	SC_HANDLE hService;

	if((hManager=OpenSCManager(NULL,NULL,SC_MANAGER_CREATE_SERVICE))==NULL)
	{
		//如果打开服务控制管理器失败
		return false;
	}

	//创建服务
	hService=CreateService(
		hManager,lpName,lpDisplayName, 
		SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, 
		ulStartMode, SERVICE_ERROR_NORMAL,
		lpPath, 0, 0, 0, 0, 0 );

	//如果创建服务失败
	if(!hService)
	{
		CloseServiceHandle(hManager);
		return false;
	}

	//如果成功
	CloseServiceHandle(hService);
	CloseServiceHandle(hManager);

	return true;
}

//删除服务
//lpName:服务名称
//返回:true:成功，false:失败
bool common_delService(LPTSTR lpName)
{
	SC_HANDLE hManager;
	SC_HANDLE hService;
	SERVICE_STATUS status;

	if((hManager=OpenSCManager(NULL,NULL,SC_MANAGER_CREATE_SERVICE))==NULL)
	{
		//如果打开服务控制管理器失败
		return false;
	}

	//打开已存在服务
	hService=OpenService(hManager,lpName,SERVICE_ALL_ACCESS|DELETE);

	//如果打开失败
	if (!hService)
	{
		CloseServiceHandle(hManager);
		return false;
	}

	//查询服务状态
	BOOL blResult=QueryServiceStatus(hService,&status);

	//如果失败
	if (!blResult)
	{
		CloseServiceHandle(hService);
		CloseServiceHandle(hManager);
		return false;
	}

	//判断服务状态
	if (status.dwCurrentState!=SERVICE_STOPPED)
	{
		//如果服务没有被标记删除,就发现服务控制代码
		blResult=ControlService(hService,SERVICE_CONTROL_STOP,&status);
		//发送功能代码失败
		if (!blResult)
		{
			CloseServiceHandle(hService);
			CloseServiceHandle(hManager);
			return false;
		}
	}
	
	//服务停止后，删除此服务
	blResult=DeleteService(hService);

	//删除服务失败
	if (!blResult)
	{
		CloseServiceHandle(hService );
		CloseServiceHandle(hManager);
		return false;
	}

	CloseServiceHandle(hService );
	CloseServiceHandle(hManager);

	return true;
}

//设置代理
//lpIP:IP地址
//lpPort:端口
//返回:true:成功，false:失败
bool common_setProxy(LPTSTR lpIP,LPTSTR lpPort)
{		 
	INTERNET_PER_CONN_OPTION_LIST list;
	BOOL    bRet;

	//填充结构体
	list.dwSize = sizeof(list);
	// NULL等于Lan
	list.pszConnection = NULL;
	// 设置三个选项
	list.dwOptionCount = 3;
	list.pOptions = new INTERNET_PER_CONN_OPTION[3];

	// 确保内存被分配
	if(!list.pOptions)
	{
		return false;
	}

	//设置标志
	list.pOptions[0].dwOption = INTERNET_PER_CONN_FLAGS;
	list.pOptions[0].Value.dwValue = PROXY_TYPE_DIRECT | PROXY_TYPE_PROXY;
	//设置代理名称
	list.pOptions[1].dwOption = INTERNET_PER_CONN_PROXY_SERVER;

	TCHAR proxyAddress[MAX_PATH];
	//清零
	memset(proxyAddress,0,MAX_PATH);
	//处理字符串
	lstrcat(proxyAddress,L"http://");
	lstrcat(proxyAddress,lpIP);
	lstrcat(proxyAddress,L":");
	lstrcat(proxyAddress,lpPort);

	//设置连接字符串
	list.pOptions[1].Value.pszValue =proxyAddress;
	//重写代理
	list.pOptions[2].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
	list.pOptions[2].Value.pszValue = L"local";

	//设置选项
	bRet = InternetSetOption(NULL,INTERNET_OPTION_PER_CONNECTION_OPTION, &list, sizeof(list));

	//释放内存
	delete [] list.pOptions;

	if (!bRet)
	{
		return false;
	}

	return true;		
}

//取消代理
//返回:true:成功，false:失败
bool common_delProxy()
{
	HKEY hKey;
	DWORD dwValue=0;

	//打开注册表
	if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings", 0, KEY_ALL_ACCESS, &hKey))
	{
		return false;
	}		 

	//设置注册表键值
	if (RegSetValueEx(hKey, L"ProxyEnable", 0, REG_DWORD,(LPBYTE)&dwValue, sizeof(DWORD)))
	{
		RegCloseKey(hKey);
		return false;
	}		 

	RegCloseKey(hKey);

	//发出通知
	InternetSetOption(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);    
	InternetSetOption(NULL ,INTERNET_OPTION_REFRESH , NULL, 0);

	return true;
}

//判断文件是否是PE文件
//lpFilePath:目标文件
//返回:true:是，false:不是
bool common_isPEFile(LPTSTR lpFilePath){
	//打开目标文件
	HANDLE hFile = CreateFile(lpFilePath,GENERIC_READ,FILE_SHARE_READ,NULL,3,0,NULL);

	//打开失败
	if (!hFile)
	{
		return false;
	}

	//文件映射
	HANDLE hMap = CreateFileMapping(hFile,NULL,PAGE_READONLY,0,0,NULL);

	//映射失败
	if (!hMap)
	{			
		CloseHandle(hFile);
		return false;
	}

	//文件到内存的映射
	PVOID pBase = MapViewOfFile(hMap,FILE_MAP_READ,0,0,0);

	//失败
	if (!pBase)
	{
		CloseHandle(hMap);
		CloseHandle(hFile);
		return false;
	}

	//填充相关PE结构体
	//获得DOS头部
	PIMAGE_DOS_HEADER pImage_dos_header=(PIMAGE_DOS_HEADER)pBase;

	//不是有效PE标志
	if ((pImage_dos_header->e_magic!=IMAGE_DOS_SIGNATURE))
	{
		CloseHandle(hMap);
		CloseHandle(hFile);
		UnmapViewOfFile(pBase);
		return false;
	}

	//NT头部
	PIMAGE_NT_HEADERS pImage_nt_header = (PIMAGE_NT_HEADERS)((ULONG)pImage_dos_header + pImage_dos_header->e_lfanew);

	//不是有效PE标志
	if (pImage_nt_header->Signature != IMAGE_NT_SIGNATURE)
	{
		CloseHandle(hMap);
		CloseHandle(hFile);
		UnmapViewOfFile(pBase);
		return false;
	}

	CloseHandle(hMap);
	CloseHandle(hFile);
	UnmapViewOfFile(pBase);

	return true;
}

//获得快捷方式文件所指向的路径
//lpwsLinkName:lnk文件的路径
//lpwsLinkPath:用于存放所指程序路径的缓冲区
//返回:HRESULT
HRESULT common_getLnkPath(IN LPWSTR lpwsLinkName,OUT LPWSTR lpwsLinkPath)
{
	HRESULT hResult;
	IShellLink *pIShellLink;
	WIN32_FIND_DATA wfd;

	//初始化
	CoInitialize(NULL);
	//创建实例
	hResult = CoCreateInstance((REFIID)CLSID_ShellLink,NULL,CLSCTX_INPROC_SERVER,(REFIID)IID_IShellLink,(LPVOID *)&pIShellLink);

	//如果成功
	if (SUCCEEDED(hResult))
	{
		IPersistFile *pIPersistFile;
		//查询相关信息
		hResult = pIShellLink->QueryInterface((REFIID)IID_IPersistFile,(LPVOID *)&pIPersistFile);

		//如果查询成功
		if (SUCCEEDED(hResult))
		{
			//加载快捷方式文件
			hResult = pIPersistFile->Load(lpwsLinkName, STGM_READ);

			//如果成功
			if (SUCCEEDED(hResult))
			{
				//解析
				hResult = pIShellLink->Resolve(NULL,SLR_ANY_MATCH | SLR_NO_UI);

				if (SUCCEEDED(hResult))
				{
					//获得快捷方式的制定的路径
					hResult = pIShellLink->GetPath(lpwsLinkPath,MAX_PATH,&wfd,SLGP_SHORTPATH);
				}

			}

			pIPersistFile->Release();
		}

		pIShellLink->Release();
	}

	return hResult;
}

//生成IP地址字符串
PCHAR GetIP(unsigned int ipaddr)
{
	static char pIP[20];
	unsigned int nipaddr = htonl(ipaddr);
	sprintf_s(pIP, "%d.%d.%d.%d",
		(nipaddr>>24) &0xFF,
		(nipaddr>>16) &0xFF,
		(nipaddr>>8) &0xFF,
		(nipaddr)&0xFF);
	return pIP;
}
