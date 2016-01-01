//**************************************************************************
//*
//*          A盾电脑防护 website:http://www.3600safe.com/
//*        
//*文件说明：
//*         A盾电脑防护主文件
//**************************************************************************
#include "SafeSystem.h"

VOID DriverUnload(
	IN PDRIVER_OBJECT		DriverObject
	)
{
	KdPrint(("Driver Unload Called\n"));

}
VOID IsKernelBooting(IN PVOID Context)
{
	PEPROCESS EProcess;
	NTSTATUS status;
	PUCHAR KiFastCallEntry;

	while (1)
	{
		EProcess = NULL;
		if (LookupProcessByName("Smss.exe",&EProcess) == STATUS_SUCCESS)
		{
			_asm
			{
				pushad;
				mov ecx, 0x176;
				rdmsr;
				mov KiFastCallEntry, eax;
				popad;
			}
			if (*KiFastCallEntry == 0xe9)
			{
				KdPrint(("Terminate System Thread"));
				PsTerminateSystemThread(STATUS_SUCCESS);
			}
			if (ReLoadNtos(PDriverObject,RetAddress) == STATUS_SUCCESS)
			{
				InitControl();

				if (bKernelBooting)
				{
					//---------------------------------------
					//demo，深度服务扫描隐藏
					//---------------------------------------
					DepthServicesRegistry = (PSERVICESREGISTRY)ExAllocatePool(NonPagedPool,(sizeof(SERVICESREGISTRY)+sizeof(SERVICESREGISTRY_INFORMATION))*1024);
					if (DepthServicesRegistry)
					{
						memset(DepthServicesRegistry,0,(sizeof(SERVICESREGISTRY)+sizeof(SERVICESREGISTRY_INFORMATION))*1024);
						status = QueryServicesRegistry(DepthServicesRegistry);
						if (status == STATUS_SUCCESS)
						{
							Safe_CreateValueKey(
								L"\\Registry\\Machine\\SYSTEM\\CurrentControlSet\\Control\\3600safe",
								REG_SZ,
								L"QueryServicesRegistry",
								L"success"
								);
						}
					}
					bKernelBooting = FALSE;
				}
				PsTerminateSystemThread(STATUS_SUCCESS);

			}else
			{
				//不是随机启动，就退出线程
				if (!bKernelBooting)
					PsTerminateSystemThread(STATUS_SUCCESS);
			}
		}
		else
		{
			bKernelBooting = TRUE;
		}
		WaitMicroSecond(1000);
	}
}
NTSTATUS DriverEntry( IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING theRegistryPath )
{
	ULONG ulSize;
	ULONG ulKeServiceDescriptorTable;
	int i = 0;
	PEPROCESS EProcess;
	HANDLE HThreadHandle;
	HANDLE ThreadHandle;

	DriverObject->DriverUnload = DriverUnload;

	PDriverObject = DriverObject;
	RetAddress=*(DWORD*)((DWORD)&DriverObject-4);

	ulMyDriverBase = DriverObject->DriverStart;
	ulMyDriverSize = DriverObject->DriverSize;

	DebugOn = FALSE;  //开启调式信息

	KdPrint(("//---------------------------------------//\r\n"
	       	"//  A-Protect Anti-Rootkit Kernel Module  //\r\n"
			"//      Kernel Module Version 0.2.5      //\r\n"
		     "//  website:http://www.3600safe.com     //\r\n"
	         "//---------------------------------------//\r\n"));


	SystemEProcess = PsGetCurrentProcess();

	WinVersion = GetWindowsVersion();  //初始化系统版本
	if (WinVersion)
		KdPrint(("Init Windows version Success\r\n"));

	DepthServicesRegistry = NULL;
	//-----------------------------------------
	//注册一个系统回调
	if (PsCreateSystemThread(
		&HThreadHandle,
		0,
		NULL,
		NULL,
		NULL,
		IsKernelBooting,
		NULL) == STATUS_SUCCESS)
	{
		ZwClose(HThreadHandle);
	}
	return STATUS_SUCCESS;
}