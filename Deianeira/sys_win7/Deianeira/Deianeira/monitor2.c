#include "monitor2.h"


//保存原先开头的5个自己
BYTE OldObAddress[5]={0};
//jmp新的函数地址
BYTE NewObAddress[5]={0xe9,0,0,0,0};


//在这里InlineHook
VOID monitor2_inlineHookObReferenceObjectByHandle()
{

	//赋值前面定义的数组
	KIRQL Irql;

	//保存函数前五个字节内容
	RtlCopyMemory(OldObAddress,(PBYTE)ObReferenceObjectByHandle,5);
	//保存新函数五个字节之后偏移
	*(PULONG)(NewObAddress+1)=(ULONG)NewObReferenceObjectByHandle-((ULONG)ObReferenceObjectByHandle+5);

	//开始inline hook
	//关闭内存写保护
	UnProtected();

	//提升IRQL中断级
	Irql=KeRaiseIrqlToDpcLevel();
	//函数开头五个字节写JMP 
	RtlCopyMemory((PBYTE)ObReferenceObjectByHandle,NewObAddress,5);
	//恢复Irql
	KeLowerIrql(Irql);

	//开启内存写保护
	Protected();
}


//恢复
VOID monitor2_unInlineHookObReferenceObjectByHandle()
{

	//把五个字节再写回到原函数
	KIRQL Irql;

	//关闭写保护
	UnProtected();

	//提升IRQL到Dpc
	Irql=KeRaiseIrqlToDpcLevel();
	//用原先保存的地址覆盖前5个字节
	RtlCopyMemory((PBYTE)ObReferenceObjectByHandle,OldObAddress,5);
	//提升IRQL到Dpc
	KeLowerIrql(Irql);

	//开启写保护
	Protected();
}


//自己构造的原始函数
_declspec(naked) NTSTATUS OldObReferenceObjectByHandle(
	IN HANDLE  Handle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_TYPE  ObjectType  OPTIONAL,
	IN KPROCESSOR_MODE  AccessMode,
	OUT PVOID  *Object,
	OUT POBJECT_HANDLE_INFORMATION  HandleInformation  OPTIONAL
	)
{
	_asm
	{   
		mov edi,edi
			push ebp
			mov ebp,esp
			mov eax,ObReferenceObjectByHandle
			add eax,5
			jmp eax                
	}
}

//新的ObReferenceObjectByHandle函数
NTSTATUS
NewObReferenceObjectByHandle(
							 IN HANDLE  Handle,
							 IN ACCESS_MASK  DesiredAccess,
							 IN POBJECT_TYPE  ObjectType  OPTIONAL,
							 IN KPROCESSOR_MODE  AccessMode,
							 OUT PVOID  *Object,
							 OUT POBJECT_HANDLE_INFORMATION  HandleInformation  OPTIONAL
						  )
{
	//Drivers can use the following flags for handles to processes:
	//
	//Flag Allowed operations 
	//PROCESS_CREATE_PROCESS Create a new child process of the process. 
	//PROCESS_CREATE_THREAD Create a new thread in the context of the process. 
	//PROCESS_DUP_HANDLE Duplicate handles to or from the context of the process, such as by calling the user-mode DuplicateHandle routine. 
	//PROCESS_SET_QUOTA Set the working set size for the process, such as by calling the user-mode SetProcessWorkingSetSize routine. 
	//PROCESS_SET_INFORMATION Modify process settings, such as by calling the user-mode SetPriorityClass routine. 
	//PROCESS_SUSPEND_RESUME Suspend or resume the process. 
	//PROCESS_TERMINATE Terminate the process, such as by calling the user-mode TerminateProcess routine.. 
	//PROCESS_VM_OPERATIONS Modify the address space of the process, such as by calling the user-mode WriteProcessMemory and VirtualProtectEx routines. 
	//PROCESS_VM_WRITE Write to the address space of the process, such as by calling the user-mode WriteProcessMemory routine. 
	//
	//Drivers can use the following flags for handles to threads:
	//
	//Flag Allowed operations 
	//THREAD_DIRECT_IMPERSONATION Enable a server thread to impersonate one of its clients. 
	//THREAD_IMPERSONATE Impersonate the operating system's anonymous logon token, such as by calling the user-mode ImpersonateAnonymousToken routine. 
	//THREAD_SET_CONTEXT Modify the thread's execution context, such as by calling the user-mode SetThreadContext routine. 
	//THREAD_SET_INFORMATION Modify thread settings, such as by calling the user-mode SetThreadIdealProcessor routine. The operations that are permitted by this access right are a superset of those that are permitted by the THREAD_SET_LIMITED_INFORMATION access right. 
	//THREAD_SET_LIMITED_INFORMATION Modify a limited set of thread settings, such as by calling the user-mode SetThreadAffinityMask and SetThreadPriorityBoost routines. 
	//THREAD_SET_THREAD_TOKEN Modify properties of the thread's impersonation token, such as by calling the user-mode SetTokenInformation routine. 
	//THREAD_SUSPEND_RESUME Suspend or resume the thread, such as by calling the user-mode SuspendThread and ResumeThread routines. 
	//THREAD_TERMINATE Terminate the thread, such as by calling the user-mode TerminateThread routine. 

	//ObReferenceObjectByHandle returns an NTSTATUS value. The possible return values include:

	//STATUS_SUCCESS
	//STATUS_OBJECT_TYPE_MISMATCH
	//STATUS_ACCESS_DENIED
	//STATUS_INVALID_HANDLE

	NTSTATUS status,status2;

	//调用原函数
	status=OldObReferenceObjectByHandle(Handle,DesiredAccess,ObjectType,AccessMode,Object,HandleInformation);

	//如果成功
	if (NT_SUCCESS(status))
	{
		if(ObjectType == *PsProcessType)
		{
			switch(DesiredAccess)
			{
				//创建进程
			case PROCESS_CREATE_PROCESS:
				{
					//在这里禁止创建进程
					if (bGlobal_disableCreateProcess)
					{
						ObDereferenceObject(*Object);
						//返回句柄无效
						return STATUS_ACCESS_DENIED;
					}

					//这里是指定目标进程操作
					if (b_disableCreateProcess)
					{
						PDISABLE_CREATE_PROCESS p = NULL;
						PEPROCESS pEpr = NULL;

						//遍历单向列表
						for (p=pDisableCreateProcessInfo;p;p=p->next)
						{
							status2 = PsLookupProcessByProcessId((HANDLE)p->ulPID,&pEpr);

							if (NT_SUCCESS(status2))
							{
								//如果目标进程的EPROCESS 等于 ring3下的应用程序EPROCESS
								if (PsGetCurrentProcess() == pEpr)
								{
									ObDereferenceObject(*Object);
									//返回句柄无效
									return STATUS_ACCESS_DENIED;
								}
							}																			
						}		
					}
				}
				//创建线程
			case PROCESS_CREATE_THREAD:
				{
					//在这里禁止创建线程
					if (bGlobal_disableCreateThread)
					{
						ObDereferenceObject(*Object);
						//返回句柄无效
						return STATUS_ACCESS_DENIED;
					}
				}
			case PROCESS_DUP_HANDLE:
			case PROCESS_SET_QUOTA:
			case PROCESS_SET_INFORMATION:
			case PROCESS_SUSPEND_RESUME:
				//结束进程
			case PROCESS_TERMINATE:
				{
					//在这里禁止结束进程
					if (bGlobal_disableTerminateProcess)
					{
						ObDereferenceObject(*Object);
						//返回句柄无效
						return STATUS_ACCESS_DENIED;
					}
					//这里是指定目标进程操作
					if (b_disableTerminateProcess)
					{
						PDISABLE_TERMINATE_PROCESS p = NULL;
						PEPROCESS pEpr = NULL;

						//遍历单向列表
						for (p=pDisableTerminateProcessInfo;p;p=p->next)
						{
							status2 = PsLookupProcessByProcessId((HANDLE)p->ulPID,&pEpr);

							if (NT_SUCCESS(status2))
							{
								//如果目标进程的EPROCESS 等于 ring3下的应用程序EPROCESS
								if ((PEPROCESS)(*Object) == pEpr)
								{
									ObDereferenceObject(*Object);
									//返回句柄无效
									return STATUS_ACCESS_DENIED;
								}
							}																			
						}		
					}
				}
			case PROCESS_VM_OPERATION:
			case PROCESS_VM_WRITE:
				{
					if(b_startSelfProtectionByObReferenceObjectByHandle)
					{
						//如果是进程自己操作自己的话
						if(PsGetCurrentProcessId() == ulProcessID)
						{
							return status;
						}

						//如果别人在操作自己的进程句柄的话
						//如果目标进程的EPROCESS 等于 ring3下的应用程序EPROCESS
						if ((PEPROCESS)(*Object) == pEProcess)
						{
							ObDereferenceObject(*Object);
							//返回句柄无效
							return STATUS_ACCESS_DENIED;
						}
					}
				}
				break;
			}
		}
		else if(Object == *PsThreadType)
		{
			switch(DesiredAccess)
			{
				  //线程结束
			case THREAD_TERMINATE:
				  {
					  //在这里禁止结束线程
					  if (bGlobal_disableTerminateThread)
					  {
						  ObDereferenceObject(*Object);
						  //返回句柄无效
						  return STATUS_ACCESS_DENIED;
					  }
				  }
				  break;
			}
		}
	}

	return status;
}

//从DisableCreateProcessInfo中查找目标进程ID
//返回
//0表示没有找到，1表示找到了
ULONG monitor2_findPIDInDisableCreateProcessInfo(ULONG ulPID)
{
	PDISABLE_CREATE_PROCESS p = NULL;

	__try
	{
		//遍历单向列表
		for (p=pDisableCreateProcessInfo;p;p=p->next)
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
		DbgPrint("monitor2_findPIDInDisableCreateProcessInfo EXCEPTION_EXECUTE_HANDLER error!");
		return 0;
	}
}

//从DisableCreateProcessInfo中删除目标进程ID
VOID monitor2_deletePIDInDisableCreateProcessInfo(ULONG ulPID)
{
	PDISABLE_CREATE_PROCESS p = NULL;
	PDISABLE_CREATE_PROCESS p2 = NULL;

	__try
	{
		for (p2 = p =pDisableCreateProcessInfo;p;p2=p,p=p->next)
		{
			if (p->ulPID == ulPID)
			{
				if (pDisableCreateProcessInfo==p)
				{
					pDisableCreateProcessInfo = pDisableCreateProcessInfo->next;
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
		DbgPrint("monitor2_deletePIDInDisableCreateProcessInfo EXCEPTION_EXECUTE_HANDLER error!");
	}
}

//从DisableTerminateProcessInfo中查找目标进程ID
//返回
//0表示没有找到，1表示找到了
ULONG monitor2_findPIDInDisableTerminateProcessInfo(ULONG ulPID)
{
	PDISABLE_TERMINATE_PROCESS p = NULL;

	//遍历单向列表
	for (p=pDisableTerminateProcessInfo;p;p=p->next)
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

//从DisableTerminateProcessInfo中删除目标进程ID
VOID monitor2_deletePIDInDisableTerminateProcessInfo(ULONG ulPID)
{
	PDISABLE_TERMINATE_PROCESS p = NULL;
	PDISABLE_TERMINATE_PROCESS p2 = NULL;

	__try
	{
		for (p2 = p =pDisableTerminateProcessInfo;p;p2=p,p=p->next)
		{
			if (p->ulPID == ulPID)
			{
				if (pDisableTerminateProcessInfo==p)
				{
					pDisableTerminateProcessInfo = pDisableTerminateProcessInfo->next;
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
		DbgPrint("monitor2_deletePIDInDisableTerminateProcessInfo EXCEPTION_EXECUTE_HANDLER error!");
	}
}