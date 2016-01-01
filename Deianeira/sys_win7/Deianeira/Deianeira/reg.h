#ifndef _REG_
#define _REG_

#include "common.h"


//在这里InlineHook,禁止创建项和键值
VOID reg_inlineHook_disableCreate();

//停止禁止创建项和键值
VOID reg_inlineHook_disableCreate_stop();

//在这里InlineHook,禁止删除项和键值
VOID reg_inlineHook_disableDelete();

//停止禁止删除项和键值
VOID reg_inlineHook_disableDelete_stop();

//在这里InlineHook,禁止查询
VOID reg_inlineHook_disableQuery();

//停止禁止查询
VOID reg_inlineHook_disableQuery_stop();

//从DisableCreateRegInfo中查找目标进程ID
//返回
//0表示没有找到，1表示找到了
ULONG reg_findPIDInDisableCreateRegInfo(ULONG ulPID);

//从DisableDeleteRegInfo中查找目标进程ID
//返回
//0表示没有找到，1表示找到了
ULONG reg_findPIDInDisableDeleteRegInfo(ULONG ulPID);

//从DisableOpenRegInfo中查找目标进程ID
//返回
//0表示没有找到，1表示找到了
ULONG reg_findPIDInDisableOpenRegInfo(ULONG ulPID);

//从DisableCreateRegInfo中删除目标进程ID
VOID reg_deletePIDInDisableCreateRegInfo(ULONG ulPID);

//从DisableDeleteRegInfo中删除目标进程ID
VOID reg_deletePIDInDisableDeleteRegInfo(ULONG ulPID);

//从DisableOpenRegInfo中删除目标进程ID
VOID reg_deletePIDInDisableOpenRegInfo(ULONG ulPID);

#endif //_REG_
