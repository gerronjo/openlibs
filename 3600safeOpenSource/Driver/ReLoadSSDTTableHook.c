//**************************************************************************
//*
//*          A盾电脑防护 website:http://www.3600safe.com/
//*        
//*文件说明：
//*         hook自己reload的ssdt表
//**************************************************************************
#include "ReLoadSSDTTableHook.h"

extern PSERVICE_DESCRIPTOR_TABLE Safe_ServiceDescriptorTable;
extern PSERVICE_DESCRIPTOR_TABLE OriginalServiceDescriptorTable;

BOOL SystemCallEntryTableHook(char *FunctionName,int *Index,DWORD NewFuctionAddress)
{
	KIRQL oldIrql; 
	BOOL bRetOK = FALSE;
	DWORD FunctionAddress;

	if (!GetFunctionIndexByName(FunctionName,Index))
	{
		return bRetOK;
	}

	//KdPrint(("%s:%d",FunctionName,*Index));

	__try
	{
		//if (DebugOn)
		//	KdPrint(("SystemCallEntry:%ws",FunctionName->Buffer));

		if (MmIsAddressValid(Safe_ServiceDescriptorTable) &&
			MmIsAddressValid(Safe_ServiceDescriptorTable->ServiceTable))
		{
			//if (DebugOn)
			//	KdPrint(("Safe_ServiceDescriptorTable:%d %d",*Index,Safe_ServiceDescriptorTable->TableSize));

			if (*Index >= 0 && *Index < Safe_ServiceDescriptorTable->TableSize)
			{
				//因为是自己reload表，所以不需要开启cr0
// 				_asm
// 				{
// 					CLI  ;                 
// 					MOV    EAX, CR0  ;    
// 					AND EAX, NOT 10000H ;
// 					MOV    CR0, EAX;        
// 				}
//				Safe_ServiceDescriptorTable->ServiceTable[*Index]=NewFuctionAddress;
				//if (DebugOn)
					//KdPrint(("Safe_ServiceDescriptorTable:%x %d %08x",*Index,Safe_ServiceDescriptorTable->TableSize,(ULONG)Safe_ServiceDescriptorTable));

				InterlockedExchange(&Safe_ServiceDescriptorTable->ServiceTable[*Index],NewFuctionAddress);
				bRetOK = TRUE;
// 				_asm 
// 				{
// 					MOV    EAX, CR0;          
// 					OR    EAX, 10000H;            
// 					MOV    CR0, EAX ;              
// 					STI;                    
// 				}
			}
		}

	}__except(EXCEPTION_EXECUTE_HANDLER){

	}
	return bRetOK;

}
BOOL SystemCallEntryTableUnHook(int Index)
{
	KIRQL oldIrql; 

	//oldIrql = KeRaiseIrqlToDpcLevel(); //注意自旋锁

	Safe_ServiceDescriptorTable->ServiceTable[Index] = 0;

	//KeLowerIrql(oldIrql);

	return TRUE;
}