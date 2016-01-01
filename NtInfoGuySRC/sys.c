/*
通用内核代码
*/
#include "sys.h"
#include "sysext.h"

UNICODE_STRING DevName,SymlnkName;

DDKAPI NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject,\
	PUNICODE_STRING pRegistryPath) __attribute__((section("INIT")));
	
DDKAPI NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject,\
	PUNICODE_STRING pRegistryPath)
{	
	PDEVICE_OBJECT pDevObj;
	NTSTATUS status = STATUS_DEVICE_CONFIGURATION_ERROR;
	
	PRINT("[%s]enter DriverEntry...\n",__func__);
	
	RtlInitUnicodeString(&DevName,DEVNAMEW);
	RtlInitUnicodeString(&SymlnkName,SYMLNKNAMEW);
	
	if(IoCreateDevice(pDriverObject,0,&DevName,FILE_DEVICE_UNKNOWN,\
		0,false,&pDevObj) != STATUS_SUCCESS)
	{
		PRINT("[%s]IoCreateDevice Failed!\n",__func__);
		goto QUIT;
	}
	
	if(IoCreateSymbolicLink(&SymlnkName,&DevName) != STATUS_SUCCESS)
	{
		IoDeleteDevice(pDevObj);
		PRINT("[%s]IoCreateSymbolicLink Failed!\n",\
			__func__);
		goto QUIT;
	}
	
	pDevObj->Flags |= DO_BUFFERED_IO;
		
	pDriverObject->DriverUnload = DriverUnload;
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = \
		DrvDispatchCreateClose;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = \
		DrvDispatchCreateClose;
	pDriverObject->MajorFunction[IRP_MJ_READ] = \
		DispatchRead;
	pDriverObject->MajorFunction[IRP_MJ_WRITE] = \
		DispatchWrite;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = \
		DrvDispatchControl;
	pDriverObject->MajorFunction[IRP_MJ_SHUTDOWN] = \
		DrvShutdown;
	if(!NT_SUCCESS(IoRegisterShutdownNotification(\
		pDriverObject->DeviceObject)))
	{
		PRINT("[%s]err : Register Shutdown Failed!\n",\
			__func__);
	}
		
	DbgPrint("[%s]g_ddk_val is %08x\n",__func__,g_ddk_val);
	if(!DDKHello())
		DbgPrint("[%s]err : DDKHello Failed!\n",__func__);
	
	sysext_init();
		
	PRINT("[%s]leave DriverEntry...\n",__func__);
	
	status = STATUS_SUCCESS;
QUIT:
	return status;
}

DDKAPI NTSTATUS DispatchRead(IN PDEVICE_OBJECT pDriverObject,\
	IN PIRP pIrp)
{
	PRINT("[Rs]enter DispatchRead...\n",__func__);
	NTSTATUS status = STATUS_SUCCESS;
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(pIrp);
	PVOID buf = pIrp->AssociatedIrp.SystemBuffer;
	size_t length = stack->Parameters.Read.Length;
	PRINT("[%s]SystemBuf is %p , Read Length is %d\n",\
		__func__,buf,length);
	RtlFillMemory(buf,length,'?');
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = length;
	IoCompleteRequest(pIrp,IO_NO_INCREMENT);
	PRINT("[%s]leave DispatchRead...\n",__func__);
	return status;
}

DDKAPI NTSTATUS DispatchWrite(IN PDEVICE_OBJECT pDriverObject,\
	IN PIRP pIrp)
{
	NTSTATUS status = STATUS_SUCCESS;
	PRINT("[%s]enter DispatchWrite...\n",__func__);
	
	/*
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(pIrp);
	PVOID buf = pIrp->AssociatedIrp.SystemBuffer;
	size_t length = stack->Parameters.Write.Length;
	PRINT("[%s]SystemBuf is %p , Write Length is %d\n",\
		__func__,buf,length);
	*/
	
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0;			//length;
	IoCompleteRequest(pIrp,IO_NO_INCREMENT);
	PRINT("[%s]leave DispatchWrite...\n",__func__);
	return status;
}

DDKAPI NTSTATUS DrvShutdown(IN PDEVICE_OBJECT pDriverObject,\
	IN PIRP pIrp)
{
	PRINT("[%s]enter DriverShutdown...\n",__func__);
	
	PRINT("[%s]leave DriverShutdown...\n",__func__);
	return STATUS_SUCCESS;
}

DDKAPI void DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	//__asm("int $3");			//A
	
	PRINT("[%s]enter DriverUnload...\n",__func__);
	
	if(IoDeleteSymbolicLink(&SymlnkName) != STATUS_SUCCESS)
	{
		PRINT("[%s]IoDeleteSymbolicLink Failed!\n",\
			__func__);
	}
	
	IoUnregisterShutdownNotification(pDriverObject->DeviceObject);
	
	IoDeleteDevice(pDriverObject->DeviceObject);
	
	PRINT("[%s]leave DriverUnload...\n",__func__);
	//__asm("xor %eax,%eax");	//must add this if A is set!!!
}

DDKAPI NTSTATUS DrvDispatchCreateClose(PDEVICE_OBJECT pDeviceObject,\
	PIRP pIrp)
{
	PRINT("[%s]enter DrvDispatchCreateClose...\n",\
		__func__);
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(pIrp);
	UCHAR type = stack->MajorFunction;
	switch(type)
	{
	case IRP_MJ_CREATE:
		PRINT("[%s]Device Create!\n",__func__);
		break;
	case IRP_MJ_CLOSE:
		PRINT("[%s]Device Close!\n",__func__);
		break;
	default:
		PRINT("[%s]err : Unkown MajorFunction!\n",__func__);
		break;
	}
	
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;
	
	IofCompleteRequest(pIrp,IO_NO_INCREMENT);
	
	PRINT("[%s]leave DrvDispatchCreateClose...\n",\
		__func__);
	
	return STATUS_SUCCESS;
}

DDKAPI NTSTATUS DrvDispatchControl(PDEVICE_OBJECT pDeviceObject,\
	PIRP pIrp)
{
	NTSTATUS  status = STATUS_INVALID_DEVICE_REQUEST;
	DWORD RetBytes = 0;
	PIO_STACK_LOCATION pIoStackLoc = IoGetCurrentIrpStackLocation(pIrp);
	DWORD cbout = \
		pIoStackLoc->Parameters.DeviceIoControl.OutputBufferLength;
	//DWORD cbin = pIoStackLoc->Parameters.DeviceIoControl.InputBufferLength;
	PVOID buf = (PVOID)pIrp->AssociatedIrp.SystemBuffer;
	
	PRINT("[%s]enter DrvDispatchControl...\n",\
		__func__);
	
	ULONG IoCtrlCode = pIoStackLoc->Parameters.DeviceIoControl.IoControlCode;
	if(IoCtrlCode == IOCTL_CTL_GET_SSDT)
	{
		PRINT("[%s]Into IOCTL_CTL_GET_SSDT Path!\n",\
			__func__);
		PRINT("[%s]buf at %p , size is %d\n",__func__,buf,cbout);
		RetBytes = HyGetSSDTCore(buf,cbout);
		if(!RetBytes)
			PRINT("[%s]err : CoreHyGetStdTable Failed!\n",__func__);
		else
			status = STATUS_SUCCESS;
	}
	else if(IoCtrlCode == IOCTL_CTL_GET_SSDT_SDW)
	{
		PRINT("[%s]Into IOCTL_CTL_GET_SSDT_SDW Path!\n",\
			__func__);
		PRINT("[%s]TableShadow at %p,buf at %p , size is %d\n",\
			__func__,*(PULONG)buf,buf,cbout);
		RetBytes = HyGetSSDTSdwCore(*(PVOID*)buf,buf,cbout);
		if(!RetBytes)
			PRINT("[%s]err : CoreHyGetStdTable Failed!\n",__func__);
		else
			status = STATUS_SUCCESS;
	}
	else if(IoCtrlCode == IOCTL_CTL_GET_TABLE_BASE)
	{
		PRINT("[%s]Into IOCTL_CTL_GET_TABLE_BASE Path!\n",\
			__func__);
		if(!HyGetSSDTBasesCore(buf,cbout))
			PRINT("[%s]err : HyGetSSDTBasesCore Failed!\n",__func__);
		else
		{
			RetBytes = sizeof(PVOID) * 2;
			status = STATUS_SUCCESS;
		}
	}
	else if(IoCtrlCode == IOCTL_CTL_GET_IDT)
	{
		PRINT("[%s]Into IOCTL_CTL_GET_IDT Path!\n",\
			__func__);
		RetBytes = HyGetIDTCore(buf,cbout);
		if(!RetBytes)
			PRINT("[%s]err : HyGetIDTCore Failed!\n",__func__);
		else
			status = STATUS_SUCCESS;
	}
	else if(IoCtrlCode == IOCTL_CTL_GET_GDT)
	{
		PRINT("[%s]Into IOCTL_CTL_GET_GDT Path!\n",\
			__func__);
		RetBytes = HyGetGDTCore(buf,cbout);
		if(!RetBytes)
			PRINT("[%s]err : HyGetGDTCore Failed!\n",__func__);
		else
			status = STATUS_SUCCESS;
	}
	else if(IoCtrlCode == IOCTL_CTL_ENUM_SYSMODLIST)
	{
		PRINT("[%s]Into IOCTL_CTL_ENUM_SYSMODLIST Path!\n",\
			__func__);
		PRINT("[%s]msg : SysModListAddr is %p\n",__func__,\
			*(PVOID*)buf);
		HyEnumSysModCore(*(PVOID*)buf);
		status = STATUS_SUCCESS;
	}
	else if(IoCtrlCode == IOCTL_CTL_GET_SYSVAL)
	{
		PRINT("[%s]Into IOCTL_CTL_GET_SYSVAL Path!\n",\
			__func__);
		if(!HyGetSysValCore(*(PVOID*)buf,buf,cbout))
			PRINT("[%s]err : HyGetSysValCore Failed!\n",\
				__func__);
		else
			status = STATUS_SUCCESS;
	}
	else
	{
		PRINT("[%s]error : Unknow IoCtrlCode!\n",\
			__func__);
	}

	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = RetBytes;
	
	IofCompleteRequest(pIrp,IO_NO_INCREMENT);
		
	PRINT("[%s]leave DrvDispatchControl...\n",\
		__func__);
	
	return status;
}
