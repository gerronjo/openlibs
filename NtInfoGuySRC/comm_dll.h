#if !defined(COMM_DLL_H)
	#define COMM_DLL_H
	
	#include "comm.h"
	#include <stdlib.h>
	#include <stdio.h>
	#include <windows.h>
	
	#if defined(_GOTO_DEBUG_)
		#define PRINT(...) printf(__VA_ARGS__)
	#else
		#define PRINT(...) ((void)0)
	#endif
	
	#if defined(IN_DLL_FILE_)
		#define DLLEXP __declspec(dllexport) WINAPI
		#define DLLEXPVAL __declspec(dllexport)
	#else
		#define DLLEXP __declspec(dllimport) WINAPI
		#define DLLEXPVAL __declspec(dllimport)
	#endif
#endif
