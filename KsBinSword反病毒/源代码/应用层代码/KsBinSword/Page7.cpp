// Page7.cpp : 实现文件
//

#include "stdafx.h"
#include "KsBinSword.h"
#include "Page7.h"


// CPage7 对话框

IMPLEMENT_DYNAMIC(CPage7, CDialog)

CPage7::CPage7(CWnd* pParent /*=NULL*/)
	: CDialog(CPage7::IDD, pParent)

{	


}

CPage7::~CPage7()
{
}

void CPage7::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Edit);
	DDX_Control(pDX, IDC_SLIDER1, m_slider);


	DDX_Control(pDX, IDC_CHECK1, m_address);
	DDX_Control(pDX, IDC_CHECK2, m_hex);
	DDX_Control(pDX, IDC_CHECK3, m_ascii);
	DDX_Control(pDX, IDC_CHECK4, m_48);
	DDX_Control(pDX, IDC_EDIT2,  m_Sector);
	DDX_Control(pDX, IDC_RADIO1, m_HexCheck);
	DDX_Control(pDX, IDC_EDIT3, m_FilePath);
	DDX_Control(pDX, IDC_EDIT4, m_PrintLCN);
}


BEGIN_MESSAGE_MAP(CPage7, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK1, &CPage7::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CPage7::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CPage7::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CPage7::OnBnClickedCheck4)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDOK, &CPage7::OnBnClickedReadSector)
	ON_BN_CLICKED(IDCANCEL, &CPage7::OnBnClickedWriteSector)
	ON_WM_DROPFILES()

	ON_EN_UPDATE(IDC_EDIT3, &CPage7::OnEnUpdateEdit3)
	ON_BN_CLICKED(IDOK2, &CPage7::OnBnClickedReadSectorByDrive)
	ON_BN_CLICKED(IDCANCEL2, &CPage7::OnBnClickedWriteSectorByDrive)
END_MESSAGE_MAP()


// CPage7 消息处理程序
HBRUSH CPage7::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);


	CBrush* brBkgnd = new CBrush();
	brBkgnd->CreateSolidBrush( RGB(250, 249, 250) );
	hbr = *brBkgnd;                


	return hbr;
}
BOOL CPage7::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_address.SetCheck(1)  ;
	m_hex.SetCheck(1)  ;
    m_ascii.SetCheck(1)  ;
	m_48.SetCheck(1)  ;

	m_slider.SetRange(1, 16);
	m_slider.SetPos(16);
    m_Edit.SetBPR(m_slider.GetPos());
	m_HexCheck.SetCheck(true);
	
	return true;
}

void CPage7::OnBnClickedCheck1()
{
	UpdateData(TRUE);

	m_Edit.SetOptions(m_address.GetCheck(), m_hex.GetCheck(), m_ascii.GetCheck(), m_48.GetCheck());
	m_Edit.RedrawWindow();
}

void CPage7::OnBnClickedCheck2()
{
	UpdateData(TRUE);

    m_Edit.SetOptions(m_address.GetCheck(), m_hex.GetCheck(), m_ascii.GetCheck(), m_48.GetCheck());

	m_Edit.RedrawWindow();
}

void CPage7::OnBnClickedCheck3()
{
	UpdateData(TRUE);
	m_Edit.SetOptions(m_address.GetCheck(), m_hex.GetCheck(), m_ascii.GetCheck(), m_48.GetCheck());
	m_Edit.RedrawWindow();
}

void CPage7::OnBnClickedCheck4()
{
	UpdateData(TRUE);
	m_Edit.SetOptions(m_address.GetCheck(), m_hex.GetCheck(), m_ascii.GetCheck(), m_48.GetCheck());
	m_Edit.RedrawWindow();

}
void CPage7::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	m_Edit.SetBPR(m_slider.GetPos());
	m_Edit.RedrawWindow();
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPage7::OnBnClickedReadSector()//读取硬盘，普通方式
{
	__int64 Sector;
	CString csStrSec;
	char cSource[16]={'\0'};
	BYTE OutBuf[0x200]={'\0'};
	m_Sector.GetWindowText(csStrSec);

	if (csStrSec.GetLength()>16)
	{
		AfxMessageBox(L"输入扇区数过大！请重新输入");
		return;
	}

	if (m_HexCheck.GetCheck())
	{
        Sector=CMySystem::HexToBigInt(csStrSec);			
	}
	else
	{

	    CMySystem::WCHARToChar(cSource,csStrSec.GetBuffer());
	    Sector=_atoi64(cSource);
	}
	CMySystem::ReadSector((Sector), OutBuf);

	m_Edit.SetData(OutBuf, 0x200);

	m_Edit.m_AddOff=(ULONG)(Sector)*512;

	m_Edit.RedrawWindow();

}

void CPage7::OnBnClickedWriteSector()//写入硬盘，普通方式
{
	__int64 Sector;
	CString csStrSec;
	char cSource[16]={'\0'};
	BYTE OutBuf[0x200]={'\0'};
	if (IDCANCEL==::MessageBox(0,L"确定输入吗？这样或许对系统造成损失！",0,1))
	{
         return;
	}
	m_Sector.GetWindowText(csStrSec);

	if (csStrSec.GetLength()>16)
	{
		AfxMessageBox(L"输入扇区数过大！请重新输入");
		return;
	}

	if (m_HexCheck.GetCheck())
	{
		Sector=CMySystem::HexToBigInt(csStrSec);			
	}
	else
	{

		CMySystem::WCHARToChar(cSource,csStrSec.GetBuffer());
		Sector=_atoi64(cSource);
	}
	CMySystem::WriteSector(Sector, m_Edit.m_pData);
	
}

void CPage7::OnBnClickedReadSectorByDrive()//驱动方式读取硬盘
{
	ULONG Sector;
	CString csStrSec;
	char cSource[16]={'\0'};
	BYTE OutBuf[0x200]={'\0'};
	m_Sector.GetWindowText(csStrSec);

	if (csStrSec.GetLength()>16)
	{
		AfxMessageBox(L"输入扇区数过大！请重新输入");
		return;
	}

	if (m_HexCheck.GetCheck())
	{
		Sector=CMySystem::HexToBigInt(csStrSec);			
	}
	else
	{

		CMySystem::WCHARToChar(cSource,csStrSec.GetBuffer());
		Sector=atoi(cSource);
	}
	//CMySystem::ReadSectorByDrive(Sector, OutBuf);
    CMySystem:: ReadSectorByDrive( CMySystem::hMyHandle, Sector, OutBuf);
	m_Edit.SetData(OutBuf, 0x200);

	m_Edit.m_AddOff=(ULONG)Sector*512;

	m_Edit.RedrawWindow();
}

void CPage7::OnBnClickedWriteSectorByDrive()//驱动方式写入硬盘
{
	ULONG Sector;
	CString csStrSec;
	char cSource[16]={'\0'};
	BYTE OutBuf[0x200]={'\0'};
	if (IDCANCEL==::MessageBox(0,L"确定输入吗？这样或许对系统造成损失！",0,1))
	{
		return;
	}
	m_Sector.GetWindowText(csStrSec);

	if (csStrSec.GetLength()>16)
	{
		AfxMessageBox(L"输入扇区数过大！请重新输入");
		return;
	}

	if (m_HexCheck.GetCheck())
	{
		Sector=CMySystem::HexToBigInt(csStrSec);			
	}
	else
	{

		CMySystem::WCHARToChar(cSource,csStrSec.GetBuffer());
		Sector=atoi(cSource);
	}
	//CMySystem::WriteSector(Sector, m_Edit.m_pData);
	CMySystem::WriteSectorByDrive(CMySystem:: hMyHandle,Sector,m_Edit.m_pData);
}

void CPage7::OnDropFiles(HDROP hDropInfo)
{
	// 被拖拽的文件的文件名
	WCHAR szFileName[MAX_PATH + 1];
	// 得到被拖拽的文件名
	DragQueryFile(hDropInfo, 0, szFileName, MAX_PATH);
	// 把文件名显示出来
	m_FilePath.SetWindowText(szFileName);

	CDialog::OnDropFiles(hDropInfo);
}
void CPage7::OnEnUpdateEdit3()//计算LCN
{
    CString csFileName;
	m_FilePath.GetWindowText(csFileName);

	HANDLE hFile;
//	DWORD dwSizeHigh;
//	LONG64 *SecArr;
//  BYTE*pReadBuf;
//	LONG64 Clu;//簇号
	LONG64 *CluArr;//存放簇号的数组
	LONG64 CluCount=0;//簇数量
	LONG64 LogicSec=0;//逻辑扇区号
	DWORD LogicOff=0;//逻辑扇区的偏移号,相当于文件内的第几个扇区
	LONG64 SecCount=0;
//	HANDLE hFileMapping;
	LONG64 qwFileSize = 0x4000000;
	LONG64 qwFileOffset = 0;
	int i,j,k;BYTE* OneSect=(BYTE*)malloc(sizeof(BYTE)*BytesPerSector_);
//	PBYTE pbFile;
	MEMORY_BASIC_INFORMATION mbi={0};   //内存信息
	DWORD Protect=0;
    PFNtFsControlFile NtFsControlFile ;
	BYTE NumSectInClu =8;
	DWORD status;

	WCHAR ErrName[MAX_PATH];
    CString csLCNList;
    CString csLCN;
	IO_STATUS_BLOCK ioStatus;
	ULONGLONG startVcn;
	PGET_RETRIEVAL_DESCRIPTOR fileMappings;
	int lines = 0;
//	int S_;
	DWORD dwReadSize = 0;
	ULONGLONG FileMap[FILEMAPSIZE];
	PLinkLcn pLinkLcn,pLinkLcn_,FisrtLink;
	FisrtLink=pLinkLcn = (PLinkLcn)malloc(sizeof(LinkLcn));
	pLinkLcn->Lcn=0;
	pLinkLcn->Len=0;
	if (!(NtFsControlFile = (PFNtFsControlFile)GetProcAddress(GetModuleHandle(L"ntdll.dll"), 
		"NtFsControlFile")))
	{
		AfxMessageBox(L"Could not find NtFsControlFile entry point in NTDLL.DLL\n");
		return;
	}
	csFileName=csFileName;

	fileMappings = (PGET_RETRIEVAL_DESCRIPTOR)FileMap;
	startVcn = 0;
	hFile = CreateFileW(csFileName.GetBuffer(),
		GENERIC_READ | GENERIC_WRITE,0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if ((HANDLE)-1==hFile)
    {
			m_PrintLCN.SetWindowText(L"文件打开失败！");
			return;
    }
	while (!(status = NtFsControlFile(hFile, NULL, NULL, 0, &ioStatus, 
		FSCTL_GET_RETRIEVAL_POINTERS, 
		&startVcn, sizeof(startVcn),
		fileMappings, FILEMAPSIZE *sizeof(LARGE_INTEGER))) || 
		status == STATUS_BUFFER_OVERFLOW || 
		status == STATUS_PENDING)
	{

		if (status == STATUS_PENDING)
		{
			WaitForSingleObject(hFile, INFINITE);
			if (ioStatus.Status != STATUS_SUCCESS &&ioStatus.Status != STATUS_BUFFER_OVERFLOW)
			{
				printf("Enumerate file clusters: ");
				
				wsprintf(ErrName,L"Err Num is :%x",ioStatus.Status);
				AfxMessageBox(ErrName);
				return ;
			}
		}
		startVcn = fileMappings->StartVcn;

		for (k = 0; k < (ULONGLONG)fileMappings->NumberOfPairs; k++)
		{

			if (fileMappings->Pair[k].Lcn == LLINVALID)
			{
				printf("VCN: %I64d VIRTUAL LEN: %I64d\n",
					startVcn, fileMappings->Pair[k].Vcn - startVcn);
				return ;
			}
			else
			{
				printf("   VCN: %I64x LCN: %I64x LEN: %I64x\n", 

					startVcn, 
					fileMappings->Pair[k].Lcn,
					fileMappings->Pair[k].Vcn - startVcn);

				pLinkLcn_= (PLinkLcn)malloc(sizeof(LinkLcn));
				pLinkLcn->Next=pLinkLcn_;pLinkLcn=pLinkLcn_;
				pLinkLcn_->Lcn=fileMappings->Pair[k].Lcn;
				pLinkLcn_->Len=fileMappings->Pair[k].Vcn - startVcn;

			}

			startVcn = fileMappings->Pair[k].Vcn;

		}
		if (!status)
			break;
	}
	pLinkLcn->Next=NULL;
	CloseHandle(hFile);
	pLinkLcn=FisrtLink;
	while((pLinkLcn=pLinkLcn->Next,pLinkLcn))
	{
		CluCount+=pLinkLcn->Len;
	}
	CluArr=(LONG64*)malloc(sizeof(LONG64)*(CluCount+1));
	pLinkLcn=FisrtLink;
	i=0;
	while((pLinkLcn_=pLinkLcn->Next,free(pLinkLcn),pLinkLcn=pLinkLcn_,pLinkLcn))
	{
		for (j=0;j<pLinkLcn->Len;j++)
		{

			CluArr[i++]=pLinkLcn->Lcn+(LONG64)j;
			printf("LCN:%x\n",pLinkLcn->Lcn+(LONG64)j);
			csLCN.Format(L"LCN:%x\n",pLinkLcn->Lcn+(LONG64)j);
			csLCNList+=csLCN+L"\r\n";
			m_PrintLCN.SetWindowText(csLCN);
		}

	}

}
