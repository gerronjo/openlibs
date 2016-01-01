//**************************************************************************
//*
//*          A盾电脑防护 website:http://www.3600safe.com/
//*        
//*文件说明：
//*         枚举系统驱动模块
//**************************************************************************
#include "SysModule.h"

///////////////////////////////////////////////////////////////
__declspec(naked) NTSTATUS ExAllocatePoolWithTagHookZone(,...)
{
	_asm
	{
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		jmp [ExAllocatePoolWithTagRet];
	}
}
//////////////////////////////////////////////////
__declspec (naked) NewExAllocatePoolWithTag()
{
	_asm
	{
		pushad
		pushfd
		mov eax,dword ptr [esp+24h]
		push eax
		call StealthCodeScanInExAllocatePoolWithTag
		popfd
		popad
		jmp [ExAllocatePoolWithTagHookZone]
	}
}
/*
BOOL ScanHookCodePage(ULONG ulPage)
{
	BOOL bRetOK = FALSE;
	int i = 0;

	for (i=0;i<SysModuleInfo->ulCount;i++)
	{
		if (ulPage > SysModuleInfo->SysInfo[i].ulSysBase && ulPage < SysModuleInfo->SysInfo[i].ulSysBase + SysModuleInfo->SysInfo[i].SizeOfImage)
		{
			bRetOK = TRUE; //不是隐藏代码~
		}
	}
	return bRetOK;
}
*/
BOOL CheckCode(ULONG ulPage,PDRIVER_OBJECT DriverObject,PSYSINFO SysModuleInfo)
{
	PLDR_DATA_TABLE_ENTRY LdrDataTable,HideLdrDataTable;
	BOOL bRetOK = FALSE;
	int i = 0;

	__try
	{
		LdrDataTable=(PLDR_DATA_TABLE_ENTRY)DriverObject->DriverSection;
		do 
		{

			//	KdPrint(("%wZ\n",&LdrDataTable->BaseDllName));
			if (LdrDataTable->BaseDllName.Length>0&&LdrDataTable->BaseDllName.Buffer!=NULL)
			{
				if(RMmIsAddressValid(&LdrDataTable->BaseDllName.Buffer[LdrDataTable->BaseDllName.Length/2-1]))
				{
					/*SysModuleInfo->SysInfo[i].IntHideType = 0;  //正常驱动
					SysModuleInfo->SysInfo[i].LoadCount = LdrDataTable->TlsIndex;
					SysModuleInfo->SysInfo[i].ulSysBase = LdrDataTable->DllBase;
					SysModuleInfo->SysInfo[i].SizeOfImage = LdrDataTable->SizeOfImage;
					SafeCopyMemory(LdrDataTable->FullDllName.Buffer,SysModuleInfo->SysInfo[i].lpwzFullSysName,LdrDataTable->FullDllName.Length);
					SafeCopyMemory(LdrDataTable->BaseDllName.Buffer,SysModuleInfo->SysInfo[i].lpwzBaseSysName,LdrDataTable->BaseDllName.Length);
					i++;
					SysModuleInfo->ulCount = i;
					*/
					if (ulPage > (ULONG)LdrDataTable->DllBase && ulPage < (ULONG)LdrDataTable->DllBase + (ULONG)LdrDataTable->SizeOfImage)
					{
						bRetOK = TRUE; //不是隐藏代码~
						__leave;
					}
				}
			}
			LdrDataTable=(PLDR_DATA_TABLE_ENTRY)LdrDataTable->InLoadOrderLinks.Flink;

		} while ((PLDR_DATA_TABLE_ENTRY)DriverObject->DriverSection!=LdrDataTable&&LdrDataTable!=NULL);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		if (DebugOn)
			DbgPrint("PsLoadedModuleList Error \r\n");
	}
	return bRetOK;
}
/////////////////////////////////////////////////////////////////////
//ExAllocatePoolWithTag过滤函数
/////////////////////////////////////////////////////////////////////
VOID __stdcall StealthCodeScanInExAllocatePoolWithTag(ULONG ulPage)
{
	ULONG ulHookModuleBase;
	ULONG ulHookModuleSize;
	char lpszHookModuleImage[256];

	//如果地址无效，则返回
/*	RMmIsAddressValid = ReLoadNtosCALL(L"MmIsAddressValid",SystemKernelModuleBase,ImageModuleBase);
	if (!RMmIsAddressValid)
	{
		return;
	}
*/
	if (!RMmIsAddressValid((PVOID)ulPage) ||
		SysModuleInfo == NULL)
	{
		return;
	}
	if (!CheckCode(ulPage,PDriverObject,SysModuleInfo))
	{
		if (DebugOn)
			KdPrint(("发现隐藏驱动！%08x\r\n",ulPage));
	}
}
BOOL QueryDriverFromPsLoadedModuleList(PDRIVER_OBJECT DriverObject,PSYSINFO SysModuleInfo)
{
	PLDR_DATA_TABLE_ENTRY LdrDataTable,HideLdrDataTable;
	BOOL bRetOK = FALSE;
	int i = 0;

	__try
	{
		LdrDataTable=(PLDR_DATA_TABLE_ENTRY)DriverObject->DriverSection;
		do 
		{

			//	KdPrint(("%wZ\n",&LdrDataTable->BaseDllName));
			if (LdrDataTable->BaseDllName.Length>0&&LdrDataTable->BaseDllName.Buffer!=NULL)
			{
				if(MmIsAddressValid(&LdrDataTable->BaseDllName.Buffer[LdrDataTable->BaseDllName.Length/2-1]))
				{
					SysModuleInfo->SysInfo[i].IntHideType = 0;  //正常驱动
					SysModuleInfo->SysInfo[i].LoadCount = LdrDataTable->LoadCount;
					SysModuleInfo->SysInfo[i].ulSysBase = LdrDataTable->DllBase;
					SysModuleInfo->SysInfo[i].SizeOfImage = LdrDataTable->SizeOfImage;
					SafeCopyMemory(LdrDataTable->FullDllName.Buffer,SysModuleInfo->SysInfo[i].lpwzFullSysName,LdrDataTable->FullDllName.Length);
					SafeCopyMemory(LdrDataTable->BaseDllName.Buffer,SysModuleInfo->SysInfo[i].lpwzBaseSysName,LdrDataTable->BaseDllName.Length);
					i++;
					SysModuleInfo->ulCount = i;
				}
			}
			LdrDataTable=(PLDR_DATA_TABLE_ENTRY)LdrDataTable->InLoadOrderLinks.Flink;

		} while ((PLDR_DATA_TABLE_ENTRY)DriverObject->DriverSection!=LdrDataTable&&LdrDataTable!=NULL);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		if (DebugOn)
			DbgPrint("PsLoadedModuleList Error \r\n");
	}
	return bRetOK;
}
VOID EnumKernelModule(PDRIVER_OBJECT DriverObject,PSYSINFO SysModuleInfo)
{
	ULONG ulExAllocatePoolWithTag;

	QueryDriverFromPsLoadedModuleList(DriverObject,SysModuleInfo);

	//注释，作为保留以后更新把
/*
	ulExAllocatePoolWithTag = GetSystemRoutineAddress(1,L"ExAllocatePoolWithTag");
	if (ulExAllocatePoolWithTag)
	{
		//开始hook~
		if (HookFunctionByHeaderAddress(NewExAllocatePoolWithTag,ulExAllocatePoolWithTag,ExAllocatePoolWithTagHookZone,&ExAllocatePoolWithTagPatchCodeLength,&ExAllocatePoolWithTagRet))
		{
			DbgPrint("hook success\r\n");
		}
	}
*/
	return STATUS_SUCCESS;
}