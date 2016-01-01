#pragma once
#include "windows.h"
#include "MyNativeHead.h"
#include "SSDT.h"


#pragma warning( disable: 4312 )
#pragma warning( disable: 4311 )
#pragma warning( disable: 4244 )
#pragma warning( disable: 4267 )
#pragma warning( disable: 4996 )


//#include <Ntsecapi.h>
#include <Aclapi.h>
#include <tlhelp32.h>
#include "winioctl.h"
#define KSBINSWORD_IOCTL_BASE	0x800
#define FILE_DEVICE_KSBINSWORD	0x8000
#define CTL_CODE_KSBINSWORD(i) CTL_CODE(FILE_DEVICE_KSBINSWORD, KSBINSWORD_IOCTL_BASE+i, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_KSBINSWORD_HELLO	CTL_CODE_KSBINSWORD(0)
#define IOCTL_KSBINSWORD_TEST	CTL_CODE_KSBINSWORD(1)
#define IOCTL_KSBINSWORD_GETPRONUM	CTL_CODE_KSBINSWORD(2)//得到进程的数量，随便分配内存
#define IOCTL_KSBINSWORD_GETPRO	CTL_CODE_KSBINSWORD(3)//得到进程的各项内容
#define IOCTL_KSBINSWORD_GETTHRNUM	CTL_CODE_KSBINSWORD(4)//得到线程
#define IOCTL_KSBINSWORD_GETTHR	CTL_CODE_KSBINSWORD(5)//得到线程
#define IOCTL_KSBINSWORD_PMON CTL_CODE_KSBINSWORD(6)//进程监视开
#define IOCTL_KSBINSWORD_PMOFF CTL_CODE_KSBINSWORD(7)//进程监视关
#define IOCTL_KSBINSWORD_RMON CTL_CODE_KSBINSWORD(8)//注册表监视开
#define IOCTL_KSBINSWORD_RMOFF CTL_CODE_KSBINSWORD(9)//注册表监视关
#define IOCTL_KSBINSWORD_MMON CTL_CODE_KSBINSWORD(10)//模块监视开
#define IOCTL_KSBINSWORD_MMOFF CTL_CODE_KSBINSWORD(11)//模块监视关
#define IOCTL_KSBINSWORD_GETSSDT CTL_CODE_KSBINSWORD(12)//得到SSDT
#define IOCTL_KSBINSWORD_SETSSDT CTL_CODE_KSBINSWORD(13)//设置SSDT
#define IOCTL_KSBINSWORD_GETHOOK CTL_CODE_KSBINSWORD(14)//查询SSDT HOOK函数地址
#define IOCTL_KSBINSWORD_SETHOOK CTL_CODE_KSBINSWORD(15)//设置SSDT HOOK函数地址
#define IOCTL_KSBINSWORD_KILLTHREAD CTL_CODE_KSBINSWORD(16)//强制kill 线程

#define IOCTL_KSBINSWORD_KILLPROCESS CTL_CODE_KSBINSWORD(17)//强制kill 进程
#define IOCTL_KSBINSWORD_CLEARPROCESS CTL_CODE_KSBINSWORD(18)//清除查询进程痕迹
#define IOCTL_KSBINSWORD_CLEARTHREAD CTL_CODE_KSBINSWORD(19)//清除查询线程痕迹

#define IOCTL_KSBINSWORD_SCSI_READ  CTL_CODE_KSBINSWORD(20)//SRB读取扇区
#define IOCTL_KSBINSWORD_SCSI_WRITE CTL_CODE_KSBINSWORD(21)//SRB写入扇区

#define IOCTL_KSBINSWORD_GETMONINFO CTL_CODE_KSBINSWORD(22)//得到xx启动的信息

#define MAX_MODULE_NAME_LEN 16

typedef struct tagThreadParam 
{
	DWORD dwFreeLibrary;                    // FreeLibrary 地址
	DWORD dwGetModuleHandle;                // GetModuleHandle 地址
	TCHAR szModuleName[MAX_MODULE_NAME_LEN];// 需要卸载的模块的名称
}ThreadParam,*PThreadParam;

typedef struct tagKillThreadParam 
{
	DWORD dwOpenThread;                    // OpenThread 地址
	DWORD dwTerminateThread;                // TerminateThread 地址
	DWORD dwCid;// 需要卸载的线程Cid
}KillThreadParam,*PKillThreadParam;

typedef struct tagKillThreadStruct
{
   ULONG dwProcessId;
   ULONG dwCid;
}KillThreadStruct,*PKillThreadStruct;

typedef struct tagReadSectorStruct
{
	__int64 Sect;
	BYTE OutBuf[0x200];
}ReadSectorStruct,*PReadSectorStruct;



class CMySystem
{
public:
	CMySystem(void);
public:
	~CMySystem(void);
public:
	static int HexToInt(CString Hex);
	static __int64 HexToBigInt(CString Hex);
public:
	//BOOL static EnablePrivilege(HANDLE hToken,LPCTSTR szPrivName,BOOL fEnable);
	BOOL static EnablePrivilege(LPCTSTR szPrivName);
public:
	DWORD static GetPidByName(LPCWSTR szName) ;
public:
	static int  KillProcess(ULONG dwProcessId);
	static BOOL ForceKillProcess(HANDLE hMyHandle,ULONG dwProcessId);

	static BOOL ForceKillThread(HANDLE hMyHandle,ULONG dwProcessId,ULONG dwCid);

	static bool ClearPro(HANDLE hMyHandle);
	static bool ClearThr(HANDLE hMyHandle);
public:
	void static RemoteThreadFun(PThreadParam PParam);
	bool static AdjustPurview(void);
	BOOL static FreeModuleByPid(TCHAR * szModuleName,DWORD dwPid);
	HANDLE static
		My_OpenProcess (
		DWORD dwDesiredAccess,
		BOOL   bInhert,
		DWORD ProcessId,
		BOOL   bOpenIt,
		LPDWORD aryPids
		);
	HANDLE static OpenDevice(LPCTSTR lpszDevicePath);
	int static GetProNum(HANDLE hMyHandle);
	DWORD static GetPro(HANDLE hMyHandle,void*ProcessNum,int size);
	int static GetThrNum(HANDLE hMyHandle,int Tid);
	int static GetThr(HANDLE hMyHandle,void*ThreadNum,int size);
	void static CMySystem::BeginMon();
	void static CMySystem::ThreadMon();


	static void   LoadDriver( IN WCHAR * lpszDriverName );
	static void UnloadDriver(    WCHAR * szSvrName ) ;
	static BOOL GetSSDT( IN HANDLE hDriver, OUT PSSDT ssdt );
	static BOOL SetSSDT( IN HANDLE hDriver, IN PSSDT ssdt );
	static BOOL GetHook( IN HANDLE hDriver, IN ULONG ulIndex, OUT PULONG ulAddr );
	static BOOL SetHook( IN HANDLE hDriver, IN ULONG ulIndex, IN OUT PULONG ulAddr );
	static PSYSMODULELIST CreateModList( OUT PULONG ulNtBase );
	static void DestroyModList( IN PSYSMODULELIST pList );
	static BOOL GetModuleNameByAddr( IN ULONG ulAddr, IN PSYSMODULELIST pList, OUT LPSTR buf, IN DWORD dwSize );
	static BOOL SSDTSTOrderByServiceNum( pSSDTSaveTable pSsdtST );
	static BOOL EnumSSDT( IN HANDLE hDriver );
	static BOOL ReSSDT( IN HANDLE hDriver );
	static BOOL ReSSDTAndThrowSpilth( IN HANDLE hDriver );

    static void CharToWCHAR(WCHAR* wctar,char*cSource);
	static void WCHARToChar(char*cSource,WCHAR* wctar);
	static DWORD GetHeaders(PCHAR ibase,
		PIMAGE_FILE_HEADER *pfh,
		PIMAGE_OPTIONAL_HEADER *poh,
		PIMAGE_SECTION_HEADER *psh);
	static DWORD FindKiServiceTable(HMODULE hModule,DWORD dwKSDT);
	static	void GetOldSSDTAddress();

	static __int64 MyFileSeek (HANDLE hf, __int64 distance, DWORD MoveMethod);
	static BOOL WriteSector(__int64 Sect,BYTE* InBuf);
	static BOOL ReadSector(__int64 Sect,BYTE* OutBuf);
	static int WriteSectorByDrive(HANDLE hMyHandle,ULONG Sect,BYTE* InBuf);//写一个扇区
	static int ReadSectorByDrive(HANDLE hMyHandle,ULONG Sect,BYTE* InBuf);//读一个扇区
	
	static void TrimPathW(WCHAR szModPath[]);

	static void TrimPath(char szModPath[]);
	static void KillThreadByTerminateThread(DWORD dwProcessId,DWORD dwCid);
	static void KillThreadFun(PKillThreadParam PParam);
	HANDLE static  hMyHandle;

	BOOL static bdrv;
	DWORD static controlbuff[64];
	char static outputbuff[256]; 
	DWORD static dw;
	DWORD static stringcount;
	static char * strings[256]; 


	static ULONG NTBase ;			//NT 基址
	static ULONG TotalSSDTCount ;		//SSDT服务个数
	static pSSDTSaveTable pSSDTST ;	//保存的SSDT缓冲表
	static PSYSMODULELIST pList ;	//模块信息链表
	static ULONG RealCount ;		//枚举所找到的服务个数
    static pNtQuerySystemInformationProto pNtQuerySystemInformation ;
};

