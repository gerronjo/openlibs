/*
内核代码的头文件
*/
#if !defined(SYS_H_)
	#define SYS_H_
	
	#include "comm_sys.h"
	
	extern unsigned g_ddk_val;
	
	extern DDKAPI bool DDKHello(void);
	
	DDKAPI void DriverUnload(PDRIVER_OBJECT pDriverObject);
	DDKAPI NTSTATUS DrvDispatchCreateClose(PDEVICE_OBJECT pDeviceObject,\
		PIRP pIrp);
	DDKAPI NTSTATUS DrvDispatchControl(PDEVICE_OBJECT pDeviceObject,\
		PIRP pIrp);
	DDKAPI NTSTATUS DrvShutdown(IN PDEVICE_OBJECT pDriverObject,\
		IN PIRP pIrp);
	DDKAPI NTSTATUS DispatchRead(IN PDEVICE_OBJECT pDriverObject,\
		IN PIRP pIrp);
	DDKAPI NTSTATUS DispatchWrite(IN PDEVICE_OBJECT pDriverObject,\
		IN PIRP pIrp);
#endif
