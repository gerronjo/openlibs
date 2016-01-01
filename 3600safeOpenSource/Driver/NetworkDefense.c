//**************************************************************************
//*
//*          A盾电脑防护 website:http://www.3600safe.com/
//*        
//*文件说明：
//*         防御日志
//**************************************************************************
#include "NetworkDefense.h"
// 
// //需要过滤的系统模块
// //可以自己添加~！
#define DLL_COUNT 16

WCHAR wzFilterModule[DLL_COUNT][256] = {

	L"\\Windows\\System32\\ntdll.dll",
	L"\\Windows\\System32\\kernel32.dll",
	L"\\Windows\\System32\\user32.dll",
	L"\\windows\\system32\\ntoskrnl.exe",
	L"\\windows\\system32\\ntkrnlpa.exe",
	L"\\windows\\system32\\drivers\\tcpip.sys",
	L"\\windows\\system32\\drivers\\ntfs.sys",
	L"\\windows\\system32\\drivers\\kdbclass.sys",
	L"\\windows\\system32\\drivers\\mouclass.sys",
	L"\\windows\\system32\\drivers\\atapi.sys",
	L"\\windows\\system32\\drivers\\nsiproxy.sys",
	L"\\Program Files\\Internet Explorer\\IEXPLORE.EXE",
	L"\\windows\\system32\\reg.exe",
	L"\\windows\\system32\\mswsock.dll",
    L"\\WINDOWS\\system32\\dnsapi.dll",
    L"\\windows\\system32\\wsock32.dll"
};
ULONG CheckExeFileOrDllFileBySectionHandle(HANDLE hSection)
{
	NTSTATUS status;
	PVOID BaseAddress = NULL;
	SIZE_T size=0;
	PIMAGE_NT_HEADERS PImageNtHeaders;

	if (!hSection)
		return NULL;

	RZwMapViewOfSection = ReLoadNtosCALL(L"ZwMapViewOfSection",SystemKernelModuleBase,ImageModuleBase);
	if (!RZwMapViewOfSection)
		return NULL;

	status = RZwMapViewOfSection(
		hSection, 
		NtCurrentProcess(),
		&BaseAddress, 
		0,
		1000, 
		0,
		&size,
		(SECTION_INHERIT)1,
		MEM_TOP_DOWN, 
		PAGE_READWRITE
		); 
	if(NT_SUCCESS(status))
	{
		if (DebugOn)
			KdPrint(("ZwMapViewOfSection success"));

		PImageNtHeaders = RtlImageNtHeader(BaseAddress);
		if (PImageNtHeaders)
		{
			if (DebugOn)
				KdPrint(("Text:%08x\r\n",PImageNtHeaders->FileHeader.Characteristics));
			return PImageNtHeaders->FileHeader.Characteristics;
		}
	}
	return NULL;
}
////////////////////////////////////////////////
/*
ZwCreateSection hook, DKOM type.
*/
NTSTATUS _stdcall NewZwCreateSection(
	__out     PHANDLE SectionHandle,
	__in      ACCESS_MASK DesiredAccess,
	__in_opt  POBJECT_ATTRIBUTES ObjectAttributes,
	__in_opt  PLARGE_INTEGER MaximumSize,
	__in      ULONG SectionPageProtection,
	__in      ULONG AllocationAttributes,
	__in_opt  HANDLE FileHandle)
{
	NTSTATUS status;
	PEPROCESS Eprocess;
	PFILE_OBJECT FileObject;
	PVOID object_temp;
	POBJECT_HEADER ObjectHeader;
	POBJECT_TYPE FileObjectType;
	WIN_VER_DETAIL WinVer;
	BOOL bRetOK = FALSE;
	BOOL bInherited = FALSE;
	int i;
	WCHAR *lpwzExeFile = NULL;
	WCHAR *lpwzExeNtFile = NULL;
	KPROCESSOR_MODE PreviousMode;
	UNICODE_STRING UnicodeDNSAPI_DLL;
	UNICODE_STRING UnicodeExeNtFilePath;
	char *lpszProName = NULL;
	BOOL bNetworkDefence = FALSE;
	BOOL bInitAPISuccess = FALSE;
	POBJECT_NAME_INFORMATION DosFullPath=NULL;
	ULONG ulExeFileCharacteristics,ulDllFileCharacteristics;
	STRING lpszProString;
	STRING lpszSvchostString;
	STRING lpszWinlogonString;
	STRING lpszServicesString;
	STRING lpszCmdString;
	BOOL bIsInjectDllInto3600 = FALSE;
	ULONG ulPathSize;
	ZWCREATESECTION OldZwCreateSection;

	//RPsGetCurrentProcess = ReLoadNtosCALL(L"PsGetCurrentProcess",SystemKernelModuleBase,ImageModuleBase);
	RObReferenceObjectByHandle = ReLoadNtosCALL(L"ObReferenceObjectByHandle",SystemKernelModuleBase,ImageModuleBase);
	RPsGetCurrentProcessId = ReLoadNtosCALL(L"PsGetCurrentProcessId",SystemKernelModuleBase,ImageModuleBase);
	RPsGetProcessImageFileName = ReLoadNtosCALL(L"PsGetProcessImageFileName",SystemKernelModuleBase,ImageModuleBase);
	RZwClose = ReLoadNtosCALL(L"ZwClose",SystemKernelModuleBase,ImageModuleBase);
	RExAllocatePool = ReLoadNtosCALL(L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
	RExFreePool = ReLoadNtosCALL(L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
	RIoQueryFileDosDeviceName = ReLoadNtosCALL(L"IoQueryFileDosDeviceName",SystemKernelModuleBase,ImageModuleBase);
	RRtlCompareString = ReLoadNtosCALL(L"RtlCompareString",SystemKernelModuleBase,ImageModuleBase);
	RRtlInitString = ReLoadNtosCALL(L"RtlInitString",SystemKernelModuleBase,ImageModuleBase);
	RRtlCompareUnicodeString = ReLoadNtosCALL(L"RtlCompareUnicodeString",SystemKernelModuleBase,ImageModuleBase);
	if (RPsGetCurrentProcess &&
		RObReferenceObjectByHandle &&
		RPsGetCurrentProcessId &&
		RPsGetProcessImageFileName &&
		RZwClose &&
		RExAllocatePool &&
		RExFreePool &&
		RIoQueryFileDosDeviceName &&
		RRtlCompareString &&
		RRtlInitString &&
		RRtlCompareUnicodeString)
	{
// 		if (DebugOn)
// 			KdPrint(("Init API Success\r\n"));
		bInitAPISuccess = TRUE;
	}
	if (!bInitAPISuccess)
		goto _FunctionRet;

	if (bDisCreateProcess == FALSE)  //禁止创建进程
	{
		//过滤自己啊
		if (RPsGetCurrentProcess() != ProtectEProcess)
		{
			return STATUS_UNSUCCESSFUL;
		}
	}
	if (RMmIsAddressValid(OriginalServiceDescriptorTable->ServiceTable[ZwCreateSectionIndex]))
	{
		OldZwCreateSection = OriginalServiceDescriptorTable->ServiceTable[ZwCreateSectionIndex];
	}else
		OldZwCreateSection = KeServiceDescriptorTable->ServiceTable[ZwCreateSectionIndex];

	status = OldZwCreateSection(
		SectionHandle,
		DesiredAccess,
		ObjectAttributes,
		MaximumSize,
		SectionPageProtection,
		AllocationAttributes,
		FileHandle
		);
	if (!NT_SUCCESS(status))
	{
		if (DebugOn)
			KdPrint(("ZwCreateSection failed\r\n"));
		return status;
	}
	//3600safe初始化OK
	if (!bIsInitSuccess)
	{
		if (DebugOn)
			KdPrint(("bIsInitSuccess failed\r\n"));
		return status;
	}

	if (!ARGUMENT_PRESENT(FileHandle))
	{
// 		if (DebugOn)
// 			KdPrint(("FileHandle failed\r\n"));
		return status;
	}
	if (KeGetCurrentIrql() > PASSIVE_LEVEL)
	{
		if (DebugOn)
			KdPrint(("IRQL > PASSIVE_LEVEL\r\n"));
		return status;
	}
	if ((AllocationAttributes == 0x1000000) && (SectionPageProtection & PAGE_EXECUTE))
	{
		PreviousMode = KeGetPreviousMode();
		if (PreviousMode != KernelMode)
		{
			__try{
				ProbeForRead(FileHandle,sizeof(HANDLE),sizeof(ULONG));
			}__except (EXCEPTION_EXECUTE_HANDLER) {
				goto _FunctionRet;
			}
		}
		status = RObReferenceObjectByHandle(
			FileHandle,
			NULL,
			*IoFileObjectType,
			KernelMode,
			(PVOID *)&object_temp,
			NULL);
		if (!NT_SUCCESS(status))
		{
			if (DebugOn)
				KdPrint(("get Object failed:%d\r\n",RtlNtStatusToDosError(status)));
			goto _FunctionRet;
		}
		ObDereferenceObject(object_temp);  //清除引用对象

		Eprocess = RPsGetCurrentProcess();
		//保险起见，还是在object_temp里判断type才更准确~！
		WinVer = GetWindowsVersion();
		switch (WinVer)
		{
		case WINDOWS_VERSION_XP:
		case WINDOWS_VERSION_2K3_SP1_SP2:
			ObjectHeader = OBJECT_TO_OBJECT_HEADER(object_temp);
			FileObjectType = ObjectHeader->Type;
			break;
		case WINDOWS_VERSION_7:
			//RtlInitUnicodeString(&UnicodeFunction,L"ObGetObjectType");
			//MyGetObjectType=(OBGETOBJECTTYPE)MmGetSystemRoutineAddress(&UnicodeFunction);  //xp~2008下无此函数，如果直接调用，则导致驱动加载失败，因此需要动态获取地址
			MyGetObjectType = GetSystemRoutineAddress(1,L"ObGetObjectType");
			if(MyGetObjectType)
			{
				FileObjectType = MyGetObjectType((PVOID)object_temp);
			}
			break;
		}
		if (FileObjectType != *IoFileObjectType)
		{
			goto _FunctionRet;
		}
		FileObject = (PFILE_OBJECT)object_temp;
		if (RMmIsAddressValid(FileObject) &&
			ValidateUnicodeString(&FileObject->FileName))
		{
			RRtlInitUnicodeString(&UnicodeDNSAPI_DLL,L"\\windows\\system32\\DNSapi.DLL");
			if (RRtlCompareUnicodeString(&FileObject->FileName,&UnicodeDNSAPI_DLL,TRUE) == 0)
			{
				if (LogDefenseInfo->ulCount < 1000)   //记录超过1000条，则不记录。
				{
					LogDefenseInfo->LogDefense[ulLogCount].EProcess = RPsGetCurrentProcess();
					LogDefenseInfo->LogDefense[ulLogCount].ulPID = RPsGetCurrentProcessId();
					LogDefenseInfo->LogDefense[ulLogCount].Type = 2;
					ulLogCount++;
				}
			}
			//记录所有这些进程作为父进程创建子进程的行为
			lpszProName = (char *)RPsGetProcessImageFileName(Eprocess);
			RRtlInitString(&lpszProString,lpszProName);

			RRtlInitString(&lpszSvchostString,"svchost.exe");
			RRtlInitString(&lpszWinlogonString,"winlogon.exe");
			RRtlInitString(&lpszServicesString,"services.exe");
			RRtlInitString(&lpszCmdString,"cmd.exe");

			if (RRtlCompareString(&lpszSvchostString,&lpszProString,TRUE) == 0 ||
				RRtlCompareString(&lpszWinlogonString,&lpszProString,TRUE) == 0 ||
				RRtlCompareString(&lpszServicesString,&lpszProString,TRUE) == 0 ||
				RRtlCompareString(&lpszCmdString,&lpszProString,TRUE) == 0 ||
				Eprocess == ProtectEProcess)
			{
				if (LogDefenseInfo->ulCount < 1000 &&
					ulLogCount < 1000)   //记录超过1000条，则不记录。
				{
					if (FileObject->FileName.Buffer != NULL &&
						FileObject->FileName.Length >30 &&
						RIoQueryFileDosDeviceName(FileObject,&DosFullPath) == STATUS_SUCCESS)
					{
						ulPathSize = DosFullPath->Name.Length;

						lpwzExeFile = RExAllocatePool(NonPagedPool,ulPathSize);
						if (!lpwzExeFile)
						{
							if (DosFullPath)
								RExFreePool(DosFullPath);
							goto _FunctionRet;
						}
						memset(lpwzExeFile,0,ulPathSize);
						SafeCopyMemory(DosFullPath->Name.Buffer,lpwzExeFile,ulPathSize);

						if (DosFullPath)
							RExFreePool(DosFullPath);

						//xp/2003
						//0x10f = ExeFile
						//0x210e = DllFile

						//win7
						//0x102 = ExeFile
						//0x2102 = DllFile
						WinVer = GetWindowsVersion();
						switch (WinVer)
						{
						case WINDOWS_VERSION_XP:
						case WINDOWS_VERSION_2K3_SP1_SP2:
							ulExeFileCharacteristics = 0x10f;
							//ulDllFileCharacteristics = 0x210e;
							break;
						case WINDOWS_VERSION_7:
							ulExeFileCharacteristics = 0x102;
							//ulDllFileCharacteristics = 0x2102;
							break;
						}
						if (CheckExeFileOrDllFileBySectionHandle(*SectionHandle) == ulExeFileCharacteristics ||
							Eprocess == ProtectEProcess)
						{
							__try
							{
								if (Eprocess == ProtectEProcess)
								{
									//排除ntdll.dll，ntos*
									for (i=0;i<DLL_COUNT;i++)
									{
										RRtlInitUnicodeString(&UnicodeDNSAPI_DLL,wzFilterModule[i]);
										if (RRtlCompareUnicodeString(&FileObject->FileName,&UnicodeDNSAPI_DLL,TRUE) == 0)
										{
											if (DebugOn)
												KdPrint(("3600safe Module:%ws",wzFilterModule[i]));

											goto _FunctionRet;
										}
									}
									LogDefenseInfo->LogDefense[ulLogCount].Type = 4;   //DLL注入
									bIsInjectDllInto3600 = TRUE;

								}else
									LogDefenseInfo->LogDefense[ulLogCount].Type = 3;

								memset(LogDefenseInfo->LogDefense[ulLogCount].lpwzCreateProcess,0,sizeof(LogDefenseInfo->LogDefense[ulLogCount].lpwzCreateProcess));
								SafeCopyMemory(lpwzExeFile,LogDefenseInfo->LogDefense[ulLogCount].lpwzCreateProcess,ulPathSize);

								if (DebugOn)
									KdPrint(("ExePath:%ws\r\n",LogDefenseInfo->LogDefense[ulLogCount].lpwzCreateProcess));

								LogDefenseInfo->LogDefense[ulLogCount].EProcess = RPsGetCurrentProcess();
								LogDefenseInfo->LogDefense[ulLogCount].ulPID = RPsGetCurrentProcessId();
								ulLogCount++;

							}__except (EXCEPTION_EXECUTE_HANDLER) {
								if (lpwzExeFile)
									RExFreePool(lpwzExeFile);
								goto _FunctionRet;
							}

						}
						if (RtlCompareString(&lpszSvchostString,&lpszProString,TRUE) == 0)
						{
							__try
							{
								memset(LogDefenseInfo->LogDefense[ulLogCount].lpwzCreateProcess,0,sizeof(LogDefenseInfo->LogDefense[ulLogCount].lpwzCreateProcess));
								SafeCopyMemory(lpwzExeFile,LogDefenseInfo->LogDefense[ulLogCount].lpwzCreateProcess,ulPathSize);

								if (DebugOn)
									KdPrint(("DLLPath:%ws\r\n",LogDefenseInfo->LogDefense[ulLogCount].lpwzCreateProcess));

								LogDefenseInfo->LogDefense[ulLogCount].EProcess = RPsGetCurrentProcess();
								LogDefenseInfo->LogDefense[ulLogCount].ulPID = RPsGetCurrentProcessId();
								LogDefenseInfo->LogDefense[ulLogCount].Type = 3;
								ulLogCount++;

							}__except (EXCEPTION_EXECUTE_HANDLER) {
								if (lpwzExeFile)
									RExFreePool(lpwzExeFile);
								goto _FunctionRet;
							}
						}
						if (lpwzExeFile)
							RExFreePool(lpwzExeFile);
					}
				}
			}
		}
	}
_FunctionRet:

	//如果是注入，并且保护进程的时候，就拒绝
	if (bIsInjectDllInto3600 &&
		bProtectProcess)
	{
		bIsInjectDllInto3600 = FALSE;
		return STATUS_UNSUCCESSFUL;
	}
	//最后一次调用返回
	return STATUS_SUCCESS;
}
BOOL InitNetworkDefence()
{
	UNICODE_STRING UnicdeFunction;

	//RtlInitUnicodeString(&UnicdeFunction,L"ZwCreateSection");
	if (SystemCallEntryTableHook(
		"ZwCreateSection",
		&ZwCreateSectionIndex,
		NewZwCreateSection) == TRUE)
	{
		if (DebugOn)
			KdPrint(("Create Control Thread success 3\r\n"));
	}
}