#include "ntifs.h"
#include "windef.h"
#define DEVICE_NAME L"\\Device\\explorer"
#define DOS_NAME    L"\\DosDevices\\explorer"
ULONG  Num=0;
KSPIN_LOCK SDTSpinLock;
void WPOFF();
VOID WPON();
ULONG g_uCr0 = 0;
ULONG  addrofKeRaiseIrqlToDpcLevel=0;
ULONG  addrofIofCompleteRequest=0;
#define IOCTL_MT_GETDIRINF   \
CTL_CODE(FILE_DEVICE_DISK_FILE_SYSTEM, 0x201, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_MT_GETDIRNUMINF   \
CTL_CODE(FILE_DEVICE_DISK_FILE_SYSTEM, 0x202, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_MT_GETIOCOMPLETEREQUESTADDR   \
CTL_CODE(FILE_DEVICE_DISK_FILE_SYSTEM, 0x203, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_MT_GETKERAISEIRQLADDR   \
CTL_CODE(FILE_DEVICE_DISK_FILE_SYSTEM, 0x204, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_MT_REKERAISEIRQL   \
CTL_CODE(FILE_DEVICE_DISK_FILE_SYSTEM, 0x205, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_MT_REIOCOMPLETEREQUEST   \
CTL_CODE(FILE_DEVICE_DISK_FILE_SYSTEM, 0x206, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_MT_KILLFILE   \
CTL_CODE(FILE_DEVICE_DISK_FILE_SYSTEM, 0x207, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
typedef struct _DIRECTORY_INFO{
	char FileName[50];      
	   LARGE_INTEGER  AllocationSize;
       TIME_FIELDS    CreationTime;
	   TIME_FIELDS    LastAccessTime ; 
	   TIME_FIELDS             LastWriteTime;  
	   TIME_FIELDS			   ChangeTime; 	
	   ULONG     FileAttributes ;    
}DIRECTORY_INFO ,*PDIRECTORY_INFO ;
typedef struct _DELETE_INFO{
	BOOLEAN  IsDelete;
}DELETE_INFO,*PDELETE_INFO;
typedef struct _IOFCOM_CODE
{
	unsigned int  a;
	ULONG   b;
	BYTE   c;
}IOFCOM_CODE,*PIOFCOM_CODE;
PVOID GetDirectory(char *lpDirName,PULONG dwRetSize);
NTSTATUS
MyIoControl(
			IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp
			);
NTSTATUS
MydrvDispatch (
			   IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp
			   );
NTSTATUS
FORCEINLINE
MyIoCallDriver(
			   IN PDEVICE_OBJECT DeviceObject,
			   IN OUT PIRP Irp
			   );
BOOLEAN
SKillDeleteFile(
				IN HANDLE    FileHandle
				);
BOOLEAN
SKillStripFileAttributes(
						 IN HANDLE    FileHandle
						 );
NTSTATUS
SkillSetFileCompletion(
					   IN PDEVICE_OBJECT DeviceObject,
					   IN PIRP Irp,
					   IN PVOID Context
					   );
HANDLE
SkillIoOpenFile(
				IN PCSTR FileName,
				IN ACCESS_MASK DesiredAccess,
				IN ULONG ShareAccess
				);
BOOLEAN
DeleteFile(
           IN   char   *name
           );
void WPOFF()
{
    
    ULONG uAttr;
    
    _asm
    {
        push eax;
        mov eax, cr0;
        mov uAttr, eax;
        and eax, 0FFFEFFFFh; // CR0 16 BIT = 0
        mov cr0, eax;
        pop eax;
        cli
    };
    
    g_uCr0 = uAttr; //保存原有的 CRO 屬性
    
}

VOID WPON()
{
    
    _asm
    {
        sti
            push eax;
        mov eax, g_uCr0; //恢復原有 CR0 屬性
        mov cr0, eax;
        pop eax;
    };
    
}

void Unload(IN PDRIVER_OBJECT DriverObject)
{
	PDEVICE_OBJECT deviceObject = DriverObject->DeviceObject;
    UNICODE_STRING uniWin32NameString;
    RtlInitUnicodeString( &uniWin32NameString, DOS_NAME );  
    IoDeleteSymbolicLink( &uniWin32NameString );   
    if ( deviceObject != NULL )
    {
        IoDeleteDevice( deviceObject );
    }
	return;
}
NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject , IN PUNICODE_STRING RegistryPath)
{
	UNICODE_STRING  nameString, linkString;
	NTSTATUS  status=STATUS_SUCCESS;
	ULONG     i;
	PDEVICE_OBJECT      deviceObject;
	RtlInitUnicodeString(&nameString,DEVICE_NAME);
	RtlInitUnicodeString(&linkString,DOS_NAME);
	status = IoCreateDevice( DriverObject,0,&nameString,FILE_DEVICE_UNKNOWN,0,TRUE,&deviceObject);	
	if (!NT_SUCCESS( status ))
		return status;
	status = IoCreateSymbolicLink (&linkString, &nameString);
	if (!NT_SUCCESS( status ))
	{
		IoDeleteDevice (DriverObject->DeviceObject);
		return status;
	}   
	for ( i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		DriverObject->MajorFunction[i] = MydrvDispatch;
	}
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = MyIoControl;
	DriverObject->DriverUnload=Unload;
	return status;
}
NTSTATUS
MydrvDispatch (
			   IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp
			   )
{
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0L;
	IoCompleteRequest( Irp, 0 );
	return Irp->IoStatus.Status;
}
NTSTATUS
MyIoControl(
			IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp
			)
{
	NTSTATUS  status=STATUS_SUCCESS;
	PIO_STACK_LOCATION irpStack; 
	PVOID ioBuffer; 
	PCHAR   ioDirName;
	ULONG inputBufferLength; 
	ULONG outputBufferLength;		
	ULONG ioControlCode;
	ULONG  dwRetSize;
	PVOID  pDir;
	DELETE_INFO   delete_info;
	UNICODE_STRING   name;
	IOFCOM_CODE   x;
	ULONG    code;
	PUCHAR KeRaisecode;
	KIRQL OldIrql ;
	BYTE a[2]={0xff,0x25};
	BYTE b={0xcc};
	irpStack = IoGetCurrentIrpStackLocation(Irp); 
	ioControlCode= irpStack->Parameters.DeviceIoControl.IoControlCode; 
	ioBuffer = Irp->AssociatedIrp.SystemBuffer; 
	inputBufferLength = irpStack->Parameters.DeviceIoControl.InputBufferLength; 
	outputBufferLength = irpStack->Parameters.DeviceIoControl.OutputBufferLength;
	switch (ioControlCode)
	{
	case IOCTL_MT_GETDIRINF:
		{
			ioDirName=ioBuffer;
			KdPrint(("%s\n",ioDirName));
			pDir=GetDirectory(ioDirName,&dwRetSize);
			//Num=dwRetSize/sizeof(DIRECTORY_INFO);
			//RtlCopyMemory(ioBuffer,&Num,sizeof(ULONG));
			RtlCopyMemory(ioBuffer,pDir,dwRetSize);
			Num=0;
			if(pDir!=NULL)
			{
                ExFreePool(pDir);
			}
			break;
		}
	case IOCTL_MT_GETDIRNUMINF:
		{
			ioDirName=ioBuffer;
			KdPrint(("%s\n",ioDirName));
			pDir=GetDirectory(ioDirName,&dwRetSize);
			Num=dwRetSize/sizeof(DIRECTORY_INFO);
			RtlCopyMemory(ioBuffer,&Num,sizeof(ULONG));
			//RtlCopyMemory(ioBuffer,pDir,dwRetSize);
			if(pDir!=NULL)
			{
                ExFreePool(pDir);
			}
			break;
		}
	case IOCTL_MT_GETIOCOMPLETEREQUESTADDR:
		{
			RtlInitUnicodeString(&name,L"IofCompleteRequest");
			addrofIofCompleteRequest=(ULONG)MmGetSystemRoutineAddress(&name);
			RtlCopyMemory(ioBuffer,&addrofIofCompleteRequest,sizeof(ULONG));
			break;
		}
	case IOCTL_MT_GETKERAISEIRQLADDR:
		{
			RtlInitUnicodeString(&name,L"KeRaiseIrqlToDpcLevel");
			addrofKeRaiseIrqlToDpcLevel=(ULONG)MmGetSystemRoutineAddress(&name);
			RtlCopyMemory(ioBuffer,&addrofKeRaiseIrqlToDpcLevel,sizeof(ULONG));
			break;
		}
	case IOCTL_MT_REKERAISEIRQL:
		{
			KeRaisecode=(PUCHAR)ioBuffer;
			KeAcquireSpinLock( &SDTSpinLock, &OldIrql );
			WPOFF();
			RtlCopyMemory((PVOID)addrofKeRaiseIrqlToDpcLevel,KeRaisecode,6);
			WPON();
			KeReleaseSpinLock( &SDTSpinLock, OldIrql );
			break;
		}
	case IOCTL_MT_REIOCOMPLETEREQUEST:
		{
			code=*(PULONG)ioBuffer;
			KeAcquireSpinLock( &SDTSpinLock, &OldIrql );
			WPOFF();
			RtlCopyMemory((PVOID)addrofIofCompleteRequest,a,2);
			RtlCopyMemory((PVOID)(addrofIofCompleteRequest+2),&code,4);
            RtlCopyMemory((PVOID)(addrofIofCompleteRequest+6),&b,1);
            WPON();
			KeReleaseSpinLock( &SDTSpinLock, OldIrql );
			break;
		}
	case  IOCTL_MT_KILLFILE:
		{
			delete_info.IsDelete=DeleteFile(ioBuffer);
			//if(DeleteFile(ioBuffer))
			RtlCopyMemory(ioBuffer,&delete_info,sizeof(delete_info));
			break;
		}
	}
    Irp->IoStatus.Status = status; 
	Irp->IoStatus.Information = outputBufferLength; 
	IoCompleteRequest(Irp, IO_NO_INCREMENT); 
	return    status;
			}
NTSTATUS 
EventCompletion( 
				IN PDEVICE_OBJECT DeviceObject, 
				IN PIRP Irp, 
				IN PVOID Context 
				) 
{ 
	PIO_STATUS_BLOCK lpiosb; 
	lpiosb = Irp->UserIosb; 
	lpiosb->Status = Irp->IoStatus.Status; 
	lpiosb->Information = Irp->IoStatus.Information; 
	KeSetEvent(Irp->UserEvent,0,FALSE); 
	IoFreeIrp(Irp); 
	return STATUS_MORE_PROCESSING_REQUIRED; 
} 
PDEVICE_OBJECT
MyIoGetRelatedDeviceObject(
						   IN PFILE_OBJECT FileObject
						   )
{
    PDEVICE_OBJECT deviceObject;
    if (FileObject->Vpb != NULL && FileObject->Vpb->DeviceObject != NULL&&(!(FileObject->Flags & FO_DIRECT_DEVICE_OPEN))) {
        deviceObject = FileObject->Vpb->DeviceObject;
    } else if (!(FileObject->Flags & FO_DIRECT_DEVICE_OPEN) &&
		FileObject->DeviceObject->Vpb != NULL &&
		FileObject->DeviceObject->Vpb->DeviceObject != NULL) {
		deviceObject = FileObject->DeviceObject->Vpb->DeviceObject;
    } else {
        deviceObject = FileObject->DeviceObject;
    }
    if (deviceObject->AttachedDevice != NULL) {
        deviceObject = IoGetAttachedDevice( deviceObject );
    }
    return deviceObject;
}
PVOID GetDirectory(char *lpDirName,PULONG dwRetSize) 
{
	NTSTATUS status; 
	ULONG dwBytesReturned; 
	OBJECT_ATTRIBUTES oa; 
	PDEVICE_OBJECT lpDeviceObject; 
	KEVENT event; 
	IO_STACK_LOCATION iost; 
	PIO_STACK_LOCATION lpsp; 
	IO_STATUS_BLOCK ios; 
	PIRP lpirp = NULL; 
	HANDLE hFile; 
	PVOID lpSystemBuffer; 
	PFILE_DIRECTORY_INFORMATION lpInformation; 
	PFILE_DIRECTORY_INFORMATION lpRealInformation; 
	PDIRECTORY_INFO lpDirInfo; 
	PFILE_OBJECT lpFileObject; 
	UNICODE_STRING unFileName; 
	UNICODE_STRING UN; 
	ANSI_STRING anFileName; 
	CHAR buffer[1024]; 
	PUCHAR lpNext; 
	dwBytesReturned = 0; 
	status = STATUS_UNSUCCESSFUL; 
	RtlZeroMemory(buffer,1024); 
	strcpy(buffer,"\\DosDevices\\"); 
	strcat(buffer,lpDirName); 
	RtlInitAnsiString(&anFileName,buffer); 
	RtlAnsiStringToUnicodeString(&unFileName,&anFileName,TRUE); 
	InitializeObjectAttributes(&oa,&unFileName,OBJ_CASE_INSENSITIVE + OBJ_KERNEL_HANDLE,NULL,NULL); 
	status = ZwOpenFile(&hFile,FILE_LIST_DIRECTORY + SYNCHRONIZE+FILE_ANY_ACCESS,&oa,&ios,FILE_SHARE_READ + FILE_SHARE_WRITE + FILE_SHARE_DELETE,FILE_DIRECTORY_FILE + FILE_SYNCHRONOUS_IO_NONALERT); 
	if(NT_SUCCESS(status)) 
	{ 
		DbgPrint("ZwOpenFile Success\n"); 
	}
	else 
		goto endcddir; 
	status =ObReferenceObjectByHandle(hFile,FILE_LIST_DIRECTORY + SYNCHRONIZE,0,KernelMode,&lpFileObject,NULL); 
	if(!NT_SUCCESS(status)) 
	{ 
		ZwClose(hFile); 
		goto endcddir; 
	} 
	DbgPrint("open file object success\n"); 
	lpDeviceObject = MyIoGetRelatedDeviceObject(lpFileObject); 
	lpirp = IoAllocateIrp(lpDeviceObject->StackSize,FALSE); 
	if(!lpirp) 
	{ 
		DbgPrint("allocate irp failed\n"); 
		ObDereferenceObject(lpFileObject); 
		ZwClose(hFile); 
		goto endcddir; 
	} 
	DbgPrint("allocate irp success\n"); 
	KeInitializeEvent(&event,SynchronizationEvent,FALSE); 
	lpInformation = ExAllocatePool(PagedPool,655350); 
	lpSystemBuffer = ExAllocatePool(PagedPool,655350); 
	RtlZeroMemory(lpSystemBuffer,655350); 
	RtlZeroMemory(lpInformation,655350); 
	lpirp->UserEvent = &event; 
	lpirp->UserBuffer = lpInformation; 
	lpirp->AssociatedIrp.SystemBuffer = lpInformation; 
	lpirp->MdlAddress = NULL; 
	lpirp->Flags = 0; 
	lpirp->UserIosb = &ios; 
	lpirp->Tail.Overlay.OriginalFileObject = lpFileObject; 
	lpirp->Tail.Overlay.Thread = PsGetCurrentThread(); 
	lpirp->RequestorMode = KernelMode; 
	lpsp = IoGetNextIrpStackLocation(lpirp); 
	lpsp->MajorFunction = IRP_MJ_DIRECTORY_CONTROL; 
	lpsp->MinorFunction = IRP_MN_QUERY_DIRECTORY; 
	lpsp->FileObject = lpFileObject; 
	lpsp->DeviceObject = lpDeviceObject; 
	lpsp->Flags = SL_RESTART_SCAN; 
	lpsp->Control = 0; 
	lpsp->Parameters.QueryDirectory.FileIndex = 0; 
	lpsp->Parameters.QueryDirectory.FileInformationClass = FileDirectoryInformation; 
	lpsp->Parameters.QueryDirectory.FileName = NULL; 
	lpsp->Parameters.QueryDirectory.Length = 655350; 
	IoSetCompletionRoutine(lpirp,EventCompletion,0,TRUE,TRUE,TRUE); 
	status = MyIoCallDriver(lpDeviceObject,lpirp); 
	KeWaitForSingleObject(&event,Executive,KernelMode,TRUE,0); 
	lpDirInfo = (PDIRECTORY_INFO)lpSystemBuffer; 
	lpRealInformation = lpInformation; 
	while(1) 
	{ 
		UN.Length = (USHORT)lpInformation->FileNameLength; 
		UN.MaximumLength = (USHORT)lpInformation->FileNameLength; 
		UN.Buffer = &(lpInformation->FileName[0]); 
		RtlUnicodeStringToAnsiString(&anFileName,&UN,TRUE); 
		strcpy(lpDirInfo->FileName,anFileName.Buffer); 
		KdPrint(("%s\n",anFileName.Buffer));
		RtlFreeAnsiString(&anFileName); 
		lpDirInfo->AllocationSize=lpInformation->AllocationSize;
		lpDirInfo->FileAttributes=lpInformation->FileAttributes;
		RtlTimeToTimeFields(&(lpInformation->CreationTime),&(lpDirInfo->CreationTime)); 
		RtlTimeToTimeFields(&(lpInformation->LastAccessTime),&(lpDirInfo->LastAccessTime));  
		RtlTimeToTimeFields(&(lpInformation->LastWriteTime),&(lpDirInfo->LastWriteTime));  
		RtlTimeToTimeFields(&(lpInformation->ChangeTime),&(lpDirInfo->ChangeTime));  
		lpDirInfo->FileAttributes = lpInformation->FileAttributes; 
		dwBytesReturned+=sizeof(DIRECTORY_INFO); 
		if(!lpInformation->NextEntryOffset) goto exit; 
		lpNext = (PUCHAR)lpInformation; 
		lpNext+=lpInformation->NextEntryOffset; 
		lpInformation = (PFILE_DIRECTORY_INFORMATION)(lpNext);  
		lpDirInfo++; 
	} 
endcddir: 
	RtlFreeUnicodeString(&unFileName); 
	return NULL; 
exit: 
	ExFreePool(lpRealInformation); 
	ObDereferenceObject(lpFileObject); 
	ZwClose(hFile); 
	RtlFreeUnicodeString(&unFileName); 
	*dwRetSize = dwBytesReturned; 
	return lpSystemBuffer;               
}
NTSTATUS
FORCEINLINE
MyIoCallDriver(
			   IN PDEVICE_OBJECT DeviceObject,
			   IN OUT PIRP Irp
			   )
{
    PIO_STACK_LOCATION irpSp;
    PDRIVER_OBJECT driverObject;
    NTSTATUS status;
    ASSERT( Irp->Type == IO_TYPE_IRP );
    Irp->CurrentLocation--;
    if (Irp->CurrentLocation <= 0) {
        KeBugCheckEx( NO_MORE_IRP_STACK_LOCATIONS, (ULONG_PTR) Irp, 0, 0 ,0);
    }
    irpSp = IoGetNextIrpStackLocation( Irp );
    Irp->Tail.Overlay.CurrentStackLocation = irpSp;
    irpSp->DeviceObject = DeviceObject;
    driverObject = DeviceObject->DriverObject;
    status = driverObject->MajorFunction[irpSp->MajorFunction]( DeviceObject,
		Irp );
    return status;
}  
HANDLE
SkillIoOpenFile(
				IN PCSTR FileName,
				IN ACCESS_MASK DesiredAccess,
				IN ULONG ShareAccess
				)
{
	NTSTATUS              ntStatus;
	UNICODE_STRING        uniFileName;
	OBJECT_ATTRIBUTES     objectAttributes;
	HANDLE                ntFileHandle;
	IO_STATUS_BLOCK       ioStatus;
	ANSI_STRING           anFileName; 
	CHAR                  buffer[256]; 
	if (KeGetCurrentIrql() > PASSIVE_LEVEL)
	{
		return 0;
	}
	RtlZeroMemory(buffer,256); 
	strcpy(buffer,"\\DosDevices\\"); 
	strcat(buffer,FileName); 
	RtlInitAnsiString(&anFileName,buffer);
	RtlAnsiStringToUnicodeString(&uniFileName,&anFileName,TRUE); 
	KdPrint(("%wZ\n",&uniFileName));
	InitializeObjectAttributes(&objectAttributes, &uniFileName,
		OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, NULL);
	ntStatus = IoCreateFile(&ntFileHandle,
		DesiredAccess,
		&objectAttributes,
		&ioStatus,
		0,
		FILE_ATTRIBUTE_NORMAL,
		ShareAccess,
		FILE_OPEN,
		0,
		NULL,
		0,
		0,
		NULL,
		IO_NO_PARAMETER_CHECKING);
	if (!NT_SUCCESS(ntStatus))
	{
		return 0;
	}
	
	return ntFileHandle;
}
BOOLEAN
DeleteFile(
           IN   char   *name
           )
{
	   HANDLE    hFileHandle ;
	   hFileHandle = SkillIoOpenFile(name, 
		   FILE_READ_ATTRIBUTES,
		   FILE_SHARE_DELETE);
	   if (hFileHandle!=NULL)
	   {
		   KdPrint(("ok"));
		   if(SKillDeleteFile(hFileHandle))
		   {
			   ZwClose(hFileHandle);
			   return TRUE;
		   }
		   ZwClose(hFileHandle);
	   } 
	   return FALSE;
}   
NTSTATUS
SkillSetFileCompletion(
					   IN PDEVICE_OBJECT DeviceObject,
					   IN PIRP Irp,
					   IN PVOID Context
					   )
{
	Irp->UserIosb->Status = Irp->IoStatus.Status;
	Irp->UserIosb->Information = Irp->IoStatus.Information;
	KeSetEvent(Irp->UserEvent, IO_NO_INCREMENT, FALSE);
	IoFreeIrp(Irp);
	return STATUS_MORE_PROCESSING_REQUIRED;
}

BOOLEAN
SKillStripFileAttributes(
						 IN HANDLE    FileHandle
						 )
{
	NTSTATUS          ntStatus = STATUS_SUCCESS;
	PFILE_OBJECT      fileObject;
	PDEVICE_OBJECT    DeviceObject;
	PIRP              Irp;
	KEVENT            event;
	FILE_BASIC_INFORMATION    FileInformation;
	IO_STATUS_BLOCK ioStatus;
	PIO_STACK_LOCATION irpSp;
	
	ntStatus = ObReferenceObjectByHandle(FileHandle,
		DELETE,
		*IoFileObjectType,
		KernelMode,
		&fileObject,
		NULL);
	
	if (!NT_SUCCESS(ntStatus))
	{
		return FALSE;
	}
	
	DeviceObject = IoGetRelatedDeviceObject(fileObject);
	Irp = IoAllocateIrp(DeviceObject->StackSize, TRUE);
	
	if (Irp == NULL)
	{
		ObDereferenceObject(fileObject);
		return FALSE;
	}
	KeInitializeEvent(&event, SynchronizationEvent, FALSE);
	memset(&FileInformation,0,0x28);
	FileInformation.FileAttributes = FILE_ATTRIBUTE_NORMAL;
	Irp->AssociatedIrp.SystemBuffer = &FileInformation;
	Irp->UserEvent = &event;
	Irp->UserIosb = &ioStatus;
	Irp->Tail.Overlay.OriginalFileObject = fileObject;
	Irp->Tail.Overlay.Thread = (PETHREAD)KeGetCurrentThread();
	Irp->RequestorMode = KernelMode;
	irpSp = IoGetNextIrpStackLocation(Irp);
	irpSp->MajorFunction = IRP_MJ_SET_INFORMATION;
	irpSp->DeviceObject = DeviceObject;
	irpSp->FileObject = fileObject;
	irpSp->Parameters.SetFile.Length = sizeof(FILE_BASIC_INFORMATION);
	irpSp->Parameters.SetFile.FileInformationClass = FileBasicInformation;
	irpSp->Parameters.SetFile.FileObject = fileObject;
	IoSetCompletionRoutine(
		Irp,
		SkillSetFileCompletion,
		&event,
		TRUE,
		TRUE,
		TRUE);
	IoCallDriver(DeviceObject, Irp);
	KeWaitForSingleObject(&event, Executive, KernelMode, TRUE, NULL);
	ObDereferenceObject(fileObject);
	return TRUE;
}
BOOLEAN
SKillDeleteFile(
				IN HANDLE    FileHandle
				)
{
	NTSTATUS          ntStatus = STATUS_SUCCESS;
	PFILE_OBJECT      fileObject;
	PDEVICE_OBJECT    DeviceObject;
	PIRP              Irp;
	KEVENT            event;
	FILE_DISPOSITION_INFORMATION    FileInformation;
	IO_STATUS_BLOCK ioStatus;
	PIO_STACK_LOCATION irpSp;
	PSECTION_OBJECT_POINTERS pSectionObjectPointer;     
	SKillStripFileAttributes( FileHandle);          //去掉只读属性，才能删除只读文件
	ntStatus = ObReferenceObjectByHandle(FileHandle,
		DELETE,
		*IoFileObjectType,
		KernelMode,
		&fileObject,
		NULL);
	if (!NT_SUCCESS(ntStatus))
	{
		return FALSE;
	}
	DeviceObject = MyIoGetRelatedDeviceObject(fileObject);
	Irp = IoAllocateIrp(DeviceObject->StackSize, TRUE);
	if (Irp == NULL)
	{
		ObDereferenceObject(fileObject);
		return FALSE;
	}
	KeInitializeEvent(&event, SynchronizationEvent, FALSE);
	FileInformation.DeleteFile = TRUE;
	Irp->AssociatedIrp.SystemBuffer = &FileInformation;
	Irp->UserEvent = &event;
	Irp->UserIosb = &ioStatus;
	Irp->Tail.Overlay.OriginalFileObject = fileObject;
	Irp->Tail.Overlay.Thread = (PETHREAD)KeGetCurrentThread();
	Irp->RequestorMode = KernelMode;
	irpSp = IoGetNextIrpStackLocation(Irp);
	irpSp->MajorFunction = IRP_MJ_SET_INFORMATION;
	irpSp->DeviceObject = DeviceObject;
	irpSp->FileObject = fileObject;
	irpSp->Parameters.SetFile.Length = sizeof(FILE_DISPOSITION_INFORMATION);
	irpSp->Parameters.SetFile.FileInformationClass = FileDispositionInformation;
	irpSp->Parameters.SetFile.FileObject = fileObject;
	IoSetCompletionRoutine(
		Irp,
		SkillSetFileCompletion,
		&event,
		TRUE,
		TRUE,
		TRUE);
	pSectionObjectPointer = fileObject->SectionObjectPointer;
	pSectionObjectPointer->ImageSectionObject = 0;
	pSectionObjectPointer->DataSectionObject = 0;
	IoCallDriver(DeviceObject, Irp);
	KeWaitForSingleObject(&event, Executive, KernelMode, TRUE, NULL);
	ObDereferenceObject(fileObject);
	return TRUE;
}  