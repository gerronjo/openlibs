#include "reg.h"


PVOID NtCreateKey = NULL;
PVOID NtSetValueKey = NULL;
PVOID NtDeleteKey = NULL;
PVOID NtDeleteValueKey = NULL;
PVOID NtQueryKey=NULL;
PVOID NtQueryValueKey = NULL;


//新的处理函数
NTSTATUS NewObReferenceObjectByHandle_forNtCreateKeyAndNtSetValueKey(
	IN HANDLE  Handle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_TYPE  ObjectType  OPTIONAL,
	IN KPROCESSOR_MODE  AccessMode,
	OUT PVOID  *Object,
	OUT POBJECT_HANDLE_INFORMATION  HandleInformation  OPTIONAL
	)
{
	//全局
	if (bGlobal_disableCreateReg)
	{
		//直接返回失败
		return STATUS_ACCESS_DENIED;
	}

	//这里是指定目标进程操作
	if (b_disableCreateReg)
	{
		NTSTATUS status;
		PDISABLE_CREATE_REG p= NULL;
		PEPROCESS pEpr = NULL;

		//遍历单向列表
		for (p=pDisableCreateRegInfo;p;p=p->next)
		{
			status = PsLookupProcessByProcessId((HANDLE)p->ulPID,&pEpr);

			if (NT_SUCCESS(status))
			{
				//如果目标进程的EPROCESS 等于 ring3下的应用程序EPROCESS
				if (PsGetCurrentProcess() == pEpr)
				{
					//返回句柄无效
					return STATUS_ACCESS_DENIED;
				}
			}																			
		}		
	}

	return ObReferenceObjectByHandle(Handle,DesiredAccess,ObjectType,AccessMode,Object,HandleInformation);
}

//新的处理函数
NTSTATUS NewObReferenceObjectByHandle_forNtDeleteKeyAndNtDeleteVauleKey(
	IN HANDLE  Handle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_TYPE  ObjectType  OPTIONAL,
	IN KPROCESSOR_MODE  AccessMode,
	OUT PVOID  *Object,
	OUT POBJECT_HANDLE_INFORMATION  HandleInformation  OPTIONAL
	)
{
	if (bGlobal_disableDeleteReg)
	{
		//直接返回失败
		return STATUS_ACCESS_DENIED;
	}

	//这里是指定目标进程操作
	if (b_disableDeleteReg)
	{
		NTSTATUS status;
		PDISABLE_DELETE_REG p= NULL;
		PEPROCESS pEpr = NULL;

		//遍历单向列表
		for (p=pDisableDeleteRegInfo;p;p=p->next)
		{
			status = PsLookupProcessByProcessId((HANDLE)p->ulPID,&pEpr);

			if (NT_SUCCESS(status))
			{
				//如果目标进程的EPROCESS 等于 ring3下的应用程序EPROCESS
				if (PsGetCurrentProcess() == pEpr)
				{
					//返回句柄无效
					return STATUS_ACCESS_DENIED;
				}
			}																			
		}		
	}

	return ObReferenceObjectByHandle(Handle,DesiredAccess,ObjectType,AccessMode,Object,HandleInformation);
}

//新的处理函数
NTSTATUS NewObReferenceObjectByHandle_forNtQueryKeyAndNtQueryValueKey(
	IN HANDLE  Handle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_TYPE  ObjectType  OPTIONAL,
	IN KPROCESSOR_MODE  AccessMode,
	OUT PVOID  *Object,
	OUT POBJECT_HANDLE_INFORMATION  HandleInformation  OPTIONAL
	)
{
	if (bGlobal_disableOpenReg)
	{
		//直接返回失败
		return STATUS_ACCESS_DENIED;
	}

	//这里是指定目标进程操作
	if (b_disableOpenReg)
	{
		NTSTATUS status;
		PDISABLE_OPEN_REG p= NULL;
		PEPROCESS pEpr = NULL;

		//遍历单向列表
		for (p=pDisableOpenRegInfo;p;p=p->next)
		{
			status = PsLookupProcessByProcessId((HANDLE)p->ulPID,&pEpr);

			if (NT_SUCCESS(status))
			{
				//如果目标进程的EPROCESS 等于 ring3下的应用程序EPROCESS
				if (PsGetCurrentProcess() == pEpr)
				{
					//返回句柄无效
					return STATUS_ACCESS_DENIED;
				}
			}																			
		}		
	}

	return ObReferenceObjectByHandle(Handle,DesiredAccess,ObjectType,AccessMode,Object,HandleInformation);
}


//在这里InlineHook,禁止创建项和键值
VOID reg_inlineHook_disableCreate()
{
	__try
	{
		//获得原有地址
		NtCreateKey = (PVOID)GetSSDTFuncAddress(ZwCreateKey);
		NtSetValueKey = (PVOID)GetSSDTFuncAddress(ZwSetValueKey);
		//inline hook
		common_callAddrHook(NtCreateKey,ObReferenceObjectByHandle,NewObReferenceObjectByHandle_forNtCreateKeyAndNtSetValueKey,PAGE_SIZE);
		common_callAddrHook(NtSetValueKey,ObReferenceObjectByHandle,NewObReferenceObjectByHandle_forNtCreateKeyAndNtSetValueKey,PAGE_SIZE);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("reg_inlineHook_disableCreate EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//停止禁止创建项和键值
VOID reg_inlineHook_disableCreate_stop()
{
	__try
	{
		//inline hook
		common_callAddrHook(NtCreateKey,NewObReferenceObjectByHandle_forNtCreateKeyAndNtSetValueKey,ObReferenceObjectByHandle,PAGE_SIZE);
		common_callAddrHook(NtSetValueKey,NewObReferenceObjectByHandle_forNtCreateKeyAndNtSetValueKey,ObReferenceObjectByHandle,PAGE_SIZE);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("reg_inlineHook_disableCreate_stop EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//在这里InlineHook,禁止删除项和键值
VOID reg_inlineHook_disableDelete()
{
	__try
	{
		//获得原有地址
		NtDeleteKey = (PVOID)GetSSDTFuncAddress(ZwDeleteKey);
		NtDeleteValueKey = (PVOID)GetSSDTFuncAddress(ZwDeleteValueKey);
		//inline hook
		common_callAddrHook(NtDeleteKey,ObReferenceObjectByHandle,NewObReferenceObjectByHandle_forNtDeleteKeyAndNtDeleteVauleKey,PAGE_SIZE);
		common_callAddrHook(NtDeleteValueKey,ObReferenceObjectByHandle,NewObReferenceObjectByHandle_forNtDeleteKeyAndNtDeleteVauleKey,PAGE_SIZE);
	
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("reg_inlineHook_disableDelete EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//停止禁止删除项和键值
VOID reg_inlineHook_disableDelete_stop()
{
	__try
	{
		//inline hook
		common_callAddrHook(NtDeleteKey,NewObReferenceObjectByHandle_forNtDeleteKeyAndNtDeleteVauleKey,ObReferenceObjectByHandle,PAGE_SIZE);
		common_callAddrHook(NtDeleteValueKey,NewObReferenceObjectByHandle_forNtDeleteKeyAndNtDeleteVauleKey,ObReferenceObjectByHandle,PAGE_SIZE);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("reg_inlineHook_disableDelete_stop EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//在这里InlineHook,禁止查询
VOID reg_inlineHook_disableQuery()
{
	__try
	{
		//获得原有地址
		NtQueryValueKey = (PVOID)GetSSDTFuncAddress(ZwQueryValueKey);
		NtQueryKey = (PVOID)GetSSDTFuncAddress(ZwQueryKey);

		common_callAddrHook(NtQueryKey,ObReferenceObjectByHandle,NewObReferenceObjectByHandle_forNtQueryKeyAndNtQueryValueKey,PAGE_SIZE);
		common_callAddrHook(NtQueryValueKey,ObReferenceObjectByHandle,NewObReferenceObjectByHandle_forNtQueryKeyAndNtQueryValueKey,PAGE_SIZE);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("reg_inlineHook_disableQuery EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//停止禁止查询
VOID reg_inlineHook_disableQuery_stop()
{
	__try
	{
		common_callAddrHook(NtQueryKey,NewObReferenceObjectByHandle_forNtQueryKeyAndNtQueryValueKey,ObReferenceObjectByHandle,PAGE_SIZE);
		common_callAddrHook(NtQueryValueKey,NewObReferenceObjectByHandle_forNtQueryKeyAndNtQueryValueKey,ObReferenceObjectByHandle,PAGE_SIZE);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("reg_inlineHook_disableQuery_stop EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//从DisableCreateRegInfo中查找目标进程ID
//返回
//0表示没有找到，1表示找到了
ULONG reg_findPIDInDisableCreateRegInfo(ULONG ulPID)
{
	PDISABLE_CREATE_REG p = NULL;

	__try
	{
		//遍历单向列表
		for (p=pDisableCreateRegInfo;p;p=p->next)
		{
			if (p->ulPID == ulPID)
			{
				//表示找到了
				return 1;
			}
		}

		//没找到的话
		return 0;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("reg_findPIDInDisableCreateRegInfo EXCEPTION_EXECUTE_HANDLER error!");
		return 0;
	}
}

//从DisableDeleteRegInfo中查找目标进程ID
//返回
//0表示没有找到，1表示找到了
ULONG reg_findPIDInDisableDeleteRegInfo(ULONG ulPID)
{
	PDISABLE_DELETE_REG p = NULL;

	__try
	{
		//遍历单向列表
		for (p=pDisableDeleteRegInfo;p;p=p->next)
		{
			if (p->ulPID == ulPID)
			{
				//表示找到了
				return 1;
			}
		}

		//没找到的话
		return 0;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("reg_findPIDInDisableDeleteRegInfo EXCEPTION_EXECUTE_HANDLER error!");
		return 0;
	}
}

//从DisableOpenRegInfo中查找目标进程ID
//返回
//0表示没有找到，1表示找到了
ULONG reg_findPIDInDisableOpenRegInfo(ULONG ulPID)
{
	PDISABLE_OPEN_REG p = NULL;

	__try
	{
		//遍历单向列表
		for (p=pDisableOpenRegInfo;p;p=p->next)
		{
			if (p->ulPID == ulPID)
			{
				//表示找到了
				return 1;
			}
		}

		//没找到的话
		return 0;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("reg_findPIDInDisableOpenRegInfo EXCEPTION_EXECUTE_HANDLER error!");
		return 0;
	}
}


//从DisableCreateRegInfo中删除目标进程ID
VOID reg_deletePIDInDisableCreateRegInfo(ULONG ulPID)
{
	PDISABLE_CREATE_REG p = NULL;
	PDISABLE_CREATE_REG p2 = NULL;

	__try
	{
		for (p2 = p =pDisableCreateRegInfo;p;p2=p,p=p->next)
		{
			if (p->ulPID == ulPID)
			{
				if (pDisableCreateRegInfo==p)
				{
					pDisableCreateRegInfo = pDisableCreateRegInfo->next;
				}
				else
				{
					p2->next=p->next;
				}
			}
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("reg_deletePIDInDisableCreateRegInfo EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//从DisableDeleteRegInfo中删除目标进程ID
VOID reg_deletePIDInDisableDeleteRegInfo(ULONG ulPID)
{
	PDISABLE_DELETE_REG p = NULL;
	PDISABLE_DELETE_REG p2 = NULL;

	__try
	{
		for (p2 = p =pDisableDeleteRegInfo;p;p2=p,p=p->next)
		{
			if (p->ulPID == ulPID)
			{
				if (pDisableDeleteRegInfo==p)
				{
					pDisableDeleteRegInfo = pDisableDeleteRegInfo->next;
				}
				else
				{
					p2->next=p->next;
				}
			}
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("reg_deletePIDInDisableDeleteRegInfo EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//从DisableOpenRegInfo中删除目标进程ID
VOID reg_deletePIDInDisableOpenRegInfo(ULONG ulPID)
{
	PDISABLE_OPEN_REG p = NULL;
	PDISABLE_OPEN_REG p2 = NULL;

	__try
	{
		for (p2 = p =pDisableOpenRegInfo;p;p2=p,p=p->next)
		{
			if (p->ulPID == ulPID)
			{
				if (pDisableOpenRegInfo==p)
				{
					pDisableOpenRegInfo = pDisableOpenRegInfo->next;
				}
				else
				{
					p2->next=p->next;
				}
			}
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("reg_deletePIDInDisableOpenRegInfo EXCEPTION_EXECUTE_HANDLER error!");
	}
}