//**************************************************************************
//*
//*          A盾电脑防护 website:http://www.3600safe.com/
//*        
//*文件说明：
//*         枚举进程DLL模块
//**************************************************************************
#include "ProcessModule.h"

/*
NTSTATUS SafeMmUnmapViewOfSection(PEPROCESS Process,ULONG lpBase)  //NtUnmapViewOfSection
{
	NTSTATUS status = STATUS_UNSUCCESSFUL;

	RMmUnmapViewOfSection = ReLoadNtosCALL(L"MmUnmapViewOfSection",SystemKernelModuleBase,ImageModuleBase);
	if (RMmUnmapViewOfSection)
	{
		ObReferenceObject(Process);
		status = RMmUnmapViewOfSection(Process,lpBase);
		ObDereferenceObject(Process);
	}
	return status;
}
*/
BOOL MyParseVadTreeRoutine(BYTE* VadNode,PDLLINFO PDll)
{
	DWORD ControlAreaOffset;
	PCONTROL_AREA ControlArea=NULL;
	PFILE_OBJECT FileObject=NULL;
	DWORD StartingVpnOffset=0;
	DWORD StartingVpn;
	DWORD Subsection;
	DWORD SubsectionOffset;
	PEX_FAST_REF FilePointer;
	POBJECT_NAME_INFORMATION UnicodeDosFullPath = NULL;
	WIN_VER_DETAIL WinVer;
	BOOL bRetOK = FALSE;

	if (KeGetCurrentIrql() > PASSIVE_LEVEL)
	{
		return bRetOK;
	}
	__try
	{
		WinVer=GetWindowsVersion();
		switch (WinVer)
		{
		case WINDOWS_VERSION_XP:
			StartingVpnOffset=0x00;
			ControlAreaOffset=0x18;
			break;
		case WINDOWS_VERSION_2K3:
			StartingVpnOffset=0x0c;
			ControlAreaOffset=0x18;
			break;
		case WINDOWS_VERSION_2K3_SP1_SP2:
			StartingVpnOffset=0x0c;
			ControlAreaOffset=0x18;
			break;
		case WINDOWS_VERSION_VISTA_2008:                             //Vista+2008开始
			StartingVpnOffset=0x0c;
			SubsectionOffset=0x24;
			break;
			/*case 6002:                             //2008
			StartingVpnOffset=0x0c;
			SubsectionOffset=0x24;
			break;
			*/
		case WINDOWS_VERSION_7:
			StartingVpnOffset=0x0c;
			SubsectionOffset=0x24;
			break;
		}
		if ( WinVer == WINDOWS_VERSION_XP || WinVer == WINDOWS_VERSION_2K3 || WinVer == WINDOWS_VERSION_2K3_SP1_SP2 )   //xp || 2003
		{
			if (!RMmIsAddressValid((VadNode+StartingVpnOffset)))
			{
				bRetOK = FALSE;
				__leave;
			}
			StartingVpn=*(DWORD*)(VadNode+StartingVpnOffset);
			if (!StartingVpn)
			{
				bRetOK = FALSE;
				__leave;
			}
			// StartingVpn<<12 is base (xp下exe模块的地址一般都是 0x01000000)
			if (NULL != StartingVpn<<12)
			{
				ControlArea=(PCONTROL_AREA)(*(DWORD*)(VadNode+ControlAreaOffset));
				if (RMmIsAddressValid(ControlArea))
				{
					FileObject=ControlArea->FilePointer;
					if (RMmIsAddressValid(FileObject) &&
						ValidateUnicodeString(&FileObject->FileName) &&
						FileObject->FileName.Buffer != NULL &&
						FileObject->FileName.Length > 0)
					{
						if (ModuleCount < 518)  //会有518个dll么？
						{
							if (sizeof(PDll->DllInfo[ModuleCount].lpwzDllModule) > FileObject->FileName.Length)
							{
								memset(PDll->DllInfo[ModuleCount].lpwzDllModule,0,sizeof(PDll->DllInfo[ModuleCount].lpwzDllModule));

								SafeCopyMemory(
									FileObject->FileName.Buffer,
									PDll->DllInfo[ModuleCount].lpwzDllModule,
									FileObject->FileName.Length
									);
								PDll->DllInfo[ModuleCount].ulBase = (ULONG)StartingVpn<<12;
								ModuleCount++;
								PDll->ulCount = ModuleCount;
								bRetOK = TRUE;
							}
						}
					}
				}
			}

		}
		else if ( WinVer == WINDOWS_VERSION_VISTA_2008 || WinVer == WINDOWS_VERSION_7) //win7||vista||2008
		{
			if (!RMmIsAddressValid((VadNode+StartingVpnOffset)))
			{
				bRetOK = FALSE;
				__leave;
			}
			StartingVpn=*(DWORD*)(VadNode+StartingVpnOffset);
			if (!StartingVpn)
			{
				bRetOK = FALSE;
				__leave;
			}
			// StartingVpn<<12 is base (xp下exe模块的地址一般都是 0x01000000)
			if (NULL != StartingVpn<<12)
			{
				if (DebugOn)
					KdPrint(("[%08x]VadNode:%X\n",StartingVpn<<12,VadNode));

				Subsection=*(DWORD*)(VadNode+SubsectionOffset); //win7下新加的结构
				if (RMmIsAddressValid(Subsection))
				{
					ControlArea=(PCONTROL_AREA)(*(DWORD*)Subsection);
					if (RMmIsAddressValid(ControlArea))
					{
						FilePointer=(PEX_FAST_REF)&ControlArea->FilePointer;
						if (RMmIsAddressValid(FilePointer))
						{
							FileObject=(PFILE_OBJECT)(((DWORD)FilePointer->Object)&(~FilePointer->RefCnt));
							if (RMmIsAddressValid(FileObject) &&
								ValidateUnicodeString(&FileObject->FileName) &&
								FileObject->FileName.Buffer != NULL &&
								FileObject->FileName.Length > 0)
							{
								if (ModuleCount < 518)  //会有518个dll么？
								{
									if (sizeof(PDll->DllInfo[ModuleCount].lpwzDllModule) > FileObject->FileName.Length)
									{
										memset(PDll->DllInfo[ModuleCount].lpwzDllModule,0,sizeof(PDll->DllInfo[ModuleCount].lpwzDllModule));

										SafeCopyMemory(
											FileObject->FileName.Buffer,
											PDll->DllInfo[ModuleCount].lpwzDllModule,
											FileObject->FileName.Length
											);
										PDll->DllInfo[ModuleCount].ulBase = (ULONG)StartingVpn<<12;
										ModuleCount++;
										PDll->ulCount = ModuleCount;
										bRetOK = TRUE;
									}
								}
							}
						}
					}
				}
			}

		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		if (DebugOn)
			DbgPrint("VadTree Error \r\n");
	}
	return bRetOK;
}
/*
lkd> dt_MMVAD 81e7dbd0
nt!_MMVAD
+0x000 u1               : __unnamed
+0x004 LeftChild        : (null) 
+0x008 RightChild       : 0x81e68a50 _MMVAD
+0x00c StartingVpn      : 0
+0x010 EndingVpn        : 0
+0x014 u                : __unnamed
+0x018 ControlArea      : 0x81e68a50 _CONTROL_AREA
+0x01c FirstPrototypePte : (null) 
+0x020 LastContiguousPte : 0x0a130053 _MMPTE
+0x024 u2               : __unnamed
*/

VOID ParseVadTree(BYTE* VadNode,PDLLINFO PDll)
{
	DWORD LeftChildOfsset=0,RightChildOffset=0;
	WIN_VER_DETAIL WinVer;


	WinVer=GetWindowsVersion();
	switch (WinVer)
	{
	case WINDOWS_VERSION_XP:
		LeftChildOfsset=0x0c;
		RightChildOffset=0x10;
		break;
	case WINDOWS_VERSION_2K3:
		LeftChildOfsset=0x04;
		RightChildOffset=0x08;
		break;
	case WINDOWS_VERSION_2K3_SP1_SP2:
		LeftChildOfsset=0x04;
		RightChildOffset=0x08;
		break;
	case WINDOWS_VERSION_VISTA_2008:  //vista + 2008
		LeftChildOfsset=0x04;
		RightChildOffset=0x08;
		break;
		/*case 6002:                             //2008
		LeftChildOfsset=0x04;
		RightChildOffset=0x08;
		break;
		*/
	case WINDOWS_VERSION_7:
		LeftChildOfsset=0x04;
		RightChildOffset=0x08;
		break;
	}
	if (LeftChildOfsset==0||RightChildOffset==0)
	{
		return;
	}
	if (RMmIsAddressValid(VadNode))
	{
// 		if (ModuleCount >= 518)
// 		{
// 			return;
// 		}
		if (RMmIsAddressValid((VadNode+LeftChildOfsset)) &&
			RMmIsAddressValid((VadNode+RightChildOffset)))  //2008下是没有MMVAD这个结构好像，所以要进行合法性判断
		{
			ParseVadTree((BYTE*)(*(DWORD*)(VadNode+LeftChildOfsset)),PDll);   //2008下蓝屏~，由此可见，2008下是没有MMVAD这个结构，所以 VadNode+LeftChildOfsset 地址的就是无效的
			MyParseVadTreeRoutine(VadNode,PDll);
			ParseVadTree((BYTE*)(*(DWORD*)(VadNode+RightChildOffset)),PDll);
		}
	}

}
VOID ProcessFullPathFromVAD(PEPROCESS Process,PDLLINFO PDll)
{
	DWORD VadOffset=0;
	PVOID VadRootParent;
	WIN_VER_DETAIL WinVer;

	DWORD ControlAreaOffset;
	PCONTROL_AREA ControlArea=NULL;
	DWORD Subsection;
	DWORD SubsectionOffset;

	WinVer=GetWindowsVersion();

	switch (WinVer)
	{
	case WINDOWS_VERSION_XP:
		VadOffset=0x11c;
		ControlAreaOffset=0x18;
		VadRootParent=(PVOID)*(ULONG*)((DWORD)Process+VadOffset);
		break;
	case WINDOWS_VERSION_2K3:
		VadOffset=0x258;
		ControlAreaOffset=0x18;
		VadRootParent=(PVOID)*(ULONG*)((DWORD)Process+VadOffset);
		break;
	case WINDOWS_VERSION_2K3_SP1_SP2:  //2003sp1 sp2
		VadOffset=0x250;
		ControlAreaOffset=0x18;
		VadRootParent=(PVOID)*(ULONG*)((DWORD)Process+VadOffset);
		break;
	case WINDOWS_VERSION_VISTA_2008:      //Vista+2008
		VadOffset=0x238;
		SubsectionOffset=0x24;
		VadRootParent=(PVOID)((DWORD)Process+VadOffset);
		break;
		/*case WINDOWS_VERSION_VISTA_2008:      //2008的版本号其实和Vista一样~
		VadOffset=0x238;
		break;
		*/
	case WINDOWS_VERSION_7:
		VadOffset=0x278;
		SubsectionOffset=0x24;
		VadRootParent=(PVOID)((DWORD)Process+VadOffset);
		break;
	}
	if (!RMmIsAddressValid(VadRootParent))  //要对vadroot（PMM_AVL_TABLE）进行检查即可，因为windows2008下没有 MMVAD 这个结构了
	{
		if (DebugOn)
			KdPrint(("VAD is not a Valid Address\n"));
		return;
	}
	//首先判断PCONTROL_AREA是可以访问的，然后再枚举，修正：
	//http://bbs.pediy.com/showthread.php?t=148971

	switch (WinVer)
	{
	case WINDOWS_VERSION_XP:
	case WINDOWS_VERSION_2K3_SP1_SP2:
		ControlArea=(PCONTROL_AREA)(*(DWORD*)((ULONG)VadRootParent+ControlAreaOffset));
		if (!RMmIsAddressValid(ControlArea))
		{
			if (DebugOn)
				KdPrint(("ControlArea=0x%08X failed\n",ControlArea));
			return;
		}
		break;
	case WINDOWS_VERSION_7:
		Subsection=*(DWORD*)((ULONG)VadRootParent+SubsectionOffset); //win7下新加的结构
		if (!RMmIsAddressValid(Subsection))
		{
			return;
		}
		ControlArea=(PCONTROL_AREA)(*(DWORD*)Subsection);
		if (!RMmIsAddressValid(ControlArea))
		{
			if (DebugOn)
				KdPrint(("ControlArea=0x%08X failed\n",ControlArea));
			return;
		}
		break;
	}
	ParseVadTree(VadRootParent,PDll);
}
/*
NTSTATUS DriverEntry(
    IN OUT PDRIVER_OBJECT   DriverObject,
    IN PUNICODE_STRING      RegistryPath
    )
{
	
    PEPROCESS Eprocess;
	DWORD ProcessBase;
	BOOL bRet;

    DriverObject->DriverUnload = DriverUnload;
	
	if (LookupProcessByName("explorer.exe",&Eprocess) == STATUS_SUCCESS)
	{
		KdPrint(("Eprocess:%08x\n",Eprocess));
		ProcessFullPathFromVAD(Eprocess);
	}
    return STATUS_SUCCESS;
}
*/
DWORD GetDllModuleFromPeb(PEPROCESS Process,PDLLINFO PDll)
{

	DWORD PebOffset=0;
	DWORD PebLdrOffset = 0;
	DWORD Peb;
	PPEB_LDR_DATA PebLdr;
	PLDR_DATA_TABLE_ENTRY LdrTableEntry;
	PLIST_ENTRY pListHead,pListNext;
	int i = 0;
	WIN_VER_DETAIL WinVer;
	BOOL bRetOK = FALSE;

	if (!RMmIsAddressValid(Process))
	{
		return bRetOK;
	}
	__try
	{
		WinVer=GetWindowsVersion();
		switch (WinVer)
		{
		case WINDOWS_VERSION_XP:     //xp
			PebOffset=0x1b0;
			PebLdrOffset = 0x0c;
			break;
		case WINDOWS_VERSION_2K3_SP1_SP2:     //2003
			PebOffset=0x1a0;
			PebLdrOffset = 0x0c;
			break;
		case WINDOWS_VERSION_7:     //win7
			PebOffset=0x1a8;
			PebLdrOffset = 0x0c;
			break;
		}
		if (PebOffset==0 ||
			PebLdrOffset==0)
		{
			bRetOK = FALSE;
			__leave;
		}
		Peb = *(DWORD*)((DWORD)Process+PebOffset);

		ProbeForRead((PVOID)Peb,4,1);//PEB是用户空间的，可能会不能访问

		if (Peb == 0 ||
			!RMmIsAddressValid(Peb))
		{
			if (DebugOn)
				DbgPrint("Peb is null\n");
			bRetOK = FALSE;
			__leave;
		}

		PebLdr=(PPEB_LDR_DATA)*(DWORD*)(Peb+PebLdrOffset);

		ProbeForRead((PVOID)PebLdr,4,1);

		if (!RMmIsAddressValid(PebLdr))
		{
			if (DebugOn)
				DbgPrint("PebLdr offset is null\n");
			bRetOK = FALSE;
			__leave;
		}
		pListHead=&PebLdr->InLoadOrderModuleList;
		pListNext=pListHead->Flink;
		while (pListHead!=pListNext)
		{
			LdrTableEntry=(PLDR_DATA_TABLE_ENTRY)pListNext;
			if (!RMmIsAddressValid(LdrTableEntry))
			{
				break;
			}
			if (ValidateUnicodeString(&LdrTableEntry->FullDllName) &&
				LdrTableEntry->FullDllName.Buffer != NULL &&
				LdrTableEntry->FullDllName.Length > 0 )
			{
				if (i < 518)  //会有518个dll么？
				{
					if (sizeof(PDll->DllInfo[i].lpwzDllModule) > LdrTableEntry->FullDllName.Length)
					{
						if (DebugOn)
							DbgPrint("DllBase:%x  dll path %ws:%d\n",LdrTableEntry->DllBase,LdrTableEntry->FullDllName.Buffer,LdrTableEntry->FullDllName.Length);

						PDll->DllInfo[i].ulBase = LdrTableEntry->DllBase;

						memset(PDll->DllInfo[i].lpwzDllModule,0,sizeof(PDll->DllInfo[i].lpwzDllModule));
						SafeCopyMemory(
							LdrTableEntry->FullDllName.Buffer,
							PDll->DllInfo[i].lpwzDllModule,
							LdrTableEntry->FullDllName.Length
							);
						i++;
						PDll->ulCount = i;
					}
				}
			}
			pListNext=pListNext->Flink;

		}
		bRetOK = TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		if (DebugOn)
			DbgPrint("GetDllNameFromPeb Error \r\n");
	}
	return bRetOK;
}
VOID EunmProcessModuleByVirtualMemory(ULONG ulPid,PDLLINFO PDll)
{
	ULONG dwStartAddr = 0x00000000;
	HANDLE hProcess;
	MEMORY_BASIC_INFORMATION mbi;
	PUNICODE_STRING SectionName = NULL;
	OBJECT_ATTRIBUTES ObjectAttributes;
	CLIENT_ID ClientId={0};
	NTSTATUS status;
	int i=0;
	int count = 0;
	ULONG ulDllSize;

	BOOL bInit = FALSE;

	RZwOpenProcess = ReLoadNtosCALL(L"ZwOpenProcess",SystemKernelModuleBase,ImageModuleBase);
	RZwQueryVirtualMemory = ReLoadNtosCALL(L"ZwQueryVirtualMemory",SystemKernelModuleBase,ImageModuleBase);
	RExAllocatePool = ReLoadNtosCALL(L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
	RExFreePool = ReLoadNtosCALL(L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
	RZwClose = ReLoadNtosCALL(L"ZwClose",SystemKernelModuleBase,ImageModuleBase);
	if (RZwOpenProcess &&
		RZwQueryVirtualMemory &&
		RExAllocatePool &&
		RExFreePool &&
		RZwClose)
	{
		bInit = TRUE;
	}
	if (!bInit)
		return NULL;

	ClientId.UniqueProcess = ulPid;
	InitializeObjectAttributes(
		&ObjectAttributes,
		NULL,
		0,
		NULL, 
		NULL 
		);
	status=RZwOpenProcess(
		&hProcess,
		PROCESS_ALL_ACCESS,
		&ObjectAttributes,
		&ClientId
		);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("ZwOpenProcess failed:%d",RtlNtStatusToDosError(status)));
		return;
	}
	SectionName = (PUNICODE_STRING)RExAllocatePool(NonPagedPool,260*sizeof(WCHAR));
	if (!SectionName)
	{
		RZwClose(hProcess);
		return;
	}
	memset(SectionName,0,260*sizeof(WCHAR));

	__try
	{
		for (dwStartAddr=0; dwStartAddr<0x80000000; dwStartAddr=dwStartAddr+0x10000)
		{
			status = RZwQueryVirtualMemory(
				hProcess,
				(PVOID)dwStartAddr,
				MemoryBasicInformation,
				&mbi,
				sizeof(MEMORY_BASIC_INFORMATION),
				0
				);
			if (NT_SUCCESS(status))
			{
				if(mbi.Type == MEM_IMAGE)
				{
					status = RZwQueryVirtualMemory(
						hProcess,
						(PVOID)dwStartAddr,
						MemorySectionName,
						SectionName,
						260*sizeof(WCHAR),
						0
						);
					if (NT_SUCCESS(status))
					{
						if (i)
							count = i - 1;

						//如果当前的DLL模块路径 不等于前一个，则说明是开始了下一个DLL的枚举了。比如：
						//c:\windows\system32\ntdll.dll
						//c:\windows\system32\ntdll.dll
						//c:\windows\system32\ntdll.dll
						//c:\windows\system32\ntdll.dll
						//c:\windows\system32\ntdll.dll
						//c:\windows\system32\kernel32.dll
						if (_wcsnicmp(PDll->DllInfo[count].lpwzDllModule,SectionName->Buffer,SectionName->Length/2) != 0 &&
							sizeof(PDll->DllInfo[i].lpwzDllModule) > SectionName->Length)
						{
							if (DebugOn)
								KdPrint(("DLL-->%08x:%08x:%ws\r\n",mbi.BaseAddress,dwStartAddr,SectionName->Buffer));

							PDll->DllInfo[i].ulBase = dwStartAddr;
							SafeCopyMemory(
								SectionName->Buffer,
								PDll->DllInfo[i].lpwzDllModule,
								SectionName->Length
								);
							i++;
							PDll->ulCount = i;

							if (i > 518)
							{
								break;
							}

							//获取dll大小
							//备注一下这个方法
// 							for (ulDllSize = dwStartAddr+mbi.RegionSize;ulDllSize < 0x80000000;ulDllSize += mbi.RegionSize)
// 							{
// 								status = ZwQueryVirtualMemory(
// 									hProcess,
// 									(PVOID)ulDllSize,
// 									MemoryBasicInformation,
// 									&mbi,
// 									sizeof(MEMORY_BASIC_INFORMATION),
// 									0
// 									);
// 								if (NT_SUCCESS(status))
// 								{
// 									if(mbi.Type != MEM_IMAGE)
// 									{
// 										KdPrint(("DLL-->%08x:%08x:%ws\r\n",dwStartAddr,ulDllSize-dwStartAddr,SectionName->Buffer));
// 										break;
// 									}
// 								}
// 							}
						}
					}
				}
			}
		}

	}__except(EXCEPTION_EXECUTE_HANDLER){
		goto _FunctionRet;
	}
_FunctionRet:

	RExFreePool(SectionName);

	if (DebugOn)
		KdPrint(("ZwQueryVirtualMemory DLL count:%d-%d\r\n",i,PDll->ulCount));

	RZwClose(hProcess);
}
//为什么VAD的地址有时候是无效呢
VOID EunmProcessModule(ULONG Eprocess,PDLLINFO PDll)
{
	//KAPC_STATE KApc;
	WIN_VER_DETAIL WinVer;
	ULONG g_Offset_Eprocess_ProcessId;
	ULONG ulPid;

	if (!RMmIsAddressValid(Eprocess) ||
		!IsExitProcess(Eprocess))
	{
		return;
	}
	if (PDll)
	{
		ModuleCount = 0;
		ProcessFullPathFromVAD(Eprocess,PDll);

		if (!PDll->ulCount)
		{
			if (!WinVersion)
				WinVer = GetWindowsVersion();
			else
				WinVer = WinVersion;

			switch(WinVer)
			{
			case WINDOWS_VERSION_XP:
				g_Offset_Eprocess_ProcessId = 0x84;
				break;
			case WINDOWS_VERSION_7:
				g_Offset_Eprocess_ProcessId = 0xb4;
				break;
			case WINDOWS_VERSION_VISTA_2008:
				g_Offset_Eprocess_ProcessId = 0x09c;
				break;
			case WINDOWS_VERSION_2K3_SP1_SP2:
				g_Offset_Eprocess_ProcessId = 0x94;
				break;
			case WINDOWS_VERSION_2K3:
				g_Offset_Eprocess_ProcessId = 0x084;
				break;
			}
			if (!g_Offset_Eprocess_ProcessId)
			{
				return;
			}
			ulPid = *(ULONG *)(Eprocess + g_Offset_Eprocess_ProcessId);

			if (ulPid > 0 &&
				ulPid < 123456)
			{
				//VAD失败就到VirtualMemory
				EunmProcessModuleByVirtualMemory(ulPid,PDll);
			}

			if (!PDll->ulCount)
			{
				RKeAttachProcess = ReLoadNtosCALL(L"KeAttachProcess",SystemKernelModuleBase,ImageModuleBase);
				RKeDetachProcess  = ReLoadNtosCALL(L"KeDetachProcess",SystemKernelModuleBase,ImageModuleBase);
				if (RKeAttachProcess &&
					RKeDetachProcess)
				{
					RKeAttachProcess(Eprocess);

					//如果VAD和VirtualMemory都失败，就从peb里获取吧
					//挂载进程
					GetDllModuleFromPeb(Eprocess,PDll);

					RKeDetachProcess();
				}
			}
		}
	}
	return;
}