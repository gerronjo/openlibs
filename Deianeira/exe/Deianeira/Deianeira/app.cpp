#include "stdafx.h"
#include "app.h"


PAPP_MSG_INFO pAppMsgInfo,pAppMsgInfoHead;

PNtSystemDebugControl NtSystemDebugControl; 
PNtReadVirtualMemory NtReadVirtualMemory;


PCLIENT_ID app_getMsgClientID(PVOID pWin32Thread)
{
	DWORD dwRet;
	MEMORY_CHUNKS QueryBuff;
	PULONG pEThread = new ULONG;

	QueryBuff.ulAddress = (ULONG)pWin32Thread;
	QueryBuff.pBuffer = pEThread;
	QueryBuff.ulSize = sizeof(ULONG);

	NtSystemDebugControl(
		SysDbgCopyMemoryChunks_0,
		&QueryBuff,
		sizeof(MEMORY_CHUNKS),
		NULL,
		0,
		&dwRet);

	//已经获取了ETHREAD的地址，偏移加上0x1ec，是CLIENTID内容
	PCLIENT_ID pClientID = new CLIENT_ID;

	QueryBuff.ulAddress = (ULONG)(*pEThread) + 0x1ec;
	QueryBuff.pBuffer = pClientID;
	QueryBuff.ulSize = sizeof(CLIENT_ID);

	NtSystemDebugControl(
		SysDbgCopyMemoryChunks_0,
		&QueryBuff,
		sizeof(MEMORY_CHUNKS),
		NULL,
		0,
		&dwRet);

	return pClientID;
}

void app_updateMsgInfo(HANDLE hMsg,LPWSTR lpType,ULONG ulFuncAddr,ULONG ulTID,ULONG ulPID,bool bGlobal){
	 PAPP_MSG_INFO p;

	 p = new APP_MSG_INFO;

	 p->hMsg = hMsg;
	 wcscpy_s(p->Type,sizeof(p->Type),lpType);
	 p->ulFuncAddr = ulFuncAddr;
	 p->ulTID = ulTID;
	 p->ulPID = ulPID;
	 p->bGlobal = bGlobal;
	 p->next = NULL;

	 pAppMsgInfo->next = p;
	 pAppMsgInfo = p;
}


void app_freeMsgInfo(){
	 PAPP_MSG_INFO p;

	 while(pAppMsgInfo)
	 {
		 p = pAppMsgInfo->next;
		 free(pAppMsgInfo);
		 pAppMsgInfo = p;
	 }
}


bool app_getMsgInfo(bool bGetGlobal){
	NTSTATUS status; 
	DWORD dwRet; 
	bool bGlobal;
	SHAREDINFO SharedInfo; 
	PSHAREDINFO pSharedInfo; 
	SERVERINFO ServerInfo; 
	PHANDLEENTRY pHandleEntry; 
	PHOOK_INFO pHookInfo; 
	MEMORY_CHUNKS mc; 
	ULONG ulHandleCount;

	ULONG uWowHandler =(ULONG)GetProcAddress(LoadLibrary(L"user32.dll"), "UserRegisterWowHandlers"); 

	for(ULONG ulAddr = uWowHandler; ulAddr <= uWowHandler + 0x250; ulAddr++) 
	{ 
		if(*(PWORD)ulAddr == 0x40c7 && *(PBYTE)(ulAddr+7) == 0xb8) 
		{ 
			pSharedInfo=(PSHAREDINFO)(*(PDWORD)(ulAddr+8)); 
			break; 
		} 
	}

	NtSystemDebugControl =(PNtSystemDebugControl)GetProcAddress(LoadLibrary(L"ntdll.dll"), "NtSystemDebugControl"); 
	NtReadVirtualMemory =(PNtReadVirtualMemory)GetProcAddress(LoadLibrary(L"ntdll.dll"), "NtReadVirtualMemory"); 

	status =NtReadVirtualMemory(GetCurrentProcess(),pSharedInfo, &SharedInfo, sizeof(SHAREDINFO), &dwRet); 
	if (!NT_SUCCESS(status)) 
	{ 
		return false; 
	} 

	status =NtReadVirtualMemory(GetCurrentProcess(),SharedInfo.psi, &ServerInfo, sizeof(SERVERINFO), &dwRet); 
	if (!NT_SUCCESS(status)) 
	{ 
		return false; 
	} 

	//句柄数目
	ulHandleCount = ServerInfo.cHandleEntries;

	pHandleEntry =(PHANDLEENTRY)VirtualAlloc(NULL, sizeof(HANDLEENTRY)*ulHandleCount, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE); 

	NtReadVirtualMemory(GetCurrentProcess(),SharedInfo.aheList, pHandleEntry, sizeof(HANDLEENTRY)*ulHandleCount,&dwRet); 
	if (!NT_SUCCESS(status)) 
	{ 
		return false; 
	} 

	pHookInfo =(PHOOK_INFO)VirtualAlloc(NULL, sizeof(HOOK_INFO), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE); 

	//缓冲区
	pAppMsgInfo = pAppMsgInfoHead = new APP_MSG_INFO;

	for (ULONG i = 0;i < ulHandleCount;i++) 
	{ 
		if (pHandleEntry[i].bType == 5) 
		{ 
			mc.ulAddress = (ULONG)pHandleEntry[i].phead; 
			mc.ulSize = sizeof(HOOK_INFO); 
			mc.pBuffer = pHookInfo; 
	
			status =NtSystemDebugControl(SysDbgCopyMemoryChunks_0, &mc, sizeof(mc), NULL, 0, &dwRet); 
			if (!NT_SUCCESS(status)) 
			{ 
				VirtualFree(pHookInfo, sizeof(HOOK_INFO), MEM_DECOMMIT); 
				VirtualFree(pHandleEntry, sizeof(HANDLEENTRY)*ulHandleCount, MEM_DECOMMIT); 
				return false; 
			} 

			//获得ClientID
			PCLIENT_ID pClientID = app_getMsgClientID(pHookInfo->Win32Thread);

			//全局消息钩子的判断
			if (pHookInfo->iHookFlags == 1 || pHookInfo->iHookFlags == 3)
			{
				bGlobal = true;
			}
			else
			{
				bGlobal = false;
				//如果只检测全局消息钩子的话
				if (bGetGlobal)
				{
					continue;
				}
			}

			switch(pHookInfo->iHookType) 
			{ 
			case R_WH_MSGFILTER: 
				{
					app_updateMsgInfo(pHookInfo->hHandle,L"WH_MSGFILTER",pHookInfo->OffPfn,pClientID->ulTID,pClientID->ulPID,bGlobal);
				}
				break; 
			case R_WH_JOURNALRECORD: 
				{
					app_updateMsgInfo(pHookInfo->hHandle,L"WH_JOURNALRECORD",pHookInfo->OffPfn,pClientID->ulTID,pClientID->ulPID,bGlobal);
				}
				break; 
			case R_WH_JOURNALPLAYBACK: 
				{
					app_updateMsgInfo(pHookInfo->hHandle,L"WH_JOURNALPLAYBACK",pHookInfo->OffPfn,pClientID->ulTID,pClientID->ulPID,bGlobal);
				}
				break; 
			case R_WH_KEYBOARD: 
				{
					app_updateMsgInfo(pHookInfo->hHandle,L"WH_KEYBOARD",pHookInfo->OffPfn,pClientID->ulTID,pClientID->ulPID,bGlobal);
				}
				break; 
			case R_WH_GETMESSAGE: 
				{
					app_updateMsgInfo(pHookInfo->hHandle,L"WH_GETMESSAGE",pHookInfo->OffPfn,pClientID->ulTID,pClientID->ulPID,bGlobal);
				}
				break; 
			case R_WH_CALLWNDPROC: 
				{
					app_updateMsgInfo(pHookInfo->hHandle,L"WH_CALLWNDPROC",pHookInfo->OffPfn,pClientID->ulTID,pClientID->ulPID,bGlobal);
				}
				break; 
			case R_WH_CBT: 
				{
					app_updateMsgInfo(pHookInfo->hHandle,L"WH_CBT",pHookInfo->OffPfn,pClientID->ulTID,pClientID->ulPID,bGlobal);
				}
				break; 
			case R_WH_SYSMSGFILTER: 
				{
					app_updateMsgInfo(pHookInfo->hHandle,L"WH_SYSMSGFILTER",pHookInfo->OffPfn,pClientID->ulTID,pClientID->ulPID,bGlobal);
				}
				break; 
			case R_WH_MOUSE: 
				{
					app_updateMsgInfo(pHookInfo->hHandle,L"WH_MOUSE",pHookInfo->OffPfn,pClientID->ulTID,pClientID->ulPID,bGlobal);
				}
				break; 
			case R_WH_HARDWARE: 
				{
					app_updateMsgInfo(pHookInfo->hHandle,L"WH_HARDWARE",pHookInfo->OffPfn,pClientID->ulTID,pClientID->ulPID,bGlobal);
				}
				break; 
			case R_WH_DEBUG: 
				{
					app_updateMsgInfo(pHookInfo->hHandle,L"WH_DEBUG",pHookInfo->OffPfn,pClientID->ulTID,pClientID->ulPID,bGlobal);
				}
				break; 
			case R_WH_SHELL: 
				{
					app_updateMsgInfo(pHookInfo->hHandle,L"WH_SHELL",pHookInfo->OffPfn,pClientID->ulTID,pClientID->ulPID,bGlobal);
				}
				break; 
			case R_WH_FOREGROUNDIDLE: 
				{
					app_updateMsgInfo(pHookInfo->hHandle,L"WH_FOREGROUNDIDLE",pHookInfo->OffPfn,pClientID->ulTID,pClientID->ulPID,bGlobal);
				}
				break; 
			case R_WH_CALLWNDPROCRET: 
				{
					app_updateMsgInfo(pHookInfo->hHandle,L"WH_CALLWNDPROCRET",pHookInfo->OffPfn,pClientID->ulTID,pClientID->ulPID,bGlobal);
				}
				break; 
			case R_WH_KEYBOARD_LL: 
				{
					app_updateMsgInfo(pHookInfo->hHandle,L"WH_KEYBOARD_LL",pHookInfo->OffPfn,pClientID->ulTID,pClientID->ulPID,bGlobal);
				}
				break; 
			case R_WH_MOUSE_LL: 
				{
					app_updateMsgInfo(pHookInfo->hHandle,L"WH_MOUSE_LL",pHookInfo->OffPfn,pClientID->ulTID,pClientID->ulPID,bGlobal);
				}
				break; 
			} 
		}
	}

	pAppMsgInfo = pAppMsgInfoHead->next;

	VirtualFree(pHookInfo, sizeof(HOOK_INFO), MEM_DECOMMIT); 
	VirtualFree(pHandleEntry, sizeof(HANDLEENTRY) *ServerInfo.cHandleEntries, MEM_DECOMMIT); 

	return true;
}