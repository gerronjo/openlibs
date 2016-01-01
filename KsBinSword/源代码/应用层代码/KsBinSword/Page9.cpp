// Page9.cpp : 实现文件
//

#include "stdafx.h"
#include "KsBinSword.h"
#include "Page9.h"
#pragma warning( disable: 4311 )
#pragma warning( disable: 4312 )
#pragma warning( disable: 4996 )


// CPage9 对话框

IMPLEMENT_DYNAMIC(CPage9, CDialog)

CPage9::CPage9(CWnd* pParent /*=NULL*/)
: CDialog(CPage9::IDD, pParent)
{

}

CPage9::~CPage9()
{
}

void CPage9::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_PePrint);
	DDX_Control(pDX, IDC_EDIT2, m_PEPath);
}


BEGIN_MESSAGE_MAP(CPage9, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDOK, &CPage9::OnBnClickedOk)
END_MESSAGE_MAP()

HBRUSH CPage9::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);


	CBrush* brBkgnd = new CBrush();
	brBkgnd->CreateSolidBrush( RGB(250, 249, 250) );
	hbr = *brBkgnd;                // Control bkgnd


	return hbr;
}
// CPage9 消息处理程序

BOOL CPage9::LoadFile(LPTSTR lpFilename,PMAP_FILE_STRUCT pstMapFile)
{

	HANDLE hFile;
	HANDLE hMapping;
	LPVOID ImageBase;

	memset(pstMapFile,0,sizeof(MAP_FILE_STRUCT));

	hFile=CreateFile(lpFilename,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL,0);

	if (!hFile)       
		return FALSE;

	hMapping=CreateFileMapping(hFile,NULL,PAGE_READONLY,0,0,NULL);
	if(!hMapping)
	{         
		CloseHandle(hFile);
		return FALSE;
	}
	ImageBase=MapViewOfFile(hMapping,FILE_MAP_READ,0,0,0);
	if(!ImageBase)
	{         
		CloseHandle(hMapping);
		CloseHandle(hFile);
		return FALSE;
	}
	pstMapFile->hFile=hFile;
	pstMapFile->hMapping=hMapping;
	pstMapFile->ImageBase=ImageBase;
	return TRUE;
}

void CPage9::UnLoadFile(PMAP_FILE_STRUCT pstMapFile)
{
	if(pstMapFile->ImageBase)
		UnmapViewOfFile(pstMapFile->ImageBase);

	if(pstMapFile->hMapping)
		CloseHandle(pstMapFile->hMapping);

	if(pstMapFile->hFile)
		CloseHandle(pstMapFile->hFile);

}


BOOL CPage9::IsPEFile(LPVOID ImageBase)
{
	PIMAGE_DOS_HEADER pDH=NULL;
	PIMAGE_NT_HEADERS pNtH=NULL;

	if(!ImageBase)
		return FALSE;

	pDH=(PIMAGE_DOS_HEADER)ImageBase;
	if(pDH->e_magic!=IMAGE_DOS_SIGNATURE)
		return FALSE;

	pNtH=(PIMAGE_NT_HEADERS32)((DWORD)pDH+pDH->e_lfanew);
	if (pNtH->Signature != IMAGE_NT_SIGNATURE )
		return FALSE;

	return TRUE;

}

PIMAGE_NT_HEADERS CPage9::GetNtHeaders(LPVOID ImageBase)
{

	if(!IsPEFile(ImageBase))
		return NULL;
	PIMAGE_NT_HEADERS pNtH;
	PIMAGE_DOS_HEADER pDH;
	pDH=(PIMAGE_DOS_HEADER)ImageBase;
	pNtH=(PIMAGE_NT_HEADERS)((DWORD)pDH+pDH->e_lfanew);

	return pNtH;

}

PIMAGE_FILE_HEADER   CPage9::GetFileHeader(LPVOID ImageBase)
{      
	PIMAGE_NT_HEADERS pNtH;
	PIMAGE_FILE_HEADER pFH=NULL;
	pNtH=GetNtHeaders(ImageBase);
	pFH=&pNtH->FileHeader;
	return pFH;
}

PIMAGE_OPTIONAL_HEADER CPage9::GetOptionalHeader(LPVOID ImageBase)
{
	PIMAGE_OPTIONAL_HEADER pOH=NULL;

	pOH=&(GetNtHeaders(ImageBase))->OptionalHeader;
	return pOH;
}

PIMAGE_SECTION_HEADER CPage9::GetFirstSectionHeader(LPVOID ImageBase)
{
	PIMAGE_NT_HEADERS     pNtH=NULL;
	PIMAGE_SECTION_HEADER pSH=NULL;

	pNtH=GetNtHeaders(ImageBase);
	pSH=IMAGE_FIRST_SECTION(pNtH);
	return pSH;
}

LPVOID CPage9::RvaToPtr(PIMAGE_NT_HEADERS pNtH,LPVOID ImageBase,DWORD dwRVA)
{ 
	return ::ImageRvaToVa(pNtH,ImageBase,dwRVA,NULL);

}

LPVOID CPage9::GetDirectoryEntryToData(LPVOID ImageBase,USHORT DirectoryEntry)
{
	DWORD dwDataStartRVA;
	LPVOID pDirData=NULL;
	PIMAGE_NT_HEADERS     pNtH=NULL;
	PIMAGE_OPTIONAL_HEADER pOH=NULL;

	pNtH=GetNtHeaders(ImageBase);
	if(!pNtH)
		return NULL;
	pOH=GetOptionalHeader(ImageBase);//有点问题！
	if(!pOH)
		return NULL;
	dwDataStartRVA=pOH->DataDirectory[DirectoryEntry].VirtualAddress;
	if(!dwDataStartRVA)
		return NULL;

	pDirData=RvaToPtr(pNtH,ImageBase,dwDataStartRVA);
	if(!pDirData)
		return NULL; 
	return pDirData;
}




void CPage9::PrintFileHeader(PIMAGE_FILE_HEADER pFileHeader)
{
	tm *createTime;
	switch(pFileHeader->Machine)
	{
	case 0X14C:
		PrintEditTem.Format(L"可执行文件的目标CPU为 Intel i386\r\n");PrintEdit+=PrintEditTem;
		break;
	case 0X162:
		PrintEditTem.Format(L"可执行文件的目标CPU为 MIPS R3000\r\n");PrintEdit+=PrintEditTem;
		break;
	case 0x166:
		PrintEditTem.Format(L"可执行文件的目标CPU为 MIPS R4000\r\n");PrintEdit+=PrintEditTem;
		break;
	case 0x184:
		PrintEditTem.Format(L"可执行文件的目标CPU为 Alpha AXP\r\n");PrintEdit+=PrintEditTem;
		break;
	case 0x1F0:
		PrintEditTem.Format(L"可执行文件的目标CPU为 Power PC\r\n");PrintEdit+=PrintEditTem;
		break;
	default:PrintEditTem.Format(L"可执行文件的目标CPU未知\r\n");

	}
	PrintEditTem.Format(L"文件的区块数目:%d\r\n",pFileHeader->NumberOfSections);PrintEdit+=PrintEditTem;
	createTime=gmtime((time_t *)&pFileHeader->TimeDateStamp);
	WCHAR wBuf[50];
	char  cBuf[50];
	strcpy(cBuf,asctime(createTime));
	CMySystem::CharToWCHAR(wBuf,cBuf);
	PrintEditTem.Format(L"文件的创建时间GMT is: %ws\r\n", wBuf);

	PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"IAMGE_OPTIONAL_HEADER32结构大小:%d\r\n",pFileHeader->SizeOfOptionalHeader);PrintEdit+=PrintEditTem;
	if(pFileHeader->Characteristics==0x1000)
		PrintEditTem.Format(L"该文件为系统文件\r\n");PrintEdit+=PrintEditTem;
	if(pFileHeader->Characteristics==0x2000)
		PrintEditTem.Format(L"该文件为DLL文件\r\n");PrintEdit+=PrintEditTem;
}

void CPage9::PrintOptionAlHeader(PIMAGE_OPTIONAL_HEADER pNt)
{
	PrintEditTem.Format(L"\r\n连接器的主版本号:%d,次版本号:%d\r\n",pNt->MajorLinkerVersion,pNt->MinorLinkerVersion);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"所有含有代码区块的总大小%ld\r\n",pNt->SizeOfCode);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"所有初始化数据区块的总大小%ld\r\n",pNt->SizeOfInitializedData);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"所有未初始化数据区块的总大小%ld\r\n",pNt->SizeOfUninitializedData);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"程序执行入口RAV(AddressOfEntryPoint):%X\r\n",pNt->AddressOfEntryPoint);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"代码区块起始RVA(BaseOfCode):%X\r\n",pNt->BaseOfCode);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"数据区块起始RVA(BaseOfData):%X\r\n",pNt->BaseOfData);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"数据默认装入基地址(ImageBase):%X\r\n",pNt->ImageBase);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"操作系统主版本号%d\r\n",pNt->MajorOperatingSystemVersion);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"操作系统次版本号%d\r\n",pNt->MinorOperatingSystemVersion);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"所需要的的子系统主版本号%d\r\n",pNt->MajorSubsystemVersion);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"所需要的的子系统次版本号%d\r\n",pNt->MinorSubsystemVersion);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"映像装入内存后的总尺寸(SizeOfIamge):%d\r\n",pNt->SizeOfImage);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"DOS头,PE头,区块表总大小%d\r\n",pNt->SizeOfHeaders);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"映像效验和%ld\r\n",pNt->CheckSum);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"文件子系统%d\r\n",pNt->Subsystem);PrintEdit+=PrintEditTem;
	PrintEditTem.Format(L"数据目录表的项数%d\r\n",pNt->NumberOfRvaAndSizes);PrintEdit+=PrintEditTem;

}

void CPage9::PrintSectionInfo()
{
	PIMAGE_FILE_HEADER       pFH=NULL;
	PIMAGE_SECTION_HEADER   pSH=NULL;
	WCHAR wBuf[50];
	char  cBuf[50];
	pFH=GetFileHeader(MapFile.ImageBase);
	if(!pFH)
		return;
	pSH=GetFirstSectionHeader(MapFile.ImageBase);
	PrintEditTem.Format(L"\r\nPE文件的区块信息:\r\n");PrintEdit+=PrintEditTem;
	for(int i=0;i<pFH->NumberOfSections;i++)
	{

		PrintEditTem.Format(L"输入模块:%ws\r\n",wBuf);
		strcpy(cBuf,(char *)pSH->Name);
		CMySystem::CharToWCHAR(wBuf,cBuf);
		PrintEditTem.Format(L"段名:%ws\t",wBuf);PrintEdit+=PrintEditTem;

		PrintEditTem.Format(L"VirtualSize:%x\t",pSH->Misc);PrintEdit+=PrintEditTem;
		PrintEditTem.Format(L"VirtualAddress:%x\t",pSH->VirtualAddress);PrintEdit+=PrintEditTem;
		PrintEditTem.Format(L"SizeOfRawData:%x\t",pSH->SizeOfRawData);PrintEdit+=PrintEditTem;
		PrintEditTem.Format(L"SizeOfRawData:%x\t",pSH->SizeOfRawData);PrintEdit+=PrintEditTem;
		PrintEditTem.Format(L"文件属性(Characteristics):%X\r\n",pSH->Characteristics);PrintEdit+=PrintEditTem;
		pSH++;
	}
}

void CPage9::printIAT()
{
	//ULONG size;
	WCHAR wBuf[60];
	char  cBuf[60];
	PIMAGE_IMPORT_DESCRIPTOR pImport;
	pImport = (IMAGE_IMPORT_DESCRIPTOR *)GetDirectoryEntryToData(MapFile.ImageBase,IMAGE_DIRECTORY_ENTRY_IMPORT);
	if (!pImport)
	{
		PrintEditTem.Format(L"不存在输入模块！\r\n");PrintEdit+=PrintEditTem;
		return;
	}

	//pImport=(PIMAGE_IMPORT_DESCRIPTOR)((BYTE *)MapFile.ImageBase+GetOptionalHeader(MapFile.ImageBase)->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	while(pImport->FirstThunk)
	{
		char *pszDllName=(char *)RvaToPtr(GetNtHeaders(MapFile.ImageBase),MapFile.ImageBase,pImport->Name);//(char *)((BYTE *)MapFile.ImageBase+pImport->Name);
		strcpy(cBuf,pszDllName);
		CMySystem::CharToWCHAR(wBuf,cBuf);
		PrintEditTem.Format(L"输入模块:%ws\r\n",wBuf);
		PrintEdit+=PrintEditTem;
		IMAGE_THUNK_DATA * pThunk=(IMAGE_THUNK_DATA *)(::ImageRvaToVa(GetNtHeaders(MapFile.ImageBase),MapFile.ImageBase,pImport->OriginalFirstThunk,NULL));
		int n=0;
		if (NULL==pThunk)
		{
			return;
		}
		while(pThunk->u1.Function)
		{
			char * pszFunName=(char *)(::ImageRvaToVa((PIMAGE_NT_HEADERS)GetNtHeaders(MapFile.ImageBase),MapFile.ImageBase,(unsigned long)pThunk->u1.AddressOfData,NULL));
			PDWORD lpAddr=(DWORD*)(::ImageRvaToVa(GetNtHeaders(MapFile.ImageBase),MapFile.ImageBase,pImport->FirstThunk,NULL))+n;//((BYTE *)MapFile.ImageBase+pImport->FirstThunk)+n;
			if (NULL==pszFunName)
			{
				break;
			}
			strcpy(cBuf,pszFunName+2);
		    CMySystem::CharToWCHAR(wBuf,cBuf);
			PrintEditTem.Format(L"从此模块导入的函数:%-25ws,",wBuf);
			PrintEdit+=PrintEditTem;
			PrintEditTem.Format(L"函数地址:%X\r\n",lpAddr);PrintEdit+=PrintEditTem;
			n++;
			pThunk++;
		}
		pImport++;
	}
}


void CPage9::printET()
{
	WCHAR wBuf[60];
	char  cBuf[60];
	IMAGE_EXPORT_DIRECTORY * pExport;
	pExport = (IMAGE_EXPORT_DIRECTORY *)GetDirectoryEntryToData(MapFile.ImageBase,IMAGE_DIRECTORY_ENTRY_EXPORT);
	if (!pExport)
	{
		return;
	}
	strcpy(cBuf,(char *)::ImageRvaToVa(GetNtHeaders(MapFile.ImageBase),MapFile.ImageBase,pExport->Name,NULL));
	CMySystem::CharToWCHAR(wBuf,cBuf);

	PrintEditTem.Format(L"模块的真实名称%ws\r\n",wBuf);PrintEdit+=PrintEditTem;
	int n;
	n=pExport->NumberOfFunctions;
	PrintEditTem.Format(L"模块中函数个数:%d\r\n",n);PrintEdit+=PrintEditTem;
	DWORD *AddressF=(DWORD *)::ImageRvaToVa(GetNtHeaders(MapFile.ImageBase),MapFile.ImageBase,pExport->AddressOfFunctions,NULL);
	DWORD *AddressName=(DWORD *)::ImageRvaToVa(GetNtHeaders(MapFile.ImageBase),MapFile.ImageBase,pExport->AddressOfNames,NULL);
	WORD *AddressNameO=(WORD *)::ImageRvaToVa(GetNtHeaders(MapFile.ImageBase),MapFile.ImageBase,pExport->AddressOfNameOrdinals,NULL);
	for(int i=0;i<n;i++)
	{
		strcpy(cBuf,(char *)ImageRvaToVa(GetNtHeaders(MapFile.ImageBase),MapFile.ImageBase,*AddressName,NULL));
		CMySystem::CharToWCHAR(wBuf,cBuf);
		
		PrintEditTem.Format(L"函数的名称:%-30s,地址:%-4x,顺列号:%d\r\n",wBuf,*AddressF,*AddressNameO);PrintEdit+=PrintEditTem;
		AddressName++;
		AddressF++;
		AddressNameO++;
	}

}
void CPage9::OnDropFiles(HDROP hDropInfo)
{
	// 被拖拽的文件的文件名
	WCHAR szFileName[MAX_PATH + 1];
	// 得到被拖拽的文件名
	DragQueryFile(hDropInfo, 0, szFileName, MAX_PATH);
	// 把文件名显示出来
	m_PEPath.SetWindowText(szFileName);

	CDialog::OnDropFiles(hDropInfo);
}
void CPage9::OnBnClickedOk()
{
	
	LPTSTR fileName=L"C:\\Documents and Settings\\Administrator\\桌面\\编译仓库\\Debug\\DecodeSystemBuffer.exe";
    CString csPath;
	m_PEPath.GetWindowText(csPath);
	m_PePrint.SetWindowText(L"");
	PrintEdit.Empty();
	LoadFile(csPath.GetBuffer(),&MapFile);
	if(!IsPEFile(MapFile.ImageBase))
	{
		PrintEditTem.Format(L"PE文件信息获取失败。请确保是PE文件，且文件未被使用！\r\n");PrintEdit+=PrintEditTem;
		goto _exit ;
	}
	PrintFileHeader(GetFileHeader(MapFile.ImageBase));
	PrintOptionAlHeader(GetOptionalHeader(MapFile.ImageBase));
	PrintSectionInfo();
	printET();
	printIAT();
	getchar();
	UnLoadFile(&MapFile);

	
	
_exit:
	m_PePrint.SetWindowText(PrintEdit);
    return ;

}


