// KsBinSwordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "KsBinSword.h"
#include "KsBinSwordDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define WM_ICON_NOTIFY  WM_USER+10
#define WM_ICON_LBUTTONDBLCLK  WM_USER+11

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()


};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)

	
END_MESSAGE_MAP()


// CKsBinSwordDlg 对话框




CKsBinSwordDlg::CKsBinSwordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKsBinSwordDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKsBinSwordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab1);
	DDX_Control(pDX, IDC_COMBO2, m_ComboSkin);
}

BEGIN_MESSAGE_MAP(CKsBinSwordDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CKsBinSwordDlg::OnBnClickedOk)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB1, &CKsBinSwordDlg::OnTcnSelchangingTab1)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CKsBinSwordDlg::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDTOSYSTRAY, &CKsBinSwordDlg::OnBnClickedCancel)
	ON_MESSAGE(WM_ICON_NOTIFY, OnTrayNotification)
	ON_COMMAND(WM_ICON_LBUTTONDBLCLK, OnShowPage)
	ON_COMMAND(ID_MENU_11, &CKsBinSwordDlg::OnMenuShowMain)
	ON_COMMAND(ID_MENU_1222, &CKsBinSwordDlg::OnMenuExit)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CKsBinSwordDlg::OnCbnSelchangeSkin)
END_MESSAGE_MAP()


// CKsBinSwordDlg 消息处理程序

BOOL CKsBinSwordDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_NORMAL);

	// TODO: 在此添加额外的初始化代码
    PageArr[0]=&m_Page1;
	PageArr[1]=&m_Page2;
	PageArr[2]=&m_Page3;
	PageArr[3]=&m_Page4;
	PageArr[4]=&m_Page5;
	PageArr[5]=&m_Page6;
    PageArr[6]=&m_Page7;
	PageArr[7]=&m_Page8;
	PageArr[8]=&m_Page9;

	m_tab1.InsertItem(0, _T(" 进程管理"));
	PageArr[0]->Create(IDD_DIALOG1, &m_tab1);

	m_tab1.InsertItem(1, _T(" 监控配置"));
    PageArr[1]->Create(IDD_DIALOG2, &m_tab1);

	m_tab1.InsertItem(2, _T(" 驱动模块枚举"));
    PageArr[2]->Create(IDD_DIALOG3, &m_tab1);

	m_tab1.InsertItem(3, _T("监控信息"));
    PageArr[3]->Create(IDD_DIALOG4, &m_tab1);

	m_tab1.InsertItem(4, _T(" SSDT"));
    PageArr[4]->Create(IDD_DIALOG5, &m_tab1);

	m_tab1.InsertItem(5, _T(" 文件管理器"));
	PageArr[5]->Create(IDD_DIALOG6, &m_tab1);

	m_tab1.InsertItem(6, _T(" 磁盘编辑器"));
	PageArr[6]->Create(IDD_DIALOG7, &m_tab1);

	m_tab1.InsertItem(7, _T(" 网络防火墙"));
	PageArr[7]->Create(IDD_DIALOG8, &m_tab1);

	m_tab1.InsertItem(8, _T(" PE文件信息查看"));
	PageArr[8]->Create(IDD_DIALOG9, &m_tab1);

    m_tab1.SetCurSel(0);
	CRect rc;
	m_tab1.GetClientRect(rc);
	rc.top += 20;
	rc.bottom -= 8;
	rc.left += 10;
	rc.right -= 10;
    for (int i=0;i<PageNum;i++)
	{
		PageArr[i]->MoveWindow(&rc);
	}

	PageArr[0]->ShowWindow(SW_SHOW);

    m_ComboSkin.SetCurSel(0);
    //m_ComboSkin.SetWindowText(L"皮肤1");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CKsBinSwordDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CKsBinSwordDlg::OnPaint()
{
	CPaintDC    dc(this);   
	CRect    rect;     
	GetClientRect(rect);     
	dc.FillSolidRect(rect,RGB(250, 249, 250));     

	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CKsBinSwordDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CKsBinSwordDlg::OnBnClickedOk()
{

	WinExec("explorer 帮助文档.htm",0);
}

void CKsBinSwordDlg::OnTcnSelchangingTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	
	*pResult = 0;
}

void CKsBinSwordDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int CurSel; CRect rc;
	CurSel=m_tab1.GetCurSel(); 
    ShowPageX(CurSel);
    *pResult = 0;


}

void CKsBinSwordDlg::ShowPageX(int Index)
{
	for (int i=0;i<PageNum;i++)
	{
		PageArr[i]->ShowWindow(FALSE);
	}
	PageArr[Index]->ShowWindow(TRUE);
}


void CKsBinSwordDlg::OnBnClickedCancel()//最小化进入系统托盘
{
	m_TrayIcon.Create(this,WM_ICON_NOTIFY,L"系统防御软件KsBinSword",m_hIcon,IDR_MENU5); //构造
	ShowWindow(SW_HIDE); //隐藏窗口
	//OnOK();


}
LRESULT CKsBinSwordDlg::OnTrayNotification(WPARAM wParam,LPARAM lParam)
{
	return m_TrayIcon.OnTrayNotification(wParam,lParam);
}

void CKsBinSwordDlg::OnMenuShowMain()
{
	OnShowPage();
}
void CKsBinSwordDlg::OnShowPage()
{
	m_TrayIcon.RemoveIcon();
	ShowWindow(SW_SHOW);
}
void CKsBinSwordDlg::OnMenuExit()
{
	OnCancel();
}

void CKsBinSwordDlg::OnCbnSelchangeSkin()
{


	CString csSkin;
	//theApp.USkinExit();
	m_ComboSkin.GetWindowText(csSkin);
	if (csSkin==L"皮肤1")
	{
		theApp.USkinLoadSkin("Skins\\1.u3");
		return;
	}
	if (csSkin==L"皮肤2")
	{
		theApp.USkinLoadSkin("Skins\\2.u3");
		return;
	}
	if (csSkin==L"皮肤3")
	{
		theApp.USkinLoadSkin("Skins\\3.u3");
		return;
	}
	if (csSkin==L"皮肤4")
	{
		theApp.USkinLoadSkin("Skins\\4.u3");
		return;
	}
	if (csSkin==L"皮肤5")
	{
		theApp.USkinLoadSkin("Skins\\5.u3");
		return;
	}
	if (csSkin==L"皮肤6")
	{
		theApp.USkinLoadSkin("Skins\\6.u3");
		return;
	}
	if (csSkin==L"皮肤7")
	{
		theApp.USkinLoadSkin("Skins\\7.u3");
		return;
	}
	if (csSkin==L"皮肤8")
	{
		theApp.USkinLoadSkin("Skins\\8.u3");
		return;
	}
	if (csSkin==L"皮肤9")
	{
		theApp.USkinLoadSkin("Skins\\9.u3");
		return;
	}
	if (csSkin==L"皮肤10")
	{
		theApp.USkinLoadSkin("Skins\\10.u3");
		return;
	}
	if (csSkin==L"皮肤11")
	{
		theApp.USkinLoadSkin("Skins\\11.u3");
		return;
	}

}
