#ifndef _KERNEL_
#define _KERNEL_

#include "..\common\common.h"

//检测到的系统回调函数中的信息
typedef struct _KernelSystemCallbackInfo{
	//回调类型
	ULONG ulType;
	//驱动路径
	WCHAR PathName[256];
	//
	ULONG ulEntryAddress;
	//回调函数地址
	ULONG ulCallBackAddress;
	//附加信息
	ULONG ulAttatch;

	//指向下一个
	_KernelSystemCallbackInfo* next;
}KERNEL_SYSTEM_CALLBACK_INFO,*PKERNEL_SYSTEM_CALLBACK_INFO;

//检测到的过滤驱动的信息
typedef struct _KernelFilterDriverInfo{
	ULONG ulObjType;
	WCHAR FileName[64];
	WCHAR FilePath[256];
	ULONG ulAttachDevice;
	//宿主
	WCHAR HostFileName[64];
	//指向下一个
	_KernelFilterDriverInfo* next;
}KERNEL_FILTERDRIVER_INFO,*PKERNEL_FILTERDRIVER_INFO;

#endif //_KERNEL_