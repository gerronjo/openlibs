/*
第三方dll扩充代码
*/
#define IN_DLL_FILE_
#include "dll.h"
#include "dllext.h"
#include <Wincrypt.h>
#include "gccWinTrust.h"

//全局系统路径全名 : "c:\windows\system32\"
static char g_SysPath[MAX_PATH];
//全局系统路径短名 : "c:\windows\"
static char g_SysPathShort[MAX_PATH];
//全局系统驱动路径名 : "c:\windows\system32\drivers\"
static char g_SysDriversPath[MAX_PATH];

static HCATADMIN g_hCatAdmin = NULL;

void dllext_init(void)
{
	PRINT("[%s]msg : enter dllext_init\n",__func__);
	//取得系统目录名
	GetSystemDirectory(g_SysPath,sizeof(g_SysPath));
	strcat(g_SysPath,"\\");
	strcpy(g_SysDriversPath,g_SysPath);
	strcat(g_SysDriversPath,"Drivers\\");
	
	char *pSplit = strstr(g_SysPath,"sys");
	*pSplit = '\0';
	strcpy(g_SysPathShort,g_SysPath);
	*pSplit = 's';
	
	PRINT("[%s]g_SysPath : %s\n",__func__,g_SysPath);
	PRINT("[%s]g_SysPathShort : %s\n",__func__,g_SysPathShort);
	PRINT("[%s]g_SysDriversPath : %s\n",__func__,g_SysDriversPath);
	
	PRINT("[%s]msg : leave %s\n",__func__,__func__);
}

void dllext_clear(void)
{
	PRINT("[%s]msg : enter dllext_clear!\n",__func__);
	if(g_hCatAdmin)
		CryptCATAdminReleaseContext(g_hCatAdmin,0); 
}

//打印win NT错误的文字描述信息。
DLLEXP void PrintErr(void)
{
	char *buf;
	DWORD ErrID = GetLastError();
	
	if(FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |\
		FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,\
		NULL,ErrID,LANG_NEUTRAL,(LPTSTR)&buf,0,NULL) == 0)
	{
		PRINT("[%s]FormatMessage Failed!\n",__func__);
		return;
	}
	
	PRINT("Err Code : %08x [ %d ]\n",(unsigned)ErrID,\
		(unsigned)ErrID);
	PRINT("Err Text : %s\n",buf);
	
	if(LocalFree(buf) != NULL)
	{
		PRINT("[%s]LocalFree Failed!\n",__func__);
	}
}

DLLEXP bool HyGetSSDT(PSvrHnds pSH,PVOID buf,size_t size)
{
	return CallDrv(pSH,IOCTL_CTL_GET_SSDT,NULL,0,buf,size);
}

DLLEXP bool HyGetSSDTSdw(PSvrHnds pSH,PVOID SSDTSdwAddr,\
	PVOID buf,size_t size)
{
	if(!SSDTSdwAddr) return false;
	return CallDrv(pSH,IOCTL_CTL_GET_SSDT_SDW,&SSDTSdwAddr,\
		sizeof(SSDTSdwAddr),buf,size);
}

DLLEXP bool HyGetSSDTBases(PSvrHnds pSH,PVOID buf,size_t size)
{
	if(size < sizeof(PVOID)*2) return false;
	return CallDrv(pSH,IOCTL_CTL_GET_TABLE_BASE,NULL,0,buf,size);
}

DLLEXP bool HyGetIDT(PSvrHnds pSH,PVOID buf,size_t size)
{
	return CallDrv(pSH,IOCTL_CTL_GET_IDT,NULL,0,buf,size);
}

DLLEXP bool HyGetGDT(PSvrHnds pSH,PVOID buf,size_t size)
{
	return CallDrv(pSH,IOCTL_CTL_GET_GDT,NULL,0,buf,size);
}

DLLEXP bool HyEnumSysMod(PSvrHnds pSH,PVOID SysModListAddr)
{
	return CallDrv(pSH,IOCTL_CTL_ENUM_SYSMODLIST,&SysModListAddr,\
		sizeof(SysModListAddr),NULL,0);
}

DLLEXP bool HyGetSysVal(PSvrHnds pSH,PVOID addr,PVOID buf,size_t size)
{
	return CallDrv(pSH,IOCTL_CTL_GET_SYSVAL,&addr,sizeof(addr),\
		buf,size);
}

//验证文件是否是受到windows信任的。若是返回true，否则返回false。
DLLEXP bool HyIsWinTrustFile(LPCWSTR FileNameW)
{
	bool bRet = false;
    WINTRUST_DATA wd = {0};
    WINTRUST_FILE_INFO wfi = {0};
    WINTRUST_CATALOG_INFO wci = {0};
    CATALOG_INFO ci = {0};

	if(!g_hCatAdmin)
	{
		if (!CryptCATAdminAcquireContext(&g_hCatAdmin, NULL,0))
		{
			g_hCatAdmin = NULL;
			PRINT("[%s]err : CryptCATAdminAcquireContext Failed!\n",__func__);
		}
	}
	
    HANDLE hFile = CreateFileW(FileNameW,GENERIC_READ,FILE_SHARE_READ,\
        NULL,OPEN_EXISTING,0,NULL);
    if(INVALID_HANDLE_VALUE == hFile)
    {
        //CryptCATAdminReleaseContext(hCatAdmin, 0);
		PRINT("[%s]err : Open File Failed!\n",__func__);
        return false;
    }

    DWORD dwCnt = 100;
    BYTE byHash[100];
    CryptCATAdminCalcHashFromFileHandle(hFile,&dwCnt,byHash,0);
    CloseHandle(hFile);

	LPWSTR pszMemberTag = malloc((dwCnt*2+1)*sizeof(WCHAR));
    for(DWORD dw = 0;dw < dwCnt;++dw )
    {
        wsprintfW(&pszMemberTag[dw * 2],L"%02X",byHash[dw]);
    }

    HCATINFO hCatInfo = (HCATINFO)CryptCATAdminEnumCatalogFromHash(\
		g_hCatAdmin,byHash,dwCnt,0,NULL);
    if(NULL == hCatInfo)
    {
        wfi.cbStruct       = sizeof(WINTRUST_FILE_INFO);
        wfi.pcwszFilePath  = FileNameW;
        wfi.hFile          = NULL;
        wfi.pgKnownSubject = NULL;
        wd.cbStruct            = sizeof(WINTRUST_DATA);
        wd.dwUnionChoice       = WTD_CHOICE_FILE;
        wd.noname.pFile        = &wfi;
        wd.dwUIChoice          = WTD_UI_NONE;
        wd.fdwRevocationChecks = WTD_REVOKE_NONE;
        wd.dwStateAction       = WTD_STATEACTION_IGNORE;
        wd.dwProvFlags         = WTD_SAFER_FLAG;
        wd.hWVTStateData       = NULL;
        wd.pwszURLReference    = NULL;
    }
    else
    {
        CryptCATCatalogInfoFromContext(hCatInfo,&ci,0);
        wci.cbStruct             = sizeof(WINTRUST_CATALOG_INFO);
        wci.pcwszCatalogFilePath = ci.wszCatalogFile;
        wci.pcwszMemberFilePath  = FileNameW;
        wci.pcwszMemberTag       = pszMemberTag;
        wd.cbStruct            = sizeof(WINTRUST_DATA);
        wd.dwUnionChoice       = WTD_CHOICE_CATALOG;
        wd.noname.pCatalog     = &wci;
        wd.dwUIChoice          = WTD_UI_NONE;
        wd.fdwRevocationChecks = WTD_STATEACTION_VERIFY;
        wd.dwProvFlags         = 0;
        wd.hWVTStateData       = NULL;
        wd.pwszURLReference    = NULL;
    }
    GUID action = WINTRUST_ACTION_GENERIC_VERIFY_V2;
    HRESULT hr  = WinVerifyTrust(NULL,&action,&wd);
    bRet        = SUCCEEDED(hr);

    if (hCatInfo)
    {
        CryptCATAdminReleaseCatalogContext(g_hCatAdmin,hCatInfo,0);
    }
	
    free(pszMemberTag);
    return bRet;
}

//大小写无关的strstr
static char *strstri(const char *dst,const char *src)
{
	static char dst_tmp[MAX_PATH];
	static char src_tmp[MAX_PATH];
	strcpy(dst_tmp,dst);
	strlwr(dst_tmp);
	strcpy(src_tmp,src);
	strlwr(src_tmp);
	return strstr(dst_tmp,src_tmp);
}

//验证FileName指定的文件是否是Windows Trust文件，其中根据各种不同情况
//对FileName做了适当的修正.
//返回0表示不是Trust文件，1表示是Trust文件，返回-1表示磁盘上找不到该文件。
DLLEXP int HyCheckWinTrustFile(const char *FileName)
{
	int iRet = 0;
	if(!FileName) goto QUIT;
	static char tmp_path[MAX_PATH];
	static wchar_t tmp_pathW[MAX_PATH];
	
	//如果文件名中含有SystemRoot则转换为实际目录名.
	if(strstri(FileName,"SystemRoot"))
	{
		strcpy(tmp_path,g_SysPathShort);
		strcat(tmp_path,&FileName[12]);	//跳过"\SystemRoot\"
	}
	else if(strstr(FileName,"\\??\\"))
	{
		strcpy(tmp_path,&FileName[4]);	//跳过"\??\"
	}
	else if(FileName[0] == '\\')
	{
		g_SysPath[3] = '\0';
		strcpy(tmp_path,g_SysPath);
		g_SysPath[3] = 'W';
		strcat(tmp_path,&FileName[1]);	//跳过"\"
	}
	else if(!strstr(FileName,"\\"))	//如果FileName中没有"\"则表示是短路径名
	{
		strcpy(tmp_path,g_SysPath);
		strcat(tmp_path,FileName);
		//如果不在g_SysPath中，则假设在g_SysDriversPath中
		if(!IsFileExist(tmp_path))
		{
			strcpy(tmp_path,g_SysDriversPath);
			strcat(tmp_path,FileName);
		}
	}
	
	if(!IsFileExist(tmp_path))
	{
		PRINT("[%s]err : File [%s] Not Found!\n",__func__,tmp_path);
		iRet = -1;
		goto QUIT;
	}
	
	mbstowcs(tmp_pathW,tmp_path,sizeof(tmp_pathW));
	if(HyIsWinTrustFile(tmp_pathW)) iRet = 1;
QUIT:
	return iRet;
}
