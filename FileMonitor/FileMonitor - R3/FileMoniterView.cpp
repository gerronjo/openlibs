// FileMoniterView.cpp : implementation of the CFileMoniterView class
//

#include "stdafx.h"
#include "FileMoniter.h"

#include "FileMoniterDoc.h"
#include "FileMoniterView.h"
#include "..\FileMonitor - R0\common.h"

#include <winioctl.h>
#include <io.h>
#include <winsvc.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HANDLE gEvent;
HANDLE gDevice;
int j = 0;

#define SERVICE_NAME  "FileMonitor"
#define DRIVER_NAME	  "FileMonitor.sys"

/////////////////////////////////////////////////////////////////////////////
// CFileMoniterView

IMPLEMENT_DYNCREATE(CFileMoniterView, CListView)

BEGIN_MESSAGE_MAP(CFileMoniterView, CListView)
	//{{AFX_MSG_MAP(CFileMoniterView)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CListView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileMoniterView construction/destruction
UINT GetFileThread( LPVOID pParam )
{
	int i = 0;
	DWORD dwRet;
	CFileMoniterView *pDlg = (CFileMoniterView*)pParam;

	DeviceIoControl(gDevice, IOCTRL_STARTMON, NULL, 0, NULL, 0, &dwRet, NULL);

	while (1)
	{
		CHAR chFilePath[1024] = {0};
		CHAR ntName[24] = {0};
		
		DeviceIoControl(gDevice, IOCTL_FILEMON_GETINFO, NULL, 0, chFilePath, 1024, &dwRet, NULL);		
		if (j == 1)
		{
			break;
		}

		CString strTemp;
		strTemp.Format("%d", i+1);

		COleDateTime time = COleDateTime::GetCurrentTime();
		CString strTime = time.Format("%Y/%m/%d %H:%M:%S");

		pDlg->m_pList->InsertItem(i, strTemp);
		pDlg->m_pList->SetItemText(i, 1, strTime);
		pDlg->m_pList->SetItemText(i, 2, chFilePath);
		
		i++;
	}

	DeviceIoControl(gDevice, IOCTRL_STOPMON, NULL, 0, NULL, 0, &dwRet, NULL);
	
	return 0;
}

CFileMoniterView::CFileMoniterView()
{
	// TODO: add construction code here
	m_pList = NULL;
}

CFileMoniterView::~CFileMoniterView()
{
}

BOOL CFileMoniterView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	#if 1
	cs.style |= LVS_REPORT |
		LVS_SHOWSELALWAYS | 
		LVS_SINGLESEL;
    #endif
	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CFileMoniterView drawing

void CFileMoniterView::OnDraw(CDC* pDC)
{
	CFileMoniterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}



void CFileMoniterView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	BOOL bRet = LoadNTDriver(SERVICE_NAME, DRIVER_NAME);
	if (!bRet) 
	{
		bRet = LoadNTDriver(SERVICE_NAME, DRIVER_NAME);
		if (!bRet)
		{
			bRet = LoadNTDriver(SERVICE_NAME, DRIVER_NAME);
			if (!bRet)
			{
				MessageBox("加载驱动失败, 程序退出运行!");
				ExitProcess(0);
				return;
			}
		}
	}
	
	//最多开启服务3遍
	bRet = Startservice(SERVICE_NAME, DRIVER_NAME);
	if (!bRet)
	{
		bRet = Startservice(SERVICE_NAME, DRIVER_NAME);
		if (!bRet)
		{
			if (!bRet)
			{
				bRet = Startservice(SERVICE_NAME, DRIVER_NAME);
				if (!bRet)
				{
					MessageBox("启动服务失败, 程序退出运行!");
					ExitProcess(0);
					return;
				}
			}
		}
	}

	m_pList = &GetListCtrl();
	m_pList->InsertColumn(0, "计数");
	m_pList->InsertColumn(1, "时间");
	m_pList->InsertColumn(2, "路径");
	
	m_pList->SetColumnWidth(0, 50);
	m_pList->SetColumnWidth(1, 150);
	m_pList->SetColumnWidth(2, 750);

	DWORD dwRet;

	gEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    gDevice = CreateFile("\\\\.\\FileMonitor",
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
    if ( gDevice == INVALID_HANDLE_VALUE )
	{
		MessageBox("打开失败~");
	}
	else
	{
	//	MessageBox("打开成功~");
		DeviceIoControl(gDevice, IOCTL_FILEMON_SETEVENT, &gEvent, sizeof(HANDLE), NULL, 0, &dwRet, NULL);
	}

	AfxBeginThread(GetFileThread, this);
}

/////////////////////////////////////////////////////////////////////////////
// CFileMoniterView printing

BOOL CFileMoniterView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CFileMoniterView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CFileMoniterView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CFileMoniterView diagnostics

#ifdef _DEBUG
void CFileMoniterView::AssertValid() const
{
	CListView::AssertValid();
}

void CFileMoniterView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CFileMoniterDoc* CFileMoniterView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFileMoniterDoc)));
	return (CFileMoniterDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFileMoniterView message handlers

int CFileMoniterView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	#if 1
	DWORD dwStyle = SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP;
	SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)dwStyle);
	#endif

	return 0;
}

BOOL CFileMoniterView::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	j = 1;
	return CListView::DestroyWindow();
}

BOOL CFileMoniterView::LoadNTDriver(CHAR* lpszDriverName, CHAR* lpszDriverPath)
{
	CHAR szDriverImagePath[256];
	BOOL bRet = TRUE;

	SC_HANDLE hServiceMgr=NULL;
	SC_HANDLE hServiceDDK=NULL;

	GetFullPathName(lpszDriverPath, 256, szDriverImagePath, NULL);
	
	hServiceMgr = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );
	
	if( hServiceMgr == NULL )  
	{
		bRet = FALSE;
		goto BeforeLeave;
	}
	
	hServiceDDK = CreateService(hServiceMgr,
								lpszDriverName, 
								lpszDriverName,  
								SERVICE_ALL_ACCESS, 
								SERVICE_KERNEL_DRIVER,
								SERVICE_DEMAND_START,
								SERVICE_ERROR_IGNORE,
								szDriverImagePath, 
								NULL, NULL, NULL, NULL, NULL
								);  
	
	DWORD dwRtn;

	if( hServiceDDK == NULL )  
	{  
		dwRtn = GetLastError();
		if( dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_EXISTS )  
		{  
			bRet = FALSE;
			goto BeforeLeave;
		}  	
		
		hServiceDDK = OpenService( hServiceMgr, lpszDriverName, SERVICE_ALL_ACCESS );  
		if( hServiceDDK == NULL )  
		{
			bRet = FALSE;
			goto BeforeLeave;
		}  
	}  
	
	bRet = TRUE;

BeforeLeave:
	if(hServiceDDK)
	{
		CloseServiceHandle(hServiceDDK);
	}
	if(hServiceMgr)
	{
		CloseServiceHandle(hServiceMgr);
	}

	return bRet;
}

BOOL CFileMoniterView::Startservice(CHAR* lpszDriverName, CHAR*  lpszDriverPath)
{
	
	CHAR szDriverImagePath[256];
	BOOL bRet = TRUE;
	
	SC_HANDLE hServiceMgr=NULL;
	SC_HANDLE hServiceDDK=NULL;

	GetFullPathName(lpszDriverPath, 256, szDriverImagePath, NULL);

	hServiceMgr = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );
	if( hServiceMgr == NULL )  
	{
		bRet = FALSE;
		goto BeforeLeave;
	}

	hServiceDDK = OpenService( hServiceMgr, lpszDriverName, SERVICE_ALL_ACCESS );  
	if( hServiceDDK == NULL )  
	{
		bRet = FALSE;
		goto BeforeLeave;
	}  

	bRet= StartService( hServiceDDK, NULL, NULL );  
	if( !bRet )  
	{  
		DWORD dwRtn = GetLastError();  
		if( dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_ALREADY_RUNNING )  
		{  
			bRet = FALSE;
			goto BeforeLeave;
		}  
		else  
		{  
			if( dwRtn == ERROR_IO_PENDING )  
			{  
				bRet = FALSE;
				goto BeforeLeave;
			}  
			else  
			{  
				bRet = TRUE;
				goto BeforeLeave;
			}  
		}  
	}

BeforeLeave:
	if(hServiceDDK)
	{
		CloseServiceHandle(hServiceDDK);
	}
	if(hServiceMgr)
	{
		CloseServiceHandle(hServiceMgr);
	}

	return bRet;
}