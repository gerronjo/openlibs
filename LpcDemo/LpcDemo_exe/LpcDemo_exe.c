#include "..\inc\Lpc.h"
#include <Windows.h>
#include <strsafe.h>
#include <stdio.h>


#define  FILE_NAME_A "\\fuck.txt"
#define  FILE_NAME_W L"\\fuck.txt"

//Using for init routine address
_CreatePort				NtCreatePort;
_ListenPort				NtListenPort;
_AcceptConnectPort		NtAcceptConnectPort;
_CompleteConnectPort	NtCompleteConnectPort;
_ReplyPort				NtReplyPort;
_ReplyWaitReceivePort	NtReplyWaitReceivePort;
_ReplyWaitReceivePortEx	NtReplyWaitReceivePortEx;
_InitUnicodeString		RtlInitUnicodeString;

//handle of connect port
HANDLE hConnectPort;
//
HANDLE hNotifyEvent;


BOOLEAN InitProcAddress( void )
{
	//init unexported functions

	HMODULE hModule;
	hModule = GetModuleHandleW(L"ntdll.dll");
	if ( hModule != NULL )
	{
		NtCreatePort = (_CreatePort )GetProcAddress ( hModule , "NtCreatePort");
		NtListenPort = (_ListenPort )GetProcAddress ( hModule , "NtListenPort");
		NtAcceptConnectPort = (_AcceptConnectPort )GetProcAddress ( hModule , "NtAcceptConnectPort");
		NtCompleteConnectPort = (_CompleteConnectPort )GetProcAddress ( hModule , "NtCompleteConnectPort");
		NtReplyPort = (_ReplyPort )GetProcAddress ( hModule , "NtReplyPort");
		NtReplyWaitReceivePort = (_ReplyWaitReceivePort )GetProcAddress ( hModule , "NtReplyWaitReceivePort");
		NtReplyWaitReceivePortEx = (_ReplyWaitReceivePortEx)GetProcAddress ( hModule , "NtReplyWaitReceivePortEx");
		RtlInitUnicodeString = (_InitUnicodeString)GetProcAddress ( hModule , "RtlInitUnicodeString");

		printf ("NtCreatePort :             %X \n",NtCreatePort);
		printf ("NtListenPort :             %X \n",NtListenPort);
		printf ("NtAcceptConnectPort :      %X \n",NtAcceptConnectPort);
		printf ("NtCompleteConnectPort :    %X \n",NtCompleteConnectPort);
		printf ("NtReplyPort :              %X \n",NtReplyPort);
		printf ("NtReplyPortReceivePort :   %X \n",NtReplyWaitReceivePort);
		printf ("NtReplyWaitReceivePortEx : %X \n",NtReplyWaitReceivePortEx);
		printf ("RtlInitUnicodeString :     %X \n",RtlInitUnicodeString);

		return TRUE;
	}
	return FALSE;
}

BOOLEAN AnwserMsg( PVOID data , DWORD dataSize , BYTE dataType , PCONNECTION_INFO pci , PPORT_MESSAGE ppm)
{
	PORT_MESSAGE replyMsg;
	NTSTATUS st;
	PVOID	writeBuffer = NULL;
	PMESSAGE_DATA_INFO   writeInfo = NULL;

	RtlZeroMemory(&replyMsg , sizeof(PORT_MESSAGE));
	//update reply message
	RtlCopyMemory ( &replyMsg , ppm ,0x20);
	//get buffer that storing information of data
	writeInfo = MESSAGE_TO_DATAINFO_BUF(replyMsg);

	if (dataSize > MAX_SMALL_DATA_SIZE)
	{
		//write to section
		writeBuffer = pci->MemInfo.memw.Base;
		writeInfo->TransInfo.bInSection =TRUE;
		
	}
	else
	{
		//write to buffer of message
		writeBuffer = MESSAGE_TO_REALDATA_BUF(replyMsg);
		writeInfo->TransInfo.bInSection =FALSE;
	}
	//write info of data that will be transfered
	writeInfo->TransInfo.DataLength = dataSize;
	writeInfo->TransInfo.DataType = dataType;
	
	if (data != NULL)
	{
		//copy exactly data
		RtlCopyMemory( writeBuffer , data ,dataSize);
	}

	//reply port
	st = NtReplyPort( pci->hDataPort , &replyMsg);
	
	return NT_SUCCESS(st);
}

//process messages from client
void ProcessMsg( PCONNECTION_INFO pcnInfo)
{
	CONNECTION_INFO ciInfo;
	PMESSAGE_DATA_INFO pmdi;
	NTSTATUS st;
	PORT_MESSAGE msg;

	ciInfo = *pcnInfo;
	_try
	{
		SetEvent ( hNotifyEvent);
	}
	_except(EXCEPTION_EXECUTE_HANDLER)
	{
		/**/
	}
	while(1)
	{
		PVOID readBuffer;
		

		pmdi = NULL;
		readBuffer = NULL;
		RtlZeroMemory( &msg , sizeof(PORT_MESSAGE));

		st = NtReplyWaitReceivePort( ciInfo.hDataPort,
									   NULL,
									   NULL,
									   &msg);

		if (!NT_SUCCESS(st))
		{
			continue;
		}
		//get the pointer of buffer that for reading
		pmdi =MESSAGE_TO_DATAINFO_BUF(msg);
		if (pmdi->TransInfo.bInSection)
		{
			readBuffer = ciInfo.MemInfo.memr.Base;
		}
		else
		{
			readBuffer = MESSAGE_TO_REALDATA_BUF(msg);
		}

		if ( readBuffer == NULL)
		{
			continue;
		}
		//to verify type of data
		switch( pmdi->TransInfo.DataType)
		{
			//to shutdown this connection
			case DATA_NOTIFY_SHUTDOWN: 
				{
					AnwserMsg ( NULL , 0 , DATA_NOTIFY_SHUTDOWN , &ciInfo , &msg);
					CloseHandle(ciInfo.hDataPort);
					return ;
				}
			
			//the data transfered contains chatting messing
			case DATA_TRANSFER_CHATTING:
				{
					char* fuckStr;
					fuckStr = (char*)readBuffer;
					printf ("%s  :\n   %s \n",CLIENTNAME ,fuckStr);
					if (strcmp ( fuckStr , ASK_METTING)==0)
					{
						AnwserMsg ( REPLY_METTING,
									strlen (REPLY_METTING),
									DATA_TRANSFER_CHATTING,
									&ciInfo,
									&msg);
					}
					else
					{
						AnwserMsg ( REPLY_TIME,
									strlen (REPLY_TIME),
									DATA_TRANSFER_CHATTING,
									&ciInfo,
									&msg);
					}
					break;
				}
			//the data transfered contains information of all process that detected by driver
			case DATA_TRANSFER_PROCESSS:
				{
					//using for outputting information
					PPROCESS_COLLECTION ppc;
					PPROCESS_INFO ppi;
					ULONG i;
					//using for opening file
					HANDLE hf;
					WCHAR fPath[MAX_PATH]={0};
					DWORD dwRet;

					ppc = (PPROCESS_COLLECTION)readBuffer;
					ppi = ppc->piInfo;
					//output each process information
					_try
					{
						printf ("%s  :\n",CLIENTNAME);
						for ( i=0 ; i<ppc->dwCount ; i++)
						{
							printf ("    %d : %s \n" , ppi[i].ulPid , ppi[i].wchName);
						}
					}
					_except(EXCEPTION_EXECUTE_HANDLER)
					{
						//end printing procedure
						printf("occur a exception while reading data , error: %x \n",GetExceptionCode());
						continue;
					}

					//try to read file which placed in current directory
					
					dwRet = GetCurrentDirectory( MAX_PATH , fPath);
					if (dwRet>0)
					{
						DWORD dwSize;

						if (wcscat ( fPath ,FILE_NAME_W) == NULL)
						{
							continue;
						}

						hf = CreateFile( fPath,
										 FILE_ALL_ACCESS,
										 0,
										 NULL,
										 OPEN_EXISTING,
										 FILE_ATTRIBUTE_NORMAL,
										 NULL);
						if (hf == INVALID_HANDLE_VALUE)
						{
							printf ("cannot open file fuck.txt \n");
							continue;
						}

						dwSize = GetFileSize ( hf , NULL);
						if ( dwSize<= 0)
						{
							printf ("cannot get size of fuck.txt \n");
							continue;
						}
						else if (dwSize < 0x10000)
						{
							BOOLEAN bDone;
							DWORD	dwReaded;
							PVOID	pTextBuffer;
							//synchronization reading

							pTextBuffer = (PVOID)VirtualAlloc ( NULL ,
																dwSize + 0x100,
																MEM_COMMIT ,
																PAGE_READWRITE);

							if (pTextBuffer == NULL)
							{
								printf ("can not allocate a buffer to store text data \n");
								continue;
							}

							bDone = ReadFile ( hf ,
											   pTextBuffer ,
											   dwSize ,
											   &dwReaded ,
											   NULL );
							if (!bDone)
							{
								printf ("read file failed \n");
								continue;
							}
							else if (dwReaded < dwSize)
							{
								printf ("only read %d/%d \n" , dwReaded , dwSize);
								continue;
							}
							AnwserMsg ( pTextBuffer ,
										dwReaded ,
										DATA_TRANSFER_PROCESSS ,
										&ciInfo ,
										&msg);
						}
					}
					else
						printf ("get current directory failed \n");
					break;
				}
			default: continue;
		}
	}
	
}

//process each connection , and return a boolean value to indicate whether the connection is established
BOOLEAN ProcessConnection ( PORT_MESSAGE* msg , PCONNECTION_INFO pcnInfo)
{
	PMESSAGE_DATA_INFO mdi;
	NTSTATUS st;
	HANDLE hDataPort = NULL;
	HANDLE hSharedMem = NULL;
	PORT_SECTION_READ psr;
	PORT_SECTION_WRITE psw;

	mdi = MESSAGE_TO_DATAINFO_BUF((*msg));

	if (mdi->bSectionUsed)
	{
		

		memset ( &psr , 0 , sizeof(PORT_SECTION_READ));
		memset ( &psw , 0 , sizeof(PORT_SECTION_WRITE));

		hSharedMem = CreateFileMapping( INVALID_HANDLE_VALUE ,
										NULL ,
										PAGE_READWRITE ,
										0 ,
										0x10000 ,
										NULL);

		psw.Length = sizeof(PORT_SECTION_WRITE);
		psw.SectionHandle = hSharedMem;
		psw.SectionOffset = 0;
		psw.ViewSize = 0x10000;

		psr.Length = sizeof(PORT_SECTION_READ);
		st = NtAcceptConnectPort(&hDataPort,
								 NULL,
								 msg,
								 TRUE,
								 &psw,
								 &psr);
	}
	else
	{
		st = NtAcceptConnectPort(&hDataPort,
								 NULL,
								 msg,
								 TRUE,
								 NULL,
								 NULL);
	}
	if (NT_SUCCESS(st))
	{
		pcnInfo ->hDataPort = hDataPort;
		pcnInfo ->MemInfo.memr.Base = psr.ViewBase;
		pcnInfo->MemInfo.memr.Size = psr.ViewSize;
		pcnInfo->MemInfo.memw.Base = psw.ViewBase;
		pcnInfo->MemInfo.memw.Size = psw.ViewSize;

		st = NtCompleteConnectPort( hDataPort);
		if (NT_SUCCESS(st))
		{
			printf ("Connection info :\n PORT : %X \n Read Base : %X   Read Size : %X \n Write Base : %X   Write Size : %X \n\n ", hDataPort,psr.ViewBase,psr.ViewSize,psw.ViewBase,psw.ViewSize);
		}
		return NT_SUCCESS(st);
	}
	return FALSE;
}

void ServerProc(void)
{
	NTSTATUS st;
	OBJECT_ATTRIBUTES obj;
	UNICODE_STRING usPortName;
	PORT_MESSAGE pmMsg;
	DWORD  dwThreaId;

	RtlInitUnicodeString( &usPortName , SERVERNAME_W);
	InitializeObjectAttributes( &obj ,
								&usPortName,
								0,
								0,
								NULL);
	st = NtCreatePort ( &hConnectPort,
						&obj,
						MAX_DATA_SIZE,
						MAX_MESSAGE_SIZE,
						0);
	if (!NT_SUCCESS(st))
	{
		printf("failed to create port 0x%08X \n",st);
		return ;
	}

	RtlZeroMemory(&pmMsg,sizeof(pmMsg));
	while(1)
	{
		BOOLEAN bConnected;
		CONNECTION_INFO cnInfo;
		
		Sleep (100);
		bConnected = FALSE;
		memset ( &cnInfo , 0 , sizeof(CONNECTION_INFO));

		st = NtListenPort ( hConnectPort , &pmMsg);
		if (!NT_SUCCESS(st) || pmMsg.MessageType!=LPC_CONNECTION_REQUEST)
				continue;
		
		bConnected = ProcessConnection ( &pmMsg , &cnInfo);
		if(!bConnected)
			continue;
		CreateThread ( NULL,
					   0,
					   (LPTHREAD_START_ROUTINE)ProcessMsg ,
					   (LPVOID)&cnInfo,
					   0,
					   &dwThreaId);
		//wait until the new thread copied the param that passed by this function
		WaitForSingleObject ( hNotifyEvent , INFINITE);
	}
}

int main(void)
{
	HANDLE hThread;
	DWORD dwThreadId;
	char chInput;
	BOOLEAN bOk;


	printf ("please type Y to exit! \n\n");
	bOk = InitProcAddress ();
	if(!bOk)
		return;
	//create a event which using for thread sync
	hNotifyEvent = CreateEvent( NULL , FALSE , FALSE ,NULL);
	if (hNotifyEvent == INVALID_HANDLE_VALUE)
	{
		printf ("cannot create the event , exiting now\n");
		return ;
	}
	hThread = CreateThread ( NULL,
							 0,
							 (LPTHREAD_START_ROUTINE)ServerProc ,
							 NULL,
							 0,
							 &dwThreadId);
_loop:
	chInput = getchar();
	if ( chInput == 'Y')
	{
		return ;
	}
	else goto _loop;
}