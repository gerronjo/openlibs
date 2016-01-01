//**************************************************************************
//*
//*          A盾电脑防护 website:http://www.3600safe.com/
//*        
//*文件说明：
//*         枚举NTFS各个例程
//**************************************************************************
#include "Ntfs.h"

//填充各个结构发送至ring3
VOID FixNtfs(PNTFSDISPATCHBAKUP NtfsDispatchBakUp,PDRIVER_OBJECT PNtfsDriverObject,int i,ULONG ulReal_Dispatch,WCHAR *lpwzDispatchName,ULONG Dispatch)
{
	ULONG ulCurrentNtfsDispatch;
	ULONG ulHookModuleBase;
	ULONG ulHookModuleSize;
	BOOL bIsHooked = FALSE;
	ULONG ulReloadNtfsDispatch;

	NtfsDispatchBakUp->NtfsDispatch[i].ulNtfsDispatch = ulReal_Dispatch;
	NtfsDispatchBakUp->NtfsDispatch[i].ulNumber = Dispatch;

	memset(NtfsDispatchBakUp->NtfsDispatch[i].lpwzNtfsDispatchName,0,sizeof(NtfsDispatchBakUp->NtfsDispatch[0].lpwzNtfsDispatchName));
	wcsncpy(NtfsDispatchBakUp->NtfsDispatch[i].lpwzNtfsDispatchName,lpwzDispatchName,wcslen(lpwzDispatchName));
	
	ulCurrentNtfsDispatch = PNtfsDriverObject->MajorFunction[Dispatch];

	if (DebugOn)
		KdPrint(("ulCurrentNtfsDispatch:%08x-%08x",ulCurrentNtfsDispatch,PNtfsDriverObject));

	if (ulCurrentNtfsDispatch == ulReal_Dispatch)
	{
		bIsHooked = TRUE;

		WinVer = GetWindowsVersion();
		if (WinVer == WINDOWS_VERSION_2K3_SP1_SP2)
		{
			goto _FunRet;
		}
		ulReloadNtfsDispatch = ulReal_Dispatch - ulNtfsModuleBase + ulReLoadNtfsModuleBase;

		//检查是否inline hook
		if (GetFunctionCodeSize(ulReal_Dispatch) == GetFunctionCodeSize(ulReloadNtfsDispatch) &&
			memcmp(ulReal_Dispatch,ulReloadNtfsDispatch,GetFunctionCodeSize(ulReal_Dispatch)) != NULL)
		{
			NtfsDispatchBakUp->NtfsDispatch[i].Hooked = 2; //fsd inline hook

			//简单的处理一下头5字节的hook的检查
			ulCurrentNtfsDispatch = *(PULONG)(ulReal_Dispatch+1)+(ULONG)(ulReal_Dispatch+5);
			//如果不是开头jmp hook，就写回原始地址
			if (!RMmIsAddressValid(ulCurrentNtfsDispatch))
			{
				ulCurrentNtfsDispatch = ulReal_Dispatch;
			}
		}
	}
_FunRet:
	if (!bIsHooked)
	{
		NtfsDispatchBakUp->NtfsDispatch[i].Hooked = 1;  //fsd hook
	}
	memset(NtfsDispatchBakUp->NtfsDispatch[i].lpszBaseModule,0,sizeof(NtfsDispatchBakUp->NtfsDispatch[0].lpszBaseModule));

	if (!IsAddressInSystem(
		ulCurrentNtfsDispatch,
		&ulHookModuleBase,
		&ulHookModuleSize,
		NtfsDispatchBakUp->NtfsDispatch[i].lpszBaseModule))
	{
		strcat(NtfsDispatchBakUp->NtfsDispatch[i].lpszBaseModule,"Unknown3");
	}
	NtfsDispatchBakUp->NtfsDispatch[i].ulCurrentNtfsDispatch = ulCurrentNtfsDispatch;
	NtfsDispatchBakUp->NtfsDispatch[i].ulModuleSize = ulHookModuleSize;
	NtfsDispatchBakUp->NtfsDispatch[i].ulModuleBase = ulHookModuleBase;

}
VOID SetFsdHook(ULONG ulNumber,ULONG ulRealDispatch)
{
	PDRIVER_OBJECT PNtfsDriverObject = NULL;
	ULONG ulReloadDispatch;

	if (RMmIsAddressValid(PNtfsDriverObjectBakup))
	{
		PNtfsDriverObject = PNtfsDriverObjectBakup;
		PNtfsDriverObject->MajorFunction[ulNumber] = ulRealDispatch;   //恢复 fsd hook

		if (GetWindowsVersion() == WINDOWS_VERSION_7 ||
			GetWindowsVersion() == WINDOWS_VERSION_XP)
		{
			ulReloadDispatch = ulRealDispatch - ulNtfsModuleBase + ulReLoadNtfsModuleBase;

			//恢复 fsd inline hook
			if (GetFunctionCodeSize(ulRealDispatch) != GetFunctionCodeSize(ulReloadDispatch))
			{
				return;
			}
			if (memcmp(ulRealDispatch,ulReloadDispatch,GetFunctionCodeSize(ulRealDispatch)) == NULL)
			{
				return;
			}
			__asm
			{
				cli
					push eax
					mov eax,cr0
					and eax,not 0x10000
					mov cr0,eax
					pop eax
			}
			memcpy(ulRealDispatch,ulReloadDispatch,GetFunctionCodeSize(ulRealDispatch));
			__asm
			{
				push eax
					mov eax,cr0
					or eax,0x10000
					mov cr0,eax
					pop eax
					sti
			}
		}
	}
}
NTSTATUS ReLoadNtfs(PDRIVER_OBJECT DriverObject,PNTFSDISPATCHBAKUP NtfsDispatchBakUp,int IniType)
{
	ULONG ulOldNtfsDispatch;
	ULONG ulNewNtfsDispatch;
	PDRIVER_OBJECT PNtfsDriverObject = NULL;
	int i=0;
	BOOL bIsWinVer = FALSE;
	BOOL bInit = FALSE;
	WIN_VER_DETAIL WinVer;
	BOOL bIsReLoadSuccess = FALSE;
	DWORD NtfsDriverEntry=0;
	ULONG ulAddress;
	ULONG ulDriverEntryToDispatchCodeOffset;
	ULONG ulOffset;
	HANDLE hSection;
	UNICODE_STRING UnicodeModule;
	ULONG ulModuleBase;

// 	RMmIsAddressValid = ReLoadNtosCALL(L"MmIsAddressValid",SystemKernelModuleBase,ImageModuleBase);
// 	if (RMmIsAddressValid)
// 	{
// 		bInit = TRUE;
// 	}
// 	if (!bInit)
// 		return STATUS_UNSUCCESSFUL;

// 	WinVer = GetWindowsVersion();
// 	if (WinVer == WINDOWS_VERSION_XP ||
// 		WinVer == WINDOWS_VERSION_7)
// 	{
// 		bIsWinVer = TRUE;
// 	}
// 	if (!bIsWinVer)
// 		return STATUS_UNSUCCESSFUL;

	//获取driverobject
	if (GetDriverObject(L"\\FileSystem\\Ntfs",&PNtfsDriverObject) == STATUS_SUCCESS)
	{
		PNtfsDriverObjectBakup = PNtfsDriverObject;

		ulNtfsModuleBase = PNtfsDriverObject->DriverStart;
		ulNtfsModuleSize = PNtfsDriverObject->DriverSize;

		WinVer = GetWindowsVersion();
		if (WinVer == WINDOWS_VERSION_2K3_SP1_SP2)
		{
			goto _FunRet;
		}
		//reload
		if (PeLoad(
			L"\\SystemRoot\\system32\\drivers\\ntfs.sys",
			&ulReLoadNtfsModuleBase,
			DriverObject,
			ulNtfsModuleBase
			))
		{
			bIsReLoadSuccess = TRUE;
		}
_FunRet:
		if (!bIsReLoadSuccess)
		{
			RtlInitUnicodeString(&UnicodeModule,L"\\SystemRoot\\system32\\drivers\\ntfs.sys");
			hSection = MapFileAsSection(&UnicodeModule,&ulModuleBase);
			if (hSection)
			{
				ulReLoadNtfsModuleBase = ulModuleBase;
				ZwClose(hSection);
			}
		}
		if (GetDriverEntryPoint(ulReLoadNtfsModuleBase,&NtfsDriverEntry))
		{
			WinVer = GetWindowsVersion();
			switch (WinVer)
			{
			case WINDOWS_VERSION_XP:
				ulDriverEntryToDispatchCodeOffset = 0xFA;   //硬编码了，xp
				ulOffset = 0;
				break;
			case WINDOWS_VERSION_7:
				ulDriverEntryToDispatchCodeOffset = 0x205;   //硬编码了，win7
				ulOffset = 0;
				break;
			case WINDOWS_VERSION_2K3_SP1_SP2:
				ulDriverEntryToDispatchCodeOffset = 0x101;   //硬编码了，2003
				ulOffset = (ULONG)PNtfsDriverObject->DriverStart - 0x10000;
				break;
			}
			ulAddress = NtfsDriverEntry + ulDriverEntryToDispatchCodeOffset;
			ulReal_IRP_MJ_LOCK_CONTROL = *(PULONG)(ulAddress+3) + ulOffset;
			ulReal_IRP_MJ_DIRECTORY_CONTROL = *(PULONG)(ulAddress+0xA) + ulOffset;
			ulReal_IRP_MJ_SET_INFORMATION = *(PULONG)(ulAddress+0x11) + ulOffset;
			ulReal_IRP_MJ_CREATE = *(PULONG)(ulAddress+0x18) + ulOffset;
			ulReal_IRP_MJ_CLOSE = *(PULONG)(ulAddress+0x1F) + ulOffset;
			ulReal_IRP_MJ_READ = *(PULONG)(ulAddress+0x26) + ulOffset;
			ulReal_IRP_MJ_WRITE = *(PULONG)(ulAddress+0x2D) + ulOffset;
			ulReal_IRP_MJ_FLUSH_BUFFERS = *(PULONG)(ulAddress+0x34) + ulOffset;
			ulReal_IRP_MJ_FILE_SYSTEM_CONTROL = *(PULONG)(ulAddress+0x3B) + ulOffset;

			ulReal_IRP_MJ_CLEANUP  = *(PULONG)(ulAddress+0x45) + ulOffset;
			ulReal_IRP_MJ_SHUTDOWN  = *(PULONG)(ulAddress+0x4c) + ulOffset;
			ulReal_IRP_MJ_PNP_POWER  = *(PULONG)(ulAddress+0x56) + ulOffset;

			if (IniType == 1)
			{
				//填充结构
				FixNtfs(NtfsDispatchBakUp,PNtfsDriverObjectBakup,0,ulReal_IRP_MJ_LOCK_CONTROL,L"IRP_MJ_LOCK_CONTROL",IRP_MJ_LOCK_CONTROL);
				FixNtfs(NtfsDispatchBakUp,PNtfsDriverObjectBakup,1,ulReal_IRP_MJ_DIRECTORY_CONTROL,L"IRP_MJ_DIRECTORY_CONTROL",IRP_MJ_DIRECTORY_CONTROL);
				FixNtfs(NtfsDispatchBakUp,PNtfsDriverObjectBakup,2,ulReal_IRP_MJ_SET_INFORMATION,L"IRP_MJ_SET_INFORMATION",IRP_MJ_SET_INFORMATION);
				FixNtfs(NtfsDispatchBakUp,PNtfsDriverObjectBakup,3,ulReal_IRP_MJ_CREATE,L"IRP_MJ_CREATE",IRP_MJ_CREATE);
				FixNtfs(NtfsDispatchBakUp,PNtfsDriverObjectBakup,4,ulReal_IRP_MJ_CLOSE,L"IRP_MJ_CLOSE",IRP_MJ_CLOSE);
				FixNtfs(NtfsDispatchBakUp,PNtfsDriverObjectBakup,5,ulReal_IRP_MJ_READ,L"IRP_MJ_READ",IRP_MJ_READ);
				FixNtfs(NtfsDispatchBakUp,PNtfsDriverObjectBakup,6,ulReal_IRP_MJ_WRITE,L"IRP_MJ_WRITE",IRP_MJ_WRITE);
				FixNtfs(NtfsDispatchBakUp,PNtfsDriverObjectBakup,7,ulReal_IRP_MJ_FLUSH_BUFFERS,L"IRP_MJ_FLUSH_BUFFERS",IRP_MJ_FLUSH_BUFFERS);
				FixNtfs(NtfsDispatchBakUp,PNtfsDriverObjectBakup,8,ulReal_IRP_MJ_FILE_SYSTEM_CONTROL,L"IRP_MJ_FILE_SYSTEM_CONTROL",IRP_MJ_FILE_SYSTEM_CONTROL);
				FixNtfs(NtfsDispatchBakUp,PNtfsDriverObjectBakup,9,ulReal_IRP_MJ_CLEANUP,L"IRP_MJ_CLEANUP",IRP_MJ_CLEANUP);
				FixNtfs(NtfsDispatchBakUp,PNtfsDriverObjectBakup,10,ulReal_IRP_MJ_SHUTDOWN,L"IRP_MJ_SHUTDOWN",IRP_MJ_SHUTDOWN);
				FixNtfs(NtfsDispatchBakUp,PNtfsDriverObjectBakup,11,ulReal_IRP_MJ_PNP_POWER,L"IRP_MJ_PNP_POWER",IRP_MJ_PNP_POWER);
				NtfsDispatchBakUp->ulCount = 12;
				goto DebugPrintOn;
			}

			switch (WinVer)
			{
			case WINDOWS_VERSION_XP:
			case WINDOWS_VERSION_7:
			//让所有的调用都走reload ntfs
				PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_LOCK_CONTROL] = ulReal_IRP_MJ_LOCK_CONTROL - ulNtfsModuleBase + ulReLoadNtfsModuleBase;
				PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_DIRECTORY_CONTROL] = ulReal_IRP_MJ_DIRECTORY_CONTROL - ulNtfsModuleBase + ulReLoadNtfsModuleBase;
				PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_SET_INFORMATION] = ulReal_IRP_MJ_SET_INFORMATION - ulNtfsModuleBase + ulReLoadNtfsModuleBase;
				PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_CREATE] = ulReal_IRP_MJ_CREATE - ulNtfsModuleBase + ulReLoadNtfsModuleBase;
				PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_CLOSE] = ulReal_IRP_MJ_CLOSE - ulNtfsModuleBase + ulReLoadNtfsModuleBase;
				PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_READ] = ulReal_IRP_MJ_READ - ulNtfsModuleBase + ulReLoadNtfsModuleBase;
				PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_WRITE] = ulReal_IRP_MJ_WRITE - ulNtfsModuleBase + ulReLoadNtfsModuleBase;
				PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_FLUSH_BUFFERS] = ulReal_IRP_MJ_FLUSH_BUFFERS - ulNtfsModuleBase + ulReLoadNtfsModuleBase;
				PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_FILE_SYSTEM_CONTROL] = ulReal_IRP_MJ_FILE_SYSTEM_CONTROL - ulNtfsModuleBase + ulReLoadNtfsModuleBase;
				PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_CLEANUP] = ulReal_IRP_MJ_CLEANUP - ulNtfsModuleBase + ulReLoadNtfsModuleBase;
				PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_SHUTDOWN] = ulReal_IRP_MJ_SHUTDOWN - ulNtfsModuleBase + ulReLoadNtfsModuleBase;
				PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_PNP_POWER] = ulReal_IRP_MJ_PNP_POWER - ulNtfsModuleBase + ulReLoadNtfsModuleBase;
				break;
			case WINDOWS_VERSION_2K3_SP1_SP2:
				//2003的话，先恢复吧
				PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_LOCK_CONTROL] = ulReal_IRP_MJ_LOCK_CONTROL;
				PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_DIRECTORY_CONTROL] = ulReal_IRP_MJ_DIRECTORY_CONTROL;
				PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_SET_INFORMATION] = ulReal_IRP_MJ_SET_INFORMATION;
				PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_CREATE] = ulReal_IRP_MJ_CREATE;
				PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_CLOSE] = ulReal_IRP_MJ_CLOSE;
				PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_READ] = ulReal_IRP_MJ_READ;
				PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_WRITE] = ulReal_IRP_MJ_WRITE;
				PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_FLUSH_BUFFERS] = ulReal_IRP_MJ_FLUSH_BUFFERS;
				PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_FILE_SYSTEM_CONTROL] = ulReal_IRP_MJ_FILE_SYSTEM_CONTROL;
				PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_CLEANUP] = ulReal_IRP_MJ_CLEANUP;
				PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_SHUTDOWN] = ulReal_IRP_MJ_SHUTDOWN;
				PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_PNP_POWER] = ulReal_IRP_MJ_PNP_POWER;
				break;
			}
DebugPrintOn:
			if (DebugOn)
				KdPrint(("[%08x-%x]ulReal_IRP_MJ_LOCK_CONTROL:%08x\r\n"
				"ulReal_IRP_MJ_DIRECTORY_CONTROL:%08x\r\n"
				"ulReal_IRP_MJ_SET_INFORMATION:%08x\r\n"
				"ulReal_IRP_MJ_CREATE:%08x\r\n"
				"ulReal_IRP_MJ_CLOSE:%08x\r\n"
				"ulReal_IRP_MJ_READ:%08x\r\n"
				"ulReal_IRP_MJ_WRITE:%08x\r\n"
				"ulReal_IRP_MJ_FLUSH_BUFFERS:%08x\r\n"
				"ulReal_IRP_MJ_FILE_SYSTEM_CONTROL:%08x\r\n"
				"ulReal_IRP_MJ_CLEANUP:%08x\r\n"
				"ulReal_IRP_MJ_SHUTDOWN:%08x\r\n"
				"ulReal_IRP_MJ_PNP_POWER:%08x\r\n",
				ulAddress,ulOffset,
				ulReal_IRP_MJ_LOCK_CONTROL,
				ulReal_IRP_MJ_DIRECTORY_CONTROL,
				ulReal_IRP_MJ_SET_INFORMATION,
				ulReal_IRP_MJ_CREATE,
				ulReal_IRP_MJ_CLOSE,
				ulReal_IRP_MJ_READ,
				ulReal_IRP_MJ_WRITE,
				ulReal_IRP_MJ_FLUSH_BUFFERS,
				ulReal_IRP_MJ_FILE_SYSTEM_CONTROL,
				ulReal_IRP_MJ_CLEANUP,
				ulReal_IRP_MJ_SHUTDOWN,
				ulReal_IRP_MJ_PNP_POWER
				));
			if (DebugOn)
				KdPrint(("[%08x]ulReal_IRP_MJ_LOCK_CONTROL:%08x\r\n"
				"ulReal_IRP_MJ_DIRECTORY_CONTROL:%08x\r\n"
				"ulReal_IRP_MJ_SET_INFORMATION:%08x\r\n"
				"ulReal_IRP_MJ_CREATE:%08x\r\n"
				"ulReal_IRP_MJ_CLOSE:%08x\r\n"
				"ulReal_IRP_MJ_READ:%08x\r\n"
				"ulReal_IRP_MJ_WRITE:%08x\r\n"
				"ulReal_IRP_MJ_FLUSH_BUFFERS:%08x\r\n"
				"ulReal_IRP_MJ_FILE_SYSTEM_CONTROL:%08x\r\n"
				"ulReal_IRP_MJ_CLEANUP:%08x\r\n"
				"ulReal_IRP_MJ_SHUTDOWN:%08x\r\n"
				"ulReal_IRP_MJ_PNP_POWER:%08x\r\n",
				ulAddress,
				ulReal_IRP_MJ_LOCK_CONTROL - ulNtfsModuleBase + ulReLoadNtfsModuleBase,
				ulReal_IRP_MJ_DIRECTORY_CONTROL - ulNtfsModuleBase + ulReLoadNtfsModuleBase,
				ulReal_IRP_MJ_SET_INFORMATION - ulNtfsModuleBase + ulReLoadNtfsModuleBase,
				ulReal_IRP_MJ_CREATE - ulNtfsModuleBase + ulReLoadNtfsModuleBase,
				ulReal_IRP_MJ_CLOSE - ulNtfsModuleBase + ulReLoadNtfsModuleBase,
				ulReal_IRP_MJ_READ - ulNtfsModuleBase + ulReLoadNtfsModuleBase,
				ulReal_IRP_MJ_WRITE - ulNtfsModuleBase + ulReLoadNtfsModuleBase,
				ulReal_IRP_MJ_FLUSH_BUFFERS - ulNtfsModuleBase + ulReLoadNtfsModuleBase,
				ulReal_IRP_MJ_FILE_SYSTEM_CONTROL - ulNtfsModuleBase + ulReLoadNtfsModuleBase,
				ulReal_IRP_MJ_CLEANUP - ulNtfsModuleBase + ulReLoadNtfsModuleBase,
				ulReal_IRP_MJ_SHUTDOWN - ulNtfsModuleBase + ulReLoadNtfsModuleBase,
				ulReal_IRP_MJ_PNP_POWER - ulNtfsModuleBase + ulReLoadNtfsModuleBase
				));
		}
	}
	return STATUS_SUCCESS;
}
NTSTATUS ReLoadNtfsFree()
{
	int i=0;
 	BOOL bIsWinVer = FALSE;
 	WIN_VER_DETAIL WinVer;
	PDRIVER_OBJECT PNtfsDriverObject = NULL;
 
//  	WinVer = GetWindowsVersion();
//  	if (WinVer == WINDOWS_VERSION_XP ||
// 		WinVer == WINDOWS_VERSION_7)
//  	{
//  		bIsWinVer = TRUE;
//  	}
//  	if (!bIsWinVer)
//  		return STATUS_UNSUCCESSFUL;

	//if (GetDriverObject("\\FileSystem\\Ntfs",&PNtfsDriverObject) == STATUS_SUCCESS)
	if (RMmIsAddressValid(PNtfsDriverObjectBakup))
	{
		PNtfsDriverObject = PNtfsDriverObjectBakup;

		if (DebugOn)
			KdPrint(("ReLoadNtfsFree success"));

		if (ulReal_IRP_MJ_LOCK_CONTROL)
			PNtfsDriverObject->MajorFunction[IRP_MJ_LOCK_CONTROL] = ulReal_IRP_MJ_LOCK_CONTROL;

		if (ulReal_IRP_MJ_DIRECTORY_CONTROL)
			PNtfsDriverObject->MajorFunction[IRP_MJ_DIRECTORY_CONTROL] = ulReal_IRP_MJ_DIRECTORY_CONTROL;

		if (ulReal_IRP_MJ_SET_INFORMATION)
			PNtfsDriverObject->MajorFunction[IRP_MJ_SET_INFORMATION] = ulReal_IRP_MJ_SET_INFORMATION;

		if (ulReal_IRP_MJ_CREATE)
			PNtfsDriverObject->MajorFunction[IRP_MJ_CREATE] = ulReal_IRP_MJ_CREATE;

		if (ulReal_IRP_MJ_CLOSE)
			PNtfsDriverObject->MajorFunction[IRP_MJ_CLOSE] = ulReal_IRP_MJ_CLOSE;

		if (ulReal_IRP_MJ_READ)
			PNtfsDriverObject->MajorFunction[IRP_MJ_READ] = ulReal_IRP_MJ_READ;

		if (ulReal_IRP_MJ_WRITE)
			PNtfsDriverObject->MajorFunction[IRP_MJ_WRITE] = ulReal_IRP_MJ_WRITE;

		if (ulReal_IRP_MJ_FLUSH_BUFFERS)
			PNtfsDriverObject->MajorFunction[IRP_MJ_FLUSH_BUFFERS] = ulReal_IRP_MJ_FLUSH_BUFFERS;

		if (ulReal_IRP_MJ_FILE_SYSTEM_CONTROL)
			PNtfsDriverObject->MajorFunction[IRP_MJ_FILE_SYSTEM_CONTROL] = ulReal_IRP_MJ_FILE_SYSTEM_CONTROL;

		if (ulReal_IRP_MJ_CLEANUP)
			PNtfsDriverObject->MajorFunction[IRP_MJ_CLEANUP] = ulReal_IRP_MJ_CLEANUP;

		if (ulReal_IRP_MJ_SHUTDOWN)
			PNtfsDriverObject->MajorFunction[IRP_MJ_SHUTDOWN] = ulReal_IRP_MJ_SHUTDOWN;

		if (ulReal_IRP_MJ_PNP_POWER)
			PNtfsDriverObject->MajorFunction[IRP_MJ_PNP_POWER] = ulReal_IRP_MJ_PNP_POWER;

	}
	return STATUS_SUCCESS;
}

//L"\\FileSystem\\Ntfs"
//L"\\SystemRoot\\system32\\drivers\\ntfs.sys"

// NTSTATUS PrintfDispatch(WCHAR *lpwzDispatch,WCHAR *lpwzFullFile)
// {
// 	PDRIVER_OBJECT PDispatchDriverObject;
// 
// 	ULONG ulModuleBase;
// 	ULONG ulModuleSize;
// 
// 	ULONG ulReLoadModuleBase;
// 	ULONG ulOldDispatch;
// 	ULONG ulNewDispatch;
// 
// 	BOOL bIsHook = FALSE;
// 
// 	//L"\\FileSystem\\Ntfs"
// 	if (GetDriverObject(lpwzDispatch,&PDispatchDriverObject) == STATUS_SUCCESS)
// 	{
// 		ulModuleBase = PDispatchDriverObject->DriverStart;
// 		ulModuleSize = PDispatchDriverObject->DriverSize;
// 
// 		//reload
// 		if (PeLoad(
// 			lpwzFullFile,
// 			&ulReLoadModuleBase,
// 			PDispatchDriverObject,
// 			ulModuleBase
// 			))
// 		{
// 			for (i = 0;i <= IRP_MJ_MAXIMUM_FUNCTION;i++)
// 			{
// 				ulOldDispatch = PDispatchDriverObject->MajorFunction[i];
// 
// 				if (ulOldDispatch > ulModuleBase && 
// 					ulOldDispatch < ulModuleBase+ulModuleSize)
// 				{
// 					bIsHook = TRUE;
// 				}
// 				if (!bIsHook)
// 				{
// 					//ntfs hook
// 				}
// 			}
// 		}
// 	}
// }