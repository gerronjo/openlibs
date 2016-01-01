#if !defined(DDK_C_H)
	#define DDK_C_H

	typedef long bool;
	typedef unsigned DWORD,*PDWORD;
	typedef unsigned char BYTE,*PBYTE;

	#ifdef __cplusplus
	extern "C"
	{
	#endif
	#include <ntddk.h>
	#ifdef __cplusplus
	}
	#endif 

	//是否调试(显示调试字符串)
	#define GOTO_DEBUG

	#define IN /*IN*/
	#define OUT /*OUT*/

	#define DDKAPI __stdcall
	#define __func__ __FUNCTION__

	enum {false = 0,true = 1};
#endif
