/*
第三方dll扩充代码头文件
*/
#if !defined(DLLEXT_H)
	#define DLLEXT_H
	
	#include "comm_dll.h"
	
	#define CONSOLE_STDOUT 7
	
	extern char *strlwr(char *);
	
	extern void dllext_init(void);
	extern void dllext_clear(void);
	DLLEXP void PrintErr(void);
	DLLEXP bool HyGetSSDT(PSvrHnds pSH,PVOID buf,size_t size);
	DLLEXP bool HyGetSSDTSdw(PSvrHnds pSH,PVOID SSDTSdwAddr,\
		PVOID buf,size_t size);
	DLLEXP bool HyGetSSDTBases(PSvrHnds pSH,PVOID buf,size_t size);
	DLLEXP bool HyGetIDT(PSvrHnds pSH,PVOID buf,size_t size);
	DLLEXP bool HyGetGDT(PSvrHnds pSH,PVOID buf,size_t size);
	DLLEXP bool HyIsWinTrustFile(LPCWSTR FileNameW);
	DLLEXP int HyCheckWinTrustFile(const char *FileName);
	DLLEXP bool HyEnumSysMod(PSvrHnds pSH,PVOID SysModListAddr);
#endif
