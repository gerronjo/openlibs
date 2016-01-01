/*
第三方内核代码
*/
#include "sysext.h"

static PVOID g_pSSDTSdw = NULL;

void sysext_init(void)
{
	PRINT("[%s]msg : sysext_init ok!\n",__func__);
	
	DWORD dwEAX = 0,dwEDX = 0;
	__asm__ __volatile__("movl $0x176,%%ecx;rdmsr;" \
						"movl %%eax,%0;movl %%edx,%1"::"m"(dwEAX),"m"(dwEDX));
	PRINT("[%s]msg : SYSENTER_EIP_MSR %08X:%08X\n",__func__,dwEDX,dwEAX);
}

//取得两个表(SSDT,SSDTSDW)的Base元素的值。
//该函数要求HyGetSSDTSdwCore先调用一次以初始化g_pSSDTSdw的值。
bool HyGetSSDTBasesCore(PVOID buf,size_t size)
{
	bool bRet = false;
	PSERVICE_TABLE_DESCRIPTOR pSDT;
	if(!buf || size < sizeof(PVOID)*2) goto QUIT;
	
	pSDT = (PSERVICE_TABLE_DESCRIPTOR)KeServiceDescriptorTable;
	*(PVOID*)buf = pSDT->Base;
	if(!g_pSSDTSdw)
	{
		PRINT("[%s]msg : g_pSSDTSdw == NULL!\n",__func__);goto QUIT;
	}
	pSDT = (PSERVICE_TABLE_DESCRIPTOR)(1 + (PSERVICE_TABLE_DESCRIPTOR)g_pSSDTSdw);
	*((PVOID*)buf+1) = pSDT->Base;
	bRet = true;
QUIT:
	return bRet;
}

//取得SSDTSdw表的所有服务地址，该函数接受RING3传过来的SSDTSdwAddr地址。
//如果参数size == 1331则表示仅仅初始化g_pSSDTSdw但不实际取得服务地址。
size_t HyGetSSDTSdwCore(PVOID SSDTSdwAddr,PVOID buf,size_t size)
{
	size_t Ret = 0;
	PSERVICE_TABLE_DESCRIPTOR pSSDTSdw;
	if(!buf || !size) goto QUIT;
	
	if(!MmIsAddressValid(SSDTSdwAddr))
	{
		PRINT("[%s]err : MmIsAddrValid Failed!\n",__func__);
		g_pSSDTSdw = NULL;goto QUIT;
	}
	g_pSSDTSdw = SSDTSdwAddr;
	
	if(size == 1331)
	{
		PRINT("[%s]msg : Only Set SSDTSdwAddr!!!\n",__func__);
		goto QUIT;
	}
	
	pSSDTSdw = (PSERVICE_TABLE_DESCRIPTOR)(1 + (PSERVICE_TABLE_DESCRIPTOR)SSDTSdwAddr);
	
	PRINT("[%s]Addr of SSDTSdw : %p , Base : %p , Limit : %d , " \
		"Number : %p\n",__func__,pSSDTSdw,pSSDTSdw->Base,pSSDTSdw->Limit,\
		pSSDTSdw->Number);
		
	if(size < pSSDTSdw->Limit * sizeof(PVOID))
	{
		PRINT("[%s]err : size < SSDTSdw Size!\n",__func__);
		goto QUIT;
	}
		
	RtlCopyMemory(buf,pSSDTSdw->Base,pSSDTSdw->Limit * sizeof(PVOID));
	Ret = pSSDTSdw->Limit * sizeof(PVOID);
QUIT:
	return Ret;
}

//取得SSDT表的所有服务。
size_t HyGetSSDTCore(PVOID buf,size_t size)
{
	size_t Ret = 0;
	PSERVICE_TABLE_DESCRIPTOR pSSDT;
	if(!buf || !size) goto QUIT;
	
	PRINT("[%s]Addr of KeServiceDescriptorTable : %p\n",\
		__func__,KeServiceDescriptorTable);
	
	pSSDT = (PSERVICE_TABLE_DESCRIPTOR)KeServiceDescriptorTable;
	PRINT("[%s]Addr of SSDT : %p , Base : %p , Limit : %d , " \
		"Number : %p\n",__func__,pSSDT,pSSDT->Base,pSSDT->Limit,\
		pSSDT->Number);
		
	if(size < pSSDT->Limit * sizeof(PVOID))
	{
		PRINT("[%s]err : size < SSDT Size!\n",__func__);
		goto QUIT;
	}
		
	RtlCopyMemory(buf,pSSDT->Base,pSSDT->Limit * sizeof(PVOID));
	Ret = pSSDT->Limit * sizeof(PVOID);
QUIT:
	return Ret;
}

//取得IDT表中的所有门元素。
//中断描述符表IDT所含的描述符只能是中断门、陷阱门和任务门.
size_t HyGetIDTCore(PVOID buf,size_t size)
{
	XDT IDT = {0};
	__asm__ __volatile__("sidt %0"::"m"(IDT));
	PRINT("[%s]msg : IDT at %p , Limit is %04x\n",__func__,IDT.Base,\
		IDT.Limit);
	if(size < IDT.Limit+1)
	{
		PRINT("[%s]err : IDT's size too small!\n",__func__);
		return 0;
	}
	RtlCopyMemory(buf,IDT.Base,IDT.Limit+1);
	return IDT.Limit+1;
}

//取得GDT表的所有段结构。
size_t HyGetGDTCore(PVOID buf,size_t size)
{
	XDT GDT = {0};
	__asm__ __volatile__("sgdt %0"::"m"(GDT));
	PRINT("[%s]msg : GDT at %p , Limit is %04x\n",__func__,GDT.Base,\
		GDT.Limit);
	if(size < GDT.Limit+1)
	{
		PRINT("[%s]err : GDT's size too small!\n",__func__);
		return 0;
	}
	RtlCopyMemory(buf,GDT.Base,GDT.Limit+1);
	return GDT.Limit+1;
}

//遍历PsLoadedModuleList
bool HyEnumSysModCore(PVOID SysModListAddr)
{
	bool bRet = false;
	if(!SysModListAddr) goto QUIT;
	PLIST_ENTRY pSysModList = SysModListAddr;
	PLDR_DATA_TABLE_ENTRY pSysMod = (PLDR_DATA_TABLE_ENTRY)pSysModList->Flink;
	size_t i = 0;
	DbgPrint("[%s]BEGIN ENUM SYSMODS ------------------>\n",__func__);
	while(pSysModList->Flink != pSysMod->InLoadOrderLinks.Flink)
	{
		DbgPrint("[%03d] %wZ @ 0X%08X %08x\n",i++,&pSysMod->BaseDllName,\
			pSysMod->DllBase,pSysMod->SizeOfImage);
		pSysMod = (PLDR_DATA_TABLE_ENTRY)pSysMod->InLoadOrderLinks.Flink;
	}
	DbgPrint("[%s]END ENUM SYSMODS ------------------>\n",__func__);
	bRet = true;
QUIT:
	return bRet;
}

//取得指定虚拟地址的内容
bool HyGetSysValCore(PVOID addr,PVOID buf,size_t size)
{
	bool bRet = false;
	if(!MmIsAddressValid(addr))
	{
		PRINT("[%s]err : MmIsAddrValid Failed!\n",__func__);
		goto QUIT;
	}
	
	RtlCopyMemory(buf,addr,size);
	bRet = true;
QUIT:
	return bRet;
}
