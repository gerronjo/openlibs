
//防止重定义
#ifndef _MONITOR_
#define _MONITOR_

#include "common.h"

	
//单向链表
typedef struct _MonitorProcessFilterInfo{
	//进程名称
	WCHAR ProcessName[256];

	//标志
	BOOL bProcessInclude;

	//指向下一个
	struct MONITOR_PROCESS_FILTER_INFO* next;
}MONITOR_PROCESS_FILTER_INFO,*PMONITOR_PROCESS_FILTER_INFO;

//单向链表
typedef struct _MonitorPathFilterInfo{
	//路径
	WCHAR PathName[256];
	//标志
	BOOL bPathInclude;

	//指向下一个
	struct MONITOR_PATH_FILTER_INFO* next;
}MONITOR_PATH_FILTER_INFO,*PMONITOR_PATH_FILTER_INFO;


//单向链表
typedef struct _MonitorInfo{
	//时间
	WCHAR Time[256];
	//进程名称
	WCHAR ProcessName[256];
	//进程路径
	WCHAR ProcessPath[256];
	//进程ID
	ULONG ulPID;
	//操作
	WCHAR Action[256];
	//路径
	WCHAR PathName[256];
	//结果
	WCHAR Result[256];
	//详细
	WCHAR Other[256];

	//指向下一个
	struct MONITOR_INFO* next;
}MONITOR_INFO,*PMONITOR_INFO;

PMONITOR_PROCESS_FILTER_INFO pMonitorProcessFilterInfo,pMonitorProcessFilterInfoHead;
PMONITOR_PATH_FILTER_INFO pMonitorPathFilterInfo,pMonitorPathFilterInfoHead;

PMONITOR_INFO pMonitorInfo,pMonitorInfoHead;
MUTEX_TYPE       MKMutex;


// 进程回调函数
VOID ProcessCallbackFunc(IN HANDLE  hParentId, IN HANDLE  hProcessId, IN BOOLEAN bCreate);

//线程回调函数
VOID ThreadCallbackFunc(IN HANDLE  hProcessId, IN HANDLE  hThreadId, IN BOOLEAN bCreate);

// LoadImage回调函数
VOID LoadImageCallbackFunc(IN PUNICODE_STRING  pUniImageName, IN HANDLE  hProcessId, IN PIMAGE_INFO pImageInfo);


NTSTATUS monitor_updateProcessMonitorFilerInfo(PMONITOR_PROCESS_FILTER_INFO p);

NTSTATUS monitor_updatePathMonitorFilerInfo(PMONITOR_PATH_FILTER_INFO p);

void monitor_freeMonitorFilerInfo();

//开启监控
NTSTATUS monitor_start();

//停止监控
NTSTATUS monitor_stop();

//释放内存
VOID monitor_free();

//获得大小
ULONG monitor_getCount();

//发送数据到ring3
NTSTATUS monitor_getInfo(PMONITOR_INFO pBuffer);

#endif //_MONITOR_