#include <stddef.h>
#include "dll.h"
#include "dllext.h"
#include "HyColorText.h"
#include "HyDbgHelp.h"

int strcmpi(const char*,const char*);
int getch(void);

static SvrHnds g_shs = {0};
//NT核心文件的全路径
static char g_KernelFilePath[MAX_PATH] = {0};
//NT核心文件的名称,不带后缀.exe
static char g_KernelFileName[32] = {0};
//DbgHelp Module函数加载NT核心文件的加载基地址
static DWORD64 g_KernelBaseLD = 0x0LLU;
//NT核心实际在系统运行时的基地址
static PVOID g_KernelBaseRT = NULL;
//运行时SSDT的地址
static PVOID g_pSSDT = NULL;
//运行时SSDT Shadow的地址
static PVOID g_pSSDTSdw = NULL;
//SSDT.Base和SSDTSdw.Base
static PVOID g_SSDTBases[2] = {NULL,NULL};
//Win32k.sys文件全路径
static char g_Win32kFilePath[MAX_PATH] = {0};
//Win32k.sys的不带后缀名称
static char *g_pWin32kFileName = "win32k";
//Win32k DgbHelp加载时基址
static DWORD64 g_Win32kBaseLD = 0x0LLU;
//Win32k系统运行时基址
static PVOID g_Win32kBaseRT = NULL;
//程序是否输出至标准控制台
bool g_bIsStdOut = false;

//等待用户输入任意键。如果输入esc键返回true，否则返回false。
bool WaitAnyKey(size_t i)
{
	//如果不是标准控制台输出，则不等待用户输入回车键。
	if(!g_bIsStdOut) return false;
	if(!(i%24) && i)
	{
		HyPrintColorText("press any key to continue...[ESC to QUIT]",\
			CLR_TEXT_BRIGHT|CLR_TEXT_GREEN);
		fflush(stdin);
		//getchar不能捕获ESC键，遂改用getch。但在输入类似F1,F2之类的键时
		//会捕获2次，待查。
		char c = getch();
		//printf("[%s]%02x\n",__func__,c);
		if(c == 0x0D) puts("");
		if(c == 0x1B) return true;
	}
	return false;
}

//将运行时内核中的地址转换为内核文件中的对应偏移地址。
inline PVOID GetKernelOffsetLD(PVOID AddrRT)
{
	return (PVOID)((ULONG)AddrRT - (ULONG)g_KernelBaseRT + (ULONG)g_KernelBaseLD);
}

//将内核文件中加载的地址转换为运行时的内核偏移地址。
inline PVOID GetKernelOffsetRT(PVOID AddrLD)
{
	return (PVOID)((ULONG)AddrLD - (ULONG)g_KernelBaseLD + (ULONG)g_KernelBaseRT);
}

static PVOID GetSysModListAddr(void)
{
	PVOID SysModListAddr = HyGetSymAddrRaw(g_KernelBaseLD,\
		"PsLoadedModuleList");
	if(!SysModListAddr)
	{
		PRINT("[%s]err : Get PsLoadedModuleList Addr Failed!\n",\
			__func__);
		goto QUIT;
	}
	//计算SSDTSdw实际运行时的偏移
	SysModListAddr = GetKernelOffsetRT(SysModListAddr);
QUIT:
	return SysModListAddr;
}

//从内核遍历系统模块列表,查看遍历结果需要开启DebugView
static void EnumSysMod(void)
{
	PVOID SysModListAddr = GetSysModListAddr();
	if(!SysModListAddr) goto QUIT;
	PRINT("[%s]SysModListAddr is 0X%08X\n",__func__,SysModListAddr);
	HyEnumSysMod(&g_shs,SysModListAddr);
QUIT:
	return;
}

//打印与MM有关的内核变量
static void HyPrintSysMmVal(void)
{
	HySymPrintStructElements(g_KernelBaseLD,"_MMSUPPORT");
	//PrintErr();
}

//打印系统内核变量的值
static void HyPrintSysVal(void)
{
	HyPrintSysMmVal();
}

//打印系统模块
static void HyPrintSysMod(void)
{
	HyGetRTModInfo("Print*",(PVOID)100);
}

//返回系统段类型
static const char *GetSysSegType(PGATE pGate)
{
	if(!pGate->P)
		return "---";
	
	//DT==0表示系统段,DT==1表示存储段
	if(pGate->DT)
		return "NOT SYSSEG!";
	
	switch(pGate->Type)
	{
	case 0:case 8:case 0xA:case 0xD:
	default:return "Unknow SysSeg";
	case 1:	return "Avail 286TSS";
	case 2:	return "LDT";
	case 3:	return "Busy 286TSS";
	case 4:	return "286 Call Gate";
	case 5:	return "Task Gate";
	case 6:	return "286 Interrupt Gate";
	case 7:	return "286 Trap Gate";
	case 9:	return "Avail 386TSS";
	case 0xB:return "Busy 386TSS";
	case 0xC:return "386 Call Gate";
	case 0xE:return "386 Interrupt Gate";
	case 0xF:return "386 Trap Gate";
	}
}

//打印系统IDT
static void HyPrintIDT(void)
{
	//在RING3下取IDT(sidt)在VMware中会出错!!!
	GATE Gates[256] = {{0}};
	HyGetIDT(&g_shs,Gates,sizeof(Gates));
	
	PSYMBOL_INFO pSI = malloc(sizeof(SYMBOL_INFO)+MAX_SYM_NAME);
	if(!pSI) goto QUIT;
	pSI->SizeOfStruct = sizeof(SYMBOL_INFO);
	pSI->MaxNameLen = MAX_SYM_NAME;
	
	puts("  NUM      OFFSET   DPL  P       TYPE             SYMBOL");
	for(size_t i = 0;i < 256;++i)
	{
		if(!HySymFromAddrRaw((DWORD64)(DWORD)GetKernelOffsetLD(\
			(PVOID)FULLOFFSET(Gates[i])),\
			pSI))
		{
			strcpy(pSI->Name,"<Unknow Symbol>");
		}
		
		printf("%04d[%02X] 0X%08X  %d   %-2s  %-18s  %-15s\n",i,\
			i,FULLOFFSET(Gates[i]),\
			Gates[i].DPL,Gates[i].P?"P":"NP",\
			GetSysSegType(&Gates[i]),pSI->Name);
		//如果用户输入esc键，则退出打印。
		if(WaitAnyKey(i)) break;
	}
QUIT:
	free(pSI);
}

//取得存储段的段界限
static DWORD GetSegLimit(PSEGMENT pSeg)
{
	DWORD dwRet = pSeg->G?0x1000:1;
	return dwRet * FULLLIMIT(*pSeg) + dwRet - 1;
}

//取得段类型
static const char *GetSegAttr(PSEGMENT pSeg)
{
	static char szRet[32];
	if(!pSeg->P)
		return "---";
	else if(!pSeg->DT)	//系统段
		return GetSysSegType((PGATE)pSeg);
	else
	{
		//存储段
		bool bIsCode = false;
		strcpy(szRet,"");
		if(pSeg->Type & 1)
			strcat(szRet,"A");		//已访问
		
		if(pSeg->Type & 1<<3)	
		{
			strcat(szRet," CS");	//代码段
			bIsCode = true;
		}
		else
			strcat(szRet," DS");	//数据段
		
		if(pSeg->Type & 1<<1 && bIsCode)	//是代码段且1位置位表示可读代码段
			strcat(szRet," R");
		else if(pSeg->Type & 1<<1 && !bIsCode)
			strcat(szRet," W");				//是数据段且1位置位表示可写数据段
		
		if(pSeg->Type & 1<<2 && bIsCode)
			strcat(szRet," C");		//一致代码段
		else if(pSeg->Type & 1<<2 && !bIsCode)
			strcat(szRet," H->L");	//数据段从高向低扩展
		return szRet;
	}
}

//打印系统GDT
static void HyPrintGDT(void)
{
	//在RING3下取GDT(sgdt)在VMware中会出错!!!
	SEGMENT Segs[128] = {{0}};
	HyGetGDT(&g_shs,Segs,sizeof(Segs));
	
	puts("  NUM       BASE       LIMIT   DPL P  BEAD TYPE    ATTRIBUTE");
	for(size_t i = 0;i < 128;++i)
	{
		printf("%04d[%02X] 0X%08X 0X%08X  %d  %s  %s  TYPE:%X  %s\n",i,\
			i,FULLBASE(Segs[i]),\
			GetSegLimit(&Segs[i]),Segs[i].DPL,Segs[i].P?"P":"NP",\
			Segs[i].G?"4KB":"1B",Segs[i].Type,\
			GetSegAttr(&Segs[i]));
		//如果用户输入esc键，则退出打印。
		if(WaitAnyKey(i)) break;
	}
}

//带颜色打印SSDT的每一个项目。
static void PrintSSDTItem(size_t id,PVOID OrgAddr,PVOID RealAddr,\
	const char *SvrName)
{
	char buf[256];
	
	if(!SvrName) return;
	printf("[%04d] 0x%08X ",id,OrgAddr);
	if(OrgAddr != RealAddr)
	{
		//若原始服务地址和实际服务地址不一致，则说明被HOOKED，则用
		//加亮红色醒目显示该服务项目。
		snprintf(buf,sizeof(buf),"*0x%08X  %s\n",(UINT)RealAddr,SvrName);
		//如果不是标准输出则 HyPrintColorText 不会实际输出到文件，遂换成
		//printf输出!
		if(g_bIsStdOut)
			HyPrintColorText(buf,CLR_TEXT_RED|CLR_TEXT_BRIGHT);
		else
			printf(buf);
	}
	else
		printf("0x%08X  %s\n",RealAddr,SvrName);
}

//返回PE文件中指定段的文件偏移地址。
//因为Win32k所支持的SSDTSdw表在Win32k.sys文件中的偏移地址
//就在其数据段的文件偏移0处。(2k,xp,2k3,win7)
//注意在某些版本的xp(sp2)下有问题，不排除第三方程序干扰的原因。
static PVOID GetPESectionOffset(FILE *pf,const char *SectionName)
{	
	PVOID RetDO = NULL;
	size_t SectionCount = 0;
	PVOID pNTHeader;
	IMAGE_NT_HEADERS ImgNTHeaders;
	
	if(!SectionName) return RetDO;
	//定位IMAGE_NT_HEADERS结构的位置信息
	fseek(pf,offsetof(IMAGE_DOS_HEADER,e_lfanew),SEEK_SET);
	//读取IMAGE_NT_HEADERS的位置
	fread(&pNTHeader,sizeof(long),1,pf);
	//将文件指针定位到IMAGE_NT_HEADERS的位置
	fseek(pf,(long)pNTHeader,SEEK_SET);
	//读取IMAGE_NT_HEADERS结构
	fread(&ImgNTHeaders,sizeof(IMAGE_NT_HEADERS),1,pf);
	//取得SECTION数目
	SectionCount = ImgNTHeaders.FileHeader.NumberOfSections;
	PRINT("[%s]msg : SectionCount is %d\n",__func__,SectionCount);
	
	//不需要显式fseek到SECTION TABLE，前面fread已经移动到正确的偏移了。
	//fseek(pf,(long)sizeof(IMAGE_NT_HEADERS),SEEK_CUR);
	IMAGE_SECTION_HEADER ImgSecHeaders[SectionCount];
	
	//读取SECTION数组
	fread(ImgSecHeaders,sizeof(ImgSecHeaders),1,pf);
	for(size_t i = 0;i<SectionCount;++i)
	{
		if(strstr(ImgSecHeaders[i].Name,SectionName))
		{
			RetDO = (PVOID)ImgSecHeaders[i].PointerToRawData;
			break;
		}
	}
	return RetDO;
}

//从Win32k.sys中获得原始SSDTSdw表中的项目。
static bool GetSSDTSdwOrg(PVOID buf,size_t size)
{
	bool bRet = false;
	if(!buf || !size) goto QUIT;
	FILE *pf = fopen(g_Win32kFilePath,"rb");
	if(!pf) goto QUIT;
	
	//获取Win32k.sys中data段的文件偏移,SSDTSdw就放在该偏移。
	PVOID SSDTSdwFileOffset = GetPESectionOffset(pf,".data");
	if(!SSDTSdwFileOffset) goto QUIT;
	
	fseek(pf,(long)SSDTSdwFileOffset,SEEK_SET);
	fread(buf,size,1,pf);
	bRet = true;
QUIT:
	if(pf) fclose(pf);
	return bRet;
}

//从内核文件中获得原始SSDT表中的项目。
static bool GetSSDTOrg(PVOID buf,size_t size)
{
	bool bRet = false;
	if(!buf || !size) goto QUIT;
	HANDLE hKernel = LoadLibrary(g_KernelFilePath);
	if(!hKernel) goto QUIT;
	
	//内核运行时SSDT的原始内容放在内核文件中，这里取得它的文件偏移。
	PVOID SSDTFileOffset = (PVOID)((ULONG)hKernel + (ULONG)g_SSDTBases[0] - \
		(ULONG)g_KernelBaseRT);
	
	memcpy(buf,SSDTFileOffset,size);
	bRet = true;
QUIT:
	if(hKernel) FreeLibrary(hKernel);
	return bRet;
}

//在控制台打印SSDT
static void HyPrintSSDT(bool bIsFixedOnly)
{
	static PVOID SSDT[512] = {0};
	static PVOID SSDTOrg[512] = {0};
	
	if(!HyGetSSDT(&g_shs,SSDT,sizeof(SSDT)))
	{
		PRINT("[%s]err : HyGetSSDT Failed!\n",__func__);
		goto QUIT;
	}
	
	if(!GetSSDTOrg(SSDTOrg,sizeof(SSDTOrg)))
		PRINT("[%s]err : GetSSDTOrg Failed!\n",__func__);
	
	PSYMBOL_INFO pSI = malloc(sizeof(SYMBOL_INFO)+MAX_SYM_NAME);
	if(!pSI) goto QUIT;
	pSI->SizeOfStruct = sizeof(SYMBOL_INFO);
	pSI->MaxNameLen = MAX_SYM_NAME;
	
	printf("  ID    ORG_ADDR  REAL_ADDR  SYMOL_NAME\n");   
	for(size_t i = 0;SSDT[i];++i)
	{
		if(!HySymFromAddrRaw((DWORD64)(DWORD)GetKernelOffsetLD(SSDT[i]),\
			pSI))
		{
			//如果SSDT已被HOOKED,则在SSDTOrg中找原始符号
			if(!HySymFromAddrRaw((DWORD64)(DWORD)SSDTOrg[i],pSI))
				strcpy(pSI->Name,"<Unknow Symbol>");
		}
		
		DWORD OrgAddr = (ULONG)SSDTOrg[i] - (ULONG)g_KernelBaseLD + \
			(ULONG)g_KernelBaseRT;
		if(!bIsFixedOnly || (PVOID)OrgAddr != SSDT[i])
		{
			PrintSSDTItem(i,(PVOID)OrgAddr,SSDT[i],pSI->Name);
			//如果用户输入esc键，则退出打印。
			if(WaitAnyKey(i)) break;
		}
	}	
QUIT:
	free(pSI);
	return;
}

//将运行时Win32k中的地址转换为Win32k.sys文件中的对应偏移地址。
inline PVOID GetWin32kOffsetLD(PVOID AddrRT)
{
	return (PVOID)((ULONG)AddrRT - (ULONG)g_Win32kBaseRT + (ULONG)g_Win32kBaseLD);
}

//在控制台打印SSDTSdw
static void HyPrintSSDTSdw(bool bIsFixedOnly)
{
	static PVOID SSDTSdw[1024] = {0};
	static PVOID SSDTSdwOrg[1024] = {0};
	if(!g_pSSDTSdw) goto QUIT;
	if(!HyGetSSDTSdw(&g_shs,g_pSSDTSdw,SSDTSdw,\
		sizeof(SSDTSdw)))
	{
		PRINT("[%s]err : HyGetSSDTSdw Failed!\n",__func__);
		goto QUIT;
	}
		
	if(!GetSSDTSdwOrg(SSDTSdwOrg,sizeof(SSDTSdwOrg)))
		PRINT("[%s]err : GetSSDTSdwOrg Failed!\n",__func__);
		
	PSYMBOL_INFO pSI = malloc(sizeof(SYMBOL_INFO)+MAX_SYM_NAME);
	if(!pSI) goto QUIT;
	pSI->SizeOfStruct = sizeof(SYMBOL_INFO);
	pSI->MaxNameLen = MAX_SYM_NAME;
	
	printf("  ID    ORG_ADDR  REAL_ADDR  SYMOL_NAME\n");
	for(size_t i = 0;SSDTSdw[i];++i)
	{
		if(!HySymFromAddrRaw((DWORD64)(DWORD)GetWin32kOffsetLD(SSDTSdw[i]),\
			pSI))
		{
			//如果SSDTSdw已被HOOKED,则在SSDTSdwOrg中找原始符号
			if(!HySymFromAddrRaw((DWORD64)(DWORD)SSDTSdwOrg[i],pSI))
				strcpy(pSI->Name,"<Unknow Symbol>");
		}
		
		DWORD OrgAddr = (ULONG)SSDTSdwOrg[i] - (ULONG)g_Win32kBaseLD + \
			(ULONG)g_Win32kBaseRT;
		if(!bIsFixedOnly || (PVOID)OrgAddr != SSDTSdw[i])
		{
			PrintSSDTItem(i,(PVOID)OrgAddr,SSDTSdw[i],pSI->Name);
			//如果用户输入esc键，则退出打印。
			if(WaitAnyKey(i)) break;
		}
	}
QUIT:
	free(pSI);
	return;
}

//取得内核文件以及Win32k文件路径信息
static void GetSysFilePath(void)
{
	char SysPath[MAX_PATH];
	GetSystemDirectory(SysPath,sizeof(SysPath));
	//如果内核开了PAE则内核文件名为ntkrnlpa,否则为ntoskrnl
	if(HyIsOpenPAE())
		strcpy(g_KernelFileName,"ntkrnlpa");
	else
		strcpy(g_KernelFileName,"ntoskrnl");
	
	//构造内核文件路径
	strcpy(g_KernelFilePath,SysPath);
	strcat(g_KernelFilePath,"\\");
	strcat(g_KernelFilePath,g_KernelFileName);
	strcat(g_KernelFilePath,".exe");
	PRINT("%s\n",g_KernelFilePath);
	
	//构造Win32k文件路径
	strcpy(g_Win32kFilePath,SysPath);
	strcat(g_Win32kFilePath,"\\");
	strcat(g_Win32kFilePath,g_pWin32kFileName);
	strcat(g_Win32kFilePath,".sys");
	PRINT("%s\n",g_Win32kFilePath);
}

//初始化内核信息,主要是载入内核文件的PDB信息
static bool InitKernelInfo(void)
{
	bool bRet = false;
	//载入内核文件的PDB
	if(!HyLoadModulePdb(g_KernelFilePath))
	{
		PRINT("[%s]err : Load NT Kernel Pdb Failed!\n",__func__);
		//goto QUIT;
	}
	//加载内核文件
	g_KernelBaseLD = HyLoadModule(g_KernelFilePath);
	if(!g_KernelBaseLD)
	{
		PRINT("[%s]err : Load NT Kernel File Failed!\n",__func__);
		goto QUIT;
	}
	PRINT("[%s]msg : NT Kernel LD Base : %I64x\n",__func__,g_KernelBaseLD);
	//打印内核文件信息
	HyPrintModuleInfo(g_KernelBaseLD);
	PrintErr();
	//取得实际内核文件
	if(!HyStoreModFile(g_KernelFilePath))
		PRINT("[%s]err : HyStoreModFile Failed!(%d)\n",__func__,\
			GetLastError());
	//取得运行时内核基址
	g_KernelBaseRT = HyGetRTModBase(g_KernelFileName);
	if(!g_KernelBaseRT)
	{
		PRINT("[%s]err : Get NT Kernel RT Base Failed!\n",__func__);
		goto QUIT;
	}
	PRINT("[%s]msg : NT Kernel RT Base : %p\n",__func__,g_KernelBaseRT);
	bRet = true;
QUIT:
	return bRet;
}

//初始化Win32k信息,主要是载入Win32k.sys文件的PDB信息
static bool InitWin32kInfo(void)
{
	bool bRet = false;
	//加载Win32k.sys的PDB
	if(!HyLoadModulePdb(g_Win32kFilePath))
	{
		PRINT("[%s]err : Load NT Win32k Pdb Failed!\n",__func__);
		//goto QUIT;
	}
	//加载Win32k.sys
	g_Win32kBaseLD = HyLoadModule(g_Win32kFilePath);
	if(!g_Win32kBaseLD)
	{
		PRINT("[%s]err : Load NT Win32k	File Failed!\n",__func__);
		goto QUIT;
	}
	PRINT("[%s]msg : NT Win32k LD Base : %I64x\n",__func__,g_Win32kBaseLD);
	//打印Win32k.sys文件信息
	HyPrintModuleInfo(g_Win32kBaseLD);
	PrintErr();
	//取得实际Win32k.sys文件
	if(!HyStoreModFile(g_Win32kFilePath))
		PRINT("[%s]err : HyStoreModFile Failed!(%d)\n",__func__,\
			GetLastError());
	//取得运行时Win32k.sys基址
	g_Win32kBaseRT = HyGetRTModBase(g_pWin32kFileName);
	if(!g_Win32kBaseRT)
	{
		PRINT("[%s]err : Get NT Win32k RT Base Failed!\n",__func__);
		goto QUIT;
	}
	PRINT("[%s]msg : NT Win32k RT Base : %p\n",__func__,g_Win32kBaseRT);
	bRet = true;
QUIT:
	return bRet;
}

//在Ring3中取得SSDTSdw的地址,函数失败返回NULL。
static PVOID GetSSDTSdwAddr(void)
{
	//取得SSDTSdw的Raw偏移
	PVOID pSSDTSdw = HyGetSymAddrRaw(g_KernelBaseLD,\
		"KeServiceDescriptorTableShadow");
	if(!pSSDTSdw)
	{
		PRINT("[%s]err : Get KeServiceDescriptorTableShadow Addr Failed!\n",\
			__func__);
		goto QUIT;
	}
	//计算SSDTSdw实际运行时的偏移
	pSSDTSdw = (PVOID)((ULONG)pSSDTSdw - (ULONG)g_KernelBaseLD + \
		(ULONG)g_KernelBaseRT);
QUIT:
	return pSSDTSdw;
}

//在Ring3中取得SSDT的地址，失败返回NULL。
static PVOID GetSSDTAddrAtRing3(void)
{
	PVOID RetAddr = NULL;
	HANDLE hKernel = LoadLibrary(g_KernelFilePath);
	if(!hKernel)
	{
		PRINT("[%s]LoadLibrary Failed!\n",__func__);
		goto QUIT;
	}
	//取得SSDT在内核文件中的偏移
	RetAddr = GetProcAddress(hKernel,"KeServiceDescriptorTable");
	if(!RetAddr)
	{
		PRINT("[%s]Get KeServiceDescriptorTable Addr Failed!\n",__func__);
		goto QUIT;
	}
	//计算SSDT在运行时的偏移
	RetAddr = (PVOID)((ULONG)RetAddr - (ULONG)hKernel + (ULONG)g_KernelBaseRT);
QUIT:
	if(hKernel) FreeLibrary(hKernel);
	return RetAddr;
}

//取得运行时SSDT.Base和SSDTSdw.Base
static bool GetSSDTBases(void)
{
	if(!HyGetSSDTBases(&g_shs,g_SSDTBases,sizeof(g_SSDTBases)))
	{
		PRINT("[%s]err : HyGetSSDTBases Failed!\n",__func__);
		return false;
	}
	PRINT("[%s]msg SSDT.Base : %p , SSDTSdw.Base : %p\n",\
		__func__,g_SSDTBases[0],g_SSDTBases[1]);
	return true;
}

//EXE的初始例程
static bool init(void)
{
	bool bRet = false;
	//创建服务
	if(!NewSvr(&g_shs))
	{
		PRINT("[%s]err : NewSvr Failed!\n",__func__);
		goto QUIT;
	}
	//启动服务
	if(!StartSvr(&g_shs))
	{
		PRINT("[%s]err : StartSvr Failed!\n",__func__);
		goto QUIT;
	}
	//刷新内核模块信息
	HyFlushSysModsInfo();
	//取得内核和Win32k文件路径
	GetSysFilePath();
	//初始化DbgHelp运行资源
	if(!HyInitDbgHelp(NULL))
	{
		PRINT("[%s]err : InitDbgHelp Failed!\n",__func__);
		goto QUIT;
	}
	//初始化内核信息
	if(!InitKernelInfo()) goto QUIT;
	//初始化Win32k信息
	if(!InitWin32kInfo()) goto QUIT;
	
	//从Ring3获得SSDT表RT基址
	g_pSSDT = GetSSDTAddrAtRing3();
	if(!g_pSSDT) goto QUIT;
	PRINT("[%s]msg : SSDT Get At Ring3 is : %p\n",__func__,g_pSSDT);
	
	//从Ring3获得SSDT Shadow表基址
	g_pSSDTSdw = GetSSDTSdwAddr();
	if(!g_pSSDTSdw) goto QUIT;
	PRINT("[%s]msg : SSDTSdw Get At Ring3 is : %p\n",__func__,g_pSSDTSdw);
	BYTE Dummy[1331];
	//只设置sysext中SSDTSdw的地址！
	HyGetSSDTSdw(&g_shs,g_pSSDTSdw,&Dummy,sizeof(Dummy));
	PrintErr();
	
	if(!GetSSDTBases())
	{
		PRINT("[%s]GetSSDTBases Failed!\n",__func__);goto QUIT;
		goto QUIT;
	}
	bRet = true;
QUIT:
	return bRet;
}

//EXE的清除例程
static void clear(void)
{
	//清除系统模块信息
	HyClearSysModsInfo();
	//如果有必要，分别卸载内核文件和Win32k.sys文件
	if(g_KernelBaseLD)
	{
		if(!HyUnloadModule(g_KernelBaseLD))
			PRINT("[%s]err : Unload Kernel Failed!\n",__func__);
	}
	else g_KernelBaseLD = 0x0LLU;
	
	if(g_Win32kBaseLD)
	{
		if(!HyUnloadModule(g_Win32kBaseLD))
			PRINT("[%s]err : Unload Win32k Failed!\n",__func__);
	}
	else g_Win32kBaseLD = 0x0LLU;
	//清除DbgHelp运行资源
	HyClearDbgHelp();
	//卸载服务
	if(!DelSvr(&g_shs))
	{
		PRINT("[%s]err : DelSvr Failed!\n",__func__);
		PRINT("[%s]Try To Force Del The Drv!\n",__func__);
		DelSvrForce();
	}
}

//打印使用帮助
static void usage(void)
{
	HyPrintColorText("NtInfoGuy! Code By 大熊猫侯佩|panda hopy 2010 v1.0\n",\
		CLR_TEXT_YELLOW|CLR_TEXT_BRIGHT);
	HyPrintColorText("熊猫的blog : http://blog.csdn.net/mydo\n",\
		CLR_TEXT_RED|CLR_TEXT_BRIGHT);
	puts("\nNtInfoGuy SSDT|SSDTSDW|GDT|IDT|SYSMOD[K]|SYSVAL [fixed]");
	puts("SSDT \t: 打印系统服务表");
	puts("SSDTSDW : 打印系统服务Shadow表");
	puts("GDT \t: 打印全局描述符表");
	puts("IDT \t: 打印中断描述符表");
	puts("SYSMOD \t: 打印系统加载模块信息");
	puts("SYSMODK : 打印系统加载模块信息(由RING0取得,在DbgView中显示)");
	puts("SYSVAL \t: 打印系统内核变量的值");
	puts("fixed \t: 只打印更改过的内容");
}

int main(int argc,char *argv[])
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	//检查程序是否是输出到标准控制台输出.
	//防止用户在使用诸如 NtInfoGuy ssdt>a.txt 重定位的情况下
	//WaitAnyKey会等待用户输入。
	g_bIsStdOut = (int)hOut == CONSOLE_STDOUT ;	//7:标准控制台输出
	
	if(!init())
	{
		printf("[%s]err : init failed!\n",__func__);
		goto QUIT;
	}
	
	if(argc > 3 || argc == 1)
	{
		usage();goto QUIT;
	}
	
	if(!strcmpi(argv[1],"SSDT"))
		HyPrintSSDT(argc==3&&!strcmpi(argv[2],"FIXED"));
	else if(!strcmpi(argv[1],"SSDTSDW"))
		HyPrintSSDTSdw(argc==3&&!strcmpi(argv[2],"FIXED"));
	else if(!strcmpi(argv[1],"GDT"))
		HyPrintGDT();
	else if(!strcmpi(argv[1],"IDT"))
		HyPrintIDT();
	else if(!strcmpi(argv[1],"SYSMOD"))
		HyPrintSysMod();
	else if(!strcmpi(argv[1],"SYSMODK"))
		EnumSysMod();
	else if(!strcmpi(argv[1],"SYSVAL"))
		HyPrintSysVal();
	else
		usage();
QUIT:
	clear();
	//getchar();
	return 0;
}
