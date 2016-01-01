/*
通用dll代码头文件
*/
#if !defined(DLL_H_)
	#define DLL_H_
	
	#include "comm_dll.h"
	
	#define INITED 0x6DE69953
	
	#define SYSNAME STRA(PRJNAME) "_free.sys"
	#define SVRNAME STRA(PRJNAME) "_SVR"
	#define SVRNOTE STRA(PRJNAME) " Code By hopy"
	
	#define ERR_OPENSVR_ERRACCESS "ERROR_ACCESS_DENIED"
	#define ERR_OPENSVR_ERRHANDLE "ERROR_INVALID_HANDLE"
	#define ERR_OPENSVR_ERRNAME "ERROR_INVALID_NAME"
	#define ERR_OPENSVR_ERRDONTEXIST "ERROR_SERVICE_DOES_NOT_EXIST"
	#define ERR_DELSVR_ERRJUSTMAKEDEL "ERROR_SERVICE_MARKED_FOR_DELETE"
	
	typedef struct _SvrHnds
	{
		HANDLE hSCM;
		HANDLE hSVR;
		HANDLE hDEV;
	}SvrHnds,*PSvrHnds;
	
	DLLEXP bool CallDrv(PSvrHnds pSH,DWORD Ctrl_Code,void *_in,\
		size_t cbin,OUT void *_out,size_t cbout);
	DLLEXP bool ReadDrv(PSvrHnds pSH,OUT PVOID buf,DWORD len,\
		LPDWORD pSizeOfBytesRead);
	DLLEXP bool WriteDrv(PSvrHnds pSH,OUT PVOID buf,DWORD len,\
		LPDWORD pSizeOfBytesWrite);
	DLLEXP bool OpenDev(PSvrHnds pSH);
	DLLEXP bool DelSvrForce(void);
	DLLEXP bool StartSvr(PSvrHnds pSH);
	DLLEXP bool OpenSvr(PSvrHnds pSH);
	DLLEXP bool StopSvr(PSvrHnds pSH);
	DLLEXP bool DelSvr(PSvrHnds pSH);
	DLLEXP bool NewSvr(PSvrHnds pSH);
	DLLEXP bool IsFileExist(const char *szName);
	
	DLLEXPVAL int RefCount;
#endif
