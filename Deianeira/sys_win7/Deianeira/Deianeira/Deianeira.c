//////////////////////////////////////////////////////////////////////////
//
//

#include "common.h"
#include "monitor.h"
#include "protection.h"
#include "process.h"
#include "file.h"
#include "shadow.h"
#include "monitor2.h"
#include "reg.h"
#include "sysmodule.h"
#include "kernel.h"


//处理IRP_MJ_CREATE
NTSTATUS DispatchCreateClose(PDEVICE_OBJECT pDevObj,PIRP pIrp)
{
	DbgPrint("DispatchCreateClose！");
	pIrp->IoStatus.Status=STATUS_SUCCESS;
	IoCompleteRequest(pIrp,IO_NO_INCREMENT);
	return STATUS_SUCCESS; 
}


//驱动卸载
VOID DriverUnload(IN PDRIVER_OBJECT   pDriverObj)
{
	PDEVICE_OBJECT    pDevObj = pDriverObj->DeviceObject;
	UNICODE_STRING    ustrLinkName;

	DbgPrint("DriverUnload！");

	RtlInitUnicodeString(&ustrLinkName, LINK_NAME);
    //删除符号链接
	IoDeleteSymbolicLink(&ustrLinkName);
    //删除设备
	IoDeleteDevice(pDevObj);
}

// I/O控制派遣例程
NTSTATUS DispatchIoctl(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;
	PVOID InputBuffer;
	PVOID OutputBuffer;
    ULONG uInSize;
    ULONG uOutSize;

	InputBuffer = OutputBuffer = pIrp->AssociatedIrp.SystemBuffer;

	//得到输入缓冲区大小
	uInSize = IoGetCurrentIrpStackLocation(pIrp)->Parameters.DeviceIoControl.InputBufferLength;
   
	//得到输出缓冲区大小
	uOutSize = IoGetCurrentIrpStackLocation(pIrp)->Parameters.DeviceIoControl.OutputBufferLength;

    //功能函数
	switch(IoGetCurrentIrpStackLocation(pIrp)->MajorFunction)
	{
	case IRP_MJ_CREATE:
		break;
	case IRP_MJ_CLOSE :
		break;
	case IRP_MJ_DEVICE_CONTROL:
		switch(IoGetCurrentIrpStackLocation(pIrp)->Parameters.DeviceIoControl.IoControlCode)
		{
			//初始化
		case IOCTL_APPLOAD:
			{

			}
			break;
			//
		case IOCTL_APPUNLOAD:
			{

			}
			break;
			//获得系统版本号
		case IOCTL_GETOSVERSION:	
			{
				__try
				{
					ulOSVersion = *(PULONG)InputBuffer;

					DbgPrint("ulOSVersion:%d",ulOSVersion);
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_GETOSVERSION error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//得到ring3下应用程序ID
		case IOCTL_GETPROCESSID:	
			{
				__try
				{
					ulProcessID = *(PULONG)InputBuffer;

					PsLookupProcessByProcessId((HANDLE)ulProcessID,&pEProcess);

					DbgPrint("ulProcessID:%d",ulProcessID);
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_GETPROCESSID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//开启ssdt自我保护
		case IOCTL_SELFPROTECTION_START_SSDT:	
			{
				__try
				{
					//开启自我保护
					protection_startInlineHook();
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_SELFPROTECTION_START_SSDT error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//停止ssdt自我保护
		case IOCTL_SELFPROTECTION_STOP_SSDT:	
			{
				__try
				{
					//停止自我保护
					protection_stopInlineHook();
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_SELFPROTECTION_STOP_SSDT error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//开启shadow自我保护
		case IOCTL_SELFPROTECTION_START_SHADOW:	
			{
				__try
				{
					//开启自我保护
					protection_startShadowSSDTHook();
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_SELFPROTECTION_START_SHADOW error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//停止shadow自我保护
		case IOCTL_SELFPROTECTION_STOP_SHADOW:	
			{
				__try
				{
					//停止自我保护
					protection_stopShadowSSDTHook();
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_SELFPROTECTION_STOP_SHADOW error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//开启所有自我保护
		case IOCTL_SELFPROTECTION_START_ALL:	
			{
				__try
				{
					//开启自我保护
                    protection_startInlineHook();
					protection_startShadowSSDTHook();

					//开启保护
					b_startSelfProtectionByObReferenceObjectByHandle = TRUE;

					//判定是否要开启
					if (!bGlobal_disableCreateProcess && !b_disableCreateProcess && !bGlobal_disableTerminateProcess && !b_disableTerminateProcess && !bGlobal_disableCreateThread && !bGlobal_disableTerminateThread)
					{
						monitor2_inlineHookObReferenceObjectByHandle();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_SELFPROTECTION_START_ALL error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//停止所有自我保护
		case IOCTL_SELFPROTECTION_STOP_ALL:	
			{
				__try
				{
					//停止自我保护
					protection_stopInlineHook();
					protection_stopShadowSSDTHook();

					//禁用保护
					b_startSelfProtectionByObReferenceObjectByHandle = FALSE;
					//判定是否要开启
					if (!bGlobal_disableCreateProcess && !b_disableCreateProcess && !bGlobal_disableTerminateProcess && !b_disableTerminateProcess && !bGlobal_disableCreateThread && !bGlobal_disableTerminateThread)
					{
						monitor2_unInlineHookObReferenceObjectByHandle();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_SELFPROTECTION_STOP_ALL error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//得到SSDT
		case IOCTL_SSDT_GET:	
			{
				__try
				{
					*((PULONG)OutputBuffer) = KeServiceDescriptorTable->NumberOfServices;
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_SSDT_GET error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//查询SSDT指定地址
		case IOCTL_SSDT_GETFUNCADDR:	
			{
				__try
				{
					//将结果传到用户输出位置
					*((PULONG)OutputBuffer) = *((PULONG)(KeServiceDescriptorTable->ServiceTableBase) + *(PULONG)InputBuffer);
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_SSDT_GETFUNCADDR error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//设置SSDT指定地址
		case IOCTL_SSDT_SETFUNCADDR:
			{
				__try
				{
					PSSDT_CLIENT_ID pSSDTClientID = NULL;
					//去掉内存保护
					UnProtected();

                    pSSDTClientID = (PSSDT_CLIENT_ID)InputBuffer;
					DbgPrint("ulIndex:%d,ulAddress:%08X",pSSDTClientID->ulIndex,pSSDTClientID->ulAddress);
					//获得数据
					
					//覆盖地址
					*((PULONG)(KeServiceDescriptorTable->ServiceTableBase) + pSSDTClientID->ulIndex) = pSSDTClientID->ulAddress;

					//开启内存保护
					Protected();
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_SSDT_SETFUNCADDR error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;	
			//查询Shadow SSDT指定地址
		case IOCTL_SHADOW_GETFUNCADDR:	
			{
				__try
				{
					//将结果传到用户输出位置
					*((PULONG)OutputBuffer) = KeServiceDescriptorTableShadow[1].ServiceTableBase[*(PULONG)InputBuffer];
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_SSDT_GETFUNCADDR error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//查询Shadow SSDT原始地址
		case IOCTL_SHADOW_GETFUNCADDR_ORI:	
			{
				__try
				{
					DbgPrint("Shadow SSDT Func ulIndex:%d",*(PULONG)InputBuffer);
					//将结果传到用户输出位置
					*((PULONG)OutputBuffer) = GetOldShadowSSDTAddress(*(PULONG)InputBuffer);
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_SHADOW_GETFUNCADDR_ORI error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//设置Shadow SSDT指定地址
		case IOCTL_SHADOW_SETFUNCADDR:
			{
				__try
				{
					PSHADOW_SSDT_CLIENT_ID pShadowSSDTClientID = NULL;
					//去掉内存保护
					UnProtected();

					pShadowSSDTClientID = (PSHADOW_SSDT_CLIENT_ID)InputBuffer;
					DbgPrint("ulIndex:%d,ulAddress:%08X",pShadowSSDTClientID->ulIndex,pShadowSSDTClientID->ulAddress);
					//获得数据

					//覆盖地址
					KeServiceDescriptorTableShadow[1].ServiceTableBase[pShadowSSDTClientID->ulIndex] = pShadowSSDTClientID->ulAddress;

					//开启内存保护
					Protected();				
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_SHADOW_SETFUNCADDR error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//挂起进程
		case IOCTL_PROCESS_SUSPEND:
			{
				__try
				{
					if(!process_suspendProcess(*(PULONG)InputBuffer))
					{
						break;
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_PROCESS_SUSPEND error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//恢复进程
		case IOCTL_PROCESS_RESUME:
			{
				__try
				{
					if(!process_resumeProcess(*(PULONG)InputBuffer))
					{
						break;
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_PROCESS_RESUME error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//进程结束,暴力结束
		case IOCTL_PROCESS_KILL:
			{
				__try
				{
					process_killProcess(*(PULONG)InputBuffer);
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_PROCESS_KILL error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//进程结束2
		case IOCTL_PROCESS_KILL2:
			{
				__try
				{
					//获得从ring3传过来的进程ID
					BOOLEAN bRet = FALSE;
					bRet = process_killProcessByPspTerminateThreadByPointer(*(PULONG)InputBuffer);

					if (!bRet)
					{
						break;
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_PROCESS_KILL2 error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//进程隐藏
		case IOCTL_PROCESS_HIDE:
			{
				__try
				{
					DbgPrint("IOCTL_PROCESS_HIDE ulPID:%d",*(PULONG)InputBuffer);
					process_hideProcess(*(PULONG)InputBuffer);
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_PROCESS_HIDE error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//结束线程
		case IOCTL_PROCESS_KILLTHREAD:
			{
				__try
				{
					CLIENT_ID clientID;
					BOOLEAN bRet = FALSE;
					//获得从ring3传过来的进程clientID
					
					//复制数据
					RtlCopyMemory(&clientID,InputBuffer,sizeof(CLIENT_ID));

					bRet = process_killThreadByPspTerminateThreadByPointer(clientID.UniqueProcess,clientID.UniqueThread);

					if (!bRet)
					{
						break;
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_PROCESS_KILLTHREAD error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//卸载进程模块
		case IOCTL_PROCESS_MODULE_UNLOAD:
			{
				__try
				{
					UNLOADDLL_CLIENT_ID clientID;
					BOOLEAN bRet = FALSE;
					//获得从ring3传过来的进程clientID

					//复制数据
					RtlCopyMemory(&clientID,InputBuffer,sizeof(UNLOADDLL_CLIENT_ID));

					//卸载
					bRet = process_unloadModule(clientID.ulPID,clientID.ulBaseAddress);

					if (!bRet)
					{
						break;
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_PROCESS_MODULE_UNLOAD error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//获得线程基址
		case IOCTL_PROCESS_THREAD_GETBASEADDR:
			{
				__try
				{
					CLIENT_ID clientID;
					ULONG ulBaseAddress;

					//获得从ring3传过来的进程clientID

					//复制数据
					RtlCopyMemory(&clientID,InputBuffer,sizeof(CLIENT_ID));

					ulBaseAddress = process_getThreadBaseAddr(clientID.UniqueProcess,clientID.UniqueThread);

					//复制数据
					RtlCopyMemory(OutputBuffer,&ulBaseAddress,sizeof(ULONG));
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_PROCESS_THREAD_GETBASEADDR error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//挂起线程
		case IOCTL_PROCESS_THREAD_SUSPEND:
			{
				__try
				{
					ULONG ulTID;
					ulTID = *(PULONG)InputBuffer;

					DbgPrint("IOCTL_PROCESS_THREAD_RESUME ulTID:%d",ulTID);

					//如果挂起失败，直接返回
					if (!process_suspendThread(ulTID))
					{
						break;
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_PROCESS_THREAD_SUSPEND error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;			
			//恢复线程运行
		case IOCTL_PROCESS_THREAD_RESUME:
			{
				__try
				{
					ULONG ulTID;
					ulTID = *(PULONG)InputBuffer;

					DbgPrint("IOCTL_PROCESS_THREAD_RESUME ulTID:%d",ulTID);

					//如果挂起失败，直接返回
					if (!process_resumeThread(ulTID))
					{
						break;
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_PROCESS_THREAD_RESUME error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//强制删除文件
		case IOCTL_FILE_DELETE:
			{
				__try
				{
                    //获得文件路径
					HANDLE  hFile;
					LPTSTR	lpFilePath = (LPTSTR)InputBuffer;

					DbgPrint("IOCTL_FILE_DELETE lpFilePath :%s",lpFilePath);

					//打开文件
					hFile = file_openFile(lpFilePath, FILE_READ_ATTRIBUTES, FILE_SHARE_DELETE);

			    	//如果打开文件成功
					if (hFile!=NULL)
					{
						if(!file_deleteFile(hFile))
						{
							//如果删除文件失败
							ZwClose(hFile);
							break;
						}
						ZwClose(hFile);
					}				
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_FILE_DELETE error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//禁止进程的创建
		case IOCTL_MONITOR2_PROCESS_DISABLECREATE:
			{
				__try
				{
					//全局
					bGlobal_disableCreateProcess = TRUE;

					//判定是否要开启
					if (!b_startSelfProtectionByObReferenceObjectByHandle && !b_disableCreateProcess && !bGlobal_disableTerminateProcess && !b_disableTerminateProcess && !bGlobal_disableCreateThread && !bGlobal_disableTerminateThread)
					{
						monitor2_inlineHookObReferenceObjectByHandle();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_PROCESS_DISABLECREATE error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//禁止进程的创建2
		case IOCTL_MONITOR2_PROCESS_DISABLECREATE2:
			{
				__try
				{
					PDISABLE_CREATE_PROCESS p;

					//
					b_disableCreateProcess = TRUE;

					p = (PDISABLE_CREATE_PROCESS)ExAllocatePool(NonPagedPool,sizeof(DISABLE_CREATE_PROCESS));
					p->ulPID = *(PULONG)InputBuffer;
					p->next = NULL;

					//如果有最后一个节点的信息，则指针移动到最后一个节点位置
					if (pDisableCreateProcessTmp)
					{
						pDisableCreateProcessInfo = pDisableCreateProcessTmp;
					}

					pDisableCreateProcessInfo->next = p;

					//记录下最后节点的位置
					pDisableCreateProcessTmp = pDisableCreateProcessInfo = p;

					//首部
					pDisableCreateProcessInfo = pDisableCreateProcessHead->next;

					//判定是否要开启
					if (!b_startSelfProtectionByObReferenceObjectByHandle && !bGlobal_disableCreateProcess && !bGlobal_disableTerminateProcess && !b_disableTerminateProcess && !bGlobal_disableCreateThread && !bGlobal_disableTerminateThread)
					{
						monitor2_inlineHookObReferenceObjectByHandle();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_PROCESS_DISABLECREATE2 error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//从单向链表里面查找PID
		case IOCTL_MONITOR2_PROCESS_DISABLECREATE2_FINDPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					ulRet = monitor2_findPIDInDisableCreateProcessInfo(ulPID);

					//把返回值传给ring3
					*(PULONG)OutputBuffer = ulRet;
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_PROCESS_DISABLECREATE2_FINDPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//从单向链表里面删除PID
		case IOCTL_MONITOR2_PROCESS_DISABLECREATE2_DELETEPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					monitor2_deletePIDInDisableCreateProcessInfo(ulPID);
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_PROCESS_DISABLECREATE2_DELETEPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//停止禁止进程的创建
		case IOCTL_MONITOR2_PROCESS_DISABLECREATE_STOP:
			{
				__try
				{
					//全局 不创建
					bGlobal_disableCreateProcess = FALSE;

					//判定
					if (!b_startSelfProtectionByObReferenceObjectByHandle && !b_disableCreateProcess && !bGlobal_disableTerminateProcess && !b_disableTerminateProcess && !bGlobal_disableCreateThread && !bGlobal_disableTerminateThread)
					{
						monitor2_unInlineHookObReferenceObjectByHandle();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_PROCESS_DISABLECREATE_STOP error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//禁止进程的结束
		case IOCTL_MONITOR2_PROCESS_DISABLETERMINATE:
			{
				__try
				{
					//全局
					bGlobal_disableTerminateProcess = TRUE;

					//判定
					if (!b_startSelfProtectionByObReferenceObjectByHandle && !bGlobal_disableCreateProcess && !b_disableCreateProcess && !b_disableTerminateProcess && !bGlobal_disableCreateThread && !bGlobal_disableTerminateThread)
					{
						monitor2_inlineHookObReferenceObjectByHandle();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_PROCESS_DISABLETERMINATE error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//禁止进程的结束2
		case IOCTL_MONITOR2_PROCESS_DISABLETERMINATE2:
			{
				__try
				{
					PDISABLE_TERMINATE_PROCESS p;

					//
					b_disableTerminateProcess = TRUE;

					p = (PDISABLE_TERMINATE_PROCESS)ExAllocatePool(NonPagedPool,sizeof(DISABLE_TERMINATE_PROCESS));
					p->ulPID = *(PULONG)InputBuffer;
					p->next = NULL;

					//如果有最后一个节点的信息，则指针移动到最后一个节点位置
					if (pDisableTerminateProcessTmp)
					{
						pDisableTerminateProcessInfo = pDisableTerminateProcessTmp;
					}

					pDisableTerminateProcessInfo->next = p;
					
					//记录下最后节点的位置
					pDisableTerminateProcessTmp = pDisableTerminateProcessInfo = p;

					//首部
					pDisableTerminateProcessInfo = pDisableTerminateProcessHead->next;

					//判定
					if (!b_startSelfProtectionByObReferenceObjectByHandle && !bGlobal_disableCreateProcess && !b_disableCreateProcess && !bGlobal_disableTerminateProcess && !bGlobal_disableCreateThread && !bGlobal_disableTerminateThread)
					{
						monitor2_inlineHookObReferenceObjectByHandle();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_PROCESS_DISABLETERMINATE2 error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//从单向链表里面查找PID
		case IOCTL_MONITOR2_PROCESS_DISABLETERMINATE2_FINDPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					ulRet = monitor2_findPIDInDisableTerminateProcessInfo(ulPID);

					//把返回值传给ring3
					*(PULONG)OutputBuffer = ulRet;
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_PROCESS_DISABLETERMINATE2_FINDPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//从单向链表里面删除PID
		case IOCTL_MONITOR2_PROCESS_DISABLETERMINATE2_DELETEPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					monitor2_deletePIDInDisableTerminateProcessInfo(ulPID);
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_PROCESS_DISABLETERMINATE2_DELETEPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//停止禁止进程的结束
		case IOCTL_MONITOR2_PROCESS_DISABLETERMINATE_STOP:
			{
				__try
				{
					//全局 不创建
					bGlobal_disableTerminateProcess = FALSE;

					//判定
					if (!b_startSelfProtectionByObReferenceObjectByHandle && !bGlobal_disableCreateProcess && !b_disableCreateProcess && !b_disableTerminateProcess && !bGlobal_disableCreateThread && !bGlobal_disableTerminateThread)
					{
						monitor2_unInlineHookObReferenceObjectByHandle();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_PROCESS_DISABLETERMINATE_STOP error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//禁止线程的创建
		case IOCTL_MONITOR2_THREAD_DISABLECREATE:
			{
				__try
				{
					//全局
					bGlobal_disableCreateThread = TRUE;

					//判定
					if (!b_startSelfProtectionByObReferenceObjectByHandle && !bGlobal_disableCreateProcess && !b_disableCreateProcess && !bGlobal_disableTerminateProcess && !b_disableTerminateProcess && !bGlobal_disableTerminateThread)
					{
						monitor2_inlineHookObReferenceObjectByHandle();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_THREAD_DISABLECREATE error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//停止禁止线程的创建
		case IOCTL_MONITOR2_THREAD_DISABLECREATE_STOP:
			{
				__try
				{
					//全局
					bGlobal_disableCreateThread = FALSE;

					//判定
					if (!b_startSelfProtectionByObReferenceObjectByHandle && !bGlobal_disableCreateProcess && !b_disableCreateProcess && !bGlobal_disableTerminateProcess && !b_disableTerminateProcess && !bGlobal_disableTerminateThread)
					{
						monitor2_unInlineHookObReferenceObjectByHandle();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_THREAD_DISABLECREATE_STOP error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//禁止线程的结束
		case IOCTL_MONITOR2_THREAD_DISABLETERMINATE:
			{
				__try
				{
					//全局
					bGlobal_disableTerminateThread = TRUE;

					//判定
					if (!b_startSelfProtectionByObReferenceObjectByHandle && !bGlobal_disableCreateProcess && !b_disableCreateProcess && !bGlobal_disableTerminateProcess && !b_disableTerminateProcess && !bGlobal_disableCreateThread)
					{
						monitor2_inlineHookObReferenceObjectByHandle();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_THREAD_DISABLETERMINATE error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//停止禁止线程的结束
		case IOCTL_MONITOR2_THREAD_DISABLETERMINATE_STOP:
			{
				__try
				{
					//全局
					bGlobal_disableTerminateThread = FALSE;

					//判定
					if (!b_startSelfProtectionByObReferenceObjectByHandle && !bGlobal_disableCreateProcess && !b_disableCreateProcess && !bGlobal_disableTerminateProcess && !b_disableTerminateProcess && !bGlobal_disableCreateThread)
					{
						monitor2_unInlineHookObReferenceObjectByHandle();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_THREAD_DISABLETERMINATE_STOP error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//禁止文件的创建
		case IOCTL_MONITOR2_FILE_DISABLECREATE:
			{
				__try
				{
					//全局
					bGlobal_disableCreateFile = TRUE;

					if (!b_disableCreateFile && !bGlobal_disableOpenFile && !b_disableOpenFile)
					{
						file_inlineHookIoCreateFile();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_FILE_DISABLECREATE error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//禁止文件的创建2
		case IOCTL_MONITOR2_FILE_DISABLECREATE2:
			{
				__try
				{
					PDISABLE_CREATE_FILE p;

					//
					b_disableCreateFile = TRUE;

					p = (PDISABLE_CREATE_FILE)ExAllocatePool(NonPagedPool,sizeof(DISABLE_CREATE_FILE));
					p->ulPID = *(PULONG)InputBuffer;
					p->next = NULL;

					//如果有最后一个节点的信息，则指针移动到最后一个节点位置
					if (pDisableCreateFileTmp)
					{
						pDisableCreateFileInfo = pDisableCreateFileTmp;
					}

					pDisableCreateFileInfo->next = p;

					//记录下最后节点的位置
					pDisableCreateFileTmp = pDisableCreateFileInfo = p;

					//首部
					pDisableCreateFileInfo = pDisableCreateFileHead->next;

					if (!bGlobal_disableCreateFile && !bGlobal_disableOpenFile && !b_disableOpenFile)
					{
						file_inlineHookIoCreateFile();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_FILE_DISABLECREATE2 error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//从单向链表里面查找PID
		case IOCTL_MONITOR2_FILE_DISABLECREATE2_FINDPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					ulRet = file_findPIDInDisableCreateFileInfo(ulPID);

					//把返回值传给ring3
					*(PULONG)OutputBuffer = ulRet;
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_FILE_DISABLECREATE2_FINDPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//从单向链表里面删除PID
		case IOCTL_MONITOR2_FILE_DISABLECREATE2_DELETEPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					file_deletePIDInDisableCreateFileInfo(ulPID);
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_FILE_DISABLECREATE2_DELETEPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//停止禁止文件的创建
		case IOCTL_MONITOR2_FILE_DISABLECREATE_STOP:
			{
				__try
				{
					//全局
					bGlobal_disableCreateFile = FALSE;

					if (!b_disableCreateFile && !bGlobal_disableOpenFile && !b_disableOpenFile)
					{
						file_unInlineHookIoCreateFile();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_FILE_DISABLECREATE_STOP error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//禁止文件删除
		case IOCTL_MONITOR2_FILE_DISABLEDELETE:
			{
				__try
				{
					//全局
					bGlobal_disableDeleteFile = TRUE;		

					if (!b_disableDeleteFile)
					{
						file_inlineHookDeleteFileFunc();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_FILE_DISABLEDELETE error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//禁止文件删除2
		case IOCTL_MONITOR2_FILE_DISABLEDELETE2:
			{
				__try
				{
					PDISABLE_DELETE_FILE p;

					//单一
					b_disableDeleteFile = TRUE;

					p = (PDISABLE_DELETE_FILE)ExAllocatePool(NonPagedPool,sizeof(DISABLE_DELETE_FILE));
					p->ulPID = *(PULONG)InputBuffer;
					p->next = NULL;

					//如果有最后一个节点的信息，则指针移动到最后一个节点位置
					if (pDisableDeleteFileTmp)
					{
						pDisableDeleteFileInfo = pDisableDeleteFileTmp;
					}

					pDisableDeleteFileInfo->next = p;

					//记录下最后节点的位置
					pDisableDeleteFileTmp = pDisableDeleteFileInfo = p;

					//首部
					pDisableDeleteFileInfo = pDisableDeleteFileHead->next;

					if (!bGlobal_disableDeleteFile)
					{
						file_inlineHookDeleteFileFunc();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_FILE_DISABLEDELETE2 error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//从单向链表里面查找PID
		case IOCTL_MONITOR2_FILE_DISABLEDELETE2_FINDPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					ulRet = file_findPIDInDisableDeleteFileInfo(ulPID);

					//把返回值传给ring3
					*(PULONG)OutputBuffer = ulRet;
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_FILE_DISABLEDELETE2_FINDPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//从单向链表里面删除PID
		case IOCTL_MONITOR2_FILE_DISABLEDELETE2_DELETEPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					file_deletePIDInDisableDeleteFileInfo(ulPID);
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_FILE_DISABLEDELETE2_DELETEPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//停止禁止文件删除
		case IOCTL_MONITOR2_FILE_DISABLEDELETE_STOP:
			{
				__try
				{
					bGlobal_disableDeleteFile = FALSE;

					if (!b_disableDeleteFile)
					{
						file_unInlineHookDeleteFileFunc();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_FILE_DISABLEDELETE_STOP error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//禁止文件打开
		case IOCTL_MONITOR2_FILE_DISABLEOPEN:
			{
				__try
				{
					//全局
					bGlobal_disableOpenFile = TRUE;

					if (!bGlobal_disableCreateFile && !b_disableCreateFile && !b_disableOpenFile)
					{
						file_inlineHookIoCreateFile();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_FILE_DISABLEOPEN error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//禁止文件打开2
		case IOCTL_MONITOR2_FILE_DISABLEOPEN2:
			{
				__try
				{
					PDISABLE_OPEN_FILE p;

					//全局
					b_disableOpenFile = TRUE;

					p = (PDISABLE_OPEN_FILE)ExAllocatePool(NonPagedPool,sizeof(DISABLE_OPEN_FILE));
					p->ulPID = *(PULONG)InputBuffer;
					p->next = NULL;

					//如果有最后一个节点的信息，则指针移动到最后一个节点位置
					if (pDisableOpenFileTmp)
					{
						pDisableOpenFileInfo = pDisableOpenFileTmp;
					}

					pDisableOpenFileInfo->next = p;

					//记录下最后节点的位置
					pDisableOpenFileTmp = pDisableOpenFileInfo = p;

					//首部
					pDisableOpenFileInfo = pDisableOpenFileHead->next;

					if (!bGlobal_disableCreateFile && !b_disableCreateFile && !bGlobal_disableOpenFile)
					{
						file_inlineHookIoCreateFile();
					}

				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_FILE_DISABLEDELETE2 error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//从单向链表里面查找PID
		case IOCTL_MONITOR2_FILE_DISABLEOPEN2_FINDPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					ulRet = file_findPIDInDisableOpenFileInfo(ulPID);

					//把返回值传给ring3
					*(PULONG)OutputBuffer = ulRet;
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_FILE_DISABLEOPEN2_FINDPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//从单向链表里面删除PID
		case IOCTL_MONITOR2_FILE_DISABLEOPEN2_DELETEPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					file_deletePIDInDisableOpenFileInfo(ulPID);
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_FILE_DISABLEOPEN2_DELETEPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//停止禁止文件的打开
		case IOCTL_MONITOR2_FILE_DISABLEOPEN_STOP:
			{
				__try
				{
					bGlobal_disableOpenFile = FALSE;

					if (!bGlobal_disableCreateFile && !b_disableCreateFile && !b_disableOpenFile)
					{
						file_unInlineHookIoCreateFile();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_FILE_DISABLEOPEN_STOP error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//禁止注册表的创建
		case IOCTL_MONITOR2_REG_DISABLECREATE:
			{
				__try
				{
					//全局禁止
					bGlobal_disableCreateReg = TRUE;

					if (!b_disableCreateReg)
					{
						reg_inlineHook_disableCreate();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_REG_DISABLECREATE error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//禁止注册表的创建2
		case IOCTL_MONITOR2_REG_DISABLECREATE2:
			{
				__try
				{
					PDISABLE_CREATE_REG p;

					//全局
					b_disableCreateReg = TRUE;

					p = (PDISABLE_CREATE_REG)ExAllocatePool(NonPagedPool,sizeof(DISABLE_CREATE_REG));
					p->ulPID = *(PULONG)InputBuffer;
					p->next = NULL;

					//如果有最后一个节点的信息，则指针移动到最后一个节点位置
					if (pDisableCreateRegTmp)
					{
						pDisableCreateRegInfo = pDisableCreateRegTmp;
					}

					pDisableCreateRegInfo->next = p;

					//记录下最后节点的位置
					pDisableCreateRegTmp = pDisableCreateRegInfo = p;

					//首部
					pDisableCreateRegInfo = pDisableCreateRegHead->next;	

					if (!bGlobal_disableCreateReg)
					{
						reg_inlineHook_disableCreate();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_REG_DISABLECREATE2 error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//从单向链表里面查找PID
		case IOCTL_MONITOR2_REG_DISABLECREATE2_FINDPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					ulRet = reg_findPIDInDisableCreateRegInfo(ulPID);

					//把返回值传给ring3
					*(PULONG)OutputBuffer = ulRet;
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_REG_DISABLECREATE2_FINDPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//从单向链表里面删除PID
		case IOCTL_MONITOR2_REG_DISABLECREATE2_DELETEPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					reg_deletePIDInDisableCreateRegInfo(ulPID);
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_REG_DISABLECREATE2_DELETEPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//停止禁止注册表的创建
		case IOCTL_MONITOR2_REG_DISABLECREATE_STOP:
			{
				__try
				{
					bGlobal_disableCreateReg = FALSE;

					if (!b_disableCreateReg)
					{
						reg_inlineHook_disableCreate_stop();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_REG_DISABLECREATE_STOP error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//禁止注册表的删除
		case IOCTL_MONITOR2_REG_DISABLEDELETE:
			{
				__try
				{
					bGlobal_disableDeleteReg = TRUE;

					if (!b_disableDeleteReg)
					{
						reg_inlineHook_disableDelete();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_REG_DISABLEDELETE error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//禁止注册表的删除2
		case IOCTL_MONITOR2_REG_DISABLEDELETE2:
			{
				__try
				{
					PDISABLE_DELETE_REG p;

					//全局
					b_disableDeleteReg = TRUE;

					p = (PDISABLE_DELETE_REG)ExAllocatePool(NonPagedPool,sizeof(DISABLE_DELETE_REG));
					p->ulPID = *(PULONG)InputBuffer;
					p->next = NULL;

					//如果有最后一个节点的信息，则指针移动到最后一个节点位置
					if (pDisableDeleteRegTmp)
					{
						pDisableDeleteRegInfo = pDisableDeleteRegTmp;
					}

					pDisableDeleteRegInfo->next = p;

					//记录下最后节点的位置
					pDisableDeleteRegTmp = pDisableDeleteRegInfo = p;

					//首部
					pDisableDeleteRegInfo = pDisableDeleteRegHead->next;

					if (!bGlobal_disableDeleteReg)
					{
						reg_inlineHook_disableDelete();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_REG_DISABLEDELETE2 error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//从单向链表里面查找PID
		case IOCTL_MONITOR2_REG_DISABLEDELETE2_FINDPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					ulRet = reg_findPIDInDisableDeleteRegInfo(ulPID);

					//把返回值传给ring3
					*(PULONG)OutputBuffer = ulRet;
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_REG_DISABLEDELETE2_FINDPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//从单向链表里面删除PID
		case IOCTL_MONITOR2_REG_DISABLEDELETE2_DELETEPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					reg_deletePIDInDisableDeleteRegInfo(ulPID);
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_REG_DISABLEDELETE2_DELETEPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//停止禁止注册表的删除
		case IOCTL_MONITOR2_REG_DISABLEDELETE_STOP:
			{
				__try
				{
					bGlobal_disableDeleteReg = FALSE;

					if (!b_disableDeleteReg)
					{
						reg_inlineHook_disableDelete_stop();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_REG_DISABLEDELETE_STOP error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//禁止注册表的查询
		case IOCTL_MONITOR2_REG_DISABLEQUERY:
			{
				__try
				{
					bGlobal_disableOpenReg = TRUE;

					if (!b_disableOpenReg)
					{
						reg_inlineHook_disableQuery();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_REG_DISABLEQUERY error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//禁止注册表的删除2
		case IOCTL_MONITOR2_REG_DISABLEQUERY2:
			{
				__try
				{
					PDISABLE_OPEN_REG p;

					//全局
					b_disableOpenReg = TRUE;

					p = (PDISABLE_OPEN_REG)ExAllocatePool(NonPagedPool,sizeof(DISABLE_OPEN_REG));
					p->ulPID = *(PULONG)InputBuffer;
					p->next = NULL;

					//如果有最后一个节点的信息，则指针移动到最后一个节点位置
					if (pDisableOpenRegTmp)
					{
						pDisableOpenRegInfo = pDisableOpenRegTmp;
					}

					pDisableOpenRegInfo->next = p;

					//记录下最后节点的位置
					pDisableOpenRegTmp = pDisableOpenRegInfo = p;

					//首部
					pDisableOpenRegInfo = pDisableOpenRegHead->next;

					if (!bGlobal_disableOpenReg)
					{
						reg_inlineHook_disableQuery();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_REG_DISABLEQUERY2 error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//从单向链表里面查找PID
		case IOCTL_MONITOR2_REG_DISABLEQUERY2_FINDPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					ulRet = reg_findPIDInDisableOpenRegInfo(ulPID);

					//把返回值传给ring3
					*(PULONG)OutputBuffer = ulRet;
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_REG_DISABLEQUERY2_FINDPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//从单向链表里面删除PID
		case IOCTL_MONITOR2_REG_DISABLEQUERY2_DELETEPID:
			{
				__try
				{
					ULONG ulRet;
					ULONG ulPID = *(PULONG)InputBuffer;

					reg_deletePIDInDisableOpenRegInfo(ulPID);
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_REG_DISABLEQUERY2_DELETEPID error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//停止禁止注册表的查询
		case IOCTL_MONITOR2_REG_DISABLEQUERY_STOP:
			{
				__try
				{
					bGlobal_disableOpenReg = FALSE;

					if (!b_disableOpenReg)
					{
						reg_inlineHook_disableQuery_stop();
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_MONITOR2_REG_DISABLEQUERY_STOP error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//禁止驱动加载
		case IOCTL_SYSMODULE_DISABLELOADDRIVER:
			{
				__try
				{
					//如果inline hook 失败
					if (!sysmodule_inlineHook_disableLoadDriver())
					{
						break;
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_SYSMODULE_DISABLELOADDRIVER error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//停止禁止加载驱动
		case IOCTL_SYSMODULE_DISABLELOADDRIVER_STOP:
			{
				__try
				{
					sysmodule_inlineHook_disableLoadDriver_stop();
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_SYSMODULE_DISABLELOADDRIVER_STOP error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//获得驱动模块列表大小
		case IOCTL_SYSMODULE_GETSIZE:
			{
				__try
				{
					ULONG ulCount=0;

					//如果全局驱动对象为空
					if(!global_pDriverObj)
					{
						break;
					}

					//获得单向链表头部
					status = sysmodule_enumDrivers(global_pDriverObj);

					//如果获取失败
					if (!NT_SUCCESS(status))
					{
						break;
					}

					//获得列表大小
					ulCount = sysmodule_getDriverModuleCount();

					*(PULONG)OutputBuffer  = ulCount;
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_SYSMODULE_GETSIZE error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//获得列表
		case IOCTL_SYSMODULE_GETINFO:
			{
				__try
				{
					//发送数据到ring3
					status = sysmodule_getDriverModuleInfo((PSYSTEM_MODULE_INFO)OutputBuffer);
					if (!NT_SUCCESS(status))
					{
						break;
					}
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_SYSMODULE_GETINFO error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
			//获得系统回调大小
		case IOCTL_KERNEL_SYSTEMCALLBACK_GETSIZE:
			{
				__try
				{
					ULONG ulCount=0;

					//获得单向链表头部
					status = kernel_systemCallbackEnum();

					//如果获取失败
					if (!NT_SUCCESS(status))
					{
						break;
					}

					//获得列表大小
					ulCount = kernel_systemCallbackGetCount();

					*(PULONG)OutputBuffer  = ulCount;
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_KERNEL_SYSTEMCALLBACK_GETSIZE error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//获得列表
		case IOCTL_KERNEL_SYSTEMCALLBACK_GETINFO:
			{
				__try
				{
					//发送数据到ring3
					status = kernel_systemCallbackGetInfo((PKERNEL_SYSTEM_CALLBACK_INFO)OutputBuffer);
					if (!NT_SUCCESS(status))
					{
						break;
					}
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_KERNEL_SYSTEMCALLBACK_GETINFO error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
			//系统回调结束
		case IOCTL_KERNEL_SYSTEMCALLBACK_KILL:
			{
				__try
				{
					SYSTEM_CALLBACK_CLIENT_ID clientID;
					BOOLEAN bRet = FALSE;
					//获得从ring3传过来的进程clientID

					//复制数据
					RtlCopyMemory(&clientID,InputBuffer,sizeof(SYSTEM_CALLBACK_CLIENT_ID));

					bRet = kernel_systemCallbackKill(clientID.ulEntryAddress,clientID.ulFuncAddress);

					if (!bRet)
					{
						break;
					}
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_PROCESS_KILLTHREAD_APC error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//获得过滤驱动大小
		case IOCTL_KERNEL_FILTERDRIVER_GETSIZE:
			{
				__try
				{
					ULONG ulCount=0;

					//获得单向链表头部
					status = kernel_filterDriverEnum();

					//如果获取失败
					if (!NT_SUCCESS(status))
					{
						break;
					}

					//获得列表大小
					ulCount = kernel_filterDriverGetCount();

					*(PULONG)OutputBuffer  = ulCount;
				}
				__except( EXCEPTION_EXECUTE_HANDLER )
				{
					DbgPrint("IOCTL_KERNEL_FILTERDRIVER_GETSIZE error!");
					break;
				}
				status = STATUS_SUCCESS;
			}
			break;
			//获得列表
		case IOCTL_KERNEL_FILTERDRIVER_GETINFO:
			{
				__try
				{
					//发送数据到ring3
					status = kernel_filterDriverGetInfo((PKERNEL_FILTERDRIVER_INFO)OutputBuffer);
					if (!NT_SUCCESS(status))
					{
						break;
					}
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_KERNEL_FILTERDRIVER_GETINFO error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
			//摘除过滤驱动
		case IOCTL_KERNEL_FILTERDRIVER_KILL:
			{
				__try
				{
					ULONG ulAttachObj=0;

					ulAttachObj = *(PULONG)InputBuffer;

					status = kernel_filterDriverKill((PDEVICE_OBJECT)ulAttachObj);
					if (!NT_SUCCESS(status))
					{
						break;
					}
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_KERNEL_FILTERDRIVER_GETINFO error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
			//反汇编
		case IOCTL_DISASM:
			{
				__try
				{
					DISASM_CLIENT_ID clientID;

					//复制数据
					RtlCopyMemory(&clientID,InputBuffer,sizeof(DISASM_CLIENT_ID));

					//复制内存数据到ring3
					RtlCopyMemory((PBYTE)OutputBuffer,(PBYTE)clientID.ulStartAddress,clientID.ulSize);
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_DISASM error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
			//开启监控
		case IOCTL_MONITOR_START:
			{
				__try
				{
					monitor_start();
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_MONITOR_START error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
			//停止监控
		case IOCTL_MONITOR_STOP:
			{
				__try
				{
					monitor_stop();
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_MONITOR_STOP error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
			//
		case IOCTL_MONITOR_GETSIZE:
			{
				__try
				{
					ULONG ulCount = 0;

					//获得列表大小
					ulCount = monitor_getCount();

					*(PULONG)OutputBuffer  = ulCount;
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_MONITOR_GETSIZE error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
			//获得监控信息
		case IOCTL_MONITOR_GETINFO:
			{
				__try
				{
					//发送数据到ring3
					status = monitor_getInfo((PMONITOR_INFO)OutputBuffer);
					if (!NT_SUCCESS(status))
					{
						break;
					}
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_MONITOR_GETINFO error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
			//获得监控过滤信息
		case IOCTL_MONITOR_GETFILTERINFO_PROCESS:
			{
				__try
				{
					PMONITOR_PROCESS_FILTER_INFO p = (PMONITOR_PROCESS_FILTER_INFO)ExAllocatePool(NonPagedPool,sizeof(MONITOR_PROCESS_FILTER_INFO));

					if (!p)
					{
						break;
					}

					DbgPrint("process name:%ws,bInclude:%d",p->ProcessName,p->bProcessInclude);

					RtlCopyMemory(p,InputBuffer,sizeof(MONITOR_PROCESS_FILTER_INFO));

					//
					status = monitor_updateProcessMonitorFilerInfo(p);

					if (!NT_SUCCESS(status))
					{
						break;
					}
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_MONITOR_GETFILTERINFO_PROCESS error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
			//获得监控过滤信息
		case IOCTL_MONITOR_GETFILTERINFO_PATH:
			{
				__try
				{
					PMONITOR_PATH_FILTER_INFO p = ExAllocatePool(NonPagedPool,sizeof(MONITOR_PATH_FILTER_INFO));

					if (!p)
					{
						break;
					}

					RtlCopyMemory(p,(PMONITOR_PATH_FILTER_INFO)InputBuffer,sizeof(MONITOR_PATH_FILTER_INFO));

					//
					status = monitor_updatePathMonitorFilerInfo(p);

					if (!NT_SUCCESS(status))
					{
						break;
					}
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_MONITOR_GETFILTERINFO_PATH error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
			//获得监控过滤信息
		case IOCTL_MONITOR_FREEFILTERINFO:
			{
				__try
				{
					monitor_freeMonitorFilerInfo();
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_MONITOR_FREEFILTERINFO error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
			//
		case IOCTL_PROCESS_GETHANDLEINFOSIZE:
			{
				__try
				{
					ULONG ulCount = 0;

					//获得进程句柄信息
					status =  process_getProcessHandleInfo(*(PULONG)InputBuffer);

					if (!NT_SUCCESS(status))
					{
						break;
					}

					//获得列表大小
					ulCount = process_getProcessHandleInfoCount();

					*(PULONG)OutputBuffer  = ulCount;
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_PROCESS_GETHANDLEINFOSIZE error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
			//获得进程句柄信息
		case IOCTL_PROCESS_GETHANDLEINFO:
			{
				__try
				{
					//发送数据到ring3
					status = process_getProcessHandleInfo_send((PPROCESS_HANDLE_INFO)OutputBuffer);
					if (!NT_SUCCESS(status))
					{
						break;
					}
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_PROCESS_GETHANDLEINFO error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
			//
		case IOCTL_FILE_GETHANDLEINFOSIZE:
			{
				__try
				{
					ULONG ulCount = 0;

					LPTSTR lpFilePath = (LPTSTR)InputBuffer;

					DbgPrint("IOCTL_FILE_GETHANDLEINFOSIZE lpFilePath:%ws",lpFilePath);

					//获得进程句柄信息
					status =  file_getFileHandleInfo(lpFilePath);

					if (!NT_SUCCESS(status))
					{
						break;
					}

					//获得列表大小
					ulCount = file_getFileHandleInfoCount();

					DbgPrint("IOCTL_FILE_GETHANDLEINFOSIZE ulCount:%d",ulCount);

					*(PULONG)OutputBuffer  = ulCount;
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_FILE_GETHANDLEINFOSIZE error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
			//
		case IOCTL_FILE_GETHANDLEINFO:
			{
				__try
				{
					//发送数据到ring3
					status = file_getFileHandleInfo_send((PFILE_HANDLE_INFO)OutputBuffer);
					if (!NT_SUCCESS(status))
					{
						break;
					}
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("IOCTL_FILE_GETHANDLEINFO error!");
					break;
				}	
				status = STATUS_SUCCESS;
			}
			break;
		}
		break;
	}
	if(status == STATUS_SUCCESS)
	{
		pIrp->IoStatus.Information = uOutSize;
	}
	else
	{
		pIrp->IoStatus.Information = 0;
	}
	// 完成请求
	pIrp->IoStatus.Status = status;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return status;
}


//驱动入口
NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObj,IN PUNICODE_STRING RegistryPath)
{
	//设备名称
	UNICODE_STRING        ustrDeviceName;
	//符号链接名称
	UNICODE_STRING        ustrLinkName;

	NTSTATUS              status;
	PDEVICE_OBJECT        pDevObj;


	DbgPrint("进入驱动程序入口！");

	RtlInitUnicodeString(&ustrDeviceName, DEVICE_NAME);
	RtlInitUnicodeString(&ustrLinkName,LINK_NAME);

	//初始化各派遣例程
	pDriverObj->MajorFunction[IRP_MJ_CREATE]=DispatchCreateClose;
	pDriverObj->MajorFunction[IRP_MJ_CLOSE]=DispatchCreateClose;
	pDriverObj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchIoctl;
	pDriverObj->DriverUnload = DriverUnload;

	//创建设备对象
	status = IoCreateDevice(pDriverObj,0x100u,&ustrDeviceName,FILE_DEVICE_UNKNOWN,
		FILE_DEVICE_SECURE_OPEN,
		TRUE,
		&pDevObj);

	//判断设备对象是否创建成功
	if (!NT_SUCCESS(status))
	{
		return status;
	}

	//创建符号链接
	status = IoCreateSymbolicLink(&ustrLinkName, &ustrDeviceName);

    //判断符号链接是否创建成功
	if (!NT_SUCCESS(status))
	{
		//删除设备对象
		IoDeleteDevice(pDevObj);
		return status;
	}

	//获得Shadow SSDT基址
	KeServiceDescriptorTableShadow = (PServiceDescriptorTable)getAddressOfShadowTable();

    //DbgPrint("KeServiceDescriptorTableShadow base:%X",KeServiceDescriptorTableShadow->ServiceTableBase);
    //DbgPrint("KeServiceDescriptorTableShadow[1] base:%X",KeServiceDescriptorTableShadow[1].ServiceTableBase);
	//DbgPrint("KeServiceDescriptorTableShadow NumberOfServices:%d",KeServiceDescriptorTableShadow[1].NumberOfServices);

	global_pDriverObj = pDriverObj;

	pDisableCreateProcessInfo = pDisableCreateProcessHead = (PDISABLE_CREATE_PROCESS)ExAllocatePool(NonPagedPool,sizeof(DISABLE_CREATE_PROCESS));
	pDisableTerminateProcessInfo = pDisableTerminateProcessHead = (PDISABLE_TERMINATE_PROCESS)ExAllocatePool(NonPagedPool,sizeof(DISABLE_TERMINATE_PROCESS));
    //
	pDisableCreateFileInfo = pDisableCreateFileHead = (PDISABLE_CREATE_FILE)ExAllocatePool(NonPagedPool,sizeof(DISABLE_CREATE_FILE));
	pDisableDeleteFileInfo = pDisableDeleteFileHead = (PDISABLE_DELETE_FILE)ExAllocatePool(NonPagedPool,sizeof(DISABLE_DELETE_FILE));
	pDisableOpenFileInfo = pDisableOpenFileHead = (PDISABLE_OPEN_FILE)ExAllocatePool(NonPagedPool,sizeof(DISABLE_OPEN_FILE));
    //
	pDisableCreateRegInfo = pDisableCreateRegHead = (PDISABLE_CREATE_REG)ExAllocatePool(NonPagedPool,sizeof(DISABLE_CREATE_REG));
    pDisableDeleteRegInfo = pDisableDeleteRegHead = (PDISABLE_DELETE_REG)ExAllocatePool(NonPagedPool,sizeof(DISABLE_DELETE_REG));
    pDisableOpenRegInfo = pDisableOpenRegHead = (PDISABLE_OPEN_REG)ExAllocatePool(NonPagedPool,sizeof(DISABLE_OPEN_REG));

	pMonitorProcessFilterInfo = pMonitorProcessFilterInfoHead = (PMONITOR_PROCESS_FILTER_INFO)ExAllocatePool(NonPagedPool,sizeof(MONITOR_PROCESS_FILTER_INFO));
    pMonitorPathFilterInfo = pMonitorPathFilterInfoHead = (PMONITOR_PATH_FILTER_INFO)ExAllocatePool(NonPagedPool,sizeof(MONITOR_PATH_FILTER_INFO));

	//
	pDisableCreateProcessInfo->next = NULL;
	pDisableTerminateProcessInfo->next = NULL;
    //
	pDisableCreateFileInfo->next = NULL;
	pDisableDeleteFileInfo->next = NULL;
	pDisableOpenFileInfo->next = NULL;
    //
	pDisableCreateRegInfo->next = NULL;
	pDisableDeleteRegInfo->next = NULL;
	pDisableOpenRegInfo->next = NULL;

	pMonitorProcessFilterInfo->next = NULL;
	pMonitorPathFilterInfo->next = NULL;

	return status;
}