//**************************************************************************
//*
//*          A盾电脑防护 website:http://www.3600safe.com/
//*        
//*文件说明：
//*         禁止加载驱动
//**************************************************************************
#include "DriverHips.h"

__declspec(naked) BOOLEAN SeSinglePrivilegeCheckHookZone(,...)
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
		jmp [SeSinglePrivilegeCheckRet];
	}
}
//权限检查的时候返回失败来达到禁止加载驱动
BOOLEAN __stdcall NewSeSinglePrivilegeCheck(
	__in  LUID PrivilegeValue,
	__in  KPROCESSOR_MODE PreviousMode
	)
{
	ULONG ulPage;

	if (!bIsInitSuccess)
		goto _FunctionRet;

	//取返回地址
	_asm
	{
		mov eax,dword ptr[ebp+4]
		mov ulPage,eax
	}
	//KdPrint(("ulPage:%08x\r\n",ulPage));
	//RPsGetCurrentProcess = ReLoadNtosCALL(L"PsGetCurrentProcess",SystemKernelModuleBase,ImageModuleBase);
	if (!RPsGetCurrentProcess)
	{
		goto _FunctionRet;
	}
	if (RPsGetCurrentProcess() == ProtectEProcess)
	{
		goto _FunctionRet;
	}

	if (ulPage >= ulNtLoadDriverBase && ulPage <= ulNtLoadDriverBase+ulNtLoadDriverSize)
		return FALSE;
	
	if (ulPage >= ulZwSetSystemInformationBase && ulPage <= ulZwSetSystemInformationBase+ulZwSetSystemInformationSize)
		return FALSE;

_FunctionRet:
	OldSeSinglePrivilegeCheck = (SeSinglePrivilegeCheck_1)SeSinglePrivilegeCheckHookZone;
	return OldSeSinglePrivilegeCheck(
		PrivilegeValue,
		PreviousMode
		);
}
//禁止驱动加载
NTSTATUS DisEnableDriverLoading()
{
	int bRet;

	ulZwSetSystemInformationBase = GetSystemRoutineAddress(1,L"ZwSetSystemInformation");
	ulNtLoadDriverBase = GetSystemRoutineAddress(1,L"ZwLoadDriver");
	if (ulNtLoadDriverBase &&
		ulZwSetSystemInformationBase)
	{
		ulNtLoadDriverSize = SizeOfProc(ulNtLoadDriverBase);
		ulZwSetSystemInformationSize = SizeOfProc(ulZwSetSystemInformationBase);
	}

	ulSeSinglePrivilegeCheck = GetSystemRoutineAddress(1,L"SeSinglePrivilegeCheck");
	if (!ulSeSinglePrivilegeCheck ||
		!ulNtLoadDriverBase ||
		!ulZwSetSystemInformationBase)
	{
		return STATUS_UNSUCCESSFUL;
	}
	//计算reload后的地址，不然判断不对
	ulNtLoadDriverBase = ulNtLoadDriverBase - SystemKernelModuleBase+ImageModuleBase;
	ulZwSetSystemInformationBase = ulZwSetSystemInformationBase - SystemKernelModuleBase+ImageModuleBase;
	ulReloadSeSinglePrivilegeCheck = ulSeSinglePrivilegeCheck - SystemKernelModuleBase+ImageModuleBase;

	//hook reload SeSinglePrivilegeCheck

	bRet = HookFunctionByHeaderAddress(ulReloadSeSinglePrivilegeCheck,ulSeSinglePrivilegeCheck,SeSinglePrivilegeCheckHookZone,&SeSinglePrivilegeCheckPatchCodeLen,&SeSinglePrivilegeCheckRet);
	if(bRet)
	{
		bRet = FALSE;
		bRet = HookFunctionByHeaderAddress(
			NewSeSinglePrivilegeCheck,
			ulReloadSeSinglePrivilegeCheck,
			SeSinglePrivilegeCheckHookZone,
			&SeSinglePrivilegeCheckPatchCodeLen,
			&SeSinglePrivilegeCheckRet
			);
		if (bRet)
		{
			SeSinglePrivilegeCheckHooked = TRUE;
			//DbgPrint("hook SeSinglePrivilegeCheck success\n");
		}
	}
	return STATUS_SUCCESS;
}
//允许驱动加载
NTSTATUS EnableDriverLoading()
{
	if (SeSinglePrivilegeCheckHooked == TRUE)
	{
		SeSinglePrivilegeCheckHooked = FALSE;
		UnHookFunctionByHeaderAddress((DWORD)ulReloadSeSinglePrivilegeCheck,SeSinglePrivilegeCheckHookZone,SeSinglePrivilegeCheckPatchCodeLen);
		UnHookFunctionByHeaderAddress((DWORD)ulSeSinglePrivilegeCheck,SeSinglePrivilegeCheckHookZone,SeSinglePrivilegeCheckPatchCodeLen);
	}
}