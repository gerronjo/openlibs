#define _KERNEL_MODE

#pragma warning(disable : 4995)

#include "..\inc\Lpc.h"
#include <ntddk.h>
#include <ntstrsafe.h>
#include <WinDef.h>

#define  CHECK_UNSUCCESSFUL_STATUS(st) if(!NT_SUCCESS(st)) return st;

//0x1F
_ConnectPort	NtConnectPort;
//0xC7
_RequestPort	NtRequestPort;
//0xC8
_RequestWaitReplyPort 	NtRequestWaitReplyPort;


PETHREAD peThread;

extern PSYSTEM_DESCRIPTOR_TABLE KeServiceDescriptorTable;

NTSTATUS GetSDTFuncAddress( ULONG ulIndex , PVOID* ppAddress )
{
	PVOID FuncAddr=NULL;
	
	PSYSTEM_DESCRIPTOR_TABLE psdt = KeServiceDescriptorTable;
	PVOID* pEntry = (PVOID*)(psdt->ntoskrnl.ServiceTable);
	
	FuncAddr = pEntry[ulIndex];
	if (MmIsAddressValid(FuncAddr))
	{
		if (ppAddress!=NULL)
		{
			*ppAddress = FuncAddr;
			return STATUS_SUCCESS;
		}
	}
	return STATUS_UNSUCCESSFUL;
}

NTSTATUS InitUnexportedAddress( void )
{
	NTSTATUS st;

	st = GetSDTFuncAddress ( 0x1F , (PVOID*)&NtConnectPort);
	CHECK_UNSUCCESSFUL_STATUS(st);
	st = GetSDTFuncAddress ( 0xC7 , (PVOID*)&NtRequestPort);
	CHECK_UNSUCCESSFUL_STATUS(st);
	st = GetSDTFuncAddress ( 0xC8 , (PVOID*)&NtRequestWaitReplyPort);
	CHECK_UNSUCCESSFUL_STATUS(st);

	KdPrint(("NtConnectPort         : %X \n",(ULONG)NtConnectPort));
	KdPrint(("NtRequstPort          : %X \n",(ULONG)NtRequestPort));
	KdPrint(("NtRequstWaitReplyPort : %X \n",(ULONG)NtRequestWaitReplyPort));
	return STATUS_SUCCESS;
}

NTSTATUS QueryProcess( PPROCESS_INFO pRecvBuffer , PULONG pulProcCount )
{
	PEPROCESS peCurrent;
	LIST_ENTRY* sEntry;
	LIST_ENTRY* nEntry;
	ULONG	  ulProcCount=0;
	
	//must exist a buffer to store the number of process
	if (pulProcCount == NULL)
	{
		return STATUS_UNSUCCESSFUL;
	}


	//init it;
	*pulProcCount = 0;
	//get current process 
	peCurrent = PsGetCurrentProcess();
	if (peCurrent == NULL)
	{
		return STATUS_UNSUCCESSFUL;
	}


	//first get the count
	nEntry = sEntry = (LIST_ENTRY*)((ULONG)peCurrent + ACTIVE_PROCESS_LINK);
	do 
	{
		ulProcCount++;
		nEntry = nEntry->Flink;
	} while (sEntry != nEntry);


	if (pRecvBuffer == NULL && pulProcCount !=NULL)
	{
		//return count of process
		*pulProcCount = ulProcCount - 1;
		return STATUS_UNSUCCESSFUL;
	}

	if (pRecvBuffer !=NULL && pulProcCount !=NULL)
	{
		//get the count that caller wanna enum
		//return info and count of all process
		PEPROCESS peTemp;
		char* chName;
		PULONG	pulProcId;
		//using for looping
		ULONG i,ulEnumCout;

		ULONG ulWillingCount = *pulProcCount;
		ulEnumCout = ((ulWillingCount < ulProcCount)? ulWillingCount : ulProcCount) ;
		
		//init
		i=0;
		nEntry = sEntry;
		do 
		{
			peTemp = POINT_TO_PROCESS(nEntry);
			chName = POINT_TO_PROCNAME(peTemp);
			pulProcId = POINT_TO_PROCID(peTemp);

			_try
			{
				pRecvBuffer[i].ulPid = *pulProcId;
				strcpy( pRecvBuffer[i].wchName , chName);
			}
			_except(EXCEPTION_CONTINUE_EXECUTION)
			{
				/**/
			}

			i++;
			ulEnumCout--;
			nEntry = nEntry->Flink;
		} while (nEntry != sEntry && ulEnumCout !=0);

		*pulProcCount = i-1;
		return STATUS_SUCCESS;
	}

	return STATUS_UNSUCCESSFUL;
}

//print data transferred by message
void ProcessMsg( PCONNECTION_INFO pciInfo , PPORT_MESSAGE pMsg)
{
	CONNECTION_INFO		ciInfo;
	PMESSAGE_DATA_INFO  pmdi;
	PORT_MESSAGE		msg;
	PVOID				pText = NULL;

	ciInfo = *pciInfo;
	msg = *pMsg;

	pmdi = MESSAGE_TO_DATAINFO_BUF(msg);
	//try to locate the position of data
	if (pmdi->TransInfo.bInSection)
	{
		pText = ( ciInfo.MemInfo.memr.Base);
	}
	else
	{
		pText = MESSAGE_TO_REALDATA_BUF(msg);
	}
	//if there exists no data , return now
	if (!pmdi->TransInfo.DataLength)
	{
		return ;
	}
	
	if ( pmdi->TransInfo.DataType == DATA_TRANSFER_PROCESSS)
	{
		//allocate a buffer to store temp string
		char    chTemp[0x30]={0};
		//init pointer
		char*   pTempText=(char*) pText;

		ULONG   i =0;
		//calculate total number in character
		ULONG   StrCchLen = pmdi->TransInfo.DataLength / sizeof(char);
		KdPrint(("%s : \n" , SERVERNAME));
		_try
		{
			do 
			{
				//copy 0x30 characters
				RtlStringCchCopyA ( chTemp , 0x30 , pTempText);
				//output string
				KdPrint(("%s\n",chTemp));
				//move pointer 
				pTempText = (char*)((ULONG)pTempText + 0x30*sizeof(char));
				//increment counter
				++i;

			} while ( i*0x30 < StrCchLen);
		}
		_except(EXCEPTION_EXECUTE_HANDLER)
		{
			return ;
		}
	}

	
	KdPrint(("%s  :\n %s \n",SERVERNAME,(char*)pText));
	
}

NTSTATUS CreateSection( PHANDLE pSectionHandle)
{
	HANDLE hSection;
	OBJECT_ATTRIBUTES obj;
	LARGE_INTEGER liNum;
	NTSTATUS st;

	liNum.HighPart =0;
	liNum.LowPart = 0x10000;
	InitializeObjectAttributes( &obj , NULL , OBJ_KERNEL_HANDLE , NULL ,NULL);

	st = ZwCreateSection ( &hSection,
						   SECTION_ALL_ACCESS,
						   &obj,
						   &liNum,
						   PAGE_READWRITE,
						   SEC_COMMIT,
						   NULL);
	if (hSection > 0)
	{
		*pSectionHandle = hSection;
	}
	return st;
}

NTSTATUS ConnectPort( PCONNECTION_INFO pciInfo)
{
	//port's name
	UNICODE_STRING     usPortName;
	//to receive value of max length of message
	ULONG			   ulMxMsgLen;
	//
	SECURITY_QUALITY_OF_SERVICE sqos;
	NTSTATUS	       st;
	//section read by client , written by server
	PORT_SECTION_READ  psr;
	//section written by client , read by server
	PORT_SECTION_WRITE psw;
	//port of section
	HANDLE			   hSection;
	//handle of port
	HANDLE			   hPort;
	//connection info , to indicate whether using section
	MESSAGE_DATA_INFO  mdi;
	ULONG			   ulConnectInfoSize = sizeof(MESSAGE_DATA_INFO);

	RtlZeroMemory( &sqos , sizeof(SECURITY_QUALITY_OF_SERVICE));
	RtlZeroMemory(&mdi , sizeof(MESSAGE_DATA_INFO));
	RtlZeroMemory( &psr , sizeof(PORT_SECTION_READ));
	RtlZeroMemory( &psw , sizeof(PORT_SECTION_WRITE));

	st = CreateSection ( &hSection);
	if (!NT_SUCCESS(st))
	{
		KdPrint(("create section failed , ERROR CODE : %X \n",st));
		return st;
	}
	//init write section
	psw.Length = sizeof(PORT_SECTION_WRITE);
	psw.SectionHandle = hSection;
	psw.SectionOffset = 0;
	psw.ViewSize = 0x10000;

	//init read section
	psr.Length = sizeof(PORT_SECTION_READ);

	//init port name
	RtlInitUnicodeString ( &usPortName , SERVERNAME_W);
	//init the structure of SECURITY_QUALITY_OF_SERVICE
	sqos.Length = sizeof(SECURITY_QUALITY_OF_SERVICE);
	sqos.ImpersonationLevel = SecurityImpersonation;
	sqos.ContextTrackingMode =TRUE;
	sqos.ContextTrackingMode = TRUE;
	//init connection info
	mdi.bSectionUsed = TRUE;
	//connecting
	_try
	{
		st = NtConnectPort( &hPort,
							&usPortName,
							&sqos,
							&psw,
							&psr,
							&ulMxMsgLen,
							(PVOID)&mdi,
							&ulConnectInfoSize);

		if (NT_SUCCESS(st))
		{

			pciInfo->hDataPort = hPort;
			pciInfo->MemInfo.memr.Base = psr.ViewBase;
			pciInfo->MemInfo.memr.Size = psr.ViewSize;
			pciInfo->MemInfo.memw.Base = psw.ViewBase;
			pciInfo->MemInfo.memw.Size = psw.ViewSize;

			KdPrint(("Connection info :\n PORT : %X \n Read Base : %X   Read Size : %X \n Write Base : %X   Write Size : %X \n ", hPort,psr.ViewBase,psr.ViewSize,psw.ViewBase,psw.ViewSize));
		}
	}
	_except(EXCEPTION_EXECUTE_HANDLER)
	{
		KdPrint(("connect failed ERROR CODE : %X \n" ,GetExceptionCode()));
	}

	
	return st;
}

NTSTATUS RequstMsg( PCONNECTION_INFO pciInfo , PVOID data , BYTE dataType , ULONG ulDataSize , PPORT_MESSAGE pReplyMsg )
{
	PORT_MESSAGE msg;
	PVOID writeBuffer;
	PMESSAGE_DATA_INFO writeInfo;
	NTSTATUS st;


	RtlZeroMemory(&msg , sizeof(PORT_MESSAGE));
	
	writeInfo = MESSAGE_TO_DATAINFO_BUF(msg);

	if (ulDataSize > MAX_SMALL_DATA_SIZE)
	{
		writeInfo->TransInfo.bInSection = TRUE;
		writeBuffer = pciInfo->MemInfo.memw.Base;
	}
	else
	{
		writeInfo->TransInfo.bInSection = FALSE;
		writeBuffer = MESSAGE_TO_REALDATA_BUF(msg);

	}
	//write info of data
	writeInfo->TransInfo.DataType = dataType;
	writeInfo->TransInfo.DataLength = ulDataSize;
	//init a message
	msg.MessageType = LPC_NEW_MESSAGE;
	msg.MessageSize = sizeof(PORT_MESSAGE);
	msg.VirtualRangesOffset = 0;
	msg.DataSize = MAX_DATA_SIZE;
	//copy data
	if (data != NULL)
	{
		_try
		{
			RtlCopyMemory( writeBuffer , data , ulDataSize);
		}
		_except(EXCEPTION_EXECUTE_HANDLER)
		{
			KdPrint(("occur a error while copying data to write-buffer , error : %x \n",GetExceptionCode()));
			return GetExceptionCode();
		}
		
	}
	st = NtRequestWaitReplyPort( pciInfo->hDataPort, &msg , pReplyMsg);

	//st = NtRequestPort( pciInfo->hDataPort , &msg);

	return st;
}

NTSTATUS ClientProc(void)
{
	//connection info
	CONNECTION_INFO ciInfo;
	NTSTATUS st;

	//using get process info
	PPROCESS_COLLECTION ppc;
	//count of process
	ULONG		  ulProcessCount;
	//message of reply
	PORT_MESSAGE ReplyMsg;


	RtlZeroMemory( &ciInfo , sizeof(CONNECTION_INFO));
	RtlZeroMemory( &ReplyMsg , sizeof(PORT_MESSAGE));

	st = ConnectPort ( &ciInfo);
	if (!NT_SUCCESS(st))
	{
		return st;
	}

	//start transfer message!!
	//-->NO.1
	st = RequstMsg ( &ciInfo , (PVOID)ASK_METTING, DATA_TRANSFER_CHATTING ,strlen (ASK_METTING) , &ReplyMsg);
	if (!NT_SUCCESS(st))
	{
		KdPrint(("Ask meeting failed ERROR CODE : %X \n",st));
		return st;
	}
	ProcessMsg ( &ciInfo , &ReplyMsg);
	RtlZeroMemory( &ReplyMsg , sizeof(PORT_MESSAGE));
	//<--NO.1



	//-->NO.2
	st = RequstMsg ( &ciInfo , (PVOID)ASK_TIME , DATA_TRANSFER_CHATTING, strlen(ASK_TIME) , &ReplyMsg);
	if (!NT_SUCCESS(st))
	{
		KdPrint(("Ask time failed ERROR CODE : %X \n",st));
		return st;
	}
	ProcessMsg ( &ciInfo , &ReplyMsg);
	RtlZeroMemory( &ReplyMsg , sizeof(PORT_MESSAGE));
	//<--NO.2



	//-->NO.3
	//try to transfer info of process
	st  = QueryProcess ( NULL , &ulProcessCount);
	if (!ulProcessCount)
	{
		KdPrint(("query prcoess count failed \n"));
		return st;
	}

	//allocate buffer for transferring
	ppc = (PPROCESS_COLLECTION)ExAllocatePool( NonPagedPool,ulProcessCount*sizeof(PROCESS_INFO)+4);
	if (ppc == NULL)
	{
		KdPrint(("cannot allocate pool \n"));
		return STATUS_UNSUCCESSFUL;
	}
	//clean buffer
	RtlZeroMemory(ppc , ulProcessCount*sizeof(PROCESS_INFO)+4);
	//try to get exactly data
	st = QueryProcess ( ppc->piInfo , &ulProcessCount);
	if (!NT_SUCCESS(st))
	{
		KdPrint(("query info of process failed \n"));
		return st;
	}
	//copy count of process, or server will print nothing
	ppc->dwCount = ulProcessCount;
	//send it to server
	st = RequstMsg ( &ciInfo , (PVOID)ppc , DATA_TRANSFER_PROCESSS , sizeof(PROCESS_INFO)*ulProcessCount+4 , &ReplyMsg);
	if (!NT_SUCCESS(st))
	{
		KdPrint(("transfer info of process failed ERROR CODE : %X\n",st));
		return st;
	}
	ProcessMsg ( &ciInfo , &ReplyMsg);
	RtlZeroMemory( &ReplyMsg , sizeof(PORT_MESSAGE));
	//<--NO.3


	//-->NO.4
	//notify shutdown connection
	st = RequstMsg ( &ciInfo , NULL , DATA_NOTIFY_SHUTDOWN , 0 , &ReplyMsg);
	if (!NT_SUCCESS(st))
	{
		KdPrint(("send shutdown message failed ERROR CODE : %X \n",st));
		return st;
	}
	ProcessMsg ( &ciInfo , &ReplyMsg);
	//<--NO.4
	//stop transferring message !!

	//close handle of port
	ZwClose ( ciInfo.hDataPort);
	//return
	return STATUS_SUCCESS;
}


void OnUnload ( PDRIVER_OBJECT driver )
{
	KeWaitForSingleObject ( peThread,
							Executive,
							KernelMode,
							FALSE,
							NULL);
	ObDereferenceObject((PVOID)peThread);
	return ;
	
}

NTSTATUS DriverEntry( PDRIVER_OBJECT driver , PUNICODE_STRING pusRegPath )
{
	NTSTATUS st;
	st = InitUnexportedAddress ();
	if (NT_SUCCESS(st))
	{
		HANDLE hThread;
		OBJECT_ATTRIBUTES obj;
		
		InitializeObjectAttributes ( &obj , NULL ,OBJ_KERNEL_HANDLE , NULL ,NULL);
		st = PsCreateSystemThread ( &hThread,
									THREAD_ALL_ACCESS,
									&obj,
									ZwCurrentProcess(),
									NULL,
									(PKSTART_ROUTINE)ClientProc,
									NULL);
		if (NT_SUCCESS(st))
		{
			st = ObReferenceObjectByHandle ( hThread,
											 THREAD_ALL_ACCESS,
											 *PsThreadType,
											 KernelMode,
											 (PVOID*)&peThread,
											 NULL);
			if (NT_SUCCESS(st))
			{
				ZwClose (hThread);
			}
		}
	}
	driver->DriverUnload = OnUnload;
	return STATUS_SUCCESS;
}