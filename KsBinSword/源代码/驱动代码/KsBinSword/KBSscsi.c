
//------以下是SRB读写磁盘的代码----------------------
NTSTATUS IrpCompletionRoutine_0(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp, IN
								PVOID Context)
{
	PMDL mdl;
	Irp->UserIosb->Status = Irp->IoStatus.Status;
	Irp->UserIosb->Information = Irp->IoStatus.Information;
	if (!Context)
	{
		mdl = Irp->MdlAddress;
		if (mdl)
		{
			DbgPrint("read size: %d..", Irp->IoStatus.Information);
			MmUnlockPages(mdl);
			IoFreeMdl(mdl);
		}
	}
	KeSetEvent(Irp->UserEvent, IO_NO_INCREMENT, 0);
	IoFreeIrp(Irp);
	return STATUS_MORE_PROCESSING_REQUIRED;
}
PDEVICE_OBJECT GetLastDiskDeviceObject(PDRIVER_OBJECT drv_object) //这个就是DR0
{
	PDEVICE_OBJECT result;
	PDEVICE_OBJECT finddev;
	finddev = drv_object->DeviceObject;
	result = NULL;
	while (finddev)
	{
		if (finddev->DeviceType == FILE_DEVICE_DISK)
			result = finddev;
		finddev = finddev->NextDevice;
	}
	return result;
}
NTSTATUS MyIoCallDriver(PDEVICE_OBJECT DeviceObject, PIRP Irp) 
//自己的IoCallDriver
{
	PIO_STACK_LOCATION stack;
	--Irp->CurrentLocation;
	stack = IoGetNextIrpStackLocation(Irp);
	Irp->Tail.Overlay.CurrentStackLocation = stack; //移动堆栈
	stack->DeviceObject = DeviceObject;
	return (DeviceObject->DriverObject->MajorFunction[(ULONG)stack
		->MajorFunction])(DeviceObject, Irp);
}

PDEVICE_OBJECT GetAtaDr0DevObject()
{
	
	UNICODE_STRING diskstr;
	PDRIVER_OBJECT diskdrv;
	PDEVICE_OBJECT dr0dev;
	RtlInitUnicodeString(&diskstr, L"\\Driver\\Disk");
	if (ObReferenceObjectByName(&diskstr, 64, 0, 0,  *IoDriverObjectType, 0, 0,
		&diskdrv) < 0)
		return NULL;
	
	dr0dev = GetLastDiskDeviceObject(diskdrv);
	//if (dr0dev)
    //DbgPrint("Eros412 said : ata dr0 dev obj is : %08x...", dr0dev);
	
	ObfDereferenceObject(diskdrv);
	return dr0dev;
}
ULONG AtapiReadWriteDisk(PDEVICE_OBJECT dev_object, 
						 ULONG MajorFunction, 
						 PVOID buffer, 
						 ULONG DiskPos,//扇区号
						 int BlockCount//读写多少个扇区
						 )
{
  NTSTATUS status;
  PSCSI_REQUEST_BLOCK srb;
  PSENSE_DATA sense;
  KEVENT Event;
  PIRP irp;
  PMDL mdl;
  IO_STATUS_BLOCK isb;
  PIO_STACK_LOCATION isl;
  PVOID psense;
  int count = 8;
  while (1)
  {
    srb = ExAllocatePool(0, sizeof(SCSI_REQUEST_BLOCK));
    if (!srb)
      break;
    sense = ExAllocatePool(0, sizeof(SENSE_DATA));
    psense = sense;
    if (!sense)
      break;
    memset(srb, 0, sizeof(SCSI_REQUEST_BLOCK));
    memset(sense, 0, sizeof(SENSE_DATA));
    srb->Length = sizeof(SCSI_REQUEST_BLOCK); 
      //更多关于srb,请看《SCSI 总线和IDE接口：协议、应用和编程》和《SCSI程序员指南》
    srb->Function = 0;
    srb->DataBuffer = buffer;
    srb->DataTransferLength = BlockCount << 9; //sector size*number of sector
    srb->QueueAction = SRB_FLAGS_DISABLE_AUTOSENSE;
    srb->SrbStatus = 0;
    srb->ScsiStatus = 0;
    srb->NextSrb = 0;
    srb->SenseInfoBuffer = sense;
    srb->SenseInfoBufferLength = sizeof(SENSE_DATA);
    if (MajorFunction == IRP_MJ_READ)
      srb->SrbFlags = SRB_FLAGS_DATA_IN;
    else
      srb->SrbFlags = SRB_FLAGS_DATA_OUT;

    if (MajorFunction == IRP_MJ_READ)
      srb->SrbFlags |= SRB_FLAGS_ADAPTER_CACHE_ENABLE;

    srb->SrbFlags |= SRB_FLAGS_DISABLE_AUTOSENSE;
    srb->TimeOutValue = (srb->DataTransferLength >> 10) + 1;
    srb->QueueSortKey = DiskPos;
    srb->CdbLength = 10;
    srb->Cdb[0] = 2 *((UCHAR)MajorFunction + 17);
    srb->Cdb[1] = srb->Cdb[1] &0x1F | 0x80;
    srb->Cdb[2] = (unsigned char)(DiskPos >> 0x18) &0xFF; //
    srb->Cdb[3] = (unsigned char)(DiskPos >> 0x10) &0xFF; //
    srb->Cdb[4] = (unsigned char)(DiskPos >> 0x08) &0xFF; //
    srb->Cdb[5] = (UCHAR)DiskPos; //填写sector位置(低)
    //srb->Cdb[6] = (UCHAR)0; 
    srb->Cdb[7] = (UCHAR)BlockCount >> 0x08;
    srb->Cdb[8] = (UCHAR)BlockCount;

    KeInitializeEvent(&Event, 0, 0);
    irp = IoAllocateIrp(dev_object->StackSize, 0);
    mdl = IoAllocateMdl(buffer, BlockCount << 9, 0, 0, irp);
    irp->MdlAddress = mdl;
    if (!mdl)
    {
      ExFreePool(srb);
      ExFreePool(psense);
      IoFreeIrp(irp);
      return STATUS_INSUFFICIENT_RESOURCES;
    }
    MmProbeAndLockPages(mdl, 0, (MajorFunction == IRP_MJ_READ ? 0 : 1));
    srb->OriginalRequest = irp;
    irp->UserIosb = &isb;
    irp->UserEvent = &Event;
    irp->IoStatus.Status = 0;
    irp->IoStatus.Information = 0;
    irp->Flags = IRP_SYNCHRONOUS_API | IRP_NOCACHE;
    irp->AssociatedIrp.SystemBuffer = 0;
    irp->Cancel = 0;
    irp->RequestorMode = 0;
    irp->CancelRoutine = 0;
    irp->Tail.Overlay.Thread = PsGetCurrentThread();
    isl = IoGetNextIrpStackLocation(irp);
    isl->DeviceObject = dev_object;
    isl->MajorFunction = IRP_MJ_SCSI;
    isl->Parameters.Scsi.Srb = srb;
    isl->CompletionRoutine = IrpCompletionRoutine_0;
    isl->Context = srb;
    isl->Control = SL_INVOKE_ON_CANCEL | SL_INVOKE_ON_SUCCESS |
      SL_INVOKE_ON_ERROR;
	//DbgBreakPoint();
	//  Classpnp!ClassInternalIoControl->
	//  ACPI!sub_16A9A->
	//  ACPI!sub_20246->
	//	ACPI!sub_1ff70->
	//	ACPI!sub_2ABCE->
	//	ACPI!sub_2AA8E->
	//	WritrIO
    status = MyIoCallDriver(dev_object, irp);
    KeWaitForSingleObject(&Event, 0, 0, 0, 0);

    if (srb->SenseInfoBuffer != psense && srb->SenseInfoBuffer)
      ExFreePool(srb->SenseInfoBuffer);

    ExFreePool(srb);
    ExFreePool(psense);

    if (status >= 0 || !count)
      return status;

    DbgPrint("Send XXX Failed..%08xrn", status);
    KeStallExecutionProcessor(1u);
    --count;
  }
  return STATUS_INSUFFICIENT_RESOURCES;
}