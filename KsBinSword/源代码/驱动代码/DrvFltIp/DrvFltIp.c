
#include <string.h>
#include <stdio.h>
#include <ntddk.h>
#include <ntddndis.h>
#include <pfhook.h>
#include "DrvFltIp.h"

#if DBG
#define dprintf DbgPrint
#else
#define dprintf(x)
#endif
#define SEPARATOR L"\r\n----------------NetMessage-----------------\r\n"
NTSTATUS DrvDispatch(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
VOID DrvUnload(IN PDRIVER_OBJECT DriverObject);

NTSTATUS SetFilterFunction(PacketFilterExtensionPtr filterFunction);

NTSTATUS AddFilterToList(IPFilter *pf);
void ClearFilterList(void);
PF_FORWARD_ACTION cbFilterFunction(IN unsigned char *PacketHeader,IN unsigned char *Packet, IN unsigned int PacketLength, IN unsigned int RecvInterfaceIndex, IN unsigned int SendInterfaceIndex, IN unsigned long RecvLinkNextHop, IN unsigned long SendLinkNextHop);

#define NT_DEVICE_NAME L"\\Device\\DrvFltIp"
#define DOS_DEVICE_NAME L"\\DosDevices\\DrvFltIp"

#define MAXPRINT 50
#define MAXSTR 70
WCHAR  g_StrArr[MAXPRINT][MAXSTR];
ULONG  g_InIndex=0;
ULONG  g_OutIndex=0;
void MyPrint(WCHAR wcStr[])
{
	if (wcslen(wcStr)>MAXSTR-1)
	{
	   return;
	}
	g_InIndex=++g_InIndex==MAXPRINT?0:g_InIndex;

	wcscpy(g_StrArr[g_InIndex],wcStr);

}
struct filterList *first = NULL;
struct filterList *last = NULL;

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{

    PDEVICE_OBJECT         deviceObject = NULL;
    NTSTATUS               ntStatus;
    UNICODE_STRING         deviceNameUnicodeString;
    UNICODE_STRING         deviceLinkUnicodeString;
    int i=0;

	RtlInitUnicodeString(&deviceNameUnicodeString, NT_DEVICE_NAME);

	ntStatus = IoCreateDevice(DriverObject, 
								0,
								&deviceNameUnicodeString, 
								FILE_DEVICE_DRVFLTIP,
								0,
								FALSE,
								&deviceObject);


    if ( NT_SUCCESS(ntStatus) )
    {

        RtlInitUnicodeString(&deviceLinkUnicodeString, DOS_DEVICE_NAME);

        ntStatus = IoCreateSymbolicLink(&deviceLinkUnicodeString, &deviceNameUnicodeString);

        if ( !NT_SUCCESS(ntStatus) )
        {
            MyPrint(L"DrvFltIp.SYS: IoCreateSymbolicLink failed\n");
        }

        DriverObject->MajorFunction[IRP_MJ_CREATE]         =
        DriverObject->MajorFunction[IRP_MJ_CLOSE]          =
        DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DrvDispatch;
        DriverObject->DriverUnload                         = DrvUnload;
    }

    if ( !NT_SUCCESS(ntStatus) )
    {
        dprintf("Error in initialization. Unloading...");

		DrvUnload(DriverObject);
    }

    return ntStatus;
}

NTSTATUS DrvDispatch(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{

    PIO_STACK_LOCATION  irpStack;
    PVOID               ioBuffer;
    ULONG               inputBufferLength;
    ULONG               outputBufferLength;
    ULONG               ioControlCode;
    NTSTATUS            ntStatus;
	ULONG i=0,j=0;
    WCHAR **wcOutArr;
    Irp->IoStatus.Status      = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;

    irpStack = IoGetCurrentIrpStackLocation(Irp);


    ioBuffer           = Irp->AssociatedIrp.SystemBuffer;
    inputBufferLength  = irpStack->Parameters.DeviceIoControl.InputBufferLength;
    outputBufferLength = irpStack->Parameters.DeviceIoControl.OutputBufferLength;

    switch (irpStack->MajorFunction)
    {
    case IRP_MJ_CREATE:

        MyPrint(L"DrvFltIp: IRP_MJ_CREATE\n");

        break;

    case IRP_MJ_CLOSE:

        MyPrint(L"DrvFltIp: IRP_MJ_CLOSE\n");

        break;

    case IRP_MJ_DEVICE_CONTROL:

        ioControlCode = irpStack->Parameters.DeviceIoControl.IoControlCode;

        switch (ioControlCode)
        {

			case START_IP_HOOK:
			{
           		SetFilterFunction(cbFilterFunction);

				break;
			}


			case STOP_IP_HOOK:
			{
				SetFilterFunction(NULL);
	            
				break;
			}


			case ADD_FILTER:
			{
				if(inputBufferLength == sizeof(IPFilter))
				{
					IPFilter *nf;

					nf = (IPFilter *)ioBuffer;
					
					AddFilterToList(nf);
				}

				break;
			}


			case CLEAR_FILTER:
			{
				ClearFilterList();

				break;
			}
			case OUT_PRINTSTR:
			{
                wcOutArr=(WCHAR**)ioBuffer;
                dprintf("i:%d,o:%d\n",g_OutIndex,g_InIndex);
				if (g_OutIndex==g_InIndex)
				{
					break;
				}
             	for (i=(g_OutIndex+1)%MAXPRINT,j=0;i!=g_InIndex;i=(++i)%MAXPRINT,j++)
				{

		            wcscpy((WCHAR *)((UCHAR*)wcOutArr+j*sizeof(WCHAR)*MAXSTR),g_StrArr[i]);
		
				}
				//DbgBreakPoint();
				wcscpy((WCHAR *)((UCHAR*)wcOutArr+j*sizeof(WCHAR)*MAXSTR),g_StrArr[i]);	
				
		
	            g_OutIndex	=i;	
				outputBufferLength=sizeof(WCHAR)*MAXPRINT*MAXSTR;
			    Irp->IoStatus.Information = sizeof(WCHAR)*MAXPRINT*MAXSTR;
				break;
			}
			default:
				Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;

				dprintf("DrvFltIp.SYS: unknown IRP_MJ_DEVICE_CONTROL\n");

				break;
        }

        break;
    }


    ntStatus = Irp->IoStatus.Status;

    IoCompleteRequest(Irp, IO_NO_INCREMENT);



    return ntStatus;
}


VOID DrvUnload(IN PDRIVER_OBJECT DriverObject)
{
    UNICODE_STRING         deviceLinkUnicodeString;

	dprintf("DrvFltIp.SYS: Unloading\n");

    SetFilterFunction(NULL);


	ClearFilterList();
   

    RtlInitUnicodeString(&deviceLinkUnicodeString, DOS_DEVICE_NAME);
    IoDeleteSymbolicLink(&deviceLinkUnicodeString);

    IoDeleteDevice(DriverObject->DeviceObject);
}




NTSTATUS SetFilterFunction(PacketFilterExtensionPtr filterFunction)
{
	NTSTATUS status = STATUS_SUCCESS, waitStatus=STATUS_SUCCESS;
	UNICODE_STRING filterName;
	PDEVICE_OBJECT ipDeviceObject=NULL;
	PFILE_OBJECT ipFileObject=NULL;

	PF_SET_EXTENSION_HOOK_INFO filterData;

	KEVENT event;
	IO_STATUS_BLOCK ioStatus;
	PIRP irp;

	dprintf("Getting pointer to IpFilterDriver\n");
	
	RtlInitUnicodeString(&filterName, DD_IPFLTRDRVR_DEVICE_NAME);
	status = IoGetDeviceObjectPointer(&filterName,STANDARD_RIGHTS_ALL, &ipFileObject, &ipDeviceObject);
	dprintf("OK:%x",status);
	if(NT_SUCCESS(status))
	{
		filterData.ExtensionPointer = filterFunction;

		KeInitializeEvent(&event, NotificationEvent, FALSE);

		irp = IoBuildDeviceIoControlRequest(IOCTL_PF_SET_EXTENSION_POINTER,
			  							    ipDeviceObject,
											(PVOID) &filterData,
											sizeof(PF_SET_EXTENSION_HOOK_INFO),
											NULL,
											0,
											FALSE,
											&event,
											&ioStatus);


		if(irp != NULL)
		{

			status = IoCallDriver(ipDeviceObject, irp);


			if (status == STATUS_PENDING) 
			{
				waitStatus = KeWaitForSingleObject(&event, Executive, KernelMode, FALSE, NULL);

				if (waitStatus 	!= STATUS_SUCCESS ) 
					dprintf("Error waiting for IpFilterDriver response.");
			}

			status = ioStatus.Status;

			if(!NT_SUCCESS(status))
				dprintf("Error, IO error with ipFilterDriver\n");
		}
		
		else
		{

			status = STATUS_INSUFFICIENT_RESOURCES;

			dprintf("Error building IpFilterDriver IRP\n");
		}

		if(ipFileObject != NULL)
		{
            dprintf("ObDereferenceObject");
			ObDereferenceObject(ipFileObject);
		}
		
		ipFileObject = NULL;
		ipDeviceObject = NULL;
	}
	
	else
		dprintf("Error while getting the pointer\n");
	
	return status;
}

NTSTATUS AddFilterToList(IPFilter *pf)
{
	struct filterList *aux=NULL;

	aux=(struct filterList *) ExAllocatePool(NonPagedPool, sizeof(struct filterList));
	
	if(aux == NULL)
	{
		dprintf("Problem reserving memory\n");
	
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	aux->ipf.destinationIp = pf->destinationIp;
	aux->ipf.sourceIp = pf->sourceIp;

	aux->ipf.destinationMask = pf->destinationMask;
	aux->ipf.sourceMask = pf->sourceMask;

	aux->ipf.destinationPort = pf->destinationPort;
	aux->ipf.sourcePort = pf->sourcePort;

	aux->ipf.protocol = pf->protocol;

	aux->ipf.drop=pf->drop;


	if(first == NULL)
	{
		first = last = aux;
		
		first->next = NULL;
	}
	
	else
	{
		last->next = aux;
		last = aux;
		last->next = NULL;
	}

	dprintf("Rule Added\n\t%x %x\n\t%x %x\n\t%x\n\t%x", aux->ipf.sourceIp
													  , aux->ipf.sourceMask
												      , aux->ipf.destinationIp
												      , aux->ipf.destinationMask
													  , aux->ipf.sourcePort
													  , aux->ipf.destinationPort);

	return STATUS_SUCCESS;
}

void ClearFilterList(void)
{
	struct filterList *aux = NULL;

	dprintf("Removing the filter List...");
	
	while(first != NULL)
	{
		aux = first;
		first = first->next;
		ExFreePool(aux);

		dprintf("One Rule removed");
	}

	first = last = NULL;

	dprintf("Removed is complete.");
}




PF_FORWARD_ACTION cbFilterFunction(IN unsigned char *PacketHeader,
								   IN unsigned char *Packet, 
								   IN unsigned int PacketLength,
								   IN unsigned int RecvInterfaceIndex,
								   IN unsigned int SendInterfaceIndex,
								   IN unsigned long RecvLinkNextHop,
								   IN unsigned long SendLinkNextHop)
{
	IPPacket *ipp;
	TCPHeader *tcph;
	UDPHeader *udph;

	int countRule=0;

	struct filterList *aux = first;
    WCHAR wcMessage[MAXSTR];

	ipp=(IPPacket *)PacketHeader;
    MyPrint(SEPARATOR);
	dprintf("PacketInfo %x, %d\r\n", PacketLength, RecvInterfaceIndex);
	dprintf("Source: %x Destination: %x Protocol: %d\r\n", ipp->ipSource, ipp->ipDestination, ipp->ipProtocol);
    swprintf(wcMessage,L"PacketLength: %x, RecvInterfaceIndex:%d\r\n", PacketLength, RecvInterfaceIndex);
	MyPrint(wcMessage);
	//swprintf(wcMessage,L"IP--Source: %x Destination: %x Protocol: %d\r\n", ipp->ipSource, ipp->ipDestination, ipp->ipProtocol);
    swprintf(wcMessage,L"NetInfomation:@%x@@%x@@@%d\r\n", ipp->ipSource, ipp->ipDestination, ipp->ipProtocol);
	MyPrint(wcMessage);

	if(ipp->ipProtocol == 6)
	{
		tcph=(TCPHeader *)Packet; 

		dprintf("FLAGS: %x\r\n", tcph->flags);
		swprintf(wcMessage,L"FLAGS: %x\r\n", tcph->flags);

		if(!(tcph->flags & 0x02)) 
			return PF_FORWARD;
	}


	while(aux != NULL)
	{
		dprintf("Comparing with Rule %d", countRule);


		if(aux->ipf.protocol == 0 || ipp->ipProtocol == aux->ipf.protocol)
		{

			if(aux->ipf.sourceIp != 0 && (ipp->ipSource & aux->ipf.sourceMask) != aux->ipf.sourceIp)
			{
				aux=aux->next;
			
				countRule++;
				continue;
			}
									

			if(aux->ipf.destinationIp != 0 && (ipp->ipDestination & aux->ipf.destinationMask) != aux->ipf.destinationIp)
			{
				aux=aux->next;

				countRule++;
				continue;
			}
			

			//tcp, protocol = 6
			if(ipp->ipProtocol == 6) 
			{
				if(aux->ipf.sourcePort == 0 || tcph->sourcePort == aux->ipf.sourcePort)
				{ 
					if(aux->ipf.destinationPort == 0 || tcph->destinationPort == aux->ipf.destinationPort) //puerto tcp destino
					{

						if(aux->ipf.drop)
								 return  PF_DROP;
							else
								return PF_FORWARD;
					}
				}
			}
				
			//udp, protocol = 17
			else if(ipp->ipProtocol == 17) 
			{
				udph=(UDPHeader *)Packet; 

				if(aux->ipf.sourcePort == 0 || udph->sourcePort == aux->ipf.sourcePort) 
				{ 
					if(aux->ipf.destinationPort == 0 || udph->destinationPort == aux->ipf.destinationPort) 
					{

						if(aux->ipf.drop)
							return  PF_DROP;
						
						else
							return PF_FORWARD;
					}
				}
			}	
			
			else
			{

				if(aux->ipf.drop)
					return  PF_DROP;
				else
					return PF_FORWARD;
			}	
		}
		

		countRule++;
		aux=aux->next;
	}


	return PF_FORWARD;
}



