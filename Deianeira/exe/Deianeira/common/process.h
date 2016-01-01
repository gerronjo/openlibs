//process.h:包含一些进程操作等
//
//
#ifndef _PROCESS_
#define _PROCESS_
#include "common.h"

//
extern PPROCESS_MODULE_INFO pProcessModuleInfo,pProcessModuleInfoHead;

//获取线程的状态
//ulPID:目标进程ID
//ulTID:目标线程ID
//返回:0:表示发生异常,1:表示进程处于挂起状态,2:表示进程没有被挂起
ULONG process_getThreadState(ULONG ulPID,ULONG ulTID);

//内核挂起线程
//hDriver:驱动句柄
//ulPID:目标进程ID
//返回:true:表示挂起成功,false:表示挂起失败
bool process_suspendThread(HANDLE hDriver,ULONG ulTID);

//内核恢复线程
//hDriver:驱动句柄
//ulPID:目标进程ID
//返回:true:表示恢复成功,false:表示恢复失败
bool process_resumeThread(HANDLE hDriver,ULONG ulTID);

//内核结束线程
//hDriver:驱动句柄
//ulTID:目标线程ID
//返回:true:表示删除成功,false:表示删除失败
bool process_killThread(HANDLE hDriver,ULONG ulTID);

//获取进程的父进程ID
//ulPID:目标进程ID
//返回:目标进程的父进程ID
ULONG process_getProcessParentPID(ULONG ulPID);

//获取进程的状态
//ulPID:目标进程ID
//返回:0:表示发生异常,1:表示进程处于挂起状态,2:表示进程没有被挂起
ULONG process_getProcessState(ULONG ulPID);

//锁掉进程
//ulPID:目标进程ID
//返回:true:表示挂起成功,false:表示挂起失败
bool process_suspendProcess(ULONG ulPID);

//内核锁掉进程
//hDriver:驱动句柄
//ulPID:目标进程ID
//返回:true:表示挂起成功,false:表示挂起失败
bool process_suspendProcess(HANDLE hDriver,ULONG ulPID);

//恢复进程
//ulPID:目标进程ID
//返回:true:表示恢复成功,false:表示恢复失败
bool process_resumeProcess(ULONG ulPID);

//内核恢复进程
//hDriver:驱动句柄
//ulPID:目标进程ID
//返回:true:表示恢复成功,false:表示恢复失败
bool process_resumeProcess(HANDLE hDriver,ULONG ulPID);

//内核结束进程
//hDriver:驱动句柄
//ulPID:目标进程ID
//返回:true:表示恢复成功,false:表示恢复失败
bool process_killProcess(HANDLE hDriver,ULONG ulPID);

//内核结束进程,内存清零
//hDriver:驱动句柄
//ulPID:目标进程ID
//返回:true:表示恢复成功,false:表示恢复失败
bool process_killProcess_memclr(HANDLE hDriver,ULONG ulPID);

//内核进程模块卸载
//hDriver:驱动句柄
//ulPID:目标进程ID
//ulDllBase:DLL模块基址
//返回:true:表示卸载成功,false:表示卸载失败
bool process_unloadModule(HANDLE hDriver,ULONG ulPID,ULONG ulDllBase);

//注入DLL
//lpFilePath:DLL文件路径
//ulPID:进程ID
//返回:true:注入成功,false:注入失败
bool process_injectDll(LPTSTR lpFilePath,ULONG ulPID);

//获得进程的模块信息列表
//返回:true:成功,false:失败
bool process_getModuleInfoTable();

//获得进程线程列表
PSYSTEM_PROCESSES GetThreadList();

#endif //_PROCESS_