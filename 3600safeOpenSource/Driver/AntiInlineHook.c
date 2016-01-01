//**************************************************************************
//*
//*          A盾电脑防护 website:http://www.3600safe.com/
//*        
//*文件说明：
//*         绕过内核hook
//**************************************************************************
#include "AntiInlineHook.h"

/*
__declspec(naked) VOID __stdcall NewHookFunctionProcess()
{
	_asm
	{
		jmp [ulReloadAddress];   //直接跳到我reload的函数地址去~~日死他
	}
}
*/
//在这里是摆设而已！
__declspec(naked) VOID HookFunctionProcessHookZone(,...)
{
	_asm
	{
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		jmp [JmpFunctionAddress];
	}
}
VOID AntiInlineHook(WCHAR *FunctionName)
{
	ULONG ulTemp = NULL;
	PUCHAR p;
	BOOL bIsHooked = FALSE;
	INSTRUCTION	Inst;
	INSTRUCTION	Instb;
	BOOL bInit = FALSE;
	ULONG ulHookFunctionAddress;
	ULONG JmpReloadFunctionAddress;

	__try
	{
		//RMmIsAddressValid = ReLoadNtosCALL(L"MmIsAddressValid",SystemKernelModuleBase,ImageModuleBase);
		if (RMmIsAddressValid)
		{
			bInit = TRUE;
		}
		if (!bInit)
			return;

		JmpFunctionAddress = GetSystemRoutineAddress(1,FunctionName);  //得到函数地址

		if (DebugOn)
			KdPrint(("Get System Routine Address:%ws:%08x\r\n",FunctionName,JmpFunctionAddress));

		if (JmpFunctionAddress)
		{
			JmpReloadFunctionAddress = JmpFunctionAddress - SystemKernelModuleBase + (ULONG)ImageModuleBase;  //获得原始地址

			if (!RMmIsAddressValid(JmpFunctionAddress) ||
				!RMmIsAddressValid(JmpReloadFunctionAddress))
			{
				if (DebugOn)
					KdPrint(("get function failed\r\n"));
				return;
			}
			if (GetFunctionCodeSize(JmpFunctionAddress) == GetFunctionCodeSize(JmpReloadFunctionAddress) &&
				memcmp(JmpFunctionAddress,JmpReloadFunctionAddress,GetFunctionCodeSize(JmpFunctionAddress)) != NULL)
			{
				if (DebugOn)
					KdPrint(("search---->%ws",FunctionName));
				//KdPrint(("---->%s:%08x",functionName,ulOldAddress));
				//开始扫描hook
				for (p=JmpFunctionAddress ;p< JmpFunctionAddress+GetFunctionCodeSize(JmpFunctionAddress); p++)
				{
					//是否结束？
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
						RMmIsAddressValid(ulTemp))
					{
						if (ulTemp > SystemKernelModuleBase &&
							ulTemp < SystemKernelModuleBase+SystemKernelModuleSize)   //太近的跳也不是
						{
							continue;
						}
						//ulTemp也不能小于 SystemKernelModuleBase
						if (ulTemp < SystemKernelModuleBase)
						{
							continue;
						}
						if (*(ULONG *)ulTemp == 0x00000000 ||
							*(ULONG *)ulTemp == 0x00000005)
						{
							continue;
						}
						if (ulTemp > ulMyDriverBase &&
							ulTemp < ulMyDriverBase + ulMyDriverSize)
						{
							if (DebugOn)
								KdPrint(("safesystem hook, denied access！"));
							return;
						}
						ulRunAddress = (ULONG)p - (ULONG)JmpFunctionAddress;   //执行到达hook点的时候，一共执行了多少长度的代码
						JmpReloadFunctionAddress = JmpReloadFunctionAddress + ulRunAddress;     //跳过前面执行的代码，继续往下执行 

						if (DebugOn)
							KdPrint(("found hook---->%ws:%08x:%08x-%x-%08x",FunctionName,ulTemp + 0x5,p,ulRunAddress,JmpReloadFunctionAddress));


						//得到正确的跳转地址，直接hook人家的hook函数的头部，然后让它跳到reload代码的ulReloadAddress地址处继续执行剩下的代码，这样就绕过hook鸟
						ulTemp = ulTemp + 0x5;

						HookFunctionByHeaderAddress(
							(DWORD)JmpReloadFunctionAddress,
							ulTemp,
							(PVOID)HookFunctionProcessHookZone,
							&HookFunctionProcessPatchCodeLen,
							&HookFunctionProcessRet
							);
					}
				}
			}
		}

	}__except(EXCEPTION_EXECUTE_HANDLER){

	}
	return;
}