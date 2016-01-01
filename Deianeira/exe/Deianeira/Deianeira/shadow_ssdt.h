//防止重定义
#ifndef _SHADOW_SSDT_
#define _SHADOW_SSDT_

#include "..\common\common.h"
#include "..\common\sysmodule.h"


#define RVATOVA(base,offset) ((PVOID)((DWORD)(base)+(DWORD)(offset)))

typedef struct { 
	WORD    offset:12;
	WORD    type:4;
}IMAGE_FIXUP_ENTRY, *PIMAGE_FIXUP_ENTRY;


//SDT结构体
typedef struct ServiceDescriptorTable {
	unsigned int *ServiceTableBase;
	unsigned int *ServiceCounterTable;
	unsigned int NumberOfServices;
	unsigned int *ParamTableBase;
}ServiceDescriptorTable,*PServiceDescriptorTable;


//保存SSDT信息
typedef struct SSDTSaveTable		
{
	ULONG ulServiceNumber;
	ULONG ulCurrentFuncAddress;	
	ULONG ulOriginalFuncAddress;
	char ServiceFunctionName[MAX_PATH];
	char ModuleName[MAX_PATH];
} SSDTSaveTable, *PSSDTSaveTable;


//定义为static只能被一个cpp访问
//SSDT用于保存相关信息
extern PSSDTSaveTable pSSDTSaveTable;
//SSDT函数数量
extern ULONG ulSSDTTotal;
//
extern PSYSTEM_MODULE_LIST pSystem_Module_List;

//获得SSDT函数索引
ULONG GetSSDTFuncIndex(LPSTR lpName);

//通过地址得到模块名
BOOL GetModuleNameByAddr(ULONG ulAddr,PSYSTEM_MODULE_LIST pSystem_module_list,LPSTR lpBuffer);

//根据服务号将SSDT缓冲表排序
VOID SSDTSTOrderByServiceNum();

//枚举SSDT
BOOL GetCurrentSSDTFunc(HANDLE hDriver);

VOID GetOldSSDTAddress();

#endif //_SHADOW_SSDT_