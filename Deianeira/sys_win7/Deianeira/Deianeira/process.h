#include "common.h"
#include "monitor2.h"

#define NtSuspendProcess_XP 253
#define NtSuspendThread_XP 254
#define NtResumeProcess_XP 205
#define NtResumeThread_XP 206
#define NtSuspendProcess_WIN7 366
#define NtSuspendThread_WIN7 367
#define NtResumeProcess_WIN7 303
#define NtResumeThread_WIN7 304

//线程信息单向列表
typedef struct _ProcessThreadInfo
{   
	//线程ID
	ULONG ulTID;
	//PETHREAD地址
	ULONG ulPEThread;
	//TEB
	ULONG ulTEB;
	//基址
	ULONG ulBaseAddress;
	//单向列表指针
	struct PROCESS_THREAD_INFO* next;
}PROCESS_THREAD_INFO, *PPROCESS_THREAD_INFO; 


//进程句柄信息
typedef struct _ProcessHandleInfo
{   
	//句柄类型
	WCHAR wstrHandleTypeName[25];
    //句柄名称
	WCHAR wstrHandleName[256];
    //句柄
	ULONG ulHandle;
    //句柄对象
	ULONG ulObject;
    //句柄类型索引
	ULONG ulTypeIndex;
    //引用计数
	ULONG ulReferenceCount;

	//单向列表指针
	struct PROCESS_HANDLE_INFO* next;
}PROCESS_HANDLE_INFO, *PPROCESS_HANDLE_INFO; 


//
PPROCESS_HANDLE_INFO pProcessHandleInfo,pProcessHandleInfoHead;


typedef NTSTATUS (*PPspTerminateThreadByPointer)(
	__in PETHREAD pEThread, 
	__in NTSTATUS status
	);


typedef NTSTATUS (*PNtUnmapViewOfSection)(
	IN HANDLE ProcessHandle,
	IN PVOID BaseAddress
	);


typedef NTSTATUS (*PMiUnmapViewOfSection)(
					 __in PEPROCESS  Process,
					 __in PVOID BaseAddress,
					 __in ULONG ulUnknown
					 );


typedef NTSTATUS (*PPsSuspendProcess) (
									   IN PEPROCESS Process
									   );

typedef NTSTATUS (*PPsResumeProcess) (
									  IN PEPROCESS Process
									  );

typedef ULONG (*PKeSuspendThread)(
								  IN PKTHREAD
								  );

typedef ULONG (*PKeResumeThread)(
								  IN PKTHREAD
								  );

//变量声明
PPspTerminateThreadByPointer PspTerminateThreadByPointer;
PMiUnmapViewOfSection MiUnmapViewOfSection;
PKeSuspendThread KeSuspendThread;
PKeResumeThread KeResumeThread;
PPsSuspendProcess PsSuspendProcess;
PPsResumeProcess PsResumeProcess;


//挂起进程
BOOLEAN process_suspendProcess(ULONG ulPID);

//恢复进程
BOOLEAN process_resumeProcess(ULONG ulPID);

//内存清零，强制结束进程
BOOLEAN process_killProcess(ULONG ulPID);

//隐藏进程
BOOLEAN process_hideProcess(ULONG ulPID);

//查找MiUnmapViewOfSection的地址
ULONG process_getMiUnmapViewOfSectionAddress();


//得到KeSuspendThread的地址
ULONG process_getKeSuspendThreadAddress();

//
BOOLEAN process_suspendThread(ULONG ulTID);

//
BOOLEAN process_resumeThread(ULONG ulTID);

//结束进程
BOOLEAN process_killProcessByPspTerminateThreadByPointer(ULONG ulPID);

//
BOOLEAN process_killThreadByPspTerminateThreadByPointer(ULONG ulPID,ULONG ulTID);


//卸载模块
//ulPID:进程ID
//ulModuleBase:模块基址
BOOLEAN process_unloadModule(ULONG ulPID,ULONG ulModuleBase);

//隐藏模块
BOOLEAN process_hideModule(ULONG ulPID);


//获得目标进程的线程信息
PPROCESS_THREAD_INFO process_getProcessThreadInfo(ULONG ulPID);

//获得线程的基址 
ULONG process_getThreadBaseAddr(ULONG ulPID,ULONG ulTID);

//获得进程句柄信息
NTSTATUS process_getProcessHandleInfo(ULONG ulPID);

//获得大小
ULONG process_getProcessHandleInfoCount();

//发送数据到ring3
NTSTATUS process_getProcessHandleInfo_send(PPROCESS_HANDLE_INFO pBuffer);

