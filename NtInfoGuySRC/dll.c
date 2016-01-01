/*
dll代码
*/
#define IN_DLL_FILE_
#include "dll.h"
#include "dllext.h"
#include "res.h"

DLLEXPVAL int RefCount \
	__attribute__((section("SHARESEG"),shared)); //default is 0

//static void PrintErr(void);
static bool DelSys(void);
static bool FreeSys(HINSTANCE hinst);

//static SvrHnds g_shs = {0};

DLLEXP bool IsFileExist(const char *szName)
{
	bool bRet = false;
	if(!szName)
		goto QUIT;
	
	PRINT("[%s]msg sys name is %s\n",__func__,szName);
	HANDLE hf = CreateFile(szName,GENERIC_READ,FILE_SHARE_READ,\
		NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hf != INVALID_HANDLE_VALUE)
		bRet = true;
		
	if(hf != INVALID_HANDLE_VALUE)
		CloseHandle(hf);
QUIT:
	return bRet;
}

static bool DelSys(void)
{
	if(RefCount > 0)
	{
		PRINT("[%s]msg : RefCount[%d] > 0,Can't Del Sys File Now!\n",\
			__func__,RefCount);
		return true;
	}
	else
		return DeleteFile(SYSNAME);
}

static bool FreeSys(HINSTANCE hinst)
{
	bool bSuccess = false;
	
	if(IsFileExist(SYSNAME))
	{
		PRINT("[%s]msg : %s is Alread Existed!\n",\
			__func__,SYSNAME);
		//if(!DelSys())	//若使用DelSys则不会成功删除文件，因为此时共享计数等于1
						//不会删除文件。
		if(!DeleteFile(SYSNAME))
		{
			PRINT("[%s]msg : Del Old %s File Failed!\n",\
				__func__,SYSNAME);
			return false;
		}
	}
	
	HRSRC hRes = FindResource(hinst,MAKEINTRESOURCE(SYSBIN),\
		RT_RCDATA);
	
	if(hRes != 0)
	{
		unsigned FileSize = SizeofResource(hinst,hRes);
		if(FileSize)
		{
			byte *pRes = LockResource(LoadResource(hinst,hRes));
			if(pRes != NULL)
			{
				//防止该文件已经存在！
				//(void)DelSys();
				FILE *pFile = fopen(SYSNAME,"w+b");
				if(pFile != NULL)
				{
					size_t ret = fwrite(pRes,1,FileSize,pFile);
					fclose(pFile);
					if(ret == FileSize)
					{
						if(SetFileAttributes(SYSNAME,\
							FILE_ATTRIBUTE_HIDDEN |\
							FILE_ATTRIBUTE_SYSTEM))
						{
							bSuccess = true;
						}
						else
						{
							PRINT("[%s]SetFileAttributes Failed!\n",\
								__func__);
						}
					}
					else
					{
						PRINT("[%s]Write File Failed!\n",\
							__func__);
					}
				}
				else
				{
					PRINT("[%s]Create File Failed!\n",\
						__func__);
				}
			}
			else
			{
				PRINT("[%s]LockResource Failed!\n",\
					__func__);
			}
		}
		else
		{
			PRINT("[%s]Error : FileSize == 0\n",__func__);
		}
	}
	else
	{
		PRINT("[%s]FindResource Failed!\n",__func__);
	}
	
	return bSuccess;
}

DLLEXP bool WriteDrv(PSvrHnds pSH,OUT PVOID buf,DWORD len,\
	LPDWORD pSizeOfBytesWrite)
{
	bool bSuccess = false;
	if(pSH == NULL || pSH->hDEV == 0)
	{
		PRINT("[%s]error : pSH == NULL or hDEV == 0!\n",\
			__func__);
		goto QUIT;
	}
	
	if(buf == NULL || pSizeOfBytesWrite == NULL)
	{
		PRINT("[%s]err : buf == NULL or " \
			"pSizeOfBytesWrite == NULL",__func__);
		goto QUIT;
	}
	
	bSuccess = WriteFile(pSH->hDEV,buf,len,pSizeOfBytesWrite,NULL);
QUIT:
	return bSuccess;
}

DLLEXP bool ReadDrv(PSvrHnds pSH,OUT PVOID buf,DWORD len,\
	LPDWORD pSizeOfBytesRead)
{
	bool bSuccess = false;
	if(pSH == NULL || pSH->hDEV == 0)
	{
		PRINT("[%s]error : pSH == NULL or hDEV == 0!\n",\
			__func__);
		goto QUIT;
	}
	
	if(buf == NULL || pSizeOfBytesRead == NULL)
	{
		PRINT("[%s]err : buf == NULL or " \
			"pSizeOfBytesRead == NULL",__func__);
		goto QUIT;
	}
	
	bSuccess = ReadFile(pSH->hDEV,buf,len,pSizeOfBytesRead,NULL);
QUIT:
	return bSuccess;
}

DLLEXP bool CallDrv(PSvrHnds pSH,DWORD Ctrl_Code,void *_in,\
		size_t cbin,OUT void *_out,size_t cbout)
{
	bool bSuccess = false;
	DWORD BytesReted = 0;
	
	if(pSH == NULL)
	{
		PRINT("[%s]error : pSH == NULL!\n",__func__);
		goto QUIT;
	}
	
	if(!pSH->hDEV || !Ctrl_Code)
	{
		PRINT("[%s]error : DN->hDEV == 0 or Ctrl_Code == 0!\n",\
			__func__);
		goto QUIT;
	}
	
	if(cbin != 0 && _in == NULL)
	{
		PRINT("[%s]err : _in is NULL\n",__func__);
		goto QUIT;
	}
	
	if(cbout != 0 && _out == NULL)
	{
		PRINT("[%s]err : _out is NULL\n",__func__);
		goto QUIT;
	}
	
	if(!DeviceIoControl(pSH->hDEV,Ctrl_Code,_in,cbin,\
		_out,cbout,&BytesReted,NULL))
	{
		PRINT("[%s]error : Call Drv Failed!\n",__func__);
		goto QUIT;
	}

	PRINT("[%s]Call Drv Receive %u Bytes.\n",__func__,\
		(unsigned)BytesReted);
			
	bSuccess = true;
QUIT:
	return bSuccess;
}

DLLEXP bool DelSvrForce(void)
{
	bool bSuccess = false;

	HANDLE hSCM = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
	if(!hSCM)
	{
		PRINT("[%s]error : OpenSCManager Failed!\n",\
			__func__);
		goto QUIT;
	}
	
	HANDLE hSVR = OpenService(hSCM,SVRNAME,SERVICE_ALL_ACCESS);
	if(!hSVR)
	{
		DWORD Err = GetLastError();
		
		switch(Err)
		{
		case ERROR_ACCESS_DENIED:
			PRINT("[In %s]error : %s == %08X\n",__func__,\
				ERR_OPENSVR_ERRACCESS,(unsigned int)Err);
			break;
		case ERROR_INVALID_HANDLE:
			PRINT("[In %s]error : %s == %08X\n",__func__,\
				ERR_OPENSVR_ERRHANDLE,(unsigned int)Err);
			break;
		case ERROR_INVALID_NAME:
			PRINT("[In %s]error : %s == %08X\n",__func__,\
				ERR_OPENSVR_ERRNAME,(unsigned int)Err);
			break;
		case ERROR_SERVICE_DOES_NOT_EXIST:
			PRINT("[In %s]error : %s == %08X\n",__func__,\
				ERR_OPENSVR_ERRDONTEXIST,(unsigned int)Err);
			break;
		default:
			PRINT("[In %s]error : Open Service Failed! %08X\n",\
				__func__,(unsigned int)Err);
			break;
		}
	}
	else
	{
		if(!DeleteService(hSVR))
		{
			DWORD Err = GetLastError();
			if(Err == ERROR_SERVICE_MARKED_FOR_DELETE)
			{
				PRINT("[In %s]error : %s == %08X\n",\
					__func__,ERR_DELSVR_ERRJUSTMAKEDEL,\
					(unsigned int)Err);
				
				bSuccess = true;
			}
			else
				PRINT("[In %s]error : %s %08X\n",__func__,\
					"DeleteService Failed!",(unsigned int)Err);
		}
		else
			bSuccess = true;
		
		CloseServiceHandle(hSVR);
		CloseServiceHandle(hSCM);
	}
QUIT:
	return bSuccess;
}

DLLEXP bool StopSvr(PSvrHnds pSH)
{
	SERVICE_STATUS SvrSta;
	if(!ControlService(pSH->hSVR,SERVICE_CONTROL_STOP,&SvrSta))
	{
		PRINT("[%s]err : Stop Svr Failed!\n",__func__);
		PrintErr();
		return false;
	}
	return true;
}

DLLEXP bool OpenSvr(PSvrHnds pSH)
{
	bool bSuccess = false;
	
	if(pSH == NULL)
	{
		PRINT("[%s]error : pSH == NULL!\n",__func__);
		goto QUIT;
	}
	
	pSH->hSCM = OpenSCManager(NULL,NULL,SC_MANAGER_CREATE_SERVICE);
	
	if(!pSH->hSCM)
	{
		PRINT("[%s]error : OpenSCManager Failed!\n",\
			__func__);
		goto QUIT;
	}
	
	pSH->hSVR = OpenService(pSH->hSCM,SVRNAME,SC_MANAGER_ALL_ACCESS);
	
	if(!pSH->hSVR)
	{
		PRINT("[%s]error : OpenService %s Failed!\n",\
			__func__,SVRNAME);
		CloseServiceHandle(pSH->hSCM);
		pSH->hSCM = 0;
		goto QUIT;
	}
	
	bSuccess = true;
QUIT:
	return bSuccess;
}

DLLEXP bool OpenDev(PSvrHnds pSH)
{
	if(pSH == NULL)
	{
		PRINT("[%s]error : pSH == NULL!\n",__func__);
		return false;
	}
	
	pSH->hDEV = CreateFile(SYMLNKNAMEA,GENERIC_READ | GENERIC_WRITE,\
		FILE_SHARE_WRITE | FILE_SHARE_READ |\
		FILE_SHARE_DELETE,NULL,OPEN_EXISTING,0,NULL);
	
	if(pSH->hDEV == INVALID_HANDLE_VALUE)
	{
		pSH->hDEV = 0;
		PRINT("[%s]error : Connect Dev Failed!\n",\
			__func__);
		PrintErr();
		return false;
	}
	return true;
}

DLLEXP bool StartSvr(PSvrHnds pSH)
{
	bool bSuccess = false;
	
	if(pSH == NULL)
	{
		PRINT("[%s]error : pSH == NULL!\n",__func__);
		goto QUIT;
	}
	
	if(pSH->hSVR == 0)
	{
		PRINT("[%s]error : pSH->hSVR == 0!\n",__func__);
		goto QUIT;
	}
	
	bool bRet = StartService(pSH->hSVR,0,NULL);
	if(!bRet && GetLastError() == ERROR_SERVICE_ALREADY_RUNNING)
	{
		PRINT("[%s]msg : Service is Already Running!\n",\
			__func__);
	}
	else if(!bRet)
	{
		PRINT("[%s]error : StartService Failed!\n",\
			__func__);
		goto QUIT;
	}
	
	if(!OpenDev(pSH))
	{
		goto QUIT;
	}
	
	bSuccess = true;
QUIT:
	return bSuccess;
}

DLLEXP bool DelSvr(PSvrHnds pSH)
{
	bool bSuccess = false;
	SERVICE_STATUS SvrSta;
	
	--RefCount;
	if(RefCount > 0)
	{
		PRINT("[%s]msg : RefCount[%d] > 0,Can't Del Svr Now!\n",\
			__func__,RefCount);
		return true;
	}
	
	if(pSH->hSVR == 0 || pSH->hSCM == 0)
	{
		PRINT("[%s]error : hSVR == 0 or hSCM == 0!\n",\
			__func__);
		goto QUIT;
	}
	
	if(pSH->hDEV)
		CloseHandle(pSH->hDEV);
	
	ControlService(pSH->hSVR,SERVICE_CONTROL_STOP,&SvrSta);
	DeleteService(pSH->hSVR);
	CloseServiceHandle(pSH->hSVR);
	CloseServiceHandle(pSH->hSCM);
	
	bSuccess = true;
QUIT:
	return bSuccess;
}

DLLEXP bool NewSvr(PSvrHnds pSH)
{
	bool bSuccess = false;
	char FullName[MAX_PATH];
	
	if(pSH == NULL)
	{
		PRINT("[%s]error : pSH == NULL!\n",__func__);
		goto QUIT;
	}
	
	pSH->hSCM = OpenSCManager(NULL,NULL,SC_MANAGER_CREATE_SERVICE);
	
	if(!pSH->hSCM)
	{
		PRINT("[%s]error : OpenSCManager Failed!\n",\
			__func__);
		goto QUIT;
	}
	
	if(GetFullPathName(SYSNAME,sizeof(FullName),\
		FullName,NULL) < strlen(SYSNAME)+1)
	{
		PRINT("[%s]error : Get SYS FullPathName Failed!\n",\
			__func__);
		goto QUIT;
	}
		
	pSH->hSVR = CreateService(pSH->hSCM,SVRNAME,SVRNOTE,\
		SERVICE_ALL_ACCESS,SERVICE_KERNEL_DRIVER,\
		SERVICE_DEMAND_START,SERVICE_ERROR_IGNORE,\
		FullName,NULL,NULL,NULL,NULL,NULL);
		
	if(!pSH->hSVR && GetLastError() == ERROR_SERVICE_EXISTS)
	{
		PRINT("[%s]msg : Servicr Existed , Try To Open!\n",\
			__func__);
		pSH->hSVR = OpenService(pSH->hSCM,SVRNAME,\
			SERVICE_ALL_ACCESS);
		if(!pSH->hSVR)
		{
			PRINT("[%s]err : OpenService Failed!\n",\
				__func__);
			goto QUIT;
		}
	}
	else if(!pSH->hSVR)
	{
		PRINT("[%s]error : CreateService Failed!\n",\
			__func__);
		goto QUIT;
	}
	
	bSuccess = true;
QUIT:
	if(!bSuccess)
	{
		if(pSH->hSCM)
			CloseServiceHandle(pSH->hSCM);
		
		if(pSH->hSVR)
			CloseServiceHandle(pSH->hSVR);
	}
	return bSuccess;
}

/*
static void PrintErr(void)
{
	char *pbuf;
	DWORD ErrID = GetLastError();
	
	if(FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |\
		FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,\
		NULL,ErrID,LANG_NEUTRAL,(LPTSTR)&pbuf,0,NULL) == 0)
	{
		PRINT("[%s]FormatMessage Failed!\n",__func__);
		return;
	}
	
	printf("Err Code : %08x [ %d ]\n",(unsigned)ErrID,(unsigned)ErrID);
	printf("Err Text : %s\n",pbuf);
	
	if(LocalFree(pbuf) != NULL)
	{
		PRINT("[%s]LocalFree Failed!\n",__func__);
	}
}
*/

static bool init(HINSTANCE hInst)
{
	bool bSuccess = false;
	
	//add dll ref!
	++RefCount;
	PRINT("[%s]Add RefCount , Now RefCount is %d\n",\
		__func__,RefCount);
	
	if(!FreeSys(hInst))
	{
		PRINT("[%s]err : FreeSys Failed!\n",__func__);
		goto QUIT;
	}

	dllext_init();
	bSuccess = true;
QUIT:
	if(!bSuccess)
	{
		PRINT("[%s]err : init failed!\n",__func__);
		PrintErr();
	}
	return bSuccess;
}

static bool clear(void)
{
	bool bSuccess = false;
	
	dllext_clear();
	if(!DelSys())
	{
		PRINT("[%s]err : clear failed!\n",__func__);
		goto QUIT;
	}
	
	bSuccess = true;
QUIT:
	return bSuccess;
}

int APIENTRY DllMain(HINSTANCE hInstance,\
	DWORD dwReason,LPVOID lpReserved)
{
	if(dwReason == DLL_PROCESS_ATTACH)
	{
		//Easy Debug For MFC
		//AllocConsole();
		//freopen("CONOUT$","w+t",stdout);
		//freopen("CONIN$","r+t",stdin);
		return init(hInstance);
	}
	else if(dwReason == DLL_PROCESS_DETACH)
	{
		if(!clear())
			PRINT("[%s]err : clear failed!\n",__func__);
	}
	return true;
}
