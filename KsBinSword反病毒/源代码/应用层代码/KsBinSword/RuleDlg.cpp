// RuleDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "KsBinSword.h"
#include "RuleDlg.h"
#include "sockUtil.h"
#include "MySystem.h"
// CRuleDlg 对话框

IMPLEMENT_DYNAMIC(CRuleDlg, CDialog)

CRuleDlg::CRuleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRuleDlg::IDD, pParent)
	
{
	m_ipsource = _T("0.0.0.0");
	m_portsource = 0;
	m_ipdestination = _T("0.0.0.0");
	m_portDestination = 0;
	m_action = _T("Drop");
	m_protocol = _T("TCP");
	m_srcMask = _T("255.255.255.255");
	m_dstMask = _T("255.255.255.255");
}

CRuleDlg::~CRuleDlg()
{
}

void CRuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_ipsource);
	DDV_MaxChars(pDX, m_ipsource, 15);
	DDX_Text(pDX, IDC_EDIT2, m_portsource);
	DDX_Text(pDX, IDC_EDIT3, m_ipdestination);
	DDV_MaxChars(pDX, m_ipdestination, 15);
	DDX_Text(pDX, IDC_EDIT4, m_portDestination);
	DDX_CBString(pDX, IDC_COMBO1, m_action);
	DDX_CBString(pDX, IDC_COMBO2, m_protocol);
	DDX_Text(pDX, IDC_EDIT6, m_srcMask);
	DDV_MaxChars(pDX, m_srcMask, 15);
	DDX_Text(pDX, IDC_EDIT5, m_dstMask);
	DDV_MaxChars(pDX, m_dstMask, 15);
}


BEGIN_MESSAGE_MAP(CRuleDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CRuleDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CRuleDlg 消息处理程序

void CRuleDlg::OnBnClickedOk()
{
	int result;
	char szTem[50]={'0'};

	UpdateData(TRUE);
    CMySystem::WCHARToChar(szTem,m_ipsource.GetBuffer());
	result = inet_addr(szTem, &srcIp);

	if(result == -1)
	{
		AfxMessageBox(L"Source Ip isn't valid.");

		return;
	}
    CMySystem::WCHARToChar(szTem,m_srcMask.GetBuffer());
	result = inet_addr(szTem, &srcMask);

	if(result == -1)
	{
		AfxMessageBox(L"Source Mask isn't valid.");

		return;
	}

    CMySystem::WCHARToChar(szTem,m_ipdestination.GetBuffer());
	result = inet_addr(szTem, &dstIp);

	if(result == -1)
	{
		AfxMessageBox(L"Destination Ip isn't valid.");

		return;
	}
    CMySystem::WCHARToChar(szTem,m_dstMask.GetBuffer());
	result = inet_addr(szTem, &dstMask);

	if(result == -1)
	{
		AfxMessageBox(L"Destination Mask isn't valid.");

		return;
	}


	if(m_protocol == L"TCP")
		protocol = 6;

	else if(m_protocol == L"UDP")
		protocol = 17;

	else if(m_protocol == L"ICMP")
		protocol = 1;

	else
		protocol = 0;


	if(m_action == L"")
	{
		AfxMessageBox(L"Select an Action, please");

		return;
	}

	else
	{
		if(m_action == L"Forward")
			cAction = 0;

		else
			cAction = 1;

	}

	srcPort = m_portsource;
	dstPort = m_portDestination;
	OnOK();
}
