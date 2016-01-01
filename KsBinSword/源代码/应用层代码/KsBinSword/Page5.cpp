// Page5.cpp : 实现文件
//

#include "stdafx.h"
#include "KsBinSword.h"
#include "Page5.h"
#include "MySystem.h"

// CPage5 对话框

IMPLEMENT_DYNAMIC(CPage5, CDialog)

CPage5::CPage5(CWnd* pParent /*=NULL*/)
	: CDialog(CPage5::IDD, pParent)
{

}

CPage5::~CPage5()
{
}

void CPage5::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, m_List);
}


BEGIN_MESSAGE_MAP(CPage5, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &CPage5::OnReSSDTAndThrowSpilth)
	ON_BN_CLICKED(IDC_BUTTON1, &CPage5::OnReShowSSDT)
	ON_BN_CLICKED(IDCANCEL, &CPage5::OnReSSDT)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST3, &CPage5::OnDrawColorForMyList)
	ON_NOTIFY(NM_RCLICK, IDC_LIST3, &CPage5::OnNMRclickList3)
	ON_COMMAND(ID_MENU_32791, &CPage5::OnMenuRusSSDT)
END_MESSAGE_MAP()


// CPage5 消息处理程序
HBRUSH CPage5::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);


	CBrush* brBkgnd = new CBrush();
	brBkgnd->CreateSolidBrush( RGB(250, 249, 250) );
	hbr = *brBkgnd;                // Control bkgnd


	return hbr;
}
BOOL CPage5::OnInitDialog()
{
	CDialog::OnInitDialog();
	if( hDriver == INVALID_HANDLE_VALUE )
	{
		//获取文件句柄失败
		// Do Nothing
	}
	//添加列
	m_List.InsertColumn( 0, L"服务号", LVCFMT_CENTER, 50, -1 );
	m_List.InsertColumn( 1, L"当前地址", LVCFMT_CENTER, 100, -1 );
	m_List.InsertColumn( 2, L"原始地址", LVCFMT_CENTER, 100, -1 );
	m_List.InsertColumn( 3, L"服务函数名", LVCFMT_LEFT, 250, -1 );
	m_List.InsertColumn( 4, L"模块名", LVCFMT_LEFT, 300, -1 );
	//使其可选择一行
	m_List.SetExtendedStyle(m_List.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
	hDriver=CMySystem::hMyHandle;
	ShowSSDT();
	return true;

}
void CPage5::OnReSSDTAndThrowSpilth()
{
	// 恢复SSDT并删除非系统表项
	
	if( IDYES == MessageBox( L"该操作有一定的危险性,是否继续?", L"恢复SSDT并清理提示", MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2 ) )
	{
		//恢复SSDT,并删掉非系统SSDT函数
		if( CMySystem::ReSSDTAndThrowSpilth( hDriver ) )
		{
			MessageBox( L"恢复SSDT并删掉非系统SSDT函数成功!", L"成功", MB_OK );
		}
		else
		{
			MessageBox( L"恢复SSDT并删掉非系统SSDT函数失败!", L"失败", MB_OK );
		}
		//重绘
		OnReShowSSDT();
	}
}

void CPage5::ShowSSDT(void)
{
	//添加内容
	WCHAR szIndex[10]={'\0'};
	WCHAR szCurAddr[18]={'\0'};
	WCHAR szOrgAddr[18]={'\0'};
	WCHAR szTemp[0x200]={'\0'};
	if( !CMySystem::EnumSSDT( hDriver ) )
	{

		//MessageBox( L"枚举SSDT失败!", L"错误", MB_OK );
		return;		//正常退出程序
	}

  
	for(ULONG i = 0; i < CMySystem::TotalSSDTCount; i ++ )
	{
		wsprintf( szIndex, L"0x%04X", 0);
		wsprintf( szIndex, L"0x%04X", \
			((pSSDTSaveTable)((ULONG)CMySystem::pSSDTST + i * sizeof(SSDTSaveTable)))->ulServiceNumber & 0xFFFF \
			);
		wsprintf( szCurAddr, L"0x%08X", \
			((pSSDTSaveTable)((ULONG)CMySystem::pSSDTST + i * sizeof(SSDTSaveTable)))->ulCurrentFunctionAddress \
			);
		wsprintf( szOrgAddr, L"0x%08X", \
			((pSSDTSaveTable)((ULONG)CMySystem::pSSDTST + i * sizeof(SSDTSaveTable)))->ulOriginalFunctionAddress \
			);
		m_List.InsertItem( m_List.GetItemCount(), szIndex );
		m_List.SetItemText( m_List.GetItemCount()-1, 1, szCurAddr );
		m_List.SetItemText( m_List.GetItemCount()-1, 2, szOrgAddr );
        CMySystem::CharToWCHAR(szTemp,((pSSDTSaveTable)((ULONG)CMySystem::pSSDTST + i * sizeof(SSDTSaveTable)))->ServiceFunctionName);
		//wsprintf(szTemp,L"%s",);
		m_List.SetItemText( m_List.GetItemCount()-1, 3, \
			szTemp \
			);
		CMySystem::CharToWCHAR(szTemp,((pSSDTSaveTable)((ULONG)CMySystem::pSSDTST + i * sizeof(SSDTSaveTable)))->ModuleName);
		m_List.SetItemText( m_List.GetItemCount()-1, 4, \
			szTemp \
			);
	}
}

void CPage5::OnReShowSSDT()
{
	m_List.DeleteAllItems();
	ShowSSDT();
}

void CPage5::OnReSSDT()
{
	//确认
	if( IDYES == MessageBox( L"该操作有一定的危险性,是否继续?", L"恢复SSDT提示", MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2 ) )
	{
		//恢复SSDT
		if( CMySystem::ReSSDT( hDriver ) )
		{
			MessageBox( L"恢复SSDT成功!", L"成功", MB_OK );
		}
		else
		{
			MessageBox( L"恢复SSDT失败!", L"失败", MB_OK );
		}
		//重绘
		OnReShowSSDT();
	}
}

void CPage5::OnDrawColorForMyList(NMHDR *pNMHDR, LRESULT *pResult)
{
    NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	*pResult = CDRF_DODEFAULT;

	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
	{
		COLORREF clrNewTextColor, clrNewBkColor;

		int    nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec );

		//如果当前SSDT地址与原始地址不一样,则显示字体为红色,否则为黑色
		if( \
			((pSSDTSaveTable)((ULONG)CMySystem::pSSDTST + nItem * sizeof(SSDTSaveTable)))->ulCurrentFunctionAddress \
			!= ((pSSDTSaveTable)((ULONG)CMySystem::pSSDTST + nItem * sizeof(SSDTSaveTable)))->ulOriginalFunctionAddress \
			)		//设置为红色
		{
			clrNewTextColor = RGB( 255, 0, 0 );
		}
		else		//设置为黑色
		{
			clrNewTextColor = RGB( 0, 0, 0 );
		}

		//设置背景色
		if( nItem%2 ==0 )
		{
			clrNewBkColor = RGB( 240, 240, 240 );	//偶数行背景色为灰色
		}
		else
		{
			clrNewBkColor = RGB( 255, 255, 255 );	//奇数行背景色为白色
		}

		pLVCD->clrText = clrNewTextColor;
		pLVCD->clrTextBk = clrNewBkColor;

		*pResult = CDRF_DODEFAULT;
	}

}

void CPage5::OnNMRclickList3(NMHDR *pNMHDR, LRESULT *pResult)//弹出右键菜单
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1 && pNMListView->iSubItem != -1)
	{ 
		CPoint pt;
		GetCursorPos(&pt);
		CMenu menu;
		menu.LoadMenu(IDR_MENU6);
		CMenu* pMenu = menu.GetSubMenu(0);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this);

	}
	*pResult = 0;
}

void CPage5::OnMenuRusSSDT()
{
	CString str;
	int CurSel=m_List.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
	str=m_List.GetItemText(CurSel,0); // 0=<col<m_list的列数
	DWORD dwIndexId=CMySystem::HexToBigInt(str);

	TRACE("OnMenuRusSSDT:%x",dwIndexId);
	if( IDYES == MessageBox( L"该操作有一定的危险性,是否继续?", L"恢复SSDT提示", MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2 ) )
	{
		//恢复SSDT
 		if( CMySystem::SetHook( hDriver, \
 			((pSSDTSaveTable)((ULONG)CMySystem::pSSDTST + dwIndexId * sizeof(SSDTSaveTable)))->ulServiceNumber, \
 			&(((pSSDTSaveTable)((ULONG)CMySystem::pSSDTST + dwIndexId * sizeof(SSDTSaveTable)))->ulOriginalFunctionAddress) \
 			) )
		{
			MessageBox( L"恢复SSDT成功!", L"成功", MB_OK );
		}
		else
		{
			MessageBox( L"恢复SSDT失败!", L"失败", MB_OK );
		}
		//重绘
		OnReShowSSDT();
	}
}
