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
	struct SYSTEM_MODULE_INFO* next;
}SYSTEM_MODULE_INFO, *PSYSTEM_MODULE_INFO; 


//全局
PSYSTEM_MODULE_INFO pSystemModuleInfo,pSystemModuleInfoHead;


//枚举驱动模块
NTSTATUS sysmodule_enumDrivers(PDRIVER_OBJECT pDriverObj);

//释放内存
VOID sysmodule_freeDriverModuleInfo();

//获得大小
ULONG sysmodule_getDriverModuleCount();

//发送数据到ring3
NTSTATUS sysmodule_getDriverModuleInfo(PSYSTEM_MODULE_INFO pBuffer);

//在这里InlineHook,禁止加载驱动
BOOLEAN sysmodule_inlineHook_disableLoadDriver();

//在这里InlineHook,禁止加载驱动
VOID sysmodule_inlineHook_disableLoadDriver_stop();

#endif //_SYSMODULE_