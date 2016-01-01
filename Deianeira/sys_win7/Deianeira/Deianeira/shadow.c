#include "shadow.h"


//获得win32k基址
ULONG GetWin32kBase()
{
	ULONG i;
    ULONG ulAddress;
  
	ULONG ulSize; 
	NTSTATUS status; 
	PSYSTEM_MODULE_LIST pSystem_module_list; 

	//第一次运行时查询返回字节数 
	ZwQuerySystemInformation(SystemModuleInformation,NULL,0, &ulSize); 

	//通过返回的字节数来分配内存 
	pSystem_module_list = (PSYSTEM_MODULE_LIST)ExAllocatePool(PagedPool,ulSize); 
	//如果分配内存失败
	if(!pSystem_module_list) 
	{ 
		//输出调试信息
		DbgPrint("shadow.c GetWin32kBase() pSystem_module_list 分配内存失败！");
		return 0; 
	} 

	//再次获得 
	status = ZwQuerySystemInformation(SystemModuleInformation,pSystem_module_list,ulSize,&ulSize); 
	//获取失败 
	if(!NT_SUCCESS(status)) 
	{ 
		//输出调试信息
		DbgPrint("shadow.c GetWin32kBase() ZwQuerySystemInformation 失败！");
		//释放申请的内存
		ExFreePool(pSystem_module_list); 
		return 0; 
	} 

	//遍历链表
	for (i=0; i < pSystem_module_list->ulCount; i++)
	{
		//匹配字符串
		if(strcmp(pSystem_module_list->smi[i].ImageName,"\\SystemRoot\\System32\\win32k.sys") == 0)
		{
			//输出调试信息
			DbgPrint("shadow.c GetWin32kBase() pSystem_module_list->smi[i].ImageName:%s",pSystem_module_list->smi[i].ImageName);
			//基址
			ulAddress = (ULONG)pSystem_module_list->smi[i].Base;
			break;
		}

	}

	//输出调试信息
	DbgPrint("shadow.c GetWin32kBase() ulAddress:%08X",ulAddress);

	//释放内存
	ExFreePool(pSystem_module_list);
	//返回地址
	return ulAddress;
}

//根据服务号获得原始函数地址
ULONG GetOldShadowSSDTAddress(ULONG ulIndex)
{
	NTSTATUS status;
    HANDLE hFile;

	ULONG ulWin32kBase;
	ULONG ulShadowBase;
	//相对偏移量
	ULONG ulShadowRVA;

	UNICODE_STRING ustrFileName;
	OBJECT_ATTRIBUTES  objectAttributes;
	IO_STATUS_BLOCK io_status = {0};

	//读取的位置
	ULONG ulLocation;
	LARGE_INTEGER offset;

	//返回的函数地址
	ULONG ulFuncAddress;

	//获得Win32k基址
	ulWin32kBase = GetWin32kBase();

	//输出调试信息
	DbgPrint("shadow.c GetOldSSDTAddress() ulWin32kBase:%08X",ulWin32kBase);

	//获得内存中的Shadow SSDT基址
	ulShadowBase = KeServiceDescriptorTableShadow[1].ServiceTableBase;

	//输出调试信息
	DbgPrint("shadow.c GetOldSSDTAddress() ulShadowBase:%08X",ulShadowBase);

	//得到Shadow相对偏移量
	ulShadowRVA = ulShadowBase - ulWin32kBase;

	//输出调试信息
	DbgPrint("shadow.c GetOldSSDTAddress() ulShadowRVA:%08X",ulShadowRVA);

	//读取的位置
	ulLocation = ulShadowRVA + ulIndex * 4;
	offset.QuadPart = ulLocation;

	//利用ZwReadFile读取文件
	//初始化OBJECT_ATTRIBUTES结构
	RtlInitUnicodeString(&ustrFileName, L"\\SystemRoot\\system32\\win32k.sys");

	//初始化结构体
	InitializeObjectAttributes( 
		&objectAttributes,
		&ustrFileName,
		OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
		NULL,
		NULL);

	//打开文件
	status = ZwCreateFile(
		&hFile,
		FILE_EXECUTE | SYNCHRONIZE,
		&objectAttributes,
		&io_status,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ,
		FILE_OPEN,
		FILE_NON_DIRECTORY_FILE |
		FILE_RANDOM_ACCESS |
		FILE_SYNCHRONOUS_IO_NONALERT,
		NULL,
		0);

	if(!NT_SUCCESS(status))
	{
		//输出调试信息
		DbgPrint("shadow.c GetOldSSDTAddress() ZwCreateFile error!");
		ZwClose(hFile);
		return NULL;
	}

	//读文件
	status = ZwReadFile(
		hFile,
		NULL,
		NULL,
		NULL,
		NULL,
		&ulFuncAddress,
		sizeof(ULONG),
		&offset,
		NULL);

	if(!NT_SUCCESS(status))
	{
		//输出调试信息
		DbgPrint("shadow.c GetOldSSDTAddress() ZwReadFile error!");
		ZwClose(hFile);
		return NULL;
	}

	//关闭
	ZwClose(hFile);

	//输出调试信息
	DbgPrint("shadow.c GetOldSSDTAddress() ulFuncAddress:%d",ulFuncAddress);

	return ulFuncAddress;
}