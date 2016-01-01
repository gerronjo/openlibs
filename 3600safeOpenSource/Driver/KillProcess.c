//**************************************************************************
//*
//*          A盾电脑防护 website:http://www.3600safe.com/
//*        
//*文件说明：
//*         结束进程模块
//**************************************************************************
#include "killprocess.h"


PETHREAD NTAPI GetNextProcessThread(IN PEPROCESS Process,
	IN PETHREAD Thread OPTIONAL)
{
	PETHREAD FoundThread = NULL;
	PLIST_ENTRY ListHead, Entry;

	PAGED_CODE();

	if (!RMmIsAddressValid(Process))
	{
		return NULL;
	}
	if (Thread)
	{
		if (!RMmIsAddressValid(Thread))
		{
			return NULL;
		}
	}
	if (Thread)
	{
		//  Entry = Thread->ThreadListEntry.Flink;;//   +0x22c ThreadListEntry  : _LIST_ENTRY
		Entry = (PLIST_ENTRY)((ULONG)(Thread)+ThreadProc);
		Entry=Entry->Flink;
	}
	else
	{
		Entry = (PLIST_ENTRY)((ULONG)(Process)+ThreadListHead);//+0x190 ThreadListHead   : _LIST_ENTRY
		Entry = Entry->Flink; 
	}
	// ListHead = &Process->ThreadListHead;
	ListHead = (PLIST_ENTRY)((ULONG)Process + ThreadListHead);
	while (ListHead != Entry)
	{
		//   FoundThread = CONTAINING_RECORD(Entry, ETHREAD, ThreadListEntry);
		FoundThread = (PETHREAD)((ULONG)Entry - ThreadProc);
		//    if (ObReferenceObjectSafe(FoundThread)) break;
		if (ObReferenceObject(FoundThread)) break;
		FoundThread = NULL;
		Entry = Entry->Flink;
	}
	if (Thread) ObDereferenceObject(Thread);
	return FoundThread;
}
NTSTATUS TerminateProcess(IN ULONG EProcess,BOOL SysVersionCheck)
{
	NTSTATUS          Status;
	PETHREAD          Thread;
	PEPROCESS         Process;
	ULONG PsLookupProcess;
	UNICODE_STRING PsLookupProcessString;


	Status = STATUS_UNSUCCESSFUL;

	if (DebugOn)
		DbgPrint( "( 0x%x ) ( 0x%x )", UniqueProcessIdOffset,ActiveProcessLinksOffset );

	Process = (PEPROCESS)EProcess;
	if (DebugOn)
		DbgPrint( "TerminateProcess( 0x%08x )", Process );

	__try
	{
		for (Thread = GetNextProcessThread(Process, NULL );
			Thread != NULL;
			Thread = GetNextProcessThread(Process, Thread ))
		{
			Status = STATUS_SUCCESS;

			if (SysVersionCheck == WINDOWS_VERSION_XP)
			{
				//DbgPrint("WINDOWS_VERSION_XP");

				Status = (*PspTerminateThreadByPointer_XP)( Thread, 0);  //XP下

			}else if (SysVersionCheck == WINDOWS_VERSION_2K3 || SysVersionCheck == WINDOWS_VERSION_2K3_SP1_SP2)
			{
				//DbgPrint("WINDOWS_2003");

				Status = (*PspTerminateThreadByPointer_K)(
					Thread,
					0,
					FALSE);  //2003下

			}else if (SysVersionCheck == WINDOWS_VERSION_VISTA_2008 ||
				SysVersionCheck == WINDOWS_VERSION_7)
			{
				//DbgPrint("WINDOWS_VERSION_VISTA_2008");

				Status = (*PspTerminateThreadByPointer_K)( 
					Thread,
					0,
					TRUE);  //vista下，煮一个参数为　ＴＲＵＥ
			}
			//DbgPrint( "PspTerminateThreadByPointer():\t0x%08x", Status );

		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint( "EXCEPTION_EXECUTE_HANDLER" );
		Status = GetExceptionCode();
	}
	if (DebugOn)
		DbgPrint("TerminateProcess() end:\t0x%08x", Status );
	return Status;
}
PVOID Search_PspTerminateThreadByPointer(BOOL SysVersionCheck)
{
	PUCHAR cPtr;
	BOOL bRetOK = FALSE;
	ULONG addret;
	ULONG ulReloadAddress;

	if (DebugOn)
		DbgPrint( "PsTerminateSystemThread:\t0x%08x", PsTerminateSystemThread);

	_try
	{
		for (cPtr = (PUCHAR)PsTerminateSystemThread;  cPtr < (PUCHAR)PsTerminateSystemThread + PAGE_SIZE; cPtr++)
		{
			//DbgPrint("search....");

			if (SysVersionCheck == WINDOWS_VERSION_XP || 
				SysVersionCheck == WINDOWS_VERSION_VISTA_2008 || 
				SysVersionCheck == WINDOWS_VERSION_2K3_SP1_SP2 ||
				SysVersionCheck == WINDOWS_VERSION_7)
			{
				//DbgPrint("WINDOWS_VERSION_XP || WINDOWS_VERSION_VISTA_2008 ||　WINDOWS_VERSION_2K3_SP1_SP2");

				if (*cPtr == 0xE8 && *(PUSHORT)(cPtr + 5) == 0xC25D) 
				{
					addret = (*(PULONG)(cPtr + 1) + (ULONG)cPtr + 5);
					if (DebugOn)
						DbgPrint( "PspTerminateThreadByPointer:\t0x%08x", (ULONG)addret);
					ulReloadAddress = addret - SystemKernelModuleBase + ImageModuleBase;
					bRetOK = TRUE;
					break;
				}

			}else if (SysVersionCheck == WINDOWS_VERSION_2K3)
			{
				//DbgPrint("WINDOWS_VERSION_2K3");

				if (*cPtr == 0xE8 && *(PUSHORT)(cPtr + 5) == 0x04C2)
				{
					addret = (*(PULONG)(cPtr + 1) + (ULONG)cPtr + 5);
					ulReloadAddress = addret - SystemKernelModuleBase + ImageModuleBase;
					if (DebugOn)
						DbgPrint( "PspTerminateThreadByPointer:\t0x%08x", (ULONG)addret);
					bRetOK = TRUE;
					break;
				}
			}

		}
	}__except(EXCEPTION_EXECUTE_HANDLER)
	{
		if (DebugOn)
			DbgPrint("Search_PspTerminateThreadByPointer Error \r\n");
	}
	if (bRetOK == TRUE)
	{
		return ulReloadAddress;
	}
	return bRetOK;
}
BOOL IniKillObject()
{
	WIN_VER_DETAIL WinVer;
	BOOL bRetOK = FALSE;

	WinVer=GetWindowsVersion();

	if (WinVer == WINDOWS_VERSION_XP)
	{
		//初始化各个变量
		ThreadProc = 0x22C;
		ThreadListHead = 0x190;

		UniqueProcessIdOffset=0x084;
		ActiveProcessLinksOffset=0x088;

		PspTerminateThreadByPointer_XP = Search_PspTerminateThreadByPointer(WinVer);
		if (PspTerminateThreadByPointer_XP != FALSE)
		{
			bRetOK = TRUE;
		}

	}else if (WinVer == WINDOWS_VERSION_2K3 || 
		WinVer == WINDOWS_VERSION_2K3_SP1_SP2 || 
		WinVer == WINDOWS_VERSION_VISTA_2008 ||
		WinVer == WINDOWS_VERSION_7)   //2003以上版本，PspTerminateThreadByPointer都是3个参数
	{
		PspTerminateThreadByPointer_K = Search_PspTerminateThreadByPointer(WinVer);
		if (PspTerminateThreadByPointer_K != FALSE)
		{
			bRetOK = TRUE;

			if (WinVer == WINDOWS_VERSION_2K3)
			{
				if (DebugOn)
					DbgPrint("WINDOWS_VERSION_2K3\r\n");
				ThreadProc = 0x234;
				ThreadListHead = 0x170;

				UniqueProcessIdOffset=0x084;
				ActiveProcessLinksOffset=0x088;

			}
			else if (WinVer == WINDOWS_VERSION_2K3_SP1_SP2)
			{
				if (DebugOn)
					DbgPrint("WINDOWS_VERSION_2K3_SP1_SP2\r\n");
				ThreadProc = 0x224;
				ThreadListHead = 0x180;

				UniqueProcessIdOffset=0x094;
				ActiveProcessLinksOffset=0x098;

			}
			else if (WinVer == WINDOWS_VERSION_VISTA_2008)
			{
				if (DebugOn)
					DbgPrint("WINDOWS_VERSION_VISTA_2008!!\r\n");

				ThreadProc = 0x248;
				ThreadListHead = 0x168;

				UniqueProcessIdOffset=0x09c;
				ActiveProcessLinksOffset=0x0a0;
			}
			else if (WinVer == WINDOWS_VERSION_7)
			{
				if (DebugOn)
					DbgPrint("WINDOWS_VERSION_7!!\r\n");

				ThreadProc = 0x268;
				ThreadListHead = 0x188;

				UniqueProcessIdOffset=0x0b4;
				ActiveProcessLinksOffset=0x0b8;
			}
		}

	}else
	{
		if (DebugOn)
			DbgPrint("Unkown!!\r\n");
		bRetOK = FALSE;
	}
	return bRetOK;
}
NTSTATUS KillProcess(ULONG ulEprocess)
{
	WIN_VER_DETAIL WinVer;

	if (!ulEprocess ||
		!RMmIsAddressValid(ulEprocess) ||
		!IsExitProcess(ulEprocess))
	{
		return STATUS_UNSUCCESSFUL;
	}
	WinVer = GetWindowsVersion();
	if (IniKillObject())
	{
		return TerminateProcess(ulEprocess,WinVer); //结束啊
	}
	return STATUS_UNSUCCESSFUL;
}