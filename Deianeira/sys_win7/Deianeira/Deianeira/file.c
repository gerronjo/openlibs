
#include "file.h"


//保存原先开头的5个自己
BYTE file_IoCreateFileOldAddress[5]={0};
//jmp新的函数地址
BYTE file_IoCreateFileNewAddress[5]={0xe9,0,0,0,0};

//保存原先开头的5个自己
BYTE file_NtDeleteFileOldAddress[5]={0};
//jmp新的函数地址
BYTE file_NtDeleteFileNewAddress[5]={0xe9,0,0,0,0};

//保存原先开头的5个自己
BYTE file_NtSetInformationFileOldAddress[5]={0};
//新函数地址
BYTE file_NtSetInformationFileNewAddress[5]={0xe9,0,0,0,0};
//jmp函数地址
BYTE file_NtSetInformationFileJmpAddress[5]={0xe9,0,0,0,0};

PNtDeleteFile NtDeleteFile = NULL;

//高层驱动使用IoAllocateIrp或IoBuildAsynchronousFsdRequest 分配/建立 IRP，
//在调用IoCallDriver时传递驱动 分配/建立 的IRP参数前必须传递所有InvokeOnXXX参数为TRUE来调用此完成例程。
//当完成例程通过这个IRP被调用时它必须释放驱动建立的IRP和任何其他相关的资源，比如：IoBuildPartialMdl分配的MDL。
//这类驱动如果调用IoFreeIrp阻止I/0 manager完成这个驱动创建的IRP将会返回STATUS_MORE_PROCESSING_REQUIRED。
//（Such a driver should return STATUS_MORE_PROCESSING_REQUIRED when it calls IoFreeIrp to forestall the I/0 manager's completion processing for the driver-allocated IRP.）
//那种在他们的完成例程执行前可能被unload的非即插即用类驱动要使用IoSetCompletionRoutineEx。
//IoSetCompletionRoutine的调用者必须运行在IRQL<=DISPATCH_LEVEL.

//出处：http://blog.csdn.net/skyair624/archive/2009/10/09/4644154.aspx

//打开文件
HANDLE file_openFile(
				IN LPTSTR lpFileName,
				IN ACCESS_MASK DesiredAccess,
				IN ULONG ShareAccess
				)
{
	NTSTATUS              status;
	UNICODE_STRING        ustrFileName;
	OBJECT_ATTRIBUTES     objectAttributes;
	HANDLE                hFile;
	IO_STATUS_BLOCK       ioStatus;

    //IoCreateFile必须运行在PASSIVE_LEVEL
	if (KeGetCurrentIrql() > PASSIVE_LEVEL)
	{
		DbgPrint("file_openFile KeGetCurrentIrql() error!");
		return NULL;
	}

	RtlInitUnicodeString(&ustrFileName, lpFileName);

	//初始化对象属性结构体
	InitializeObjectAttributes(&objectAttributes, &ustrFileName,
		OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, NULL);

	//打开文件
	status = IoCreateFile(&hFile,
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

	//如果打开失败
	if (!NT_SUCCESS(status))
	{
		DbgPrint("file_openFile IoCreateFile() error!");
		return NULL;
	}

	//返回句柄
	return hFile;
}

//完成例程函数
NTSTATUS file_completionRoutine(
							   IN PDEVICE_OBJECT DeviceObject,
							   IN PIRP pIrp,
							   IN PVOID Context
							   )
{
	pIrp->UserIosb->Status = pIrp->IoStatus.Status;
	pIrp->UserIosb->Information = pIrp->IoStatus.Information;

	//设置事件
	KeSetEvent(pIrp->UserEvent, IO_NO_INCREMENT, FALSE);

	//释放IRP
	IoFreeIrp(pIrp);

	//由于是IoFreeIrp释放，所以必须返回STATUS_MORE_PROCESSING_REQUIRED
	return STATUS_MORE_PROCESSING_REQUIRED;
}

//设置文件属性
BOOLEAN file_setFileAttributes(
							IN HANDLE hFile,
							IN ULONG FileAttributes
							)
{
	NTSTATUS          status;
	PFILE_OBJECT      pFileObject;
	PDEVICE_OBJECT    pDeviceObject;
	PIRP              pIrp;
	KEVENT            kevent;
	FILE_BASIC_INFORMATION    fileBasicInformation;
	IO_STATUS_BLOCK ioStatus;
	PIO_STACK_LOCATION irpSp;

	status = ObReferenceObjectByHandle(hFile,
		DELETE,
		*IoFileObjectType,
		KernelMode,
		&pFileObject,
		NULL);

	//如果失败
	if (!NT_SUCCESS(status))
	{
		return FALSE;
	}

	//用IoGetRelatedDeviceObject这个函数来获得一个FileObject对应的最底层的那个功能驱动对象(FDO)。
	pDeviceObject = IoGetRelatedDeviceObject(pFileObject);

	//创建IRP
	pIrp = IoAllocateIrp(pDeviceObject->StackSize, TRUE);

	//创建失败
	if (pIrp == NULL)
	{
		//引用计数减一
		ObDereferenceObject(pFileObject);
		return FALSE;
	}

	//初始化事件对象
	KeInitializeEvent(&kevent, SynchronizationEvent, FALSE);

	//内存清零
	RtlZeroMemory(&fileBasicInformation,0,sizeof(FILE_BASIC_INFORMATION));

	//设置文件属性
	//fileBasicInformation.FileAttributes = FILE_ATTRIBUTE_NORMAL;
    fileBasicInformation.FileAttributes = FileAttributes;

	//填充IRP结构
	pIrp->AssociatedIrp.SystemBuffer = &fileBasicInformation;
	pIrp->UserEvent = &kevent;
	pIrp->UserIosb = &ioStatus;
	pIrp->Tail.Overlay.OriginalFileObject = pFileObject;
	pIrp->Tail.Overlay.Thread = (PETHREAD)KeGetCurrentThread();
	pIrp->RequestorMode = KernelMode;

	//Irp堆栈
	irpSp = IoGetNextIrpStackLocation(pIrp);

	//设置文件的属性，直接通过IRP来实现，不经过相关文件操作api
	irpSp->MajorFunction = IRP_MJ_SET_INFORMATION;
	irpSp->DeviceObject = pDeviceObject;
	irpSp->FileObject = pFileObject;
	irpSp->Parameters.SetFile.Length = sizeof(FILE_BASIC_INFORMATION);
	irpSp->Parameters.SetFile.FileInformationClass = FileBasicInformation;
	irpSp->Parameters.SetFile.FileObject = pFileObject;

	//注册一个IO完成例程，这个完成例程将会在调用此函数的驱动的下一层驱动完成IRP指定的操作请求时被调用
	IoSetCompletionRoutine(
		pIrp,
		file_completionRoutine,
		&kevent,
		TRUE,
		TRUE,
		TRUE);

	//向目标设置对象，分发IRP
	IoCallDriver(pDeviceObject, pIrp);

	//等待事件的完成
	KeWaitForSingleObject(&kevent, Executive, KernelMode, TRUE, NULL);

	//引用计数减一
	ObDereferenceObject(pFileObject);

	//成功返回TRUE
	return TRUE;
}

//删除文件
BOOLEAN file_deleteFile(
						IN HANDLE hFile
						)
{
	NTSTATUS          status;
	PFILE_OBJECT      pFileObject;
	PDEVICE_OBJECT    pDeviceObject;
	PIRP              pIrp;
	KEVENT            kevent;
	FILE_DISPOSITION_INFORMATION    fileDispositionInformation;
	IO_STATUS_BLOCK ioStatus;
	PIO_STACK_LOCATION irpSp;
	PSECTION_OBJECT_POINTERS pSectionObjectPointer;     

	//设置文件属性为一般属性
	file_setFileAttributes(hFile,FILE_ATTRIBUTE_NORMAL);          

	//对象间的转换
	status = ObReferenceObjectByHandle(hFile,
		DELETE,
		*IoFileObjectType,
		KernelMode,
		&pFileObject,
		NULL);

	//如果失败
	if (!NT_SUCCESS(status))
	{
		return FALSE;
	}

	//获得对应的驱动对象
	//用IoGetRelatedDeviceObject这个函数来获得一个FileObject对应的最底层的那个功能驱动对象(FDO)。
	//但是这样虽然绕过了那些过滤驱动，却同时也绕过了正常的FSD如Ntfs/Fastfat，因为正常的FSD也是作为一个过滤驱动存在的。
	//磁盘文件对象的对应的最底层的FDO是Ftdisk.sys，它已经因为过于底层而不能处理我们投递的IRP请求。
	//其实正常的FSD信息存储在一个Vpb结构中，我们可以使用IoGetBaseFileSystemDeviceObject这个未公开的内核函数来得到它。它就是我们发送IRP的目标了。

	pDeviceObject = IoGetRelatedDeviceObject(pFileObject);

	//使用IoAllocateIrp分配一个IRP
	pIrp = IoAllocateIrp(pDeviceObject->StackSize, TRUE);

	if (pIrp == NULL)
	{
		ObDereferenceObject(pFileObject);
		return FALSE;
	}

	//接着要考虑如果IRP不能及时完成，会异步的返回的情况，我们安装一个CompletionRoutine
	//在CompletionRoutine里面设置一个事件为已激活，通知我们的主线程读取或者写入操作已经完成
	// 现在可以发送IRP了。如果不采取特殊的措施的话，IRP发送目标是FileObject对应的DeviceObject
	//发送后，等待IRP的完成并且释放资源，返回

	KeInitializeEvent(&kevent, SynchronizationEvent, FALSE);

	//内存清零
	RtlZeroMemory(&fileDispositionInformation,0,sizeof(FILE_DISPOSITION_INFORMATION));

	//设置相关参数
	fileDispositionInformation.DeleteFile = TRUE;

	//填充IRP的各个参数
	pIrp->AssociatedIrp.SystemBuffer = &fileDispositionInformation;
	pIrp->UserEvent = &kevent;
	pIrp->UserIosb = &ioStatus;
	pIrp->Tail.Overlay.OriginalFileObject = pFileObject;
	pIrp->Tail.Overlay.Thread = (PETHREAD)KeGetCurrentThread();
	pIrp->RequestorMode = KernelMode;

	irpSp = IoGetNextIrpStackLocation(pIrp);
	//设置功能号为IRP_MJ_SET_INFORMATION
	irpSp->MajorFunction = IRP_MJ_SET_INFORMATION;
	irpSp->DeviceObject = pDeviceObject;
	irpSp->FileObject = pFileObject;
	irpSp->Parameters.SetFile.Length = sizeof(FILE_DISPOSITION_INFORMATION);
	irpSp->Parameters.SetFile.FileInformationClass = FileDispositionInformation;
	irpSp->Parameters.SetFile.FileObject = pFileObject;

    //注册完成例程
	IoSetCompletionRoutine(
		pIrp,
		file_completionRoutine,
		&kevent,
		TRUE,
		TRUE,
		TRUE);

	//再加上下面这三行代码 ，MmFlushImageSection    函数通过这个结构来检查是否可以删除文件。
	pSectionObjectPointer = pFileObject->SectionObjectPointer;
	pSectionObjectPointer->ImageSectionObject = 0;
	pSectionObjectPointer->DataSectionObject = 0;

    //分发给下层驱动
	IoCallDriver(pDeviceObject, pIrp);

	//等待
	KeWaitForSingleObject(&kevent, Executive, KernelMode, TRUE, NULL);

	//引用计数减一
	ObDereferenceObject(pFileObject);

	//返回成功
	return TRUE;
}

//
VOID file_inlineHookDeleteFileFunc()
{
	//赋值前面定义的数组
	KIRQL Irql;

	//获得NtDeleteFile的地址
	NtDeleteFile = (PNtDeleteFile)GetSSDTFuncAddress(ZwDeleteFile);

	//保存函数前五个字节内容
	RtlCopyMemory(file_NtDeleteFileOldAddress,(PBYTE)NtDeleteFile,5);
	RtlCopyMemory(file_NtSetInformationFileOldAddress,(PBYTE)NtSetInformationFile,5);

	//保存新函数五个字节之后偏移
	*(PULONG)(file_NtDeleteFileNewAddress+1)=(ULONG)NewZwDeleteFile-((ULONG)NtDeleteFile+5);
    *(PULONG)(file_NtSetInformationFileNewAddress+1)=(ULONG)NewNtSetInformationFile-((ULONG)NtSetInformationFile+5);

	//开始inline hook
	//关闭内存写保护
	UnProtected();

	//提升IRQL中断级
	Irql=KeRaiseIrqlToDpcLevel();
	//函数开头五个字节写JMP 
	RtlCopyMemory((PBYTE)NtDeleteFile,file_NtDeleteFileNewAddress,5);						

	RtlCopyMemory((PBYTE)NtSetInformationFile,file_NtSetInformationFileNewAddress,5);
	*(PULONG)(file_NtSetInformationFileJmpAddress+1) =(ULONG)NtSetInformationFile-((ULONG)JmpNtSetInformationFile+5);
	RtlCopyMemory((PBYTE)JmpNtSetInformationFile, file_NtSetInformationFileOldAddress, 5);
	RtlCopyMemory((PBYTE)JmpNtSetInformationFile + 5, file_NtSetInformationFileJmpAddress, 5);

	//恢复Irql
	KeLowerIrql(Irql);

	//开启内存写保护
	Protected();
}

//恢复
VOID file_unInlineHookDeleteFileFunc()
{

	//把五个字节再写回到原函数
	KIRQL Irql;

	//关闭写保护
	UnProtected();

	//提升IRQL到Dpc
	Irql=KeRaiseIrqlToDpcLevel();
	//用原先保存的地址覆盖前5个字节
	RtlCopyMemory((PBYTE)NtDeleteFile,file_NtDeleteFileOldAddress,5);
    RtlCopyMemory((PBYTE)NtSetInformationFile,file_NtSetInformationFileOldAddress,5);

	//提升IRQL到Dpc
	KeLowerIrql(Irql);

	//开启写保护
	Protected();
}


//在这里InlineHook
VOID file_inlineHookIoCreateFile()
{

	//赋值前面定义的数组
	KIRQL Irql;

	//获得NtDeleteFile的地址
	NtDeleteFile = (PNtDeleteFile)GetSSDTFuncAddress(ZwDeleteFile);

	//保存函数前五个字节内容
	RtlCopyMemory(file_IoCreateFileOldAddress,(PBYTE)IoCreateFile,5);

	//保存新函数五个字节之后偏移
	*(PULONG)(file_IoCreateFileNewAddress+1)=(ULONG)NewIoCreateFile-((ULONG)IoCreateFile+5);

	//开始inline hook
	//关闭内存写保护
	UnProtected();

	//提升IRQL中断级
	Irql=KeRaiseIrqlToDpcLevel();
	//函数开头五个字节写JMP 
	RtlCopyMemory((PBYTE)IoCreateFile,file_IoCreateFileNewAddress,5);					

	//恢复Irql
	KeLowerIrql(Irql);

	//开启内存写保护
	Protected();
}


//恢复
VOID file_unInlineHookIoCreateFile()
{

	//把五个字节再写回到原函数
	KIRQL Irql;

	//关闭写保护
	UnProtected();

	//提升IRQL到Dpc
	Irql=KeRaiseIrqlToDpcLevel();
	//用原先保存的地址覆盖前5个字节
	RtlCopyMemory((PBYTE)IoCreateFile,file_IoCreateFileOldAddress,5);

	//提升IRQL到Dpc
	KeLowerIrql(Irql);

	//开启写保护
	Protected();
}


//自己构造的原始函数
_declspec(naked) NTSTATUS OldIoCreateFile(
	__out PHANDLE FileHandle,
	__in  ACCESS_MASK DesiredAccess,
	__in  POBJECT_ATTRIBUTES ObjectAttributes,
	__out PIO_STATUS_BLOCK IoStatusBlock,
	__in_opt PLARGE_INTEGER AllocationSize,
	__in  ULONG FileAttributes,
	__in  ULONG ShareAccess,
	__in  ULONG Disposition,
	__in  ULONG CreateOptions,
	__in_opt PVOID EaBuffer,
	__in  ULONG EaLength,
	__in  CREATE_FILE_TYPE CreateFileType,
	__in_opt PVOID InternalParameters,
	__in  ULONG Options
	)
{
	_asm
	{   
		mov edi,edi
			push ebp
			mov ebp,esp
			mov eax,IoCreateFile
			add eax,5
			jmp eax                
	}
}

_declspec(naked) NTSTATUS OldZwDeleteFile(
	IN POBJECT_ATTRIBUTES ObjectAttributes
	)
{
	_asm
	{   
		mov edi,edi
			push ebp
			mov ebp,esp
			mov eax,NtDeleteFile
			add eax,5
			jmp eax                
	}
}

_declspec(naked) NTSTATUS JmpNtSetInformationFile (
	__in HANDLE FileHandle,
	__out PIO_STATUS_BLOCK IoStatusBlock,
	__in_bcount(Length) PVOID FileInformation,
	__in ULONG Length,
	__in FILE_INFORMATION_CLASS FileInformationClass
	)
{
	_asm
	{   
		_emit 0x90
			_emit 0x90
			_emit 0x90
			_emit 0x90
			_emit 0x90
			_emit 0x90
			_emit 0x90
			_emit 0x90
			_emit 0x90
			_emit 0x90              
	}
}

NTSTATUS NewIoCreateFile(
						 __out PHANDLE FileHandle,
						 __in  ACCESS_MASK DesiredAccess,
						 __in  POBJECT_ATTRIBUTES ObjectAttributes,
						 __out PIO_STATUS_BLOCK IoStatusBlock,
						 __in_opt PLARGE_INTEGER AllocationSize,
						 __in  ULONG FileAttributes,
						 __in  ULONG ShareAccess,
						 __in  ULONG Disposition,
						 __in  ULONG CreateOptions,
						 __in_opt PVOID EaBuffer,
						 __in  ULONG EaLength,
						 __in  CREATE_FILE_TYPE CreateFileType,
						 __in_opt PVOID InternalParameters,
						 __in  ULONG Options
						 )
{
	__try
	{
		//禁止文件创建
		if (Disposition == FILE_CREATE)
		{
			//全局
			if (bGlobal_disableCreateFile)
			{
				return STATUS_ACCESS_DENIED;
			}	

			//这里是指定目标进程操作
			if (b_disableCreateFile)
			{
				NTSTATUS status;
				PDISABLE_CREATE_FILE p = NULL;
				PEPROCESS pEpr = NULL;

				//遍历单向列表
				for (p=pDisableCreateFileInfo;p;p=p->next)
				{
					status = PsLookupProcessByProcessId((HANDLE)p->ulPID,&pEpr);

					if (NT_SUCCESS(status))
					{
						//如果目标进程的EPROCESS 等于 ring3下的应用程序EPROCESS
						if (PsGetCurrentProcess() == pEpr)
						{
							//返回句柄无效
							return STATUS_ACCESS_DENIED;
						}
					}																			
				}		
			}
		}

		//禁止文打开文件
		if (Disposition == FILE_OPEN)
		{
			//全局
			if (bGlobal_disableOpenFile)
			{
				//返回句柄无效
				return STATUS_ACCESS_DENIED;
			}	

			//这里是指定目标进程操作
			if (b_disableOpenFile)
			{
				NTSTATUS status;
				PDISABLE_OPEN_FILE p = NULL;
				PEPROCESS pEpr = NULL;

				//遍历单向列表
				for (p=pDisableOpenFileInfo;p;p=p->next)
				{
					status = PsLookupProcessByProcessId((HANDLE)p->ulPID,&pEpr);

					if (NT_SUCCESS(status))
					{
						//如果目标进程的EPROCESS 等于 ring3下的应用程序EPROCESS
						if (PsGetCurrentProcess() == pEpr)
						{
							//返回句柄无效
							return STATUS_ACCESS_DENIED;
						}
					}																			
				}		
			}
		}
		
		//返回
		return OldIoCreateFile(
			FileHandle,
			DesiredAccess,
			ObjectAttributes,
			IoStatusBlock,
			AllocationSize,
			FileAttributes,
			ShareAccess,
			Disposition,
			CreateOptions,
			EaBuffer,
			EaBuffer,
			CreateFileType,
			InternalParameters,
			Options
			);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("file.c NewIoCreateFile error!");
		return STATUS_UNSUCCESSFUL;
	}
}


NTSTATUS NewNtSetInformationFile (
								  __in HANDLE FileHandle,
								  __out PIO_STATUS_BLOCK IoStatusBlock,
								  __in_bcount(Length) PVOID FileInformation,
								  __in ULONG Length,
								  __in FILE_INFORMATION_CLASS FileInformationClass
								  )
{
	__try
	{
		if (FileInformationClass == FileDispositionInformation)
		{
			//全局
			if (bGlobal_disableDeleteFile)
			{
				//返回句柄无效
				return STATUS_ACCESS_DENIED;	
			}				

			//这里是指定目标进程操作
			if (b_disableDeleteFile)
			{
				NTSTATUS status;
				PDISABLE_DELETE_FILE p = NULL;
				PEPROCESS pEpr = NULL;

				//遍历单向列表
				for (p=pDisableDeleteFileInfo;p;p=p->next)
				{
					status = PsLookupProcessByProcessId((HANDLE)p->ulPID,&pEpr);

					if (NT_SUCCESS(status))
					{
						//如果目标进程的EPROCESS 等于 ring3下的应用程序EPROCESS
						if (PsGetCurrentProcess() == pEpr)
						{
							//返回句柄无效
							return STATUS_ACCESS_DENIED;
						}
					}																			
				}		
			}
		}		

		return JmpNtSetInformationFile(FileHandle,IoStatusBlock,FileInformation,Length,FileInformationClass);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("file.c NewNtSetInformationFile error!");
		return STATUS_UNSUCCESSFUL;
	}
}


NTSTATUS NewZwDeleteFile(
						 IN POBJECT_ATTRIBUTES ObjectAttributes
						 )
{
	__try
	{
		//全局
		if (bGlobal_disableDeleteFile)
		{
			return STATUS_ACCESS_DENIED;
		}

		//这里是指定目标进程操作
		if (b_disableDeleteFile)
		{
			NTSTATUS status;
			PDISABLE_DELETE_FILE p = NULL;
			PEPROCESS pEpr = NULL;

			//遍历单向列表
			for (p=pDisableDeleteFileInfo;p;p=p->next)
			{
				status = PsLookupProcessByProcessId((HANDLE)p->ulPID,&pEpr);

				if (NT_SUCCESS(status))
				{
					//如果目标进程的EPROCESS 等于 ring3下的应用程序EPROCESS
					if (PsGetCurrentProcess() == pEpr)
					{
						//返回句柄无效
						return STATUS_ACCESS_DENIED;
					}
				}																			
			}		
		}

		return OldZwDeleteFile(ObjectAttributes);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("file.c NewZwDeleteFile error!");
		return STATUS_UNSUCCESSFUL;
	}
}


//从DisableCreateFileInfo中查找目标进程ID
//返回
//0表示没有找到，1表示找到了
ULONG file_findPIDInDisableCreateFileInfo(ULONG ulPID)
{
	PDISABLE_CREATE_FILE p = NULL;

	__try
	{
		//遍历单向列表
		for (p=pDisableCreateFileInfo;p;p=p->next)
		{
			if (p->ulPID == ulPID)
			{
				//表示找到了
				return 1;
			}
		}

		//没找到的话
		return 0;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("file_findPIDInDisableCreateFileInfo EXCEPTION_EXECUTE_HANDLER error!");
		return 0;
	}
}

//从DisableCreateFileInfo中删除目标进程ID
VOID file_deletePIDInDisableCreateFileInfo(ULONG ulPID)
{
	PDISABLE_CREATE_FILE p = NULL;
	PDISABLE_CREATE_FILE p2 = NULL;

	__try
	{
		for (p2 = p =pDisableCreateFileInfo;p;p2=p,p=p->next)
		{
			if (p->ulPID == ulPID)
			{
				if (pDisableCreateFileInfo==p)
				{
					pDisableCreateFileInfo = pDisableCreateFileInfo->next;
				}
				else
				{
					p2->next=p->next;
				}
			}
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("file_deletePIDInDisableCreateFileInfo EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//从DisableDeleteFileInfo中查找目标进程ID
//返回
//0表示没有找到，1表示找到了
ULONG file_findPIDInDisableDeleteFileInfo(ULONG ulPID)
{
	PDISABLE_DELETE_FILE p = NULL;

	__try
	{
		//遍历单向列表
		for (p=pDisableDeleteFileInfo;p;p=p->next)
		{
			if (p->ulPID == ulPID)
			{
				//表示找到了
				return 1;
			}
		}

		//没找到的话
		return 0;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("file_findPIDInDisableDeleteFileInfo EXCEPTION_EXECUTE_HANDLER error!");
		return 0;
	}
}

//从DisableDeleteFileInfo中删除目标进程ID
VOID file_deletePIDInDisableDeleteFileInfo(ULONG ulPID)
{
	PDISABLE_DELETE_FILE p = NULL;
	PDISABLE_DELETE_FILE p2 = NULL;

	__try
	{
		for (p2 = p =pDisableDeleteFileInfo;p;p2=p,p=p->next)
		{
			if (p->ulPID == ulPID)
			{
				if (pDisableDeleteFileInfo==p)
				{
					pDisableDeleteFileInfo = pDisableDeleteFileInfo->next;
				}
				else
				{
					p2->next=p->next;
				}
			}
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("file_deletePIDInDisableDeleteFileInfo EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//从DisableOpenFileInfo中查找目标进程ID
//返回
//0表示没有找到，1表示找到了
ULONG file_findPIDInDisableOpenFileInfo(ULONG ulPID)
{
	PDISABLE_OPEN_FILE p = NULL;

	__try
	{
		//遍历单向列表
		for (p=pDisableOpenFileInfo;p;p=p->next)
		{
			if (p->ulPID == ulPID)
			{
				//表示找到了
				return 1;
			}
		}

		//没找到的话
		return 0;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("file_findPIDInDisableOpenFileInfo EXCEPTION_EXECUTE_HANDLER error!");
		return 0;
	}
}

//从DisableOpenFileInfo中删除目标进程ID
VOID file_deletePIDInDisableOpenFileInfo(ULONG ulPID)
{
	PDISABLE_OPEN_FILE p = NULL;
	PDISABLE_OPEN_FILE p2 = NULL;

	__try
	{
		for (p2 = p =pDisableOpenFileInfo;p;p2=p,p=p->next)
		{
			if (p->ulPID == ulPID)
			{
				if (pDisableOpenFileInfo==p)
				{
					pDisableOpenFileInfo = pDisableOpenFileInfo->next;
				}
				else
				{
					p2->next=p->next;
				}
			}
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("file_deletePIDInDisableOpenFileInfo EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//获得文件句柄信息
NTSTATUS file_getFileHandleInfo(LPTSTR lpFilePath){
	NTSTATUS status;
	int i;
	ULONG ulSize = 0x1000;
	PSYSTEM_HANDLE_INFORMATION pSystem_handle_information;
	CLIENT_ID clientID;
	OBJECT_ATTRIBUTES object_attributes;
	HANDLE hProcess;
	HANDLE hDupObj;
	OBJECT_BASIC_INFORMATION object_basic_information;
	PFILE_HANDLE_INFO p;
	PFILE_NAME_INFORMATION pFile_name_information;
	PUNICODE_STRING ustrName;
	IO_STATUS_BLOCK ioStatus;

	__try
	{
		pFileHandleInfo = pFileHandleInfoHead = (PFILE_HANDLE_INFO)ExAllocatePool(PagedPool,sizeof(FILE_HANDLE_INFO));
		pFileHandleInfo->next = NULL;

		//
		pSystem_handle_information = (PSYSTEM_HANDLE_INFORMATION)ExAllocatePool(PagedPool,ulSize); 

		while ((status = ZwQuerySystemInformation(SystemHandleInformation,pSystem_handle_information, ulSize, NULL))
			== STATUS_INFO_LENGTH_MISMATCH)            
		{                 
			ExFreePool(pSystem_handle_information);                 
			pSystem_handle_information = (PSYSTEM_HANDLE_INFORMATION)ExAllocatePool(PagedPool,ulSize = ulSize*2);   
		} 

		if (!NT_SUCCESS(status))
		{
			DbgPrint("file_getFileHandleInfo ZwQuerySystemInformation error!");
			ExFreePool(pSystem_handle_information);
			return status;
		}

		//
		//DbgPrint("pSystem_handle_information->NumberOfHandles:%d",pSystem_handle_information->NumberOfHandles);

		for (i=0;i<pSystem_handle_information->NumberOfHandles;i++)
		{
			if (pSystem_handle_information->Information[i].UniqueProcessId == ulProcessID)
			{
				continue;
			}

			if (pSystem_handle_information->Information[i].UniqueProcessId != ulProcessID)
			{
				RtlZeroMemory(&clientID,sizeof(CLIENT_ID));
				clientID.UniqueProcess = (HANDLE)pSystem_handle_information->Information[i].UniqueProcessId;
				clientID.UniqueThread = NULL;

				InitializeObjectAttributes(&object_attributes,NULL,0,NULL,NULL);

				status = ZwOpenProcess(&hProcess ,PROCESS_DUP_HANDLE ,&object_attributes ,&clientID);
				if (!NT_SUCCESS(status))
				{
					DbgPrint("file_getFileHandleInfo ZwOpenProcess error!");
					ExFreePool(pSystem_handle_information);
					return status;
				}

				status = ZwDuplicateObject(hProcess,(HANDLE)pSystem_handle_information->Information[i].HandleValue,ZwCurrentProcess(),\
					&hDupObj,PROCESS_ALL_ACCESS,0,DUPLICATE_SAME_ACCESS);
				if (!NT_SUCCESS(status))
				{
					DbgPrint("file_getFileHandleInfo ZwDuplicateObject error!");
					ExFreePool(pSystem_handle_information);
					return status;
				}
			}
			else
			{
				hDupObj = (HANDLE)pSystem_handle_information->Information[i].HandleValue;
			}

			//basic information
			RtlZeroMemory(&object_basic_information,sizeof(OBJECT_BASIC_INFORMATION));
			ZwQueryObject(hDupObj,ObjectBasicInformation,&object_basic_information,sizeof(OBJECT_BASIC_INFORMATION),NULL);

			//文件对象句柄需特别处理
			if(pSystem_handle_information->Information[i].ObjectTypeIndex == OB_TYPE_FILE)
			{
				pFile_name_information = (PFILE_NAME_INFORMATION)ExAllocatePool(PagedPool,sizeof(WCHAR)*256);
				RtlZeroMemory(pFile_name_information,sizeof(WCHAR)*256);

				status = ZwQueryInformationFile(hDupObj,&ioStatus,pFile_name_information,sizeof(WCHAR)*256,FileNameInformation);

				if (NT_SUCCESS(status))
				{
					DbgPrint("file_getFileHandleInfo fileName:%ws",pFile_name_information->FileName);

					if (_wcsnicmp(pFile_name_information->FileName,lpFilePath,wcslen(lpFilePath)) >= 0)
					{
						p = (PFILE_HANDLE_INFO)ExAllocatePool(PagedPool,sizeof(FILE_HANDLE_INFO));
						RtlZeroMemory(p,sizeof(FILE_HANDLE_INFO));

						DbgPrint("name:%ws",pFile_name_information->FileName);

						p->ulPID = pSystem_handle_information->Information[i].UniqueProcessId;
						p->ulHandle = pSystem_handle_information->Information[i].HandleValue;
						wcscpy(p->wstrHandleName,pFile_name_information->FileName);           
						p->next = NULL;

						pFileHandleInfo->next = p;
						pFileHandleInfo = p;
					}		
				}		

				ExFreePool(pFile_name_information);
			}
		}

		pFileHandleInfo = pFileHandleInfoHead;

		ExFreePool(pSystem_handle_information);
		return STATUS_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("file_getFileHandleInfo EXCEPTION_EXECUTE_HANDLER error!");
		ExFreePool(pSystem_handle_information);
		return STATUS_UNSUCCESSFUL;
	}
}

//释放内存
VOID file_freeFileHandleInfo()
{
	PFILE_HANDLE_INFO p = NULL;

	__try
	{
		while(pFileHandleInfo != NULL)
		{
			p = pFileHandleInfo->next;
			ExFreePool(pFileHandleInfo);
			pFileHandleInfo   = p;
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("file_freeFileHandleInfo EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//获得大小
ULONG file_getFileHandleInfoCount()
{
	__try
	{
		PFILE_HANDLE_INFO p=NULL;
		ULONG ulCount=0;

		DbgPrint("file_getFileHandleInfoCount in...");

		//获得大小
		for (p=pFileHandleInfo->next;p;p=p->next)
		{
			DbgPrint("ulCount... %d",ulCount);
			ulCount++;
		}

		DbgPrint("file_getFileHandleInfoCount ulCount:%d",ulCount);
		return ulCount;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("file_getFileHandleInfoCount EXCEPTION_EXECUTE_HANDLER error!");
		return 0;
	}	
}

//发送数据到ring3
NTSTATUS file_getFileHandleInfo_send(PFILE_HANDLE_INFO pBuffer)
{
	PFILE_HANDLE_INFO p=NULL;
	ULONG ulIndex = 0;

	__try
	{
		//遍历
		for (p=pFileHandleInfo->next;p;p=p->next)
		{
			RtlCopyMemory(pBuffer + ulIndex,p,sizeof(FILE_HANDLE_INFO));

			ulIndex++;
		}

		//释放内存
		file_freeFileHandleInfo();

		return STATUS_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("file_getFileHandleInfo_send EXCEPTION_EXECUTE_HANDLER error!");
		return STATUS_UNSUCCESSFUL;
	}
}