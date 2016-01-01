//sysmodule.h
#ifndef _SYSMODULE_
#define _SYSMODULE_

#include "common.h"

typedef struct _SystemModuleInfo
{   
	//base name
	WCHAR BaseName[256];
	//full name
	WCHAR FullName[256];
	//基址
	ULONG ulBaseAddress;
	//映像大小
	ULONG ulSize;
	//单向列表指针
	_SystemModuleInfo *next;
}SYSTEM_MODULE_INFO, *PSYSTEM_MODULE_INFO; 

//获得系统模块列表 
PSYSTEM_MODULE_LIST GetSystemModuleList();

#endif //_SYSMODULE_