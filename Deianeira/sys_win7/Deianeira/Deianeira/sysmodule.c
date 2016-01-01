#include "sysmodule.h"


PVOID IopOpenRegistryKey;
PVOID IopLoadUnloadDriver;

//枚举驱动模块
NTSTATUS sysmodule_enumDrivers(PDRIVER_OBJECT pDriverObj)
{
	NTSTATUS status;
	PLIST_ENTRY pListHead = NULL;
	PLDR_DATA_TABLE_ENTRY pLdr = NULL;
	PSYSTEM_MODULE_INFO pSystemModuleInfo_tmp = NULL;

	__try
	{
		//pSystemModuleInfoHead记录头部
		pSystemModuleInfoHead = pSystemModuleInfo = (PSYSTEM_MODULE_INFO)ExAllocatePool(NonPagedPool,sizeof(SYSTEM_MODULE_INFO));

		//
		pSystemModuleInfo->next = NULL;


		//作为链表头部
		pListHead = ((PLIST_ENTRY)pDriverObj->DriverSection)->Flink;

		//循环遍历链表
		do 
		{
			pLdr = CONTAINING_RECORD(pListHead,
				LDR_DATA_TABLE_ENTRY,
				InLoadOrderLinks);

			if (pLdr->EntryPoint && pLdr->FullDllName.Buffer) 
			{
				pSystemModuleInfo_tmp = (PSYSTEM_MODULE_INFO)ExAllocatePool(NonPagedPool,sizeof(SYSTEM_MODULE_INFO));

				RtlZeroMemory(pSystemModuleInfo_tmp,sizeof(SYSTEM_MODULE_INFO));

				wcscpy(pSystemModuleInfo_tmp->BaseName,pLdr->BaseDllName.Buffer);
                wcscpy(pSystemModuleInfo_tmp->FullName,pLdr->FullDllName.Buffer);

				//基址
				pSystemModuleInfo_tmp->ulBaseAddress = pLdr->DllBase;
				//大小
				pSystemModuleInfo_tmp->ulSize = pLdr->SizeOfImage;
				//
				pSystemModuleInfo_tmp->next = NULL;


				pSystemModuleInfo->next = pSystemModuleInfo_tmp;

				pSystemModuleInfo = pSystemModuleInfo_tmp;
			}

			//指针下移
			pListHead = pListHead->Flink;
		} while (pListHead != ((PLIST_ENTRY)pDriverObj->DriverSection)->Flink);

		//重新把指针移到头部
		pSystemModuleInfo = pSystemModuleInfoHead->next;

		//返回成功
		return STATUS_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("sysmodule_enumDrivers EXCEPTION_EXECUTE_HANDLER error!");

		//返回失败
		return STATUS_UNSUCCESSFUL;
	}
}

//释放内存
VOID sysmodule_freeDriverModuleInfo()
{
	PSYSTEM_MODULE_INFO p = NULL;

	__try
	{
		while(pSystemModuleInfo != NULL)
		{
			p = pSystemModuleInfo->next;
			ExFreePool(pSystemModuleInfo);
			pSystemModuleInfo   = p;
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("sysmodule_freeDriverModuleInfo EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//获得大小
ULONG sysmodule_getDriverModuleCount()
{
	__try
	{
		PSYSTEM_MODULE_INFO p=NULL;
		ULONG ulCount=0;

		//获得大小
		for (p=pSystemModuleInfo;p;p=p->next)
		{
			ulCount++;
		}

		DbgPrint("sysmodule_getDriverModuleCount ulCount:%d",ulCount);
		return ulCount;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return 0;
	}	
}

//发送数据到ring3
NTSTATUS sysmodule_getDriverModuleInfo(PSYSTEM_MODULE_INFO pBuffer)
{
	PSYSTEM_MODULE_INFO p=NULL;
    ULONG ulIndex = 0;

	__try
	{
		//遍历
		for (p=pSystemModuleInfo;p;p=p->next)
		{
			RtlCopyMemory(pBuffer + ulIndex,p,sizeof(SYSTEM_MODULE_INFO));
			ulIndex++;
		}

		//释放内存
		sysmodule_freeDriverModuleInfo();

		return STATUS_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("sysmodule_getDriverModuleInfo EXCEPTION_EXECUTE_HANDLER error!");
		return STATUS_UNSUCCESSFUL;
	}
}

//得到IopLoadUnloadDriver的地址
ULONG sysmodule_getIopLoadUnloadDriverAddress()
{
	UCHAR *cPtr, *pOpcode;
	ULONG ulLength;
	ULONG ulCallCount=1;

	ULONG ulNtLoadDriver,ulIopLoadUnloadDriver;

	__try
	{
		//获得NtLoadDriver地址
		ulNtLoadDriver = GetSSDTFuncAddress(ZwLoadDriver);

		//获得IopLoadUnloadDriver地址
		for (cPtr = (PUCHAR)ulNtLoadDriver; cPtr < (PUCHAR)ulNtLoadDriver + PAGE_SIZE;) 
		{
			//获得被反汇编的字节数
			ulLength = SizeOfCode(cPtr, &pOpcode);

			//如果反汇编失败的话
			if (ulLength==0) 
			{
				return NULL;
			}

			//E8表示call
			if (*pOpcode == 0xE8)
			{
				if (ulCallCount==7)
				{
					//而E8 后面紧跟的就是相对偏移地址
					//(*(PULONG)(pOpcode + 1)得到相对偏移地址 + 当前地址(ULONG)cPtr 在加上函数返回地址圧栈所用的5个字节 = call 后面的函数地址
					ulIopLoadUnloadDriver = (*(PULONG)(pOpcode + 1) + (ULONG)cPtr + 5);

					DbgPrint("sysmodule_getIopLoadUnloadDriverAddress address :%X, ulIopLoadUnloadDriver :%X",cPtr,ulIopLoadUnloadDriver);
					break;
				}

				//
				ulCallCount++;
			}

			//迭代
			cPtr = cPtr + ulLength;
		}

		return ulIopLoadUnloadDriver;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("sysmodule_getIopLoadUnloadDriverAddress EXCEPTION_EXECUTE_HANDLER error!");
		return NULL;
	}
}

//得到IopOpenRegistryKey的地址
ULONG sysmodule_getIopOpenRegistryKeyAddress()
{
	UCHAR *cPtr, *pOpcode;
	ULONG ulLength;
	ULONG ulCallCount=1;

	ULONG ulIopLoadUnloadDriver,ulIopOpenRegistryKey;

	__try
	{
		ulIopLoadUnloadDriver = sysmodule_getIopLoadUnloadDriverAddress();

		if (ulIopLoadUnloadDriver ==0 || ulIopLoadUnloadDriver<=0x80000000)
		{
			DbgPrint("sysmodule_getIopOpenRegistryKeyAddress ulIopLoadUnloadDriver error!");
			return NULL;
		}

		//获得IopLoadDriver地址
		for (cPtr = (PUCHAR)ulIopLoadUnloadDriver; cPtr < (PUCHAR)ulIopLoadUnloadDriver + PAGE_SIZE;) 
		{
			//获得被反汇编的字节数
			ulLength = SizeOfCode(cPtr, &pOpcode);

			//如果反汇编失败的话
			if (ulLength==0) 
			{
				return NULL;
			}

			//E8表示call
			if (*pOpcode == 0xE8)
			{
				if (ulCallCount==1)
				{
					//而E8 后面紧跟的就是相对偏移地址
					//(*(PULONG)(pOpcode + 1)得到相对偏移地址 + 当前地址(ULONG)cPtr 在加上函数返回地址圧栈所用的5个字节 = call 后面的函数地址
					ulIopOpenRegistryKey = (*(PULONG)(pOpcode + 1) + (ULONG)cPtr + 5);

					DbgPrint("sysmodule_getIopOpenRegistryKeyAddress ulIopOpenRegistryKey :%X",ulIopOpenRegistryKey);
					break;		
				}
				
				ulCallCount++;
			}

			//迭代
			cPtr = cPtr + ulLength;
		}

		//返回地址
		return ulIopOpenRegistryKey;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("sysmodule_getIopOpenRegistryKeyAddress EXCEPTION_EXECUTE_HANDLER error!");
		return NULL;
	}
}

//
NTSTATUS NewIopOpenRegistryKey(
							   OUT PHANDLE Handle,
							   IN HANDLE BaseHandle OPTIONAL,
							   IN PUNICODE_STRING KeyName,
							   IN ACCESS_MASK DesiredAccess,
							   IN BOOLEAN Create
							   )
{
	return STATUS_UNSUCCESSFUL;
}

//在这里InlineHook,禁止加载驱动
BOOLEAN sysmodule_inlineHook_disableLoadDriver()
{
	__try
	{
		//获得原有地址
		ULONG ulIopOpenRegistryKey,ulIopLoadUnloadDriver;

		ulIopLoadUnloadDriver = sysmodule_getIopLoadUnloadDriverAddress();

		if (ulIopLoadUnloadDriver==0 && ulIopLoadUnloadDriver<=0x80000000)
		{
			DbgPrint("sysmodule_inlineHook_disableLoadDriver ulIopLoadUnloadDriver error!");
			return FALSE;
		}

		IopLoadUnloadDriver = (PVOID)ulIopLoadUnloadDriver;

		ulIopOpenRegistryKey = sysmodule_getIopOpenRegistryKeyAddress();

		if (ulIopOpenRegistryKey==0 && ulIopOpenRegistryKey<=0x80000000)
		{
			DbgPrint("sysmodule_inlineHook_disableLoadDriver ulIopOpenRegistryKey error!");
			return FALSE;
		}

		IopOpenRegistryKey = (PVOID)ulIopOpenRegistryKey;

		common_callAddrHook(IopLoadUnloadDriver,IopOpenRegistryKey,NewIopOpenRegistryKey,PAGE_SIZE);

		return TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("sysmodule_inlineHook_disableLoadDriver EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//停止禁止加载
VOID sysmodule_inlineHook_disableLoadDriver_stop()
{
	__try
	{
		common_callAddrHook(IopLoadUnloadDriver,NewIopOpenRegistryKey,IopOpenRegistryKey,PAGE_SIZE);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("sysmodule_inlineHook_disableLoadDriver_stop EXCEPTION_EXECUTE_HANDLER error!");
	}
}