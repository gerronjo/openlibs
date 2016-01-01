#include "sysmodule.h"

//获得系统模块列表 
PSYSTEM_MODULE_LIST GetSystemModuleList() 
{ 
	//函数指针定义 
	PNtQuerySystemInformation NtQuerySystemInformation; 
	HMODULE hModule;  
	ULONG ulSize; 
	NTSTATUS status; 
	PSYSTEM_MODULE_LIST pSystemModuleInfo; 

	//获得模块句柄 
	if(!(hModule=LoadLibrary(L"ntdll.dll"))) 
	{ 
		return NULL; 
	} 

	//获得函数地址 
	NtQuerySystemInformation=(PNtQuerySystemInformation)GetProcAddress(hModule, "NtQuerySystemInformation"); 
	//函数地址获取失败 
	if(!NtQuerySystemInformation) 
	{ 
		FreeLibrary(hModule); 
		return NULL; 
	} 

	//第一次运行时查询返回字节数 
	NtQuerySystemInformation(SystemModuleInformation,NULL,0, &ulSize); 
	//通过返回的字节数来分配内存 
	pSystemModuleInfo = (PSYSTEM_MODULE_LIST)malloc(ulSize); 
	//如果分配内存失败
	if(!pSystemModuleInfo) 
	{ 
		FreeLibrary(hModule); 
		return NULL; 
	} 

	//再次获得 
	status = NtQuerySystemInformation(SystemModuleInformation,pSystemModuleInfo,ulSize,&ulSize); 
	//获取失败 
	if(!NT_SUCCESS(status)) 
	{ 
		//释放申请的内存
		free(pSystemModuleInfo); 
		FreeLibrary(hModule); 
		return NULL; 
	} 

	//释放内存 
	FreeLibrary(hModule); 
	//如果成功
	return pSystemModuleInfo; 
} 