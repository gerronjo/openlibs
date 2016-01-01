/*
第三方内核代码头文件
*/
#if !defined(SYSEXT_H)
	#define SYSEXT_H

	#include "comm_sys.h"
	
	typedef struct _SERVICE_TABLE_DESCRIPTOR
	{
		PULONG Base;
		PULONG Count;
		ULONG Limit;
		PUCHAR Number;
	}SERVICE_TABLE_DESCRIPTOR,*PSERVICE_TABLE_DESCRIPTOR;
	
	extern void *KeServiceDescriptorTable;
	
	typedef struct _LDR_DATA_TABLE_ENTRY
	{
		LIST_ENTRY InLoadOrderLinks;
		LIST_ENTRY InMemoryOrderLinks;
		LIST_ENTRY InInitializationOrderLinks;
		PVOID DllBase;
		PVOID EntryPoint;
		ULONG SizeOfImage;
		UNICODE_STRING FullDllName;
		UNICODE_STRING BaseDllName;
		ULONG Flags;
		USHORT LoadCount;
		USHORT TlsIndex;
		union 
		{
			LIST_ENTRY HashLinks;
			struct 
			{
				PVOID SectionPointer;
				ULONG CheckSum;
			}s0;
		}u0;
		union 
		{
			struct 
			{
				ULONG TimeDateStamp;
			}s1;
			struct 
			{
				PVOID LoadedImports;
			}s2;
		}u1;
	}LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;
	
	void sysext_init(void);
	size_t HyGetSSDTCore(PVOID buf,size_t size);
	size_t HyGetSSDTSdwCore(PVOID SSDTSdwAddr,PVOID buf,size_t size);
	bool HyGetSSDTBasesCore(PVOID buf,size_t size);
	size_t HyGetIDTCore(PVOID buf,size_t size);
	size_t HyGetGDTCore(PVOID buf,size_t size);
	bool HyEnumSysModCore(PVOID SysModListAddr);
	bool HyGetSysValCore(PVOID addr,PVOID buf,size_t size);
#endif
