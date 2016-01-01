/*
通用内核代码头文件
*/
#if !defined(COMM_SYS_H)
	#define COMM_SYS_H
	
	#include "comm.h"
	#include "ddk\ntddk.h"
	#include "ddk\ntifs.h"
	#include "comm_ext.h"
	
	#if defined(_GOTO_DEBUG_)
		#define PRINT(...) DbgPrint(__VA_ARGS__)
	#else
		#define PRINT(...) ((void)0)
	#endif
#endif
