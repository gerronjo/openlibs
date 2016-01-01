// Page8.cpp : 实现文件
//

#include "stdafx.h"
#include "KsBinSword.h"
#include "Page8.h"
#include "MySystem.h"
#include "sockUtil.cpp"
#include "RuleDlg.h"


IMPLEMENT_DYNAMIC(CPage8, CDialog)

CPage8::CPage8(CWnd* pParent /*=NULL*/)
	: CDialog(CPage8::IDD, pParent)
{

}

CPage8::~CPage8()
{
}

void CPage8::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_rules);
	DDX_Control(pDX, IDC_BUTTON10, m_Monitor);
	DDX_Control(pDX, IDC_BUTTON9, m_StopMonitor);

	DDX_Control(pDX, IDC_EDIT2, m_NetData);
}


BEGIN_MESSAGE_MAP(CPage8, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, &CPage8::OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON4, &CPage8::OnButtonDel)
	ON_BN_CLICKED(IDC_BUTTON3, &CPage8::OnButtoninStall)
	ON_BN_CLICKED(IDC_BUTTON8, &CPage8::OnLoadRules)
	ON_BN_CLICKED(IDC_BUTTON7, &CPage8::OnSaveRules)
	ON_BN_CLICKED(IDC_BUTTON6, &CPage8::OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON5, &CPage8::OnButtonStop)
	ON_BN_CLICKED(IDC_BUTTON1, &CPage8::OnButtonDesinStall)
	ON_BN_CLICKED(IDC_BUTTON10, &CPage8::OnBnClickedMonitor)
	ON_BN_CLICKED(IDC_BUTTON9, &CPage8::OnBnClickedStopMonitor)
	ON_BN_CLICKED(IDC_BUTTON11, &CPage8::OnBnClickedClearMessage)
END_MESSAGE_MAP()

BOOL CPage8::OnInitDialog()
{
	CDialog::OnInitDialog();
//	GetParentFrame()->RecalcLayout();
	//ResizeParentToFit();

	RECT rc;
	m_rules.GetClientRect(&rc);

	int width=rc.right-rc.left;//-110
	m_rules.InsertColumn(0, L" 源 Ip",LVCFMT_LEFT , width/8, 0);
	m_rules.InsertColumn(1, L" 源 Mask",LVCFMT_LEFT , width/8, 1);
	m_rules.InsertColumn(2, L" 源端口",LVCFMT_LEFT ,width/8, 2);
	m_rules.InsertColumn(3, L" 目标 Ip",LVCFMT_LEFT , width/8, 3);
	m_rules.InsertColumn(4, L" 目标 Mask",LVCFMT_LEFT , width/8, 4);
	m_rules.InsertColumn(5, L" 目标 Port",LVCFMT_LEFT , width/8, 5);
	m_rules.InsertColumn(6, L" 协议",LVCFMT_LEFT , width/8, 6);
	m_rules.InsertColumn(7, L" 行为规则",LVCFMT_LEFT ,  width/8, 7);

	m_rules.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);


	GetDlgItem( IDC_BUTTON6 )->EnableWindow(TRUE);//开始
	GetDlgItem( IDC_BUTTON5 )->EnableWindow(FALSE);
	m_Monitor.EnableWindow(TRUE);
	m_StopMonitor.EnableWindow(FALSE);
	started = FALSE;
    nRules = 0;
	memset(g_OutArr,0,MAXPRINT*sizeof(WCHAR)*MAXSTR);
	CMySystem:: LoadDriver(L"DrvFltIp");
_Again:
	filterDriver.OpenDevice(L"\\\\.\\DrvFltIp");

	if ((HANDLE)-1==filterDriver.driverHandle||(HANDLE)NULL==filterDriver.driverHandle)
	{
		   //CString csErrCode;
		   //csErrCode.Format(L"加载DrvFltIp 失败，错误码:%d\r\n",GetLastError());
		   if(IDYES==::MessageBox(0, L"系统繁忙，加载防火墙驱动DrvFltIp 失败,需要重新加载吗？\n如果不加载，某些功能将无法使用！",L"警告",MB_YESNO))
		   {
			   CMySystem:: UnloadDriver(L"DrvFltIp");
			   CMySystem:: LoadDriver(L"DrvFltIp");
		       goto _Again;
		   }
		   //AfxMessageBox(csErrCode);
		   return false;
		
	}

	hModulentdll = LoadLibraryW (L"ws2_32.dll");
	inet_ntoa=(pinet_ntoa)GetProcAddress(hModulentdll,"inet_ntoa");
	inet_addr=(pinet_addr)GetProcAddress(hModulentdll,"inet_addr");
	if (inet_addr==0||inet_ntoa==0)
	{
		 exit(-1);
	}
	//ipFltDrv.LoadDriver("IpFilterDriver", "System32\\Drivers\\IpFltDrv.sys", NULL, TRUE);

// 
// 	filterDriver.SetRemovable(FALSE);
// 
// 
// 	if(filterDriver.LoadDriver("DrvFltIp", NULL, NULL, TRUE) != DRV_SUCCESS)
// 	{
// 		AfxMessageBox(L"Error loading the driver.");
// 
// 		exit(-1);
// 	}
    return true;
}
// CPage8 消息处理程序
HBRUSH CPage8::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);


	CBrush* brBkgnd = new CBrush();
	brBkgnd->CreateSolidBrush( RGB(250, 249, 250) );
	hbr = *brBkgnd;                // Control bkgnd


	return hbr;
}
int CPage8::AddRule(unsigned long srcIp,
							 unsigned long srcMask,
							 unsigned short srcPort,
							 unsigned long dstIp,
							 unsigned long dstMask,
							 unsigned short dstPort,
							 unsigned int protocol,
							 int action)
{

	if(nRules >= MAX_RULES)
	{
		return -1;
	}

	else
	{
		rules[nRules].sourceIp		  = srcIp;
		rules[nRules].sourceMask	  = srcMask;
		rules[nRules].sourcePort	  = srcPort;
		rules[nRules].destinationIp   = dstIp;
		rules[nRules].destinationMask = dstMask;
		rules[nRules].destinationPort = dstPort;
		rules[nRules].protocol		  = protocol;
		rules[nRules].action		  = action;

		nRules++;
	}

	return 0;
}

void CPage8::ResetRules()
{
	nRules = 0;
}

void CPage8::DeleteRule(unsigned int position)
{
	if(position >= nRules)
		return;

	if(position != nRules - 1)
	{
		unsigned int i;

		for(i = position + 1;i<nRules;i++)
		{
			rules[i - 1].sourceIp		  = rules[i].sourceIp;
			rules[i - 1].sourceMask		  = rules[i].sourceMask;
			rules[i - 1].sourcePort		  = rules[i].sourcePort;
			rules[i - 1].destinationIp    = rules[i].destinationIp;
			rules[i - 1].destinationMask  = rules[i].destinationMask;
			rules[i - 1].destinationPort  = rules[i].destinationPort;
			rules[i - 1].protocol	      = rules[i].protocol;
			rules[i - 1].action		      = rules[i].action;
		}
	}
	nRules--;
}

void CPage8::UpdateList(void)
{

	m_rules.DeleteAllItems();

	unsigned int i;
	for(i=0;i<nRules;i++)
	{
		AddRuleToList(rules[i].sourceIp,
			rules[i].sourceMask,
			rules[i].sourcePort,
			rules[i].destinationIp,
			rules[i].destinationMask,
			rules[i].destinationPort,
			rules[i].protocol,
			rules[i].action);
	}
}

void CPage8::AddRuleToList(unsigned long srcIp, 
									 unsigned long srcMask,
									 unsigned short srcPort, 
									 unsigned long dstIp, 
									 unsigned long dstMask,
									 unsigned short dstPort, 
									 unsigned int protocol, 
									 int action)
{
	char ip[16];
	char port[6];

	char *pszText;
	WCHAR pwzText[20];
	LVITEM it;
	int pos;

//void CMySystem::CharToWCHAR(WCHAR* wctar,char*cSource)
	it.mask		= LVIF_TEXT;
	it.iItem	= m_rules.GetItemCount();
	it.iSubItem	= 0;
	pszText     = ( srcIp == 0) ? "All" : IpToString(ip, srcIp);
    CMySystem::CharToWCHAR(pwzText,pszText);
	it.pszText	= pwzText;
	pos			= m_rules.InsertItem(&it);

	it.iItem	= pos;
	it.iSubItem	= 1;
	pszText     = IpToString(ip, srcMask);
	CMySystem::CharToWCHAR(pwzText,pszText);
	it.pszText	= pwzText;
	m_rules.SetItem(&it);

	it.iItem	= pos;
	it.iSubItem	= 2;
	pszText     = (srcPort == 0) ? "All" : itoa(srcPort, port, 10);
	CMySystem::CharToWCHAR(pwzText,pszText);
	it.pszText	= pwzText;
	m_rules.SetItem(&it);

	it.iItem	= pos;
	it.iSubItem	= 3;
	pszText     = (dstIp == 0) ? "All" : IpToString(ip, dstIp);
	CMySystem::CharToWCHAR(pwzText,pszText);
	it.pszText	= pwzText;
	m_rules.SetItem(&it);

	it.iItem	= pos;
	it.iSubItem	= 4;
	pszText     = IpToString(ip, dstMask);
	CMySystem::CharToWCHAR(pwzText,pszText);
	it.pszText	= pwzText;
	m_rules.SetItem(&it);

	it.iItem	= pos;
	it.iSubItem = 5;
	pszText     = (dstPort == 0) ? "All" : itoa(dstPort, port, 10);
	CMySystem::CharToWCHAR(pwzText,pszText);
	it.pszText	= pwzText;
	m_rules.SetItem(&it);


	it.iItem	= pos;
	it.iSubItem	= 6;

	if(protocol == 1)
		it.pszText = L"ICMP";

	else if(protocol == 6)
		it.pszText = L"TCP";

	else if(protocol == 17)
		it.pszText = L"UDP";

	else
		it.pszText = L"All";

	m_rules.SetItem(&it);


	it.iItem	= pos;
	it.iSubItem	= 7;
	it.pszText = action ? L"Drop" : L"Forward";
	m_rules.SetItem(&it);
}

void CPage8::OnButtonAdd()
{
	CRuleDlg dlg;
	if(nRules < MAX_RULES )
	{
		if(dlg.DoModal() == IDOK)
		{	
			if(AddRule(dlg.srcIp, dlg.srcMask, dlg.srcPort, dlg.dstIp, dlg.dstMask, dlg.dstPort, dlg.protocol, dlg.cAction) != 0)
				AfxMessageBox(L"添加规则错误！");

			else
			{

				UpdateList();
			}

		}
	}

	else
		AfxMessageBox(L"不能再添加规则了，最多只能添加15个规则！");
}

void CPage8::OnButtonDel()
{
	POSITION pos = m_rules.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox(L"请选择一个规则！");

		return;
	}

	int position;
	position = m_rules.GetNextSelectedItem(pos);
	DeleteRule(position);


	UpdateList();
}

void CPage8::OnButtoninStall()
{
	unsigned int i;
	DWORD result;
	
	for(i=0;i<nRules;i++)
	{
		result = AddFilterToFw(rules[i].sourceIp, 
			rules[i].sourceMask,
			rules[i].sourcePort,
			rules[i].destinationIp,
			rules[i].destinationMask,
			rules[i].destinationPort,
			rules[i].protocol, 
			rules[i].action);


		if (!result) 
			break;
	}
}

BOOL CPage8::AddFilterToFw(unsigned long srcIp, 
							   unsigned long srcMask,
							   unsigned short srcPort, 
							   unsigned long dstIp,
							   unsigned long dstMask,
							   unsigned short dstPort,
							   unsigned int protocol,
							   int action)
{
	IPFilter pf;


	pf.protocol = protocol;

	pf.destinationIp = dstIp;			
	pf.sourceIp		 = srcIp;

	pf.destinationMask = dstMask;	
	pf.sourceMask	   = srcMask;		

	pf.destinationPort = htons(dstPort);						
	pf.sourcePort	   = htons(srcPort);				

	pf.drop = action;		


	DWORD result = filterDriver.WriteIo(ADD_FILTER, &pf, sizeof(pf));

	if (result != DRV_SUCCESS) 
	{
		AfxMessageBox(L"Error adding the rule.");

		return FALSE;
	}

	else
		return TRUE;
}
void CPage8::OnLoadRules()
{
	CFile file;
	CFileException e;
	DWORD nRead;

	CFileDialog dg(TRUE,NULL, NULL, OFN_HIDEREADONLY | OFN_CREATEPROMPT,L"Rule Files(*.rul)|*.rul|all(*.*)|*.*||", NULL);
	if(dg.DoModal()==IDCANCEL)
		return;

	CString nf=dg.GetPathName();

	if(nf.GetLength() == 0)
	{
		AfxMessageBox(L"文件名不可用！");

		return;
	}

	if( !file.Open(nf, CFile::modeRead, &e ) )
	{
		AfxMessageBox(L"打开文件错误！");

		return;
	}

	ResetRules();
	RuleInfo rule;

	do
	{
		nRead = file.Read(&rule, sizeof(RuleInfo));

		if(nRead == 0)
			break;

		if(AddRule(rule.sourceIp,
			rule.sourceMask,
			rule.sourcePort,
			rule.destinationIp,
			rule.destinationMask,
			rule.destinationPort,
			rule.protocol,				   
			rule.action)  != 0)
		{

			AfxMessageBox(L"添加规则错误！");

			break;
		}

	}while (1);
	
	UpdateList();
}

void CPage8::OnSaveRules()
{

	if(nRules == 0)
	{
		AfxMessageBox(L"没有规则");

		return;
	}


	CFileDialog dg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_CREATEPROMPT,L"Rule Files(*.rul)|*.rul|all(*.*)|*.*||", NULL);
	if(dg.DoModal()==IDCANCEL)
		return;

	CString nf=dg.GetPathName();

	if(nf.GetLength() == 0)
	{
		AfxMessageBox(L"文件名不可用");

		return;
	}

	CFile file;
	CFileException e;

	if( !file.Open( nf, CFile::modeCreate | CFile::modeWrite, &e ) )
	{
		AfxMessageBox(L"打开文件错误");

		return;
	}


	unsigned int i;

	for(i=0;i<nRules;i++)
	{
		file.Write(&rules[i], sizeof(RuleInfo));
	}

	file.Close();
}

void CPage8::OnButtonStart()
{

	if(started)
	{
		
		GetDlgItem( IDC_BUTTON6 )->EnableWindow(TRUE);//开始
		GetDlgItem( IDC_BUTTON5 )->EnableWindow(FALSE);
	}

	else
	{
		WinExec("net start ipfilterdriver",0);
		GetDlgItem( IDC_BUTTON6 )->EnableWindow(FALSE);//没开始
		GetDlgItem( IDC_BUTTON5 )->EnableWindow(TRUE);
	}
	if(filterDriver.WriteIo(START_IP_HOOK, NULL, 0) != DRV_ERROR_IO)
	{
		started = TRUE;
	}
}

void CPage8::OnButtonStop()
{
	if(started)
	{
		GetDlgItem( IDC_BUTTON6 )->EnableWindow(TRUE);//开始
		GetDlgItem( IDC_BUTTON5 )->EnableWindow(FALSE);
	}

	else
	{
		GetDlgItem( IDC_BUTTON6 )->EnableWindow(FALSE);//没开始
		GetDlgItem( IDC_BUTTON5 )->EnableWindow(TRUE);
	}

	if(filterDriver.WriteIo(STOP_IP_HOOK, NULL, 0) != DRV_ERROR_IO)
	{
		started = FALSE;
	}
}

void CPage8::OnButtonDesinStall()
{
	if(filterDriver.WriteIo(CLEAR_FILTER, NULL, 0) == DRV_ERROR_IO)
	{
		AfxMessageBox(L"卸载规则错误！");
	}
}

void CPage8::OnBnClickedMonitor()
{
	m_Monitor.EnableWindow(FALSE);
    m_StopMonitor.EnableWindow(TRUE);
	nMessageCount=1;
	SetTimer(IDT_TIMER1,2000,0);
}

void CPage8::OnBnClickedStopMonitor()
{
	m_Monitor.EnableWindow(TRUE);
	m_StopMonitor.EnableWindow(FALSE);
	KillTimer(IDT_TIMER1);

}
void CPage8::OutPrint(HANDLE hMyHandle)
{
	
	unsigned long  DeviceRet=0;
	DeviceIoControl(
		hMyHandle  ,                       // handle to volume
		OUT_PRINTSTR,                      // dwIoControlCode
		g_OutArr,                          // MOVE_FILE_DATA structure
		sizeof(WCHAR)*MAXPRINT*MAXSTR,     // size of input buffer
		g_OutArr,                          // lpOutBuffer
		sizeof(WCHAR)*MAXPRINT*MAXSTR,     // nOutBufferSize
		&DeviceRet,                        // number of bytes returned
		0);                                // OVERLAPPED structure
	return  ;
}
#define SEPARATOR L"\r\n----------------NetMessage-----------------\r\n"
void CPage8::OnTimer(UINT nIDEvent) 
{
	int j=0;
	//char szIP[8]={'0'};
	char aIP[20]={'0'};//点分十进制
	WCHAR wcIP[20]={'20'};//点分十进制
	DWORD dwIP=0;
	CString csTem,csTemContext,csNetMessage,csTemIp, csTemIpSpace;
	KillTimer(IDT_TIMER1);
	TRACE("OnTimer");
	HMODULE hModulentdll = LoadLibraryW (L"ws2_32.dll");
	pinet_ntoa inet_ntoa=(pinet_ntoa)GetProcAddress(hModulentdll,"inet_ntoa");
	pinet_addr inet_addr=(pinet_addr)GetProcAddress(hModulentdll,"inet_addr");
	nMessageCount++;
	if (nMessageCount>30)
	{
		nMessageCount=0;
		csTemContext.Empty();
	}
	csTemContext=SEPARATOR;
	m_NetData.GetWindowText(csNetMessage);
	OutPrint( filterDriver.driverHandle);//得到驱动传来的数据
	for (j=0;j<MAXPRINT;j++)
	{
		csTem.Format(L"%ws\r\n",g_OutArr[j]);
		if (csTem=="\r\n")
		{
			continue;
		}
		if (-1!=csTem.Find(L"NetInfomation"))
        {

			csTemIp=csTem.Mid(csTem.Find(L"@")+1,csTem.Find(L"@@")-csTem.Find(L"@")-1);
	    	dwIP=(DWORD)CMySystem::HexToBigInt(csTemIp);

			strcpy(aIP,inet_ntoa((DWORD)dwIP));
			CMySystem::CharToWCHAR(wcIP,aIP);
			csTemContext+=L"源IP:";csTemContext+=wcIP;csTemContext+=L"  ";
			
			csTemIp=csTem.Mid(csTem.Find(L"@@")+2,csTem.Find(L"@@@")-csTem.Find(L"@@")-2);
			dwIP=(DWORD)CMySystem::HexToBigInt(csTemIp);
			strcpy(aIP,inet_ntoa((DWORD)dwIP));
			CMySystem::CharToWCHAR(wcIP,aIP);
			csTemContext+=L"目标IP:";csTemContext+=wcIP;csTemContext+=L"  ";

			csTemIp=csTem.Right(csTem.GetLength()-csTem.Find(L"@@@")-3);
			
			csTemContext+=L"协议类型:";
			csTemContext+=ChangeProtocolToCString(csTemIp);
			csTemContext+=L"\r\n";
        }

	        
	}
	if (csTemContext==SEPARATOR)
	{
		SetTimer(IDT_TIMER1,2000,0);
		return;
	}
    csNetMessage+=csTemContext;

	m_NetData.SetWindowText(csNetMessage);
	m_NetData.SetSel(csNetMessage.GetLength(),csNetMessage.GetLength(), FALSE); 
	memset(g_OutArr,0,MAXPRINT*sizeof(WCHAR)*MAXSTR);
	SetTimer(IDT_TIMER1,2000,0);


}
CString CPage8::ChangeProtocolToCString(CString csProtocol)
{
	CString csRet=L"Unknown";
	if (csProtocol.GetAt(0)=='1'&&csProtocol.GetAt(1)=='7')
	{
		csRet=L"UDP";
	}
	if (csProtocol.GetAt(0)=='6')
	{
		csRet=L"TCP";
	}
	return csRet;
}
void CPage8::OnBnClickedClearMessage()
{
	m_NetData.SetWindowText(L"");
}
