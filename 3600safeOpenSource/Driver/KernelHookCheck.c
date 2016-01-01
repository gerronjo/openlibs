//**************************************************************************
//*
//*          A盾电脑防护 website:http://www.3600safe.com/
//*        
//*文件说明：
//*         内核hook检查
//**************************************************************************
#include "KernelHookCheck.h"

unsigned long __fastcall GetFunctionCodeSize(void *Proc)
{
	ULONG  Length;
	PUCHAR pOpcode;
	ULONG  Result = 0;
	ULONG CCINT3Count=0;
	do
	{
		Length = SizeOfCode(Proc, &pOpcode);
		Result += Length;
		if ((Length == 1) && (*pOpcode == 0xCC||*pOpcode==0x90)) CCINT3Count++;
		if (CCINT3Count>1 ||
			*pOpcode == 0x00)
		{
			break;
		}
		Proc = (PVOID)((ULONG)Proc + Length);
	} while (Length);
	return Result;
}
/*
unsigned long __fastcall GetFunctionCodeSize(void *Base)
{
	ULONG length1=0;
	size_t length,total_length=0;
	ULONG start_address=Base;
	ULONG Flagss=0;
	INSTRUCTION	Inst;

	if (!MmIsAddressValid((PVOID)Base))
	{
		return 0;
	}
	//反汇编
	while (TRUE)
	{
		length = get_instruction(&Inst,start_address,MODE_32);
		//length=xde_disasm((unsigned char *)(start_address),&code_instr);
		if (length==0)
		{
			return 0;
		}
		if ((Inst.opcode==0xcc) && length==1)
		{
			total_length++;
			return total_length;

		}
		if (length == 2)
		{
			__asm{
				mov esi,start_address
					mov ax,word ptr [esi]
				cmp ax,0x0000
					jz Cont//是add     byte ptr [eax],al
					//结束
					mov Flagss,1
Cont:

			}
			if (Flagss)//为结束继续
			{
				Flagss=0;
			}
			else
			{
				return total_length;
			}
		}
		total_length+=length;
		start_address+=length;
	}	
	return total_length;
}*/
BOOL ReSetEatHook(CHAR *lpszFunction,ULONG ulReloadKernelModule,ULONG ulKernelModule)
{
	ULONG ulModuleBase;
	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_NT_HEADERS NtDllHeader;
	IMAGE_OPTIONAL_HEADER opthdr;
	DWORD* arrayOfFunctionAddresses;
	DWORD* arrayOfFunctionNames;
	WORD* arrayOfFunctionOrdinals;
	DWORD functionOrdinal;
	DWORD Base,x,functionAddress;
	IMAGE_EXPORT_DIRECTORY *pExportTable;
	char *functionName = NULL;
	BOOL bIsEatHooked = FALSE;
	int position;
	ULONG ulFunctionOrdinal;
	

	//恢复的时候 用reload的ImageModuleBase
	ulModuleBase = ulReloadKernelModule;
	pDosHeader = (PIMAGE_DOS_HEADER)ulModuleBase;
	if (pDosHeader->e_magic!=IMAGE_DOS_SIGNATURE)
	{
		KdPrint(("failed to find NtHeader\r\n"));
		return 0;
	}
	NtDllHeader=(PIMAGE_NT_HEADERS)(ULONG)((ULONG)pDosHeader+pDosHeader->e_lfanew);
	if (NtDllHeader->Signature!=IMAGE_NT_SIGNATURE)
	{
		KdPrint(("failed to find NtHeader\r\n"));
		return 0;
	}
	opthdr = NtDllHeader->OptionalHeader;
	pExportTable =(IMAGE_EXPORT_DIRECTORY*)((BYTE*)ulModuleBase + opthdr.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT]. VirtualAddress); //得到导出表
	arrayOfFunctionAddresses = (DWORD*)( (BYTE*)ulModuleBase + pExportTable->AddressOfFunctions);  //地址表
 	arrayOfFunctionNames = (DWORD*)((BYTE*)ulModuleBase + pExportTable->AddressOfNames);         //函数名表
 	arrayOfFunctionOrdinals = (WORD*)( (BYTE*)ulModuleBase + pExportTable->AddressOfNameOrdinals);

	Base = pExportTable->Base;

	for(x = 0; x < pExportTable->NumberOfFunctions; x++) //在整个导出表里扫描
	{
		functionName = (char*)((BYTE*)ulModuleBase + arrayOfFunctionNames[x]);
		ulFunctionOrdinal = arrayOfFunctionOrdinals[x] + Base - 1; 
		ulFunctionOrdinal = arrayOfFunctionAddresses[ulFunctionOrdinal];

		functionAddress = (DWORD)((BYTE*)ulModuleBase + ulFunctionOrdinal);

		if (_stricmp(lpszFunction,functionName) == 0)
		{
			KdPrint(("reload ulFunctionOrdinal:%08x:%s",ulFunctionOrdinal,functionName));

			//开始恢复
			ulModuleBase = ulKernelModule;
			pDosHeader = (PIMAGE_DOS_HEADER)ulModuleBase;
			if (pDosHeader->e_magic!=IMAGE_DOS_SIGNATURE)
			{
				KdPrint(("failed to find NtHeader\r\n"));
				return 0;
			}
			NtDllHeader=(PIMAGE_NT_HEADERS)(ULONG)((ULONG)pDosHeader+pDosHeader->e_lfanew);
			if (NtDllHeader->Signature!=IMAGE_NT_SIGNATURE)
			{
				KdPrint(("failed to find NtHeader\r\n"));
				return 0;
			}
			opthdr = NtDllHeader->OptionalHeader;
			pExportTable =(IMAGE_EXPORT_DIRECTORY*)((BYTE*)ulModuleBase + opthdr.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT]. VirtualAddress); //得到导出表
			arrayOfFunctionAddresses = (DWORD*)( (BYTE*)ulModuleBase + pExportTable->AddressOfFunctions);  //地址表
			arrayOfFunctionNames = (DWORD*)((BYTE*)ulModuleBase + pExportTable->AddressOfNames);         //函数名表
			arrayOfFunctionOrdinals = (WORD*)( (BYTE*)ulModuleBase + pExportTable->AddressOfNameOrdinals);

			Base = pExportTable->Base;

			_asm
			{
				CLI					
					MOV	EAX, CR0		
					AND EAX, NOT 10000H 
					MOV	CR0, EAX		
			}	
			arrayOfFunctionAddresses[arrayOfFunctionOrdinals[x] + Base - 1] = ulFunctionOrdinal;
			_asm 
			{
				MOV	EAX, CR0		
					OR	EAX, 10000H			
					MOV	CR0, EAX			
					STI					
			}
			break;
		}
	}

}
ULONG GetEatHook(ULONG ulOldAddress,int x,ULONG ulSystemKernelModuleBase,ULONG ulSystemKernelModuleSize)
{
	ULONG ulModuleBase;
	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_NT_HEADERS NtDllHeader;
	IMAGE_OPTIONAL_HEADER opthdr;
	DWORD* arrayOfFunctionAddresses;
	DWORD* arrayOfFunctionNames;
	WORD* arrayOfFunctionOrdinals;
	DWORD functionOrdinal;
	DWORD Base,functionAddress;
	IMAGE_EXPORT_DIRECTORY *pExportTable;
	char *functionName = NULL;
	BOOL bIsEatHooked = FALSE;
	int position;
	ULONG ulFunctionOrdinal;

	ulModuleBase = ulSystemKernelModuleBase;
	pDosHeader = (PIMAGE_DOS_HEADER)ulModuleBase;
	if (pDosHeader->e_magic!=IMAGE_DOS_SIGNATURE)
	{
		KdPrint(("failed to find NtHeader\r\n"));
		return 0;
	}
	NtDllHeader=(PIMAGE_NT_HEADERS)(ULONG)((ULONG)pDosHeader+pDosHeader->e_lfanew);
	if (NtDllHeader->Signature!=IMAGE_NT_SIGNATURE)
	{
		KdPrint(("failed to find NtHeader\r\n"));
		return 0;
	}
	opthdr = NtDllHeader->OptionalHeader;
	pExportTable =(IMAGE_EXPORT_DIRECTORY*)((BYTE*)ulModuleBase + opthdr.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT]. VirtualAddress); //得到导出表
	arrayOfFunctionAddresses = (DWORD*)( (BYTE*)ulModuleBase + pExportTable->AddressOfFunctions);  //地址表
 	arrayOfFunctionNames = (DWORD*)((BYTE*)ulModuleBase + pExportTable->AddressOfNames);         //函数名表
 	arrayOfFunctionOrdinals = (WORD*)( (BYTE*)ulModuleBase + pExportTable->AddressOfNameOrdinals);

	Base = pExportTable->Base;

	functionName = (char*)((BYTE*)ulModuleBase + arrayOfFunctionNames[x]);
	ulFunctionOrdinal = arrayOfFunctionOrdinals[x] + Base - 1; 
	functionAddress = (DWORD)((BYTE*)ulModuleBase + arrayOfFunctionAddresses[ulFunctionOrdinal]);
	
	if (*functionName == 'Z' &&
		*(functionName+1) == 'w')
	{
		position  = *((WORD*)(functionAddress + 1));  //得到服务号
		if (position >= NULL &&
			position <= OriginalServiceDescriptorTable->TableSize)
		{
			//得到原始地址
			functionAddress = OriginalServiceDescriptorTable->ServiceTable[position] - (ULONG)ImageModuleBase + SystemKernelModuleBase;
		}
	}
	if (ulOldAddress != functionAddress)
	{
		KdPrint(("EAT HOOK %08x:%s\r\n",functionAddress,functionName));
		return functionAddress;
	}
	return 0;
}
BOOL IsFunctionInExportTable(ULONG ulModuleBase,ULONG ulFunctionAddress)
{

	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_NT_HEADERS NtDllHeader;
	IMAGE_OPTIONAL_HEADER opthdr;
	DWORD* arrayOfFunctionAddresses;
	DWORD* arrayOfFunctionNames;
	WORD* arrayOfFunctionOrdinals;
	DWORD functionOrdinal;
	DWORD Base, x, functionAddress,ulOldAddress;
	IMAGE_EXPORT_DIRECTORY *pExportTable;
	char *functionName;


	__try
	{
		pDosHeader=(PIMAGE_DOS_HEADER)ulModuleBase;
		if (pDosHeader->e_magic!=IMAGE_DOS_SIGNATURE)
		{
			KdPrint(("failed to find NtHeader\r\n"));
			return FALSE;
		}
		NtDllHeader=(PIMAGE_NT_HEADERS)(ULONG)((ULONG)pDosHeader+pDosHeader->e_lfanew);
		if (NtDllHeader->Signature!=IMAGE_NT_SIGNATURE)
		{
			KdPrint(("failed to find NtHeader\r\n"));
			return FALSE;
		}
		opthdr = NtDllHeader->OptionalHeader;
		pExportTable =(IMAGE_EXPORT_DIRECTORY*)((BYTE*)ulModuleBase + opthdr.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT]. VirtualAddress); //得到导出表
		arrayOfFunctionAddresses = (DWORD*)( (BYTE*)ulModuleBase + pExportTable->AddressOfFunctions);  //地址表
		arrayOfFunctionNames = (DWORD*)((BYTE*)ulModuleBase + pExportTable->AddressOfNames);         //函数名表
		arrayOfFunctionOrdinals = (WORD*)( (BYTE*)ulModuleBase + pExportTable->AddressOfNameOrdinals);

		Base = pExportTable->Base;

		for(x = 0; x < pExportTable->NumberOfFunctions; x++) //在整个导出表里扫描
		{
			//functionName = (char*)((BYTE*)ulModuleBase + arrayOfFunctionNames[x]);
			functionOrdinal = arrayOfFunctionOrdinals[x] + Base - 1; 
			functionAddress = (DWORD)((BYTE*)ulModuleBase + arrayOfFunctionAddresses[functionOrdinal]);
			//KdPrint(("%08x:%s\r\n",functionAddress,functionName));
			//ulOldAddress = GetSystemRoutineAddress(0,functionName);
			ulOldAddress = functionAddress - ulModuleBase + SystemKernelModuleBase;
			if (ulFunctionAddress == ulOldAddress)
			{
				//是导出函数，退出
				return TRUE;
			}
		}

	}__except(EXCEPTION_EXECUTE_HANDLER){

	}
	return FALSE;
}
//获取导出函数下一级0xe8 call函数的inlinehookcheck
ULONG GetNextFunctionAddress(ULONG ulNtDllModuleBase,ULONG ulOldAddress,char *functionName,PINLINEHOOKINFO InlineHookInfo)
{
	ULONG ulCodeSize;

	ULONG ulNextFunCodeSize;
	ULONG ulNextFunReloadCodeSize;
	PUCHAR i;

	ULONG ulNextFunctionAddress=0;
	ULONG ulReloadNextFunctionAddress=0;
	BOOL bRetOK = FALSE;
	PUCHAR ulTemp;
	ULONG ulHookFunctionAddress;
	PUCHAR p;

	INSTRUCTION	Inst;
	INSTRUCTION	Instb;

	char lpszHookModuleImage[256];
	ULONG ulHookModuleBase;
	ULONG ulHookModuleSize;
	int Flagss;

	if (!RMmIsAddressValid(ulOldAddress))
	{
		return bRetOK;
	}
	__try
	{
		ulCodeSize = GetFunctionCodeSize(ulOldAddress);
		for (i=(ULONG)ulOldAddress;i < i+ulCodeSize;i++)
		{
			if (*i == 0xe8)
			{
				ulNextFunctionAddress = *(PULONG)(i+1)+(ULONG)(i+5);
				if (RMmIsAddressValid(ulNextFunctionAddress))
				{
					//判断一下是否是导出函数
					if (IsFunctionInExportTable(ulNtDllModuleBase,ulNextFunctionAddress))
					{
						return;
					}
					//做hook 扫描
					ulReloadNextFunctionAddress = ulNextFunctionAddress - SystemKernelModuleBase + ImageModuleBase;
					if (RMmIsAddressValid(ulReloadNextFunctionAddress) &&
						RMmIsAddressValid(ulNextFunctionAddress))
					{
						ulNextFunCodeSize = GetFunctionCodeSize(ulNextFunctionAddress);
						ulNextFunReloadCodeSize = GetFunctionCodeSize(ulReloadNextFunctionAddress);

						if (ulNextFunCodeSize == ulNextFunReloadCodeSize &&
							memcmp(ulReloadNextFunctionAddress,ulNextFunctionAddress,ulNextFunCodeSize) != NULL)
						{
							//被hook了
							//KdPrint(("found hook---->%s",functionName));

							for (p=ulNextFunctionAddress ;p< ulNextFunctionAddress+ulNextFunCodeSize; p++)
							{
								//折半扫描，如果前面一半一样，则开始扫描下一半
								if (memcmp(ulReloadNextFunctionAddress,ulNextFunctionAddress,ulNextFunCodeSize/2) == NULL)
								{
									ulNextFunCodeSize = ulNextFunCodeSize + ulNextFunCodeSize/2;
									continue;
								}
								//是否结束？
								if (*p == 0xcc ||
									*p == 0xc2)
								{
									break;
								}
								ulHookFunctionAddress = (*(PULONG)(p + 1) + (ULONG)p + 5);  //得到地址
								if (!RMmIsAddressValid(ulHookFunctionAddress))
								{
									continue;
								}
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
									RMmIsAddressValid(ulTemp) &&
									RMmIsAddressValid(p))   //hook的地址也要有效才可以哦
								{
									if (ulTemp > SystemKernelModuleBase &&
										ulTemp < SystemKernelModuleBase+SystemKernelModuleSize)   //太近的跳也不是
									{
										goto Next;
									}
									//ulTemp也不能小于 SystemKernelModuleBase
									if (ulTemp < SystemKernelModuleBase)
									{
										goto Next;
									}
									if (*(ULONG *)ulTemp == 0x00000000 ||
										*(ULONG *)ulTemp == 0x00000005)
									{
										goto Next;
									}
									Flagss = 0;
									__asm{
										mov esi,ulTemp
											mov ax,word ptr [esi]
										cmp ax,0x0000
											jz Cont//是add     byte ptr [eax],al
											mov Flagss,1
Cont:
									}
									if (Flagss != 1)
										goto Next;

									ulTemp = ulTemp+0x5;
									//简单处理一下二级跳
									if (*ulTemp == 0xe9 ||
										*ulTemp == 0xe8)
									{
										if (DebugOn)
											KdPrint(("ulTemp == 0xe9"));
										ulTemp = *(PULONG)(ulTemp+1)+(ULONG)(ulTemp+5);
									}
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
									if (!RMmIsAddressValid(&InlineHookInfo->InlineHook[IntHookCount]))
									{
										return;
									}
									if (DebugOn)
										KdPrint(("found hook---->%s:%08x 0x%x",functionName,*(ULONG *)ulTemp,GetFunctionCodeSize(ulNextFunctionAddress)));

									InlineHookInfo->InlineHook[IntHookCount].ulMemoryHookBase = ulTemp+0x5;
									memset(InlineHookInfo->InlineHook[IntHookCount].lpszFunction,0,sizeof(InlineHookInfo->InlineHook[IntHookCount].lpszFunction));
									memset(InlineHookInfo->InlineHook[IntHookCount].lpszHookModuleImage,0,sizeof(InlineHookInfo->InlineHook[IntHookCount].lpszHookModuleImage));

									memcpy(InlineHookInfo->InlineHook[IntHookCount].lpszFunction,functionName,strlen(functionName));
									memcpy(InlineHookInfo->InlineHook[IntHookCount].lpszFunction+strlen(functionName),"/NextCallHook",strlen("/NextCallHook"));
									memcpy(InlineHookInfo->InlineHook[IntHookCount].lpszHookModuleImage,lpszHookModuleImage,strlen(lpszHookModuleImage));

									InlineHookInfo->InlineHook[IntHookCount].ulMemoryFunctionBase = (ULONG)p;
									InlineHookInfo->InlineHook[IntHookCount].ulHookModuleBase = ulHookModuleBase;
									InlineHookInfo->InlineHook[IntHookCount].ulHookModuleSize = ulHookModuleSize;
									IntHookCount++;
Next:
									_asm{nop}
								}
							}
						}
					}
				}
			}
			//结束鸟
			if (*i == 0xcc ||
				*i == 0xc2)
			{
				return;
			}
		}

	}__except(EXCEPTION_EXECUTE_HANDLER){

	}
}
//到处函数的hook扫描
BOOL KernelHookCheck(PINLINEHOOKINFO InlineHookInfo)
{
	UNICODE_STRING wsNtDllString;
	STRING lpszSearchFunction;
	STRING lpszFunction;

	HANDLE hNtSection;
	ULONG ulNtDllModuleBase;
	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_NT_HEADERS NtDllHeader;

	IMAGE_OPTIONAL_HEADER opthdr;
	DWORD* arrayOfFunctionAddresses;
	DWORD* arrayOfFunctionNames;
	WORD* arrayOfFunctionOrdinals;
	DWORD functionOrdinal;
	DWORD Base, x, functionAddress,position;
	char* functionName;
	IMAGE_EXPORT_DIRECTORY *pExportTable;
	BOOL bRetOK = FALSE;
	ULONG ulOldAddress;
	ULONG ulReloadAddress;
	PUCHAR ulTemp;

	PUCHAR p;

	INSTRUCTION	Inst;
	INSTRUCTION	Instb;

	char lpszHookModuleImage[256];
	ULONG ulHookModuleBase;
	ULONG ulHookModuleSize;

	int i=0;
	int y=0;
	ULONG ulCodeSize;
	int Flagss;
	BOOL bInit = FALSE;
	BOOL bIsZwFunction = FALSE;

	ULONG ulHookFunctionAddress;
	BYTE HookCodeCheck[1];
	BYTE HookCodeCheck1[1];

	IntHookCount = 0;

	__try
	{
		RZwClose = ReLoadNtosCALL(L"ZwClose",SystemKernelModuleBase,ImageModuleBase);
		if (RZwClose)
		{
			bInit = TRUE;
		}
		if (!bInit)
			return;

// 		RtlInitUnicodeString(&wsNtDllString,SystemKernelFilePath);
// 		hNtSection = MapFileAsSection(&wsNtDllString,&ulNtDllModuleBase);  //载入到内存
// 		if (!hNtSection)
// 		{
// 			return bRetOK;
// 		}
// 		RZwClose(hNtSection);

		ulNtDllModuleBase = ImageModuleBase;
		pDosHeader = (PIMAGE_DOS_HEADER)ulNtDllModuleBase;
		if (pDosHeader->e_magic!=IMAGE_DOS_SIGNATURE)
		{
			KdPrint(("failed to find NtHeader\r\n"));
			return bRetOK;
		}
		NtDllHeader=(PIMAGE_NT_HEADERS)(ULONG)((ULONG)pDosHeader+pDosHeader->e_lfanew);
		if (NtDllHeader->Signature!=IMAGE_NT_SIGNATURE)
		{
			KdPrint(("failed to find NtHeader\r\n"));
			return bRetOK;
		}
// 		NtDllHeader = RtlImageNtHeader(ulNtDllModuleBase);
// 		if (!NtDllHeader)
// 		{
// 			KdPrint(("failed to find NtHeader\r\n"));
// 			return bRetOK;
// 		}
		opthdr = NtDllHeader->OptionalHeader;
		pExportTable =(IMAGE_EXPORT_DIRECTORY*)((BYTE*)ulNtDllModuleBase + opthdr.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT]. VirtualAddress); //得到导出表
		arrayOfFunctionAddresses = (DWORD*)( (BYTE*)ulNtDllModuleBase + pExportTable->AddressOfFunctions);  //地址表
		arrayOfFunctionNames = (DWORD*)((BYTE*)ulNtDllModuleBase + pExportTable->AddressOfNames);         //函数名表
		arrayOfFunctionOrdinals = (WORD*)((BYTE*)ulNtDllModuleBase + pExportTable->AddressOfNameOrdinals);

		Base = pExportTable->Base;

		for(x = 0; x < pExportTable->NumberOfFunctions; x++) //在整个导出表里扫描
		{
			functionName = (char*)( (BYTE*)ulNtDllModuleBase + arrayOfFunctionNames[x]);
			functionOrdinal = arrayOfFunctionOrdinals[x] + Base - 1; 

		    //直接扫描reload的内核，得到reload地址之后，在通过计算得到原始的
			functionAddress = (DWORD)((BYTE*)ulNtDllModuleBase + arrayOfFunctionAddresses[functionOrdinal]);
			//position  = *((WORD*)(functionAddress + 1));  //得到服务号

			if (DebugOn)
				KdPrint(("%s\r\n",functionName));
			//RtlInitAnsiString(&AnsiFunction,functionName);
			//RtlAnsiStringToUnicodeString(&UnicodeFunction,&AnsiFunction,TRUE);
			bIsZwFunction = FALSE;

			ulReloadAddress = functionAddress;
			ulOldAddress = ulReloadAddress - (ULONG)ImageModuleBase + SystemKernelModuleBase;

// 			ulOldAddress = GetSystemRoutineAddress(0,functionName);
// 			ulReloadAddress = ulOldAddress - SystemKernelModuleBase + ImageModuleBase;
			if (!ulOldAddress ||
				!RMmIsAddressValid(ulOldAddress) ||
				!ulReloadAddress ||
				!RMmIsAddressValid(ulReloadAddress))
			{
				continue;
			}
			//检查下一层第一个call的函数的hook
			if (*functionName == 'Z' &&
				*(functionName+1) == 'w')
			{
				bIsZwFunction = TRUE;
				position  = *((WORD*)(functionAddress + 1));  //得到服务号

				if (position >= NULL &&
					position <= OriginalServiceDescriptorTable->TableSize)
				{
					//获取reload的地址，这里不能用系统导出的那个，因为那个有可能被hook了
					//因此得到的ulReloadAddress是错误的
					ulReloadAddress = OriginalServiceDescriptorTable->ServiceTable[position];
					ulOldAddress = ulReloadAddress - (ULONG)ImageModuleBase + SystemKernelModuleBase;
				}
				if (DebugOn)
					KdPrint(("[%d:%08x][%08x]%s\r\n",position,functionAddress,ulOldAddress,functionName));
			}
			if (bIsZwFunction)
			{
				//如果 bIsZwFunction == TRUE 重新效验一下地址的有效性
				if (!ulOldAddress ||
					!RMmIsAddressValid(ulOldAddress) ||
					!ulReloadAddress ||
					!RMmIsAddressValid(ulReloadAddress))
				{
					continue;
				}
			}
			else //下一层函数只扫描非Zw开头的，并且只扫描未导出函数
				GetNextFunctionAddress(ulNtDllModuleBase,ulOldAddress,functionName,InlineHookInfo);
//---------------------------------------------------------------------------------------------------------
			ulTemp = NULL;
			ulTemp = GetEatHook(ulOldAddress,x,SystemKernelModuleBase,SystemKernelModuleSize);
			if (ulTemp)
			{
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
				InlineHookInfo->InlineHook[IntHookCount].ulMemoryHookBase = ulTemp;
				memset(InlineHookInfo->InlineHook[IntHookCount].lpszFunction,0,sizeof(InlineHookInfo->InlineHook[IntHookCount].lpszFunction));
				memset(InlineHookInfo->InlineHook[IntHookCount].lpszHookModuleImage,0,sizeof(InlineHookInfo->InlineHook[IntHookCount].lpszHookModuleImage));

				memcpy(InlineHookInfo->InlineHook[IntHookCount].lpszFunction,functionName,strlen(functionName));
				memcpy(InlineHookInfo->InlineHook[IntHookCount].lpszHookModuleImage,lpszHookModuleImage,strlen(lpszHookModuleImage));

				InlineHookInfo->InlineHook[IntHookCount].ulMemoryFunctionBase = (ULONG)ulOldAddress;
				InlineHookInfo->InlineHook[IntHookCount].ulHookModuleBase = ulHookModuleBase;
				InlineHookInfo->InlineHook[IntHookCount].ulHookModuleSize = ulHookModuleSize;
				InlineHookInfo->InlineHook[IntHookCount].ulHookType = 1;  //eat hook
				IntHookCount++;
			}
			ulTemp = NULL;
//---------------------------------------------------------------------------------------------------------
			if (DebugOn)
				KdPrint(("search---->%s:%08x",functionName,ulOldAddress));
			//hook !!!
			if (GetFunctionCodeSize(ulOldAddress) == GetFunctionCodeSize(ulReloadAddress) &&
				memcmp(ulReloadAddress,ulOldAddress,GetFunctionCodeSize(ulOldAddress)) != NULL)
			{	
				//开始扫描hook
				ulCodeSize = GetFunctionCodeSize(ulOldAddress);

				if (DebugOn)
					KdPrint(("search---->%s:%08x---->%08x:%08x",functionName,ulOldAddress,functionOrdinal,functionAddress));

				for (p=ulOldAddress ;p< ulOldAddress+ulCodeSize; p++)
				{
					//折半扫描，如果前面一半一样，则开始扫描下一半
					if (memcmp(ulReloadAddress,ulOldAddress,ulCodeSize/2) == NULL)
					{
						ulCodeSize = ulCodeSize + ulCodeSize/2;
						continue;
					}
					if (*p == 0xcc ||
						*p == 0xc2)
					{
						break;
					}
					ulHookFunctionAddress = (*(PULONG)(p + 1) + (ULONG)p + 5);  //得到hook的地址
					if (!RMmIsAddressValid(ulHookFunctionAddress))
					{
						continue;
					}
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
						RMmIsAddressValid(ulTemp) &&
						RMmIsAddressValid(p))   //hook的地址也要有效才可以哦
					{
						if (ulTemp > SystemKernelModuleBase &&
							ulTemp < SystemKernelModuleBase+SystemKernelModuleSize)   //太近的跳也不是
						{
							goto Next;
						}
						//ulTemp也不能小于 SystemKernelModuleBase
						if (ulTemp < SystemKernelModuleBase)
						{
							goto Next;
						}
						//KdPrint(("%08x-%08x-%08x",p,ulTemp,(SystemKernelModuleBase + SystemKernelModuleSize + 0xfffffff)));

						if (*(ULONG *)ulTemp == 0x00000000 ||
							*(ULONG *)ulTemp == 0x00000005 ||
							*(ULONG *)ulTemp == 0xc0000012)
						{
							goto Next;
						}
						Flagss = 0;
						__asm{
							mov esi,ulTemp
								mov ax,word ptr [esi]
							cmp ax,0x0000
								jz Cont//是add     byte ptr [eax],al
								//结束
								mov Flagss,1
Cont:
						}
						if (Flagss != 1)
							goto Next;

						if (DebugOn)
							KdPrint(("found hook!!!---->%s:%08x %x",functionName,ulTemp,*(ULONG *)ulTemp));

						ulTemp = ulTemp+0x5;
						//简单处理一下二级跳
						if (*ulTemp == 0xe9 ||
							*ulTemp == 0xe8)
						{
							if (DebugOn)
								KdPrint(("ulTemp == 0xe9"));
							ulTemp = *(PULONG)(ulTemp+1)+(ULONG)(ulTemp+5);
						}
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
						if (!RMmIsAddressValid(&InlineHookInfo->InlineHook[IntHookCount]))
						{
							return;
						}
						InlineHookInfo->InlineHook[IntHookCount].ulMemoryHookBase = ulTemp;
						memset(InlineHookInfo->InlineHook[IntHookCount].lpszFunction,0,sizeof(InlineHookInfo->InlineHook[IntHookCount].lpszFunction));
						memset(InlineHookInfo->InlineHook[IntHookCount].lpszHookModuleImage,0,sizeof(InlineHookInfo->InlineHook[IntHookCount].lpszHookModuleImage));

						memcpy(InlineHookInfo->InlineHook[IntHookCount].lpszFunction,functionName,strlen(functionName));
						memcpy(InlineHookInfo->InlineHook[IntHookCount].lpszHookModuleImage,lpszHookModuleImage,strlen(lpszHookModuleImage));

						InlineHookInfo->InlineHook[IntHookCount].ulMemoryFunctionBase = (ULONG)p;
						InlineHookInfo->InlineHook[IntHookCount].ulHookModuleBase = ulHookModuleBase;
						InlineHookInfo->InlineHook[IntHookCount].ulHookModuleSize = ulHookModuleSize;
						InlineHookInfo->InlineHook[IntHookCount].ulHookType = 0;  //inline hook
						IntHookCount++;
Next:
						_asm{nop}
					}
				}
			}

			//strcat(functionName,"\r\n");
		}
		InlineHookInfo->ulCount = IntHookCount;

	}__except(EXCEPTION_EXECUTE_HANDLER){

	}
	//KdPrint(("IntHookCount:%d\r\n",IntHookCount));
	return bRetOK;
}