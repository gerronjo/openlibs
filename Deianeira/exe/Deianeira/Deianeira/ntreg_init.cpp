#include "stdafx.h"
#include "ntreg_init.h"


//
map<wstring, wstring> map_ntreg;
map<wstring, HANDLE> map2_ntreg;

//全局变量
pNtQuerySystemInformation ZwQuerySystemInformation = NULL;
pZwQueryInformationFile ZwQueryInformationFile = NULL;
pZwQueryObject ZwQueryObject = NULL;
pZwTerminateThread ZwTerminateThread = NULL;


//根据镜像名称获得进程ID
int ntreg_init_getProcessIDByImageName(LPTSTR lpName){
	//
	PROCESSENTRY32 pe32;

	//
	ZeroMemory(&pe32,sizeof(PROCESSENTRY32));

	//
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	//
	if (!hSnapShot)
	{
		//
		return -1;
	}
	//
	pe32.dwSize = sizeof(PROCESSENTRY32);
	//
	if(Process32First(hSnapShot, &pe32))
	{
		//
		do 
		{
			//
			if(_tcsicmp(pe32.szExeFile, lpName) == 0)
			{
				//
				CloseHandle(hSnapShot);
				//
				return pe32.th32ProcessID;
			}
		} while(Process32Next(hSnapShot, &pe32));
	}

	//
	CloseHandle(hSnapShot);
	//
	return -1;
}

//获得文件对象类型索引
DWORD ntreg_init_getFileObjectTypeNumber(PSYSTEM_HANDLE_INFORMATION pSystem_handle_information,HANDLE hFile){	;
	//
	DWORD dwPID = GetCurrentProcessId();

	//
	for (int i=0;i < pSystem_handle_information->NumberOfHandles;i++)
	{
		//
		if (pSystem_handle_information->Information[i].UniqueProcessId == dwPID)
		{		
			//
			if((USHORT)hFile == pSystem_handle_information->Information[i].HandleValue)
			{
				//
				return pSystem_handle_information->Information[i].ObjectTypeIndex;
			}		
		}
	}

	//
	return -1;
}

//获得系统句柄表
PSYSTEM_HANDLE_INFORMATION ntreg_init_getSystemHandleInformation()
{
	NTSTATUS status;
	DWORD dwSize = 0x1000;
	PSYSTEM_HANDLE_INFORMATION pSystem_handle_information = NULL;

	//
	pSystem_handle_information = (PSYSTEM_HANDLE_INFORMATION)malloc(dwSize); 

	//
	while ((status = ZwQuerySystemInformation(SystemHandleInformation,pSystem_handle_information, dwSize, NULL))
		== STATUS_INFO_LENGTH_MISMATCH)            
	{            
		//
		free(pSystem_handle_information);             
		pSystem_handle_information = (PSYSTEM_HANDLE_INFORMATION)malloc(dwSize = dwSize * 2); ;
	} 

	//
	if (!NT_SUCCESS(status))
	{
		//
		free(pSystem_handle_information);
		return false;
	}

	//
	return pSystem_handle_information;
}

//获得文件对象名称
PVOID ntreg_init_getFileObjectName(HANDLE hObject)
{
	PVOID pBuffer = NULL;
	DWORD dwSize = 0x1000;
	NTSTATUS	status;
	PUNICODE_STRING pUstrName; 
	IO_STATUS_BLOCK ioStatus ;

	//
	pBuffer = (PVOID)malloc(dwSize); 

	//
	while ((status = ZwQueryObject(hObject,ObjectNameInformation,pBuffer,dwSize,&dwSize))
		== STATUS_INFO_LENGTH_MISMATCH)            
	{            
		//
		free(pBuffer);             
		pBuffer = (PVOID)malloc(dwSize = dwSize * 2);
	}

	//
	pUstrName = (PUNICODE_STRING)pBuffer;

	//
	if(pUstrName->Buffer)
	{
		//
		return pUstrName;
	}

	//
	return NULL ;
}

//从注册表中获得hivelist
bool ntreg_init_getHivelist(map<wstring, wstring> &map_){
	//
	DWORD dwIndex = 0;
	HKEY hKey = NULL;
	TCHAR tcName[MAX_PATH] = { 0 };
	DWORD dwSize = MAX_PATH;
	DWORD dwType = 0;
	TCHAR tcValue[MAX_PATH] = { 0 };
	DWORD dwValueSize = MAX_PATH;

	//
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,L"SYSTEM\\CurrentControlSet\\Control\\hivelist",0,KEY_READ,&hKey) != ERROR_SUCCESS) 
	{
		//
		return false ;
	}

	//
	while(RegEnumValue(hKey,dwIndex,tcName,&dwSize,0,&dwType,(LPBYTE)tcValue,&dwValueSize) != ERROR_NO_MORE_ITEMS)
	{
		//
		map_.insert(map<wstring, wstring>::value_type(tcName,tcValue));

		//
		dwSize = MAX_PATH;
		dwValueSize = MAX_PATH;
		dwIndex++;
	}

	//
	RegCloseKey(hKey);
	return true ;
}

//复制hive文件句柄到本地进程
bool ntreg_init_getDuplicateHandle(PSYSTEM_HANDLE_INFORMATION pSystem_handle_information,DWORD dwSystemPID,DWORD dwFileObjectTypeNumber,map<wstring, wstring> &map_,map<wstring, HANDLE> &map2_)
{
	//
	HANDLE hRet = NULL;

	//
	HANDLE hSystemProcess = OpenProcess(PROCESS_DUP_HANDLE,FALSE,dwSystemPID);

	//
	if(!hSystemProcess)
	{
		//
		return false;
	}

	//
	HANDLE hCurrentHandle = GetCurrentProcess();
	
	//
	for(int i = 0; i < pSystem_handle_information->NumberOfHandles; i++)
	{
		//
		if (pSystem_handle_information->Information[i].UniqueProcessId == dwSystemPID && dwFileObjectTypeNumber == pSystem_handle_information->Information[i].ObjectTypeIndex)
		{
			//
			if (!DuplicateHandle(hSystemProcess,(HANDLE)pSystem_handle_information->Information[i].HandleValue, hCurrentHandle,&hRet,0,FALSE,DUPLICATE_SAME_ACCESS))
			{
				//
				continue;
			}

			//
			TCHAR tmpName[4096] = { 0 };
			//
			PUNICODE_STRING pObjectName = (PUNICODE_STRING)ntreg_init_getFileObjectName(hRet);

			//
			if (!pObjectName)
			{
				//
				continue;
			}

			//
			if (pObjectName->Length != 0)
			{
				//
				for(map<wstring, wstring>::reverse_iterator iter = map_.rbegin(); iter != map_.rend(); iter++)
				{
					//
					if (_wcsicmp(iter->second.c_str(),pObjectName->Buffer) == 0)
					{
						//
						map2_.insert(map<wstring, HANDLE>::value_type(iter->first,hRet));
					}
				}
			}
						
			//
			free(pObjectName);	
		}
	}
	
	//
	CloseHandle(hSystemProcess);
	return true;
}


//初始化函数
bool ntreg_init_func(map<wstring, wstring> &map_,map<wstring, HANDLE> &map2_){
	//
	HMODULE  hModule = GetModuleHandle(L"ntdll.dll");

	//
	if (!hModule)
	{
		return false;
	}

	//
	ZwQuerySystemInformation = (pNtQuerySystemInformation)GetProcAddress(hModule, "ZwQuerySystemInformation");
	ZwQueryInformationFile = (pZwQueryInformationFile)GetProcAddress(hModule, "ZwQueryInformationFile");
	ZwQueryObject = (pZwQueryObject)GetProcAddress(hModule, "ZwQueryObject");
	ZwTerminateThread = (pZwTerminateThread)GetProcAddress(hModule, "ZwTerminateThread");

	//
	if(!ZwQuerySystemInformation || !ZwQueryInformationFile || !ZwQueryObject || !ZwTerminateThread)
	{
		return false;
	}

	//
	int systemPID = ntreg_init_getProcessIDByImageName(L"System");
	//
	if (systemPID == -1)
	{
		return false;
	}

	//产生一个文件句柄
	HANDLE hFile = CreateFile(L"NUL", GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, 0);	
	//
	if(!hFile)
	{ 
		return false; 
	}

	//
	PSYSTEM_HANDLE_INFORMATION pSystem_handle_information = ntreg_init_getSystemHandleInformation();
	//
	if (!pSystem_handle_information)
	{
		return false;
	}

	//
	DWORD dwFileObjectTypeNumber = ntreg_init_getFileObjectTypeNumber(pSystem_handle_information,hFile);

	//
	if (dwFileObjectTypeNumber == -1)
	{
		return false;
	}

	//
	if (!ntreg_init_getHivelist(map_))
	{
		return false;
	}
	
	//
	ntreg_init_getDuplicateHandle(pSystem_handle_information,systemPID,dwFileObjectTypeNumber,map_,map2_);
}


//
bool ntreg_init_mapReadFile(char *filename,struct hive *hdesc)
{
	DWORD dwRead;

	//
	if (!filename) 
	{ 
		return false; 
	}

	//
	hdesc->state = 0;
	hdesc->buffer = NULL;
	hdesc->hFile = NULL;
	hdesc->filename	= str_dup(filename);

	//
	if (*filename == '\\')
	{
		//
		HANDLE hFile = NULL;

		//
		for(map<wstring, HANDLE>::reverse_iterator iter = map2_ntreg.rbegin(); iter != map2_ntreg.rend(); iter++)
		{
			//
			if (_stricmp(common_TCHARToCHAR((LPTSTR)iter->first.c_str()),filename) == 0)
			{
				//
				hFile = iter->second;
				break;
			}
		}

		//
		if (!hFile)
		{
			return false;
		}

		//
		FlushFileBuffers(hFile);

		//
		hdesc->hFile = hFile ;
		hdesc->size = GetFileSize(hFile,NULL);

		//
		HANDLE hMap = CreateFileMapping(hFile,NULL,PAGE_READWRITE,0,0,0);
		//
		if(!hMap) 
		{ 
			return false ; 
		}

		//
		PCHAR pMapView = (PCHAR)MapViewOfFileEx(hMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0, NULL);
		//
		if (!pMapView)
		{ 
			return false ;
		}

		hdesc->buffer = pMapView;
		return true ;
	} 
	else
	{
		//
		HANDLE hFile = CreateFileA(hdesc->filename,GENERIC_READ,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		//
		if (!hFile) 
		{ 
			return FALSE;
		}

		// Read the whole file
		hdesc->size = GetFileSize(hFile,NULL);
		ALLOC(hdesc->buffer,char,1,hdesc->size);
		//
		ReadFile(hFile, (void *)hdesc->buffer, hdesc->size, &dwRead, NULL);
		CloseHandle(hFile);
		return true;
	}
}

//
void ntreg_init_closeHive(struct hive *hdesc)
{
	// FREE(hdesc->filename);
	FlushFileBuffers(hdesc->hFile);

	if (hdesc->hFile) 
	{
		UnmapViewOfFile(hdesc->buffer);
	}
	else 
	{
		FREE(hdesc->buffer);
	}

	FREE(hdesc);
}

//
bool ntreg_init_writeHive(struct hive *hdesc)
{
	DWORD dwBytesWritten;

	//
	HANDLE hFile = CreateFile(L"C:\\tmp2.hiv",GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	//
	if(!hFile)
	{      
		//printf("Can't open dump file");
		return false;
	}

	//
	WriteFile(hFile, hdesc->buffer, hdesc->size,&dwBytesWritten, NULL);
	//
	if(dwBytesWritten != hdesc->size)
	{
		//printf("WriteHive error\n");
		return false;
	}

	//
	CloseHandle(hFile);
	return true;
}

//
struct hive* ntreg_init_openHive (char *filename,int mode)
{
	struct hive *hdesc;
	int vofs;
	unsigned long pofs;
	char *c;
	struct hbin_page *p;
	struct regf_header *hdr;

	int verbose = (mode & HMODE_VERBOSE);
	CREATE(hdesc,struct hive,1);

	//
	if (!ntreg_init_mapReadFile(filename, hdesc))
	{ 
		ntreg_init_closeHive(hdesc);
		return NULL ;
	}

	// Now run through file, tallying all pages
	// NOTE/KLUDGE: Assume first page starts at offset 0x1000
	pofs = 0x1000;
	hdr = (struct regf_header *)hdesc->buffer;
	if (hdr->id != 0x66676572) 
	{
		//printf("openHive(%s): File does not seem to be a registry hive!\n",filename);
		return NULL ;
	}
	//
	for (c = hdr->name; *c && (c < hdr->name + 64); c += 2) 
	{
		putchar(*c);
	}

	hdesc->rootofs = hdr->ofs_rootkey + 0x1000;
	//
	while (pofs < hdesc->size) 
	{
		//
#ifdef LOAD_DEBUG
		if (verbose) 
			hexdump(hdesc->buffer,pofs,pofs+0x20,1);
#endif
		//
		p = (struct hbin_page *)(hdesc->buffer + pofs);
		if (p->id != 0x6E696268) 
		{
			printf("Page at 0x%lx is not 'hbin', assuming file contains garbage at end",pofs);
			break;
		}

		hdesc->pages++;
		//
#ifdef LOAD_DEBUG
		if (verbose) 
			printf("\n###### Page at 0x%0lx has size 0x%0lx, next at 0x%0lx ######\n",pofs,p->len_page,p->ofs_next);
#endif
		//
		if (p->ofs_next == 0) 
		{
			//
#ifdef LOAD_DEBUG
			if (verbose) 
				printf("openhive debug: bailing out.. pagesize zero!\n");
#endif
			//
			return(hdesc);
		}

		//
#if 0
		if (p->len_page != p->ofs_next)
		{
			//
#ifdef LOAD_DEBUG
			if (verbose) 
				printf("openhive debug: len & ofs not same. HASTA!\n");
#endif
			exit(0);
		}

#endif

		vofs = pofs + 0x20; /* Skip page header */
		//
#if 1
		while (vofs-pofs < p->ofs_next)
		{
			vofs += parse_block(hdesc,vofs,verbose);
		}
#endif
		//
		pofs += p->ofs_next;
	}

	//
	return(hdesc);
}


