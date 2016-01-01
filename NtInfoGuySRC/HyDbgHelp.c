#include "HyDbgHelp.h"

bool WaitAnyKey(size_t i);

const static char *g_pDefaultMsSymUrl = "http://msdl.microsoft.com/download/symbols";
//全局系统模块信息的指针
static PVOID gp_pModulesInfo = NULL;	//gp_ : 全局私有变量

//刷新系统模块信息，返回模块信息的数量。返回0表示函数失败。
size_t HyFlushSysModsInfo(void)
{
	size_t dwRet = 0;
	NTSTATUS status;
	static size_t InitSize = 0x4000;

	if(gp_pModulesInfo)
	{
		free(gp_pModulesInfo);
		gp_pModulesInfo = NULL;
	}

	while(true)
	{
		gp_pModulesInfo = malloc(InitSize);
		if(!gp_pModulesInfo) goto QUIT;
		if(STATUS_INFO_LENGTH_MISMATCH == \
			(status = NtQuerySystemInformation(SystemModuleInformation,\
			gp_pModulesInfo,InitSize,NULL)))
		{
			free(gp_pModulesInfo);
			InitSize <<= 1;
			continue;
		}
		if(!NT_SUCCESS(status))
		{
			PRINT("[%s]err : Query Modules Info Failed!\n",__func__);
		}
		else
		{
			dwRet = *(size_t*)gp_pModulesInfo;
			PRINT("[%s]InitSize : %08x , Actual Size : %08x\n",\
				__func__,InitSize,sizeof(SYSTEM_MODULE_INFORMATION) * dwRet);
		}
		break;
	}
QUIT:
	return dwRet;
}

//获取文件的短路径。
static const char *GetShortPath(const char *Path)
{
	const char *pSplit = strrchr(Path,'\\');
	if(pSplit) return pSplit + 1;
	return Path;
}

//取得运行时模块信息。如果参数pModName为"Print*",则表示在控制台打印所有系统
//模块的信息。
bool HyGetRTModInfo(const char *pModName,OUT PSYSTEM_MODULE_INFORMATION pModInfo)
{
	bool bRet = false;
	size_t count;
	PSYSTEM_MODULE_INFORMATION l_pModInfo;	//l_ local局部变量

	if(!pModName || !gp_pModulesInfo) goto QUIT;

	count = *(size_t*)gp_pModulesInfo;
	l_pModInfo = (PSYSTEM_MODULE_INFORMATION)((PULONG)gp_pModulesInfo + 1);

	if(!strcmp(pModName,"Print*"))
	{
		ULONG Limit;
		const char *pstr = NULL;
		
		//枚举所有系统模块,但是函数返回false！
		for(size_t i = 0;i < count;++i)
		{
			switch(HyCheckWinTrustFile(l_pModInfo[i].ImageName))
			{
			case 0:
				pstr = "Not Trust Module";break;
			case 1:
				pstr = "Trust Module";break;
			case -1:
				pstr = "File Not Found!";break;
			}
			
			Limit = (ULONG)l_pModInfo[i].Base + l_pModInfo[i].Size;
			printf("[%03d]%-20s @%08X-%08X flags:%08x %s\n",i+1,\
				GetShortPath(l_pModInfo[i].ImageName),\
				l_pModInfo[i].Base,Limit,l_pModInfo[i].Flags,pstr);
			if(WaitAnyKey(i)) break;
		}
	}
	else
	{
		//取得指定系统模块信息
		if(!pModInfo) goto QUIT;
		for(size_t i = 0;i < count;++i)
		{
			if(strstr(l_pModInfo[i].ImageName,pModName))
			{
				*pModInfo = l_pModInfo[i];
				bRet = true;
				break;
			}
		}
	}
QUIT:
	return bRet;
}

//取得运行时模块的基址
PVOID HyGetRTModBase(const char *pModName)
{
	SYSTEM_MODULE_INFORMATION ModInfo;
	if(!HyGetRTModInfo(pModName,&ModInfo)) return NULL;
	//printf("[%s]%s base is %08x\n",__func__,pModName,ModInfo.Base);
	return ModInfo.Base;
}

//如果系统开启了PAE则返回true，否则返回false
bool HyIsOpenPAE(void)
{
	return (bool)HyGetRTModBase("ntkrnlpa");
}

//清除系统模块信息占用的资源
void HyClearSysModsInfo(void)
{
	free(gp_pModulesInfo);
	gp_pModulesInfo = NULL;
}

//在指定路径下建立symsrv.yse文件
static void CreateSymYesFile(const char *Path)
{
	FILE *pf = NULL;
	char FullName[MAX_PATH];

	if(!Path) return;
	strcpy(FullName,Path);
	strcat(FullName,"\\symsrv.yes");

	pf = fopen(FullName,"r");
	if(!pf) pf = fopen(FullName,"w+");
	fclose(pf);
}

//初始化DbgHelp功能所需的资源.参数为Pdb符号文件存放的路径,如果传递NULL,则
//表示在应用程序的当前路径的syms子目录中存放Pdb符号文件。
bool HyInitDbgHelp(const char *pSymPath)
{
	char pCurPath[MAX_PATH];
	char l_pSymPath[MAX_PATH*2];
	DWORD Options = SymGetOptions();
	Options |= SYMOPT_DEBUG;
	SymSetOptions(Options);

	if(!pSymPath)
	{
		//如果pSymPath为NULL,则使用应用程序的当前目录
		GetCurrentDirectory(sizeof(pCurPath),(LPTSTR)pCurPath);
		//创建symsrv.yes文件，以免弹出微软的symbol许可对话框
		CreateSymYesFile(pCurPath);
		strcpy(l_pSymPath,"SRV*");
		strcat(l_pSymPath,pCurPath);
		strcat(l_pSymPath,"\\syms*");
		strcat(l_pSymPath,g_pDefaultMsSymUrl);
		pSymPath = l_pSymPath;
	}
	PRINT("%s\n",pSymPath);
	return SymInitialize(GetCurrentProcess(),(LPSTR)pSymPath,false);
}

//清除DbgHelp所需的资源
bool HyClearDbgHelp(void)
{
	return SymCleanup(GetCurrentProcess());
}

//加载指定模块的Pdb文件.如有必要会从微软符号服务器下载,此时symsrv.dll文件必须
//能被程序动态链接.
bool HyLoadModulePdb(const char *pModName)
{
	char PdbPath[MAX_PATH];
	if(!pModName) return false;

	return SymGetSymbolFile(GetCurrentProcess(),NULL,pModName,sfPdb,PdbPath,\
		sizeof(PdbPath),PdbPath,sizeof(PdbPath));
}

//在控制台打印指定DllBase的dll信息。
void HyPrintModuleInfo(DWORD64 DllBase)
{
	char *pSymType;
	IMAGEHLP_MODULE64 ModInfo64 = {0};
	ModInfo64.SizeOfStruct = sizeof(ModInfo64); 
	
	//在某些XP SP3系统下，SymGetModuleInfo64虽然返回false,同时最近错误
	//为 ERROR_INVALID_PARAMETER 时，其仍然会正确填充ModInfo64结构。
	if(SymGetModuleInfo64(GetCurrentProcess(),DllBase,&ModInfo64) || \
		GetLastError()==ERROR_INVALID_PARAMETER)
	{
		switch(ModInfo64.SymType)
		{
		case SymCoff:
			pSymType = "COFF Syms";break;
		case SymCv:
			pSymType = "CodeView Syms";break;
		case SymDeferred:
			pSymType = "Deferred Syms";break;
		case SymDia:
			pSymType = "DIA Syms";break;
		case SymExport:
			pSymType = "Export Table Syms";break;
		case SymNone:
			pSymType = "No Syms";break;
		case SymPdb:
			pSymType = "PDB Syms";break;
		case SymSym:
			pSymType = ".sym File Syms";break;
		case SymVirtual:
			pSymType = "Virtual Syms";break;
		default:
			pSymType = "Unknow Syms";break;
		}

		PRINT("[%s]msg : \n%s@0x%I64X : ImgSize:%d , ImgStamp:%08x , \n" \
			"CheckSum:%08x , NumSyms:%d , SymType:%s\n",\
			__func__,ModInfo64.ModuleName,ModInfo64.BaseOfImage,ModInfo64.ImageSize,\
			ModInfo64.TimeDateStamp,ModInfo64.CheckSum,ModInfo64.NumSyms,pSymType);
	}
	else
		PRINT("[%s]err : SymGetModuleInfo64 Failed!\n",__func__);
}

//加载指定的Module到内存，并且返回加载的基址.
DWORD64 HyLoadModule(const char *pModName)
{
	return SymLoadModule64(GetCurrentProcess(),NULL,pModName,NULL,0,0);
}

//卸载指定DllBase的dll。
bool HyUnloadModule(DWORD64 DllBase)
{
	return SymUnloadModule64(GetCurrentProcess(),DllBase);
}

//将指定的模块文件存放到应用程序当前目录下
bool HyStoreModFile(const char *pModName)
{
	PCSTR pPath;
	char ModPath[MAX_PATH * 2];
	char CurPath[MAX_PATH];
	GetCurrentDirectory(sizeof(CurPath),(LPTSTR)CurPath);
	strcpy(ModPath,"SRV*");
	strcat(ModPath,CurPath);
	strcat(ModPath,"*");

	pPath = SymSrvStoreFile(GetCurrentProcess(),ModPath,\
		pModName,SYMSTOREOPT_OVERWRITE);
	if(pPath) 
	{
		PRINT("[%s]msg : %s\n",__func__,pPath);
		return true;
	}
	return false;
}	

//模块的枚举回调函数，由HyGetSymAddrRaw内部回调。
static bool CALLBACK EnumSymProc(PSYMBOL_INFO pSI,ULONG SymSize,PVOID Context)
{
	static int i = 0;
	char *pSymName = (char *)((PVOID*)Context)[0];
	if(!strcmp(pSymName,"Print*"))
	{
		printf("[%04d] %s@%I64X\n",++i,pSI->Name,pSI->Address);
	}
	else if(!strcmp(pSymName,pSI->Name))
	{
		((PVOID*)Context)[1] = (PVOID)(DWORD)pSI->Address;
		return false;
	}
	return true;
}

//返回指定符号的偏移地址，该地址是相对于Module默认Base的偏移，可能不是
//真正内存中的实际偏移，需要手动计算实际偏移。
//如果pSymName为"Print*",则表示在控制台打印所有该模块符号的信息。
PVOID HyGetSymAddrRaw(DWORD64 DllBase,const char *pSymName)
{
	PVOID Context[2] = {(PVOID)pSymName,NULL};
	SymEnumSymbols(GetCurrentProcess(),DllBase,0,EnumSymProc,(PVOID)Context);
	return Context[1];
}

//取得指定地址处的符号信息.
//需要手动将内存中的offset转换为实际加载中的offset Addr。
bool HySymFromAddrRaw(DWORD64 Addr,OUT PSYMBOL_INFO pSI)
{
	return SymFromAddr(GetCurrentProcess(),Addr,NULL,pSI);
}

static bool CALLBACK EnumTypeProc(PSYMBOL_INFO pSI,ULONG SymSize,PVOID Context)
{
	//static int i = 0;
	const char *TypeName = (const char *)((PVOID*)Context)[0];
	/*
	PRINT("[%d]TypeName : %s , TypeIdx : %d\n",i++,pSI->Name,\
		pSI->TypeIndex);
	*/
	if(!strcmp(pSI->Name,TypeName))
	{
		PSYMBOL_INFO pSI_Type = (PSYMBOL_INFO)((PVOID*)Context)[1];
		//如果找到则将Context[0]设置为NULL
		((PVOID*)Context)[0] = NULL;
		//注意:以下一行代码可能不能将Name赋值给pSI_Type
		*pSI_Type = *pSI;
		return false;
	}
	return true;
}

//取得指定Type的信息
bool HySymGetTypeInfo(DWORD64 ModBase,const char *StructName,\
	OUT PSYMBOL_INFO pSI)
{
	bool bRet = false;
	if(!StructName || !pSI) goto QUIT;
	PVOID Context[2] = {(PVOID)StructName,(PVOID)pSI};
	
	if(!SymEnumTypes(GetCurrentProcess(),ModBase,EnumTypeProc,\
		(PVOID)Context))
	{
		PRINT("[%s]SymEnumTypes Failed!\n",__func__);
		goto QUIT;
	}
	//如果SymEnumTypes成功，且Context[0] == NULL,则表示找到了对应
	//的Type。
	if(!Context[0])
	{
		PRINT("[%s]msg : Find Type %s !Index is %d\n",__func__,\
			StructName,pSI->TypeIndex);
	}
	else
	{
		PRINT("[%s]err : Can't Find Type %s!\n",__func__,\
			StructName);
		goto QUIT;
	}
	bRet = true;
QUIT:
	return bRet;
}

//打印出指定结构的元素名称及偏移
bool HySymPrintStructElements(DWORD64 ModBase,const char *StructName)
{
	bool bRet = false;
	PSYMBOL_INFO pSI = malloc(sizeof(SYMBOL_INFO)+MAX_SYM_NAME);
	if(!pSI) goto QUIT;
	pSI->SizeOfStruct = sizeof(SYMBOL_INFO);
	pSI->MaxNameLen = MAX_SYM_NAME;
	pSI->ModBase = ModBase;
	
	if(!HySymGetTypeInfo(ModBase,StructName,pSI))
	{
		PRINT("[%s]err : Get Struct Info Failed!\n",__func__);
		goto QUIT;
	}
	else
	{
		PRINT("[%s]msg Struct TypeIndex is %d\n",__func__,\
			pSI->TypeIndex);
	}
	
	UINT ElementCount;
	PVOID pData = &ElementCount;
	if(!SymGetTypeInfo(GetCurrentProcess(),\
		ModBase,pSI->TypeIndex,TI_GET_CHILDRENCOUNT,pData))
	{
		PRINT("[%s]err : Get Type Element Count Failed!\n",\
			__func__);
		goto QUIT;
	}
	PRINT("[%s]msg : Element Count is %d\n",__func__,\
		ElementCount);
	TI_FINDCHILDREN_PARAMS *pCP = malloc(sizeof(ULONG)*(2+ElementCount));
	if(!pCP) goto QUIT;
	
	memset(pCP,'\0',sizeof(ULONG)*(2+ElementCount));
	pCP->Count = ElementCount;
	if(!SymGetTypeInfo(GetCurrentProcess(),ModBase,\
		pSI->TypeIndex,TI_FINDCHILDREN,pCP))
	{
		PRINT("[%s]err : Get Type Element Failed!\n",__func__);
		goto QUIT;
	}
	
	WCHAR *pNameW = NULL;
	for(int i = 0;i < ElementCount;++i)
	{
		PRINT("[%02d] TypeIndex is %d\n",i,pCP->ChildId[i]);
		if(SymGetTypeInfo(GetCurrentProcess(),ModBase,\
			pCP->ChildId[i],TI_GET_SYMNAME,&pNameW))
		{
			wprintf(L"Name is %s\n",pNameW);
			LocalFree(pNameW);
		}
		else
			PrintErr();
	}
	
	bRet = true;
QUIT:
	free(pCP);
	free(pSI);
	return bRet;
}




















