//**************************************************************************
//*
//*          A盾电脑防护 website:http://www.3600safe.com/
//*        
//*文件说明：
//*         shadow ssdt hook检查
//**************************************************************************
#include "ShadowSSDT.h"
#include "SDTShadowRestore.h"


VOID ShadowSSDTHookCheck(PSHADOWSSDTINFO ShadowSSDTInfo)
{
	ULONG ulReloadShadowSSDTAddress;
	ULONG ulOldShadowSSDTAddress;
	ULONG ulCodeSize;
	ULONG ulHookFunctionAddress;
	PUCHAR ulTemp;
	INSTRUCTION	Inst;
	INSTRUCTION	Instb;
	PUCHAR p;
	ULONG ulHookModuleBase;
	ULONG ulHookModuleSize;
	CHAR lpszHookModuleImage[256];
	CHAR *lpszFunction = NULL;
	int i = 0,count=0;
	WIN_VER_DETAIL WinVer;
	ULONG ulMemoryFunctionBase;

	__try
	{
		for (i=0 ;i<ShadowSSDTTable[1].TableSize ;i++)
		{
			ulReloadShadowSSDTAddress = OriginalShadowServiceDescriptorTable->ServiceTable[i];
			ulOldShadowSSDTAddress = ulReloadShadowSSDTAddress - (ULONG)Win32kImageModuleBase + ulWin32kBase;
			if (RMmIsAddressValid(ulReloadShadowSSDTAddress) &&
				RMmIsAddressValid(ulOldShadowSSDTAddress))
			{
				ulCodeSize = GetFunctionCodeSize(ulOldShadowSSDTAddress);

				if (ulCodeSize == GetFunctionCodeSize(ulReloadShadowSSDTAddress) &&
					memcmp(ulReloadShadowSSDTAddress,ulOldShadowSSDTAddress,ulCodeSize) != NULL)
				{
					//KdPrint(("find inline hook[%d]%08x-%08x",i,ulOldShadowSSDTAddress,ulReloadShadowSSDTAddress));

					//开始扫描hook
					for (p=ulOldShadowSSDTAddress ;p< ulOldShadowSSDTAddress+ulCodeSize; p++)
					{
						//折半扫描，如果前面一半一样，则开始扫描下一半
						if (memcmp(ulReloadShadowSSDTAddress,ulOldShadowSSDTAddress,ulCodeSize/2) == NULL)
						{
							ulCodeSize = ulCodeSize + ulCodeSize/2;
							continue;
						}
						//是否结束？
						if (*p == 0xcc ||
							*p == 0xc2)
						{
							break;
						}
						ulHookFunctionAddress = *(PULONG)(p+1)+(ULONG)(p+5);  //得到hook的地址

						if (!RMmIsAddressValid(ulHookFunctionAddress))
						{
							continue;
						}
						//KdPrint(("ulHookFunctionAddress:%08x\r\n",ulHookFunctionAddress));

						ulTemp = NULL;
						get_instruction(&Inst,p,MODE_32);
						switch (Inst.type)
						{
						case INSTRUCTION_TYPE_JMP:
							if(Inst.opcode==0xFF&&Inst.modrm==0x25)
							{
								//DIRECT_JMP
								ulTemp = Inst.op1.displacement;
							}
							else if (Inst.opcode==0xEB)
							{
								ulTemp = (ULONG)(p+Inst.op1.immediate);
							}
							else if(Inst.opcode==0xE9)
							{
								//RELATIVE_JMP;
								ulTemp = (ULONG)(p+Inst.op1.immediate);
							}
							break;
						case INSTRUCTION_TYPE_CALL:
							if(Inst.opcode==0xFF&&Inst.modrm==0x15)
							{
								//DIRECT_CALL
								ulTemp = Inst.op1.displacement;
							}
							else if (Inst.opcode==0x9A)
							{
								ulTemp = (ULONG)(p+Inst.op1.immediate);
							}
							else if(Inst.opcode==0xE8)
							{
								//RELATIVE_CALL;
								ulTemp = (ULONG)(p+Inst.op1.immediate);
							}
							break;
						case INSTRUCTION_TYPE_PUSH:
							if(!RMmIsAddressValid((PVOID)(p)))
							{
								break;
							}
							get_instruction(&Instb,(BYTE*)(p),MODE_32);
							if(Instb.type == INSTRUCTION_TYPE_RET)
							{
								//StartAddress+len-inst.length-instb.length;
								ulTemp = Instb.op1.displacement;
							}
							break;
						}
						if (ulTemp &&
							RMmIsAddressValid(ulTemp))   //hook的地址也要有效才可以哦
						{
							//KdPrint(("ulTemp:%08x\r\n",ulTemp));

							if (ulTemp > ulWin32kBase &&
								ulTemp < ulWin32kBase+ulWin32kSize)   //太近的跳也不是
							{
								continue;
							}

// 							if (ulTemp < ulWin32kBase)
// 							{
// 								continue;
// 							}
							if (*(ULONG *)ulTemp == 0x00000000 ||
								*(ULONG *)ulTemp == 0x00000005)
							{
								continue;
							}
							if (DebugOn)
								KdPrint(("hook:%08x",ulTemp));

							ulTemp = ulTemp+0x5;
							//简单处理一下二级跳
							if (*ulTemp == 0xe9 ||
								*ulTemp == 0xe8)
							{
								if (DebugOn)
									KdPrint(("ulTemp == 0xe9"));
								ulTemp = *(PULONG)(ulTemp+1)+(ULONG)(ulTemp+5);
							}
							//这里就是hook了
							memset(lpszHookModuleImage,0,sizeof(lpszHookModuleImage));
							if (!IsAddressInSystem(
								ulTemp,
								&ulHookModuleBase,
								&ulHookModuleSize,
								lpszHookModuleImage))
							{
								memset(lpszHookModuleImage,0,sizeof(lpszHookModuleImage));
								strcat(lpszHookModuleImage,"Unknown4");
								ulHookModuleBase = 0;
								ulHookModuleSize = 0;
							}
							ShadowSSDTInfo->ulCount = count;
							ShadowSSDTInfo->SSDT[count].ulNumber = i;
							ShadowSSDTInfo->SSDT[count].ulMemoryFunctionBase = ulTemp;

							memset(ShadowSSDTInfo->SSDT[count].lpszFunction,0,sizeof(ShadowSSDTInfo->SSDT[count].lpszFunction));
							WinVer = GetWindowsVersion();
							switch (WinVer)
							{
							case WINDOWS_VERSION_XP:
								strncpy(ShadowSSDTInfo->SSDT[count].lpszFunction,XPProcName[i],strlen(XPProcName[i]));
								break;
							case WINDOWS_VERSION_2K3_SP1_SP2:
								strncpy(ShadowSSDTInfo->SSDT[count].lpszFunction,Win2003ProcName[i],strlen(Win2003ProcName[i]));
								break;
							case WINDOWS_VERSION_7:
								strncpy(ShadowSSDTInfo->SSDT[count].lpszFunction,Win7ProcName[i],strlen(Win7ProcName[i]));
								break;
							}

							memset(ShadowSSDTInfo->SSDT[count].lpszHookModuleImage,0,sizeof(ShadowSSDTInfo->SSDT[count].lpszHookModuleImage));
							strncpy(ShadowSSDTInfo->SSDT[count].lpszHookModuleImage,lpszHookModuleImage,strlen(lpszHookModuleImage));
							ShadowSSDTInfo->SSDT[count].ulHookModuleBase = ulHookModuleBase;
							ShadowSSDTInfo->SSDT[count].ulHookModuleSize = ulHookModuleSize;
							ShadowSSDTInfo->SSDT[count].IntHookType = SSDTINLINEHOOK;

							if (DebugOn)
								KdPrint(("[%d]Found ssdt inline hook!!:%s-%s",
								ShadowSSDTInfo->ulCount,
								ShadowSSDTInfo->SSDT[count].lpszHookModuleImage,
								ShadowSSDTInfo->SSDT[count].lpszFunction));

							count++;

							ulTemp = NULL;
						}
					}
				}
			}
			//shadowssdt hook
			bShadowHooked = TRUE;

			ulMemoryFunctionBase = ShadowSSDTTable[1].ServiceTable[i];
			if (ulMemoryFunctionBase > ulWin32kBase &&
				ulMemoryFunctionBase < ulWin32kBase + ulWin32kSize)
			{
				bShadowHooked = FALSE;
			}
			if (bShadowHooked ||
				bShadowSSDTAll == TRUE)
			{
				if (!IsAddressInSystem(
					ulMemoryFunctionBase,
					&ulHookModuleBase,
					&ulHookModuleSize,
					lpszHookModuleImage))
				{
					memset(lpszHookModuleImage,0,sizeof(lpszHookModuleImage));
					strcat(lpszHookModuleImage,"Unknown2");
					ulHookModuleBase = 0;
					ulHookModuleSize = 0;
				}
				ShadowSSDTInfo->ulCount = count;
				ShadowSSDTInfo->SSDT[count].ulNumber = i;
				ShadowSSDTInfo->SSDT[count].ulMemoryFunctionBase = ulMemoryFunctionBase;

				memset(ShadowSSDTInfo->SSDT[count].lpszFunction,0,sizeof(ShadowSSDTInfo->SSDT[count].lpszFunction));
				WinVer = GetWindowsVersion();
				switch (WinVer)
				{
				case WINDOWS_VERSION_XP:
					strncpy(ShadowSSDTInfo->SSDT[count].lpszFunction,XPProcName[i],strlen(XPProcName[i]));
					break;
				case WINDOWS_VERSION_2K3_SP1_SP2:
					strncpy(ShadowSSDTInfo->SSDT[count].lpszFunction,Win2003ProcName[i],strlen(Win2003ProcName[i]));
					break;
				case WINDOWS_VERSION_7:
					strncpy(ShadowSSDTInfo->SSDT[count].lpszFunction,Win7ProcName[i],strlen(Win7ProcName[i]));
					break;
				}

				memset(ShadowSSDTInfo->SSDT[count].lpszHookModuleImage,0,sizeof(ShadowSSDTInfo->SSDT[count].lpszHookModuleImage));
				strncpy(ShadowSSDTInfo->SSDT[count].lpszHookModuleImage,lpszHookModuleImage,strlen(lpszHookModuleImage));

				ShadowSSDTInfo->SSDT[count].ulHookModuleBase = ulHookModuleBase;
				ShadowSSDTInfo->SSDT[count].ulHookModuleSize = ulHookModuleSize;

				if (bShadowHooked == FALSE)
				{
					ShadowSSDTInfo->SSDT[count].IntHookType = NOHOOK;
				}else
					ShadowSSDTInfo->SSDT[count].IntHookType = SSDTHOOK;  //ssdt hook

				if (DebugOn)
					KdPrint(("[%d]Found ssdt hook!!:%s-%s",
					ShadowSSDTInfo->ulCount,
					ShadowSSDTInfo->SSDT[count].lpszHookModuleImage,
					ShadowSSDTInfo->SSDT[count].lpszFunction));

				count++;
			}
		}

	}__except(EXCEPTION_EXECUTE_HANDLER){

	}
}
//8888为全部，其余为单个
BOOL RestoreAllShadowSSDTFunction(ULONG IntType)
{
	ULONG ulMemoryFunctionBase;
	ULONG ulRealMemoryFunctionBase;
	ULONG ulReloadFunctionBase;

	BOOL bHooked = FALSE;
	int i = 0;
	BOOL bReSetOne = FALSE;

	for (i=0 ;i<ShadowSSDTTable[1].TableSize ;i++)
	{
		bHooked = TRUE;
		ulMemoryFunctionBase = ShadowSSDTTable[1].ServiceTable[i];

		//得到原来系统内存中shadowssdt的地址，不是reload的shadowssdt哦，亲
		ulReloadFunctionBase = OriginalShadowServiceDescriptorTable->ServiceTable[i];
		ulRealMemoryFunctionBase = ulReloadFunctionBase - (ULONG)Win32kImageModuleBase + ulWin32kBase;

		if (ulMemoryFunctionBase > ulWin32kBase &&
			ulMemoryFunctionBase < ulWin32kBase + ulWin32kSize)
		{
			bHooked = FALSE;
		}
		if (bHooked == TRUE)
		{
			if (DebugOn)
				KdPrint(("[%d]%08x  %08x",i,ulReloadFunctionBase,ulRealMemoryFunctionBase));

			//开始恢复
			//恢复全部
			if (IntType == 8888)
			{
				__asm
				{
					cli
						push eax
						mov eax,cr0
						and eax,not 0x10000
						mov cr0,eax
						pop eax
				}
				ShadowSSDTTable[1].ServiceTable[i] = ulRealMemoryFunctionBase;

				if (DebugOn)
					KdPrint(("[%d]%08x  %08x",i,ShadowSSDTTable[1].ServiceTable[i],ulRealMemoryFunctionBase));
				__asm
				{
					push eax
						mov eax,cr0
						or eax,0x10000
						mov cr0,eax
						pop eax
						sti
				}
			}   
			else  //恢复单个
			{
				if (IntType == i)
				{
					__asm
					{
						cli
							push eax
							mov eax,cr0
							and eax,not 0x10000
							mov cr0,eax
							pop eax
					}
					ShadowSSDTTable[1].ServiceTable[i] = ulRealMemoryFunctionBase;
					__asm
					{
						push eax
							mov eax,cr0
							or eax,0x10000
							mov cr0,eax
							pop eax
							sti
					}
				}
			}
		}
		//恢复inline hook
		if (IntType == 8888)
		{
			RestoreShadowInlineHook(i);
		}else
		{
			if (IntType == i)
			{
				RestoreShadowInlineHook(i);
				break;
			}
		}
	}
	return TRUE;
}
BOOL RestoreShadowInlineHook(ULONG ulNumber)
{
	ULONG ulFunction = NULL;
	ULONG ulReloadFunction;
	BOOL bInit = FALSE;
	int i=0;

	RMmIsAddressValid = ReLoadNtosCALL(L"MmIsAddressValid",SystemKernelModuleBase,ImageModuleBase);
	if (!RMmIsAddressValid)
	{
		return bInit;
	}
	for (i=0 ;i<ShadowSSDTTable[1].TableSize ;i++)
	{
		if (ulNumber == i)
		{
			ulFunction = OriginalShadowServiceDescriptorTable->ServiceTable[i] - (ULONG)Win32kImageModuleBase + ulWin32kBase;
			ulReloadFunction = OriginalShadowServiceDescriptorTable->ServiceTable[i];

			if (RMmIsAddressValid(ulFunction) &&
				RMmIsAddressValid(ulReloadFunction))
			{
				if (GetFunctionCodeSize(ulFunction) != GetFunctionCodeSize(ulReloadFunction))
				{
					return FALSE;
				}
				__asm
				{
					cli
						push eax
						mov eax,cr0
						and eax,not 0x10000
						mov cr0,eax
						pop eax
				}
				memcpy(ulFunction,ulReloadFunction,GetFunctionCodeSize(ulFunction));
				__asm
				{
					push eax
						mov eax,cr0
						or eax,0x10000
						mov cr0,eax
						pop eax
						sti
				}
				bInit = TRUE;
				break;
			}
		}
	}
	return TRUE;
}