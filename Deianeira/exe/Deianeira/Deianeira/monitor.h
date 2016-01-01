#ifndef _MONITOR_
#define _MONITOR_

#pragma once

//单向链表
typedef struct _MonitorProcessFilterInfo{
	//进程名称
	WCHAR ProcessName[256];

	//标志
	BOOL bProcessInclude;

	//指向下一个
	struct _MonitorProcessFilterInfo* next;
}MONITOR_PROCESS_FILTER_INFO,*PMONITOR_PROCESS_FILTER_INFO;

//单向链表
typedef struct _MonitorPathFilterInfo{
	//路径
	WCHAR PathName[256];
	//标志
	BOOL bPathInclude;

	//指向下一个
	struct _MonitorPathFilterInfo* next;
}MONITOR_PATH_FILTER_INFO,*PMONITOR_PATH_FILTER_INFO;

//单向链表
typedef struct _MonitorInfo{
	//时间
	WCHAR Time[256];
	//进程名称
	WCHAR ProcessName[256];
	//
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
	_MonitorInfo* next;
}MONITOR_INFO,*PMONITOR_INFO;

#endif //_MONITOR_